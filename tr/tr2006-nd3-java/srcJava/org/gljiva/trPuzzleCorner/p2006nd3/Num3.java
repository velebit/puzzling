package org.gljiva.trPuzzleCorner.p2006nd3;

import java.lang.Cloneable;
import java.lang.CloneNotSupportedException;
import java.lang.Comparable;
import java.lang.RuntimeException;
import java.util.Comparator;

/**
 * Class to implement 3-digit numbers, with digit manipulation.
 * 
 * Note: this class has a natural ordering that is inconsistent with equals.
 * If you want an ordering consistent with equals, you can use the
 * {@code COMPARE_CURRENT} comparator.
 * 
 * @author bert
 */
public class Num3
implements Cloneable, Comparable<Num3> {
    public int     val;
    public int     val0;
    public int     d1;
    public int     d2;
    public int     d3;

    /************************************************************************/
    
    /**
     * A comparator that compares the "unflipped" values of the numbers, so
     * that, for example,
     *   COMPARE_UNFLIPPED.compare((new Num3(123)).flipped(), new Num3(123))
     * returns 0.
     * 
     * Note: this comparator is inconsistent with equals.
     */
    public static Comparator<Num3> COMPARE_UNFLIPPED = new Comparator<Num3>() {
        /* (non-Javadoc)
         * @see java.util.Comparator#compare(java.lang.Object, java.lang.Object)
         */
        public int compare(Num3 o1, Num3 o2) {
            return o1.val0 - o2.val0;
        }
    };
    
    /**
     * A comparator that compares the "current" values of the numbers, so
     * that, for example,
     *   COMPARE_CURRENT.compare((new Num3(123)).flipped(), new Num3(321))
     * returns 0.
     * 
     * Note: this comparator is <b>consistent</b> with equals.
     */
    public static Comparator<Num3> COMPARE_CURRENT = new Comparator<Num3>() {
        /* (non-Javadoc)
         * @see java.util.Comparator#compare(java.lang.Object, java.lang.Object)
         */
        public int compare(Num3 o1, Num3 o2) {
            return o1.val - o2.val;
        }
    };
    
    /**
     * The default comparator.  This is the same as COMPARE_UNFLIPPED.
     */
    public static Comparator<Num3> COMPARE_DEFAULT = COMPARE_UNFLIPPED;
    
    /************************************************************************/

    /**
     * @param num The value of the number
     */
    public Num3(int num) {
        val = num;
        assert(val >= 100);
        assert(val < 1000);
        val0 = val;
        d1 = (byte)((val / 100) % 10);
        d2 = (byte)((val / 10) % 10);
        d3 = (byte)(val % 10);
    }

    /* (non-Javadoc)
     * @see java.lang.Object#clone()
     */
    @Override
    public Num3 clone() {
        try {
            return (Num3)super.clone();
        } catch (CloneNotSupportedException cns) {
            throw new RuntimeException(cns);
        }
    }

    /************************************************************************/
    
    /* (non-Javadoc)
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        return String.format("%d%d%d", d1, d2, d3); 
    }

    /* (non-Javadoc)
     * @see java.lang.Comparable#compareTo(java.lang.Object)
     */
    public int compareTo(Num3 o) {
        return COMPARE_DEFAULT.compare(this, o);
    }
    
    /* (non-Javadoc)
     * @see java.lang.Object#equals(java.lang.Object)
     */
    @Override
    public boolean equals(Object obj) {
        return ( obj instanceof Num3 ? val == ((Num3)obj).val : false );
    }

    /* (non-Javadoc)
     * @see java.lang.Object#hashCode()
     */
    @Override
    public int hashCode() {
        return val;
    }

    /************************************************************************/

    /**
     * Format the first digit from a number, even if the number is null.
     * @param num  A number, or null.
     * @return     The string representation of the first digit, or "?".
     */
    public static String d1(Num3 num) {
        return (num == null ? "?" : String.format("%d", num.d1));
    }

    /**
     * Format the second digit from a number, even if the number is null.
     * @param num  A number, or null.
     * @return     The string representation of the second digit, or "?".
     */
    public static String d2(Num3 num) {
        return (num == null ? "?" : String.format("%d", num.d2));
    }

    /**
     * Format the third digit from a number, even if the number is null.
     * @param num  A number, or null.
     * @return     The string representation of the third digit, or "?".
     */
    public static String d3(Num3 num) {
        return (num == null ? "?" : String.format("%d", num.d3));
    }
    
    /**
     * Format the 3-digit number, even if the number is null.
     * @param num  A number, or null.
     * @return     The string representation of the number, or "???".
     */
    public static String toString(Num3 num) {
        return d1(num) + d2(num) + d3(num);
    }
    
    /************************************************************************/

    public boolean isFlipped() {
        return val != val0;
    }
    
    /**
     * Reverse the digits of the number, modifying the object.
     * @return The object that was flipped (useful for chaining methods).
     */
    public Num3 flip() {
        boolean wasFlipped = isFlipped();
        assert( val == 100*d1 + 10*d2 + d3 );
        int tmp = d3;
        d3 = d1;
        d1 = tmp;
        val = 100*d1 + 10*d2 + d3;
        assert( !(wasFlipped && isFlipped()) );
        return this;
    }
    
    /**
     * Reverse the digits of the number, returning a new object.
     * @return The newly created flipped object.
     */
    public Num3 flipped() {
        return this.clone().flip();
    }

    /************************************************************************/
    
    public String describeSquare() {
        String prefix = (isFlipped() ? "~ " : "  ");
        if (Util.isSquare(val0)) {
            return String.format("%s%03d = %2d^2", prefix, val0,
                    Util.sqrt(val0)); 
        }
        return prefix + toString();
    }

}
