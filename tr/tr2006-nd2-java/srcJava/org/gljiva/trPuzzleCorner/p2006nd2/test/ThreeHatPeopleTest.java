/**
 * 
 */
package org.gljiva.trPuzzleCorner.p2006nd2.test;

import org.gljiva.trPuzzleCorner.p2006nd2.ThreeHatPeople;
import org.gljiva.trPuzzleCorner.p2006nd2.Turn;

import junit.framework.TestCase;

/**
 * @author bert
 */
public class ThreeHatPeopleTest extends TestCase {

    // -----------------------------------------------------------------------

    public void testSolve112() {
        Turn res;
        res = ThreeHatPeople.findWhoGuessed(2, 1, 1);
        assertNotNull(res);
        assertEquals(1,   res.round);
        assertEquals("A", res.who);

        res = ThreeHatPeople.findWhoGuessed(1, 2, 1);
        assertNotNull(res);
        assertEquals(1,   res.round);
        assertEquals("B", res.who);

        res = ThreeHatPeople.findWhoGuessed(1, 1, 2);
        assertNotNull(res);
        assertEquals(1,   res.round);
        assertEquals("C", res.who);
    }

    public void testSolveAssorted() {
        Turn res;
        res = ThreeHatPeople.findWhoGuessed(1, 2, 3);
        assertNotNull(res);
        assertEquals(1,   res.round);
        assertEquals("C", res.who);

        res = ThreeHatPeople.findWhoGuessed(2, 3, 1);
        assertNotNull(res);
        assertEquals(1,   res.round);
        assertEquals("B", res.who);

        res = ThreeHatPeople.findWhoGuessed(2, 3, 5);
        assertNotNull(res);
        assertEquals(1,   res.round);
        assertEquals("C", res.who);
    }
}
