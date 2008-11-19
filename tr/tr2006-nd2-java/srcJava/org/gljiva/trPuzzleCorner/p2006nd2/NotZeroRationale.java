/**
 * Copyright (c) 2007, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd2;

/**
 * @author bert
 */
public class NotZeroRationale implements Rationale {

    private final String who;

    public NotZeroRationale(String whoVal) {
        who = whoVal;
    }

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.Rationale#didntGuess(int, int, int, java.lang.String)
     */
    public String didntGuess(int a, int b, int c, String indent) {
        return indent + who + " couldn't guess 0." + NL;
    }

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.Rationale#explain(int, int, int, java.lang.String)
     */
    public String explain(int a, int b, int c, String indent) {
        return indent + who + "'s number cannot be 0." + NL;
    }
}
