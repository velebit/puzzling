/**
 * 
 */
package org.gljiva.puzzleUtil.number.text.test;

import org.gljiva.puzzleUtil.number.text.IToken;
import org.gljiva.puzzleUtil.number.text.en.Token;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

/**
 * @author bert
 *
 */
public class EnTokenTest {

    /**
     * @throws java.lang.Exception
     */
    @Before
    public void setUp() throws Exception {
    }

    /**
     * @throws java.lang.Exception
     */
    @After
    public void tearDown() throws Exception {
    }

    /**
     * Test method for {@link org.gljiva.puzzleUtil.number.text.en.Token#toString()}.
     */
    @Test
    public void testToString() {
        assertEquals("zero", Token.ZERO.toString());
        String all = " ";
        for (IToken t : Token.values()) {
            String s = t.toString();
            assertNotNull("expected a non-null token.toString()", s);
            assertNotSame("token.toString() is empty:", "", s);
            assertEquals(s.toLowerCase(), s);
            all = all + s + " ";   // horribly inefficient =)
        }
    }
}
