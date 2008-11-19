package org.gljiva.trPuzzleCorner.p2006nd2.test;

import org.gljiva.trPuzzleCorner.p2006nd2.LinearCombination;

import junit.framework.TestCase;

public class LinearCombinationTest extends TestCase {

    public void testGet() {
        LinearCombination lc;

        lc = new LinearCombination(1, -2, 3);
        assertNotNull(lc);
        assertEquals(1,  lc.getKa());
        assertEquals(-2, lc.getKb());
        assertEquals(3,  lc.getKc());

        lc = new LinearCombination(3, 6, 9);
        assertNotNull(lc);
        assertEquals(3,  lc.getKa());
        assertEquals(6,  lc.getKb());
        assertEquals(9,  lc.getKc());
    }

    public void testValue() {
        int a, b, c;
        LinearCombination lc = new LinearCombination(1, -2, 3);
        assertNotNull(lc);

        a = 12786;
        b = 89789;
        c = 80767;
        assertEquals(a - 2*b + 3*c, lc.value(a, b, c));
        
        a = -a;
        b = -b;
        assertEquals(a - 2*b + 3*c, lc.value(a, b, c));

        b = -b;
        c = -c;
        assertEquals(a - 2*b + 3*c, lc.value(a, b, c));
    }

    public void testValueIsAlwaysZero() {
        LinearCombination lc;

        lc = new LinearCombination(0, 0, 0);
        assertTrue(lc.valueIsAlwaysZero());

        int values[] = { -2, -1, 0, 1, 2 };
        for (int v1 : values) {
            for (int v2 : values) {
                for (int v3 : values) {
                    lc = new LinearCombination(v1, v2, v3);
                    assertEquals((v1 == 0) && (v2 == 0) && (v3 == 0),
                            lc.valueIsAlwaysZero());
                }
            }
        }
    }

    public void testValueIsAlwaysPositive() {
        LinearCombination lc;

        lc = new LinearCombination(1, 0, 1);
        assertTrue(lc.valueIsAlwaysPositive());

        int values[] = { -2, -1, 0, 1, 2 };
        for (int v1 : values) {
            for (int v2 : values) {
                for (int v3 : values) {
                    lc = new LinearCombination(v1, v2, v3);
                    if ((v1 == 0) && (v2 == 0) && (v3 == 0)) {
                        assertFalse(lc.valueIsAlwaysPositive());
                    } else {
                        assertEquals((v1 >= 0) && (v2 >= 0) && (v3 >= 0),
                                lc.valueIsAlwaysPositive());
                    }
                }
            }
        }
    }

    public void testValueIsAlwaysNegative() {
        LinearCombination lc;

        lc = new LinearCombination(-1, 0, -1);
        assertTrue(lc.valueIsAlwaysNegative());

        int values[] = { -2, -1, 0, 1, 2 };
        for (int v1 : values) {
            for (int v2 : values) {
                for (int v3 : values) {
                    lc = new LinearCombination(v1, v2, v3);
                    if ((v1 == 0) && (v2 == 0) && (v3 == 0)) {
                        assertFalse(lc.valueIsAlwaysNegative());
                    } else {
                        assertEquals((v1 <= 0) && (v2 <= 0) && (v3 <= 0),
                                lc.valueIsAlwaysNegative());
                    }
                }
            }
        }
    }

    public void testToString() {
        LinearCombination lc;
        String str;

        lc  = new LinearCombination(1, 2, 3);
        str = lc.toString(); 
        assertNotNull(str);
        assertTrue(str.length() > 6);

        lc  = new LinearCombination(0, 0, 0);
        str = lc.toString(); 
        assertNotNull(str);
        assertTrue(str.length() > 6);

        lc  = new LinearCombination(-10000, 20000, -30000);
        str = lc.toString(); 
        assertNotNull(str);
        assertTrue(str.length() > 6);
    }

    private static class TestEqualsHelperClass extends LinearCombination {
        public TestEqualsHelperClass(int Ka, int Kb, int Kc) {
            super(Ka, Kb, Kc);
        }
        public TestEqualsHelperClass() {
            super(1, -2, 3);  // for anonymous classes
        }
    }

    public void testEquals() {
        LinearCombination lc0 = new LinearCombination(0, 0, 0);
        LinearCombination lc1 = new LinearCombination(1, -2, 3);
        LinearCombination lc2 = new LinearCombination(1, -2, 3);
        LinearCombination lc3 = new TestEqualsHelperClass(1, -2, 3);
        LinearCombination lc4 = new TestEqualsHelperClass(1, -2, 3);
        LinearCombination lc5 = new TestEqualsHelperClass() {
            @SuppressWarnings("unused")
            public void someExtraMethodFor5() {}
        };
        LinearCombination lc6 = new TestEqualsHelperClass() {
            @SuppressWarnings("unused")
            public void someExtraMethodFor6() {}
        };
        LinearCombination lc7 = new LinearCombination(-2, 4, -6);

        assertTrue(lc0.equals(lc0));
        assertTrue(lc1.equals(lc1));
        assertTrue(lc2.equals(lc2));
        assertTrue(lc3.equals(lc3));
        assertTrue(lc4.equals(lc4));
        assertTrue(lc5.equals(lc5));
        assertTrue(lc6.equals(lc6));
        assertTrue(lc7.equals(lc7));

        assertFalse(lc0.equals(null));
        assertFalse(lc0.equals(lc1));
        assertTrue(lc1.equals(lc1));
        assertTrue(lc1.equals(lc2));
        assertFalse(lc1.equals(lc7));
        assertFalse(lc1.equals(lc3));
        assertFalse(lc2.equals(lc4));
        assertTrue(lc3.equals(lc4));
        assertFalse(lc1.equals(lc5));
        assertFalse(lc2.equals(lc6));
    }

}
