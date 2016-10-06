package org.gljiva.trPuzzleCorner.p2006nd3;

import java.lang.Cloneable;
import java.lang.CloneNotSupportedException;
import java.lang.RuntimeException;
import java.lang.String;
import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.HashSet;
import java.util.Collection;

/**
 * A 3x3 rectangular grid made up of 4 3-digit numbers (and a hole in the
 * middle).  Yes, I know that an NxN rectangle is usually called a square,
 * but in this case it would be confusing because each of the 3-digit numbers
 * is also a square (i.e. it equals x*x for some integer x).  Deal with it.
 * 
 * @author bert
 */
public class Rect3
implements Cloneable {
    public static final String STRING_LSEP = "\n";
    public static final String STRING_CTR  = " ";

    protected Num3 left  = null;
    protected Num3 top   = null;
    protected Num3 bot   = null;
    protected Num3 right = null;
    
    /************************************************************************/

    /**
     * Default constructor isn't needed for now.
     */
    //public Rect3() {}    

    /* (non-Javadoc)
     * @see java.lang.Object#clone()
     */
    @Override
    public Rect3 clone() {
        try {
            return (Rect3)super.clone();
        } catch (CloneNotSupportedException cns) {
            throw new RuntimeException(cns);
        }
    }

    /************************************************************************/
    
    public static String[] toStrings(Rect3 rect) {
        Num3 rtop   = (rect == null ? null : rect.top);
        Num3 rbot   = (rect == null ? null : rect.bot);
        Num3 rleft  = (rect == null ? null : rect.left);
        Num3 rright = (rect == null ? null : rect.right);
        String ret[] = {
                Num3.toString(rtop),
                Num3.d2(rleft) + STRING_CTR + Num3.d2(rright),
                Num3.toString(rbot)
        };
        return ret;
    }
    
    public static String toString(Rect3 rect) {
        return Util.join(STRING_LSEP, toStrings(rect));
    }

    /* (non-Javadoc)
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        return toString(this);
    }

    /************************************************************************/

    /**
     * Check if this rectangle is filled (all 3-digit numbers are present).
     * @return True if filled, false if not.
     */
    public boolean isFilled() {
        if (top == null || bot == null || left == null || right == null)
            return false;
        return true;
    }

    /**
     * Collect all Num3 objects used by this rectangle.
     * @return A list of Num3 objects, excluding any {@code null} references.
     */
    public List<Num3> getNum3List() {
        List<Num3> list = new ArrayList<Num3>();
        getNum3List(list);
        return list;
    }

    /**
     * Collect all Num3 objects used by this rectangle and add them to an
     * existing list object.
     * @param list  The list of Num3 objects.  Elements will be added using the
     *              standard {@code add} operation.
     */
    public void getNum3List(List<Num3> list) {
        if (left != null)
            list.add(left);
        if (top != null)
            list.add(top);
        if (bot != null)
            list.add(bot);
        if (right != null)
            list.add(right);
    }

    public boolean areNum3Unique() {
        return Util.areUnique(getNum3List());
    }

    /************************************************************************/
    
    /**
     * Check if this rectangle is consistent.  This means we verify that the
     * digits at each intersection between numbers are the same in both
     * of those numbers (if both are present).
     * 
     * This method is for internal use only.  Inconsistent rectangles should
     * never be returned, set or otherwise be available externally!
     * 
     * @return True if consistent, false if not.
     */
    protected boolean isConsistent() {
        if ((left != null)  && (top != null) && (top.d1 != left.d1))
            return false;
        if ((right != null) && (top != null) && (top.d3 != right.d1))
            return false;
        if ((left != null)  && (bot != null) && (bot.d1 != left.d3))
            return false;
        if ((right != null) && (bot != null) && (bot.d3 != right.d3))
            return false;
        return true;
    }

    /************************************************************************/

    static public Set<Rect3> constructPossibleRect3s(Collection<Num3> num3s) {
        Set<Rect3> set = new HashSet<Rect3>();

        for (Num3 top : num3s) {
            for (Num3 left : num3s) {
                for (Num3 right : num3s) {
                    for (Num3 bot : num3s) {
                        Rect3 rect = new Rect3();
                        rect.top   = top;
                        rect.left  = left;
                        rect.right = right;
                        rect.bot   = bot;
                        if (! rect.isConsistent())
                            continue;

                        assert(rect.isFilled());
                        set.add(rect);
                    }
                }
            }
        }
        return set;
    }
    
    static public Set<Rect3> constructUniqueRect3s(Collection<Num3> num3s) {
        Set<Rect3> set = new HashSet<Rect3>();
        for (Rect3 r : constructPossibleRect3s(num3s)) {
            if (r.areNum3Unique())
                set.add(r);
        }
        return set;
    }
}
