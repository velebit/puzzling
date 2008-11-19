/**
 * 
 */
package org.gljiva.puzzleUtil.number.text.en;

import java.util.EnumSet;
import java.util.Set;
import java.util.Arrays;
import java.util.Collections;

import org.gljiva.puzzleUtil.number.text.IToken;


/**
 * @author bert
 * Word tokens used in writing numbers in English.
 */
public enum Token implements IToken {
    
    // Digits (0-9)
    ZERO       (0, Flag.ALONE_ONLY),
    ONE        (1),
    TWO        (2),
    THREE      (3),
    FOUR       (4),
    FIVE       (5),
    SIX        (6),
    SEVEN      (7),
    EIGHT      (8),
    NINE       (9),
    
    // Teens (well, actually, 10-19)
    TEN        (10),
    ELEVEN     (11),
    TWELVE     (12),
    THIRTEEN   (13),
    FOURTEEN   (14),
    FIFTEEN    (15),
    SIXTEEN    (16),
    SEVENTEEN  (17),
    EIGHTEEN   (18),
    NINETEEN   (19),
    
    // Decades starting with 20 (20, 30, ... 90)
    TWENTY     (20),
    THIRTY     (30),
    FOURTY     (40),
    FIFTY      (50),
    SIXTY      (60),
    SEVENTY    (70),
    EIGHTY     (80),
    NINETY     (90),
    
    // Higher multipliers
    HUNDRED    (100, Flag.MULTIPLIER),
    THOUSAND   (1000, Flag.MULTIPLIER, Flag.END_GROUP),
    MILLION    (1000000, Flag.MULTIPLIER, Flag.END_GROUP),
    BILLION    (1000000000, Flag.MULTIPLIER, Flag.END_GROUP),
    
    // Other special magical words
    AND        (0, Flag.GLUE);

    protected final int           m_value;
    protected final EnumSet<Flag> m_flags;
    protected final String        m_text;
    protected final int           m_strlen;

    /**
     * Create an enumeration element with the specified value and flags.
     * @param val
     * @param flags
     */
    private Token(int val, Flag... flags) {
        m_value = val;
        if (flags.length > 0) {
            m_flags = EnumSet.copyOf(Arrays.asList(flags));
        } else {
            // Can't use copyOf for an empty collection.
            m_flags = EnumSet.noneOf(Flag.class);
        }
        m_text   = super.toString().toLowerCase();
        m_strlen = m_text.length();
    }
    
    /* (non-Javadoc)
     * @see org.gljiva.puzzleUtil.number.text.en.IToken#toString()
     * @see java.lang.Enum#toString()
     */
    @Override
    public String toString() {
        return m_text;
    }

    /* (non-Javadoc)
     * @see org.gljiva.puzzleUtil.number.text.en.IToken#getLength()
     */
    public int getStringLength() {
        return m_strlen;
    }

    /* (non-Javadoc)
     * @see org.gljiva.puzzleUtil.number.text.en.IToken#getValue()
     */
    public int getValue() {
        return m_value;
    }

    /* (non-Javadoc)
     * @see org.gljiva.puzzleUtil.number.text.en.IToken#getFlags()
     */
    public Set<Flag> getFlags() {
        return Collections.unmodifiableSet(m_flags);
    }

    /* (non-Javadoc)
     * @see org.gljiva.puzzleUtil.number.text.en.IToken#hasFlag(org.gljiva.puzzleUtil.number.text.en.Token.Flag)
     */
    public boolean hasFlag(Flag f) {
        return m_flags.contains(f);
    }
}
