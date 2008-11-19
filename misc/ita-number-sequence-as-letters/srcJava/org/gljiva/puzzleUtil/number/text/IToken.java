package org.gljiva.puzzleUtil.number.text;

import java.util.Set;


public interface IToken {

    /* (non-Javadoc)
     * @see java.lang.Enum#toString()
     */
    public abstract String toString();

    /**
     * @return
     */
    public abstract int getStringLength();

    /**
     * @return
     */
    public abstract int getValue();

    /**
     * @return
     */
    public abstract Set<Flag> getFlags();

    /**
     * @param f
     * @return
     */
    public abstract boolean hasFlag(Flag f);

    /**
     * Flags associated with various tokens.
     */
    public static enum Flag {
        ALONE_ONLY, MULTIPLIER, END_GROUP, GLUE;
    }
}