package org.gljiva.trPuzzleCorner.p2006nd2.test;

import org.gljiva.trPuzzleCorner.p2006nd2.Util;

import junit.framework.TestCase;

public class UtilTest extends TestCase {

    public void testGcd() {
        assertEquals(1, Util.gcd(7, 11));
        assertEquals(3, Util.gcd(9, 12, 33333));
        assertEquals(1, Util.gcd(-2, -7));
        assertEquals(2, Util.gcd( 4,  6,  8));
        assertEquals(2, Util.gcd(-4, -6, -8));
        assertEquals(2, Util.gcd(-4,  6, -8));
        assertEquals(9, Util.gcd(9));
    }

}
