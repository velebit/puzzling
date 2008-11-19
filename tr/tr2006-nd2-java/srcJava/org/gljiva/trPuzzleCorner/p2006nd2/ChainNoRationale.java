/**
 * Copyright (c) 2007, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd2;

/**
 * @author bert
 */
public class ChainNoRationale extends ChainRationale implements Rationale {

    public ChainNoRationale(Rule parentVal, Person personVal, Turn startVal) {
        super(parentVal, personVal, startVal);
    }

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.Rationale#explain(int, int, int, java.lang.String)
     */
    @Override
    public String explain(int a, int b, int c, String indent) {
        String indent1 = indent;
        String indent2 = indent + EXTRA_INDENT;

        String me      = getPersonId();
        int round      = getRuleRound();
        int myNumber   = getNumberForPerson(a, b, c);
        int[] whatISee = getNumbersNotForPerson(a, b, c);
        int[] myGuess  = getPossibleGuesses(a, b, c);

        StringBuilder sb = new StringBuilder(1024);
        sb.append(indent1 + me + " would see numbers " + whatISee[0] +
                " and " + whatISee[1] + "." + NL);

        if (myGuess[1] == 0) {
            // Special case: the hats we see are the same; bypass the chain.
            assert(whatISee[0] == whatISee[1]);
            sb.append(indent1 + me + " would know that their own hat has to" +
                    " be " + myGuess[2] + " (it can't be 0)." + NL);

        } else {
            assert(whatISee[0] != whatISee[1]);
            sb.append(indent1 + me + " would know that their own hat has to" +
                    " be " + myGuess[1] + " or " + myGuess[2] + "." + NL);

            int altVal = -1;
            if (myNumber == myGuess[1]) {
                altVal = myGuess[2];
            } else if (myNumber == myGuess[2]) {
                altVal = myGuess[1];
            }

            if (altVal < 0) {
                sb.append(indent1 + "But it would be " + myNumber + ".  " +
                        "Poor " + me + " would be very confused!" + NL);
                return sb.toString();
            }
                
            sb.append(indent1 + "If " + me + "'s hat was " +
                    altVal + ", then:" + NL);

            int[] altABC = substituteNumberForPerson(altVal, a, b, c);

            sb.append(getParent().explain(altABC[0], altABC[1], altABC[2],
                    indent2));
            sb.append(getParent().didntGuess(altABC[0], altABC[1], altABC[2],
                    indent1));

            sb.append(indent1 + "So " + me + " would have known their own " +
                    "hat isn't " + altVal + "." + NL);
        }

        sb.append(indent1 + me + " would guess " + myNumber + " in round " +
                round + "." + NL);

        return sb.toString();
    }
}
