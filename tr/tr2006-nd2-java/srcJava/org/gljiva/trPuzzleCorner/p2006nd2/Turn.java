/**
 * Copyright (c) 2007, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd2;

public class Turn {
    public int    round;
    public String who;

    public static final String WHO_A = "A";
    public static final String WHO_B = "B";
    public static final String WHO_C = "C";

    public Turn(int roundVal, String whoVal) {
        round = roundVal;
        who   = whoVal;
    }
}