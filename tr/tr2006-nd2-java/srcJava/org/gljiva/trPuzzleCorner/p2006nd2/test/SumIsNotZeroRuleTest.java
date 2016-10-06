package org.gljiva.trPuzzleCorner.p2006nd2.test;

import org.gljiva.trPuzzleCorner.p2006nd2.Rule;
import org.gljiva.trPuzzleCorner.p2006nd2.SumBasedRule;
import org.gljiva.trPuzzleCorner.p2006nd2.LinearCombination;
import org.gljiva.trPuzzleCorner.p2006nd2.SumIsNotZeroRule;
import org.gljiva.trPuzzleCorner.p2006nd2.SumIsZeroRule;

import junit.framework.TestCase;

public class SumIsNotZeroRuleTest extends TestCase {

    public void testCanonical() {
        SumBasedRule nz;

        nz = new SumIsNotZeroRule(1, -2, 0);
        assertEquals(1,  nz.getKa());
        assertEquals(-2, nz.getKb());
        assertEquals(0,  nz.getKc());

        nz = new SumIsNotZeroRule(-3, 6, 0);
        assertEquals(1,  nz.getKa());
        assertEquals(-2, nz.getKb());
        assertEquals(0,  nz.getKc());

        nz = new SumIsNotZeroRule(0, -6, -9);
        assertEquals(0,  nz.getKa());
        assertEquals(2, nz.getKb());
        assertEquals(3,  nz.getKc());

        nz = new SumIsNotZeroRule(0, 0, 0);
        assertEquals(0,  nz.getKa());
        assertEquals(0,  nz.getKb());
        assertEquals(0,  nz.getKc());
    }

    public void testIsSatisfied() {
        Rule nz;

        nz = new SumIsNotZeroRule(1, -2, 0);
        assertFalse(nz.isSatisfied(2, 1, 0));
        assertFalse(nz.isSatisfied(2, 1, 9999));
        assertTrue(nz.isSatisfied(-2, 1, 0));

        nz = new SumIsNotZeroRule(1, 2, 0);
        assertTrue(nz.isSatisfied(2, 1, 0));
        assertTrue(nz.isSatisfied(2, 1, 9999));
        assertTrue(nz.isSatisfied(9999, 9999, 9999));

        nz = new SumIsNotZeroRule(0, 0, 0);
        assertFalse(nz.isSatisfied(2, 1, 0));
        assertFalse(nz.isSatisfied(2, 1, 9999));
        assertFalse(nz.isSatisfied(9999, 9999, 9999));
    }

    public void testIsAlwaysSatisfied() {
        Rule nz;

        nz = new SumIsNotZeroRule(1, -2, 0);
        assertFalse(nz.isAlwaysSatisfied());

        nz = new SumIsNotZeroRule(1, 2, 0);
        assertTrue(nz.isAlwaysSatisfied());
    }

    public void testIsNeverSatisfied() {
        Rule nz;

        nz = new SumIsNotZeroRule(1, -2, 0);
        assertFalse(nz.isNeverSatisfied());

        nz = new SumIsNotZeroRule(0, 0, 0);
        assertTrue(nz.isNeverSatisfied());
    }

    public void testToString() {
        final String PREFIX = "rule: ";
        Rule nz;

        nz = new SumIsNotZeroRule(0, 0, 0);
        assertEquals(PREFIX + "0 != 0", nz.toString());

        nz = new SumIsNotZeroRule(1, -1, 0);
        assertEquals(PREFIX + "a != b", nz.toString());

        nz = new SumIsNotZeroRule(-1, 1, 0);  // will canonicalize!
        assertEquals(PREFIX + "a != b", nz.toString());

        nz = new SumIsNotZeroRule(1, 1, 0);
        assertEquals(PREFIX + "a + b != 0", nz.toString());

        nz = new SumIsNotZeroRule(1, -1, 1);
        assertEquals(PREFIX + "a + c != b", nz.toString());

        nz = new SumIsNotZeroRule(1, -3, 0);
        assertEquals(PREFIX + "a != 3*b", nz.toString());

        nz = new SumIsNotZeroRule(-5, 7, 0);  // will canonicalize!
        assertEquals(PREFIX + "5*a != 7*b", nz.toString());

        nz = new SumIsNotZeroRule(9, 11, 0);
        assertEquals(PREFIX + "9*a + 11*b != 0", nz.toString());

        nz = new SumIsNotZeroRule(12, -45, 79);
        assertEquals(PREFIX + "12*a + 79*c != 45*b", nz.toString());
    }

    public void testEquals() {
        Rule nz0  = new SumIsNotZeroRule(0, 0, 0);
        Rule nz1  = new SumIsNotZeroRule(1, -2, 3);
        Rule nz2  = new SumIsNotZeroRule(-2, 4, -6);
        LinearCombination nz3 = new LinearCombination(1, -2, 3);

        assertTrue(nz0.equals(nz0));
        assertTrue(nz1.equals(nz1));
        assertTrue(nz2.equals(nz2));
        assertTrue(nz3.equals(nz3));

        assertFalse(nz0.equals(null));
        assertFalse(nz0.equals(nz1));
        assertTrue(nz1.equals(nz2));
        assertFalse(nz1.equals(nz3));
    }

    public void testFindDisallowed() {
        SumIsNotZeroRule nz;

        nz = new SumIsNotZeroRule(2, -3, 0);
        assertEquals(15,   nz.findDisallowedA(10, 11).intValue());
        assertEquals(null, nz.findDisallowedA(11, 10));
        assertEquals(8,    nz.findDisallowedB(12, 11).intValue());
        assertEquals(null, nz.findDisallowedB(11, 12));
        assertEquals(null, nz.findDisallowedC(99, 99));

        nz = new SumIsNotZeroRule(-1, 1, 1);
        assertEquals(30,  nz.findDisallowedA(13, 17).intValue());
        assertEquals(4,   nz.findDisallowedB(17, 13).intValue());
        assertEquals(-4,  nz.findDisallowedC(13, 17).intValue());
    }

    public void testJavaAssertionsEnabled() {
        // Check that Java language assertions are enabled.
        boolean caught;
        try {
            assert(false);
            caught = false;
        } catch (AssertionError ass) {
            caught = true;
        }
        if (! caught) {
            fail("Please add '-ea' argument for tests." +
                    "  (Assertions seem to be disabled!)");
        }
    }

    public void testNewRuleWithoutA() {
        SumIsZeroRule sumIsA = new SumIsZeroRule(-1, 1, 1);
        SumIsZeroRule sumIsB = new SumIsZeroRule(1, -1, 1);
        SumIsZeroRule sumIsC = new SumIsZeroRule(1, 1, -1);

        // Check normal results.

        SumIsNotZeroRule base, next;

        base = new SumIsNotZeroRule(1, 0, 0);
        next = base.newRuleWithoutA(sumIsA);
        assertEquals(new SumIsNotZeroRule(0, 1, 1), next);
        next = base.newRuleWithoutA(sumIsB);
        assertEquals(new SumIsNotZeroRule(0, 1, -1), next);
        next = base.newRuleWithoutA(sumIsC);
        assertEquals(new SumIsNotZeroRule(0, 1, -1), next);

        base = new SumIsNotZeroRule(1, -1, 0);
        next = base.newRuleWithoutA(sumIsA);
        assertEquals(new SumIsNotZeroRule(0, 0, 1), next);
        next = base.newRuleWithoutA(sumIsB);
        assertEquals(new SumIsNotZeroRule(0, 0, 1), next);
        next = base.newRuleWithoutA(sumIsC);
        assertEquals(new SumIsNotZeroRule(0, 2, -1), next);

        base = new SumIsNotZeroRule(1, 0, -1);
        next = base.newRuleWithoutA(sumIsA);
        assertEquals(new SumIsNotZeroRule(0, 1, 0), next);
        next = base.newRuleWithoutA(sumIsB);
        assertEquals(new SumIsNotZeroRule(0, -1, 2), next);
        next = base.newRuleWithoutA(sumIsC);
        assertEquals(new SumIsNotZeroRule(0, 1, 0), next);

        // Check assertion checks.

        boolean caught;

        base = new SumIsNotZeroRule(0, 1, -1);
        try {
            next = base.newRuleWithoutA(sumIsA);
            caught = false;
        } catch (AssertionError ass) {
            caught = true;
        }
        if (! caught) {
            fail("Expected to catch an assertion error when base.k_a == 0.");
        }

        base = new SumIsNotZeroRule(1, 0, 0);
        try {
            SumIsZeroRule garbage = new SumIsZeroRule(0, 1, 2);
            next = base.newRuleWithoutA(garbage);
            caught = false;
        } catch (AssertionError ass) {
            caught = true;
        }
        if (! caught) {
            fail("Expected to catch an assertion error when arg.k_a == 0.");
        }
    }
    public void testNewRuleWithoutB() {
        SumIsZeroRule sumIsA = new SumIsZeroRule(-1, 1, 1);
        SumIsZeroRule sumIsB = new SumIsZeroRule(1, -1, 1);
        SumIsZeroRule sumIsC = new SumIsZeroRule(1, 1, -1);

        // Check normal results.

        SumIsNotZeroRule base, next;

        base = new SumIsNotZeroRule(0, 1, 0);
        next = base.newRuleWithoutB(sumIsA);
        assertEquals(new SumIsNotZeroRule(1, 0, -1), next);
        next = base.newRuleWithoutB(sumIsB);
        assertEquals(new SumIsNotZeroRule(1, 0, 1), next);
        next = base.newRuleWithoutB(sumIsC);
        assertEquals(new SumIsNotZeroRule(1, 0, -1), next);

        base = new SumIsNotZeroRule(1, -1, 0);
        next = base.newRuleWithoutB(sumIsA);
        assertEquals(new SumIsNotZeroRule(0, 0, 1), next);
        next = base.newRuleWithoutB(sumIsB);
        assertEquals(new SumIsNotZeroRule(0, 0, 1), next);
        next = base.newRuleWithoutB(sumIsC);
        assertEquals(new SumIsNotZeroRule(2, 0, -1), next);

        base = new SumIsNotZeroRule(0, 1, -1);
        next = base.newRuleWithoutB(sumIsA);
        assertEquals(new SumIsNotZeroRule(-1, 0, 2), next);
        next = base.newRuleWithoutB(sumIsB);
        assertEquals(new SumIsNotZeroRule(1, 0, 0), next);
        next = base.newRuleWithoutB(sumIsC);
        assertEquals(new SumIsNotZeroRule(1, 0, 0), next);

        // Check assertion checks.

        boolean caught;

        base = new SumIsNotZeroRule(1, 0, -1);
        try {
            next = base.newRuleWithoutB(sumIsA);
            caught = false;
        } catch (AssertionError ass) {
            caught = true;
        }
        if (! caught) {
            fail("Expected to catch an assertion error when base.k_b == 0.");
        }

        base = new SumIsNotZeroRule(0, 1, 0);
        try {
            SumIsZeroRule garbage = new SumIsZeroRule(1, 0, 2);
            next = base.newRuleWithoutB(garbage);
            caught = false;
        } catch (AssertionError ass) {
            caught = true;
        }
        if (! caught) {
            fail("Expected to catch an assertion error when arg.k_b == 0.");
        }
    }
    public void testNewRuleWithoutC() {
        SumIsZeroRule sumIsA = new SumIsZeroRule(-1, 1, 1);
        SumIsZeroRule sumIsB = new SumIsZeroRule(1, -1, 1);
        SumIsZeroRule sumIsC = new SumIsZeroRule(1, 1, -1);

        // Check normal results.

        SumIsNotZeroRule base, next;

        base = new SumIsNotZeroRule(0, 0, 1);
        next = base.newRuleWithoutC(sumIsA);
        assertEquals(new SumIsNotZeroRule(1, -1, 0), next);
        next = base.newRuleWithoutC(sumIsB);
        assertEquals(new SumIsNotZeroRule(1, -1, 0), next);
        next = base.newRuleWithoutC(sumIsC);
        assertEquals(new SumIsNotZeroRule(1, 1, 0), next);

        base = new SumIsNotZeroRule(1, 0, -1);
        next = base.newRuleWithoutC(sumIsA);
        assertEquals(new SumIsNotZeroRule(0, 1, 0), next);
        next = base.newRuleWithoutC(sumIsB);
        assertEquals(new SumIsNotZeroRule(2, -1, 0), next);
        next = base.newRuleWithoutC(sumIsC);
        assertEquals(new SumIsNotZeroRule(0, 1, 0), next);

        base = new SumIsNotZeroRule(0, 1, -1);
        next = base.newRuleWithoutC(sumIsA);
        assertEquals(new SumIsNotZeroRule(-1, 2, 0), next);
        next = base.newRuleWithoutC(sumIsB);
        assertEquals(new SumIsNotZeroRule(1, 0, 0), next);
        next = base.newRuleWithoutC(sumIsC);
        assertEquals(new SumIsNotZeroRule(1, 0, 0), next);

        // Check assertion checks.

        boolean caught;

        base = new SumIsNotZeroRule(1, -1, 0);
        try {
            next = base.newRuleWithoutC(sumIsA);
            caught = false;
        } catch (AssertionError ass) {
            caught = true;
        }
        if (! caught) {
            fail("Expected to catch an assertion error when base.k_c == 0.");
        }

        base = new SumIsNotZeroRule(0, 0, 1);
        try {
            SumIsZeroRule garbage = new SumIsZeroRule(2, 1, 0);
            next = base.newRuleWithoutC(garbage);
            caught = false;
        } catch (AssertionError ass) {
            caught = true;
        }
        if (! caught) {
            fail("Expected to catch an assertion error when arg.k_c == 0.");
        }
    }
}
