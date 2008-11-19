package org.gljiva.trPuzzleCorner.p2006nd2;

public abstract class ChainRationale implements Rationale {

    protected final Rationale parentRationale;

    public abstract String explain(int a, int b, int c, String indent);

    protected final Person person;
    protected final Turn start;

    public ChainRationale(Rule parentVal, Person personVal, Turn startVal) {
        assert(parentVal != null);
        parentRationale = parentVal.getRationale();
        assert(parentRationale != null);

        person = personVal;
        assert(person != null);
        start  = startVal;
        assert(start != null);
    }

    protected ChainRationale(Rationale rationale, Person personVal, Turn startVal) {
        parentRationale = rationale;
        assert(parentRationale != null);
        person = personVal;
        assert(person != null);
        start  = startVal;
        assert(start != null);
    }

    protected Rationale getParent() {
        return parentRationale;
    }

    protected String getPersonId() {
        String id = person.getId();
        assert(id.equals(start.who));
        return id;
    }

    protected int getRuleRound() {
        return start.round;
    }

    protected int getNumberForPerson(int a, int b, int c) {
        return person.getMyNumber(a, b, c);
    }

    protected int[] getNumbersNotForPerson(int a, int b, int c) {
        int[] num = person.getOtherNumbers(a, b, c);
        assert(num.length == 2);
        return num;
    }

    protected int[] getPossibleGuesses(int a, int b, int c) {
        int[] guesses = person.possibleGuesses(a, b, c);
        assert(guesses.length == 3);
        return guesses;
    }

    protected int[] substituteNumberForPerson(int altVal, int a, int b, int c) {
        int[] altABC = person.substituteMyNumber(altVal, a, b, c);
        assert(altABC.length == 3);
        return altABC;
    }

    public String didntGuess(int a, int b, int c, String indent) {
        String me      = getPersonId();
        int round      = getRuleRound();
        int myNumber   = getNumberForPerson(a, b, c);
    
        return(indent + "But " + me + " didn't guess " + myNumber +
                " in round " + round + "." + NL);
    }

}