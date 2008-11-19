/**
 * Copyright (c) 2007, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd2;

/**
 * @author bert
 */
public interface Rationale {

    public static final String NL = System.getProperty("line.separator");

    public static final String EXTRA_INDENT = "    ";

    public String didntGuess(int a, int b, int c, String indent);

    public String explain(int a, int b, int c, String indent);

}
