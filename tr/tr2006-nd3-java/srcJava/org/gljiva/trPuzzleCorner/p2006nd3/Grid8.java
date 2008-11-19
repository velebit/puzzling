package org.gljiva.trPuzzleCorner.p2006nd3;


import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.Collection;
import java.util.Collections;

/**
 * A crossword-like grid of 3-digit numbers, narrower than in the problem.
 * 
 * @author bert
 */
public class Grid8
implements Cloneable {

    public static final String STRING_LSEP = "\n";
    public static final String STRING_CTR  = " ";

    protected Num3  leftV  = null;
    protected Num3  leftH  = null;
    protected Rect3 cntrR  = new Rect3();
    protected Num3  rightH = null;
    protected Num3  rightV = null;
    
    /************************************************************************/

    protected static final int SLOT_LV  = 0;
    protected static final int SLOT_LH  = 1;
    protected static final int SLOT_CRL = 2;
    protected static final int SLOT_CRT = 3;
    protected static final int SLOT_CRB = 4;
    protected static final int SLOT_CRR = 5;
    protected static final int SLOT_RH  = 6;
    protected static final int SLOT_RV  = 7;

    protected static final int N_SLOTS  = 8;

    /************************************************************************/

    /**
     * Default constructor isn't needed for now.
     */
    //public Grid() {}    

    /* (non-Javadoc)
     * @see java.lang.Object#clone()
     */
    @Override
    public Grid8 clone() {
        try {
            Grid8 g2 = (Grid8)super.clone();
            g2.cntrR  = cntrR.clone();
            return g2;
        } catch (CloneNotSupportedException cns) {
            throw new RuntimeException(cns);
        }
    }

    /************************************************************************/

    public static String[] toStrings(Grid8 grid) {
        // Broken: may not show all available data for partially empty grids.
        final String csep  = STRING_CTR;

        Num3  leftV   = (grid == null ? null : grid.leftV);
        Num3  leftH   = (grid == null ? null : grid.leftH);
        Rect3 cntrR   = (grid == null ? null : grid.cntrR);
        Num3  rightH  = (grid == null ? null : grid.rightH);
        Num3  rightV  = (grid == null ? null : grid.rightV);
        
        String[] cntrRLines  = Rect3.toStrings(cntrR);

        String line1 = Num3.d1(leftV) + csep + cntrRLines[0] + csep
            + Num3.d1(rightV);
        String line2 = Num3.d2(leftV) + Num3.d2(leftH) + cntrRLines[1]
            + Num3.d2(rightH) + Num3.d2(rightV);
        String line3 = Num3.d3(leftV) + csep + cntrRLines[2] + csep
            + Num3.d3(rightV);
        String lines[] = { line1, line2, line3 };
        return lines;
    }
    
    public static String toString(Grid8 grid) {
        return Util.join(STRING_LSEP, toStrings(grid));
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
     * Check if this grid is filled (all 3-digit numbers are present).
     * @return True if filled, false if not.
     */
    public boolean isFilled() {
        if (leftV == null || leftH == null || cntrR == null
                || rightH == null || rightV == null)
            return false;
        if (! cntrR.isFilled())
            return false;
        return true;
    }

    /**
     * Check if the numbers in this grid are unique.
     * @return  True if unique, false if not.
     */
    public boolean areNum3Unique() {
        return Util.areUnique(getNum3List(), Num3.COMPARE_UNFLIPPED);
    }

    /************************************************************************/

    /**
     * Collect all Num3 objects used by this grid.
     * @return A list of Num3 objects, excluding any {@code null} references.
     */
    public List<Num3> getNum3List() {
        List<Num3> list = new ArrayList<Num3>();
        getNum3List(list);
        return list;
    }

    /**
     * Collect all Num3 objects used by this grid and add them to an existing
     * list object.
     * @param list  The list of Num3 objects.  Elements will be added using the
     *              standard {@code add} operation.
     */
    public void getNum3List(List<Num3> list) {
        if (leftV != null)
            list.add(leftV);
        if (leftH != null)
            list.add(leftH);
        list.addAll(cntrR.getNum3List());
        if (rightH != null)
            list.add(rightH);
        if (rightV != null)
            list.add(rightV);
    }

    /************************************************************************/

    /**
     * Check if this grid is consistent.  This means we verify that the
     * digits at each intersection between numbers are the same in both
     * of those numbers (if both are present).
     * 
     * This method is for internal use only.  Inconsistent grids should never
     * be returned, set or otherwise be available externally!
     * 
     * @return True if consistent, false if not.
     */
    protected boolean isConsistent() {
        return (areNum3sConsistent() && cntrR.isConsistent());
    }

    /**
     * Check if the Num3 objects in this grid are consistent with the rest of
     * the grid.  This means we verify that the digits at each intersection
     * between numbers are the same in both of those numbers (if both are
     * present).
     * 
     * This method checks only "local" Num3 objects, not Rect3s etc.  Use
     * {@code isConsistent()} to check overall consistency.
     * 
     * This method is for internal use only.  Inconsistent grids should never
     * be returned, set or otherwise be available externally!
     * 
     * @return True if "locally" consistent, false if not.
     */
    protected boolean areNum3sConsistent() {
        if (leftV != null && leftH != null
                && leftV.d2 != leftH.d1)
            return false;
        if (leftH != null && cntrR != null && cntrR.left != null
                && leftH.d3 != cntrR.left.d2)
            return false;
        if (cntrR != null && cntrR.right != null && rightH != null
                && cntrR.right.d2 != rightH.d1)
            return false;
        if (rightH != null && rightV != null
                && rightH.d3 != rightV.d2)
            return false;
        return true;
    }

    /************************************************************************/

    protected void setSlot(int slot, Num3 val) {
        switch (slot) {
        case SLOT_LV:     leftV = val;         break;
        case SLOT_LH:     leftH = val;         break;
        case SLOT_CRL:    cntrR.left   = val;  break;
        case SLOT_CRT:    cntrR.top    = val;  break;
        case SLOT_CRB:    cntrR.bot    = val;  break;
        case SLOT_CRR:    cntrR.right  = val;  break;
        case SLOT_RH:     rightH = val;        break;
        case SLOT_RV:     rightV = val;        break;
        default:
            assert(false);
        }
    }

    protected Num3 getSlot(int slot) {
        switch (slot) {
        case SLOT_LV:     return leftV;
        case SLOT_LH:     return leftH;
        case SLOT_CRL:    return cntrR.left;
        case SLOT_CRT:    return cntrR.top;
        case SLOT_CRB:    return cntrR.bot;
        case SLOT_CRR:    return cntrR.right;
        case SLOT_RH:     return rightH;
        case SLOT_RV:     return rightV;
        default:
            assert(false);
        }
        return null;
    }

    /************************************************************************/

    static private Set<Grid8> fillInSlot(int slot, Set<Grid8> prev,
            Collection<Num3> num3s, Predicate checkValid) {

        Set<Grid8> next = new HashSet<Grid8>();
        for (Grid8 gprev : prev) {
            for (Num3 n : num3s) {
                Grid8 gnext = gprev.clone();
                gnext.setSlot(slot, n);
                if (! checkValid.check(gnext))
                    continue;
                            
                next.add(gnext);
            }
        }
        return next;
    }
    
    static private Set<Grid8> fillInNextSlot(Set<Grid8> prev,
            Collection<Num3> num3s, Predicate checkValid) {

        if (prev.size() == 0)
            return null;

        Grid8 pfirst = prev.iterator().next();
        assert(pfirst != null);

        for (int i = 0;  i < N_SLOTS;  ++i) {
            if (pfirst.getSlot(i) != null)
                continue;            
            return fillInSlot(i, prev, num3s, checkValid);
        }

        return null;
    }
    
    static public Set<Grid8> constructGrids(Collection<Num3> num3s,
            Predicate checkValid) {

        Grid8 top = new Grid8();
        assert(checkValid.check(top));

        Set<Grid8> wset = Collections.singleton(top);
        while (true) {
            Set<Grid8> wnext = fillInNextSlot(wset, num3s, checkValid);
            if (wnext != null) {
                wset = wnext;
            } else {
                // Done!  Result is in "wset"  
                break;
            }
        }

        return wset;
    }

    static public Set<Grid8> constructPossibleGrids(Collection<Num3> pool) {
        return constructGrids(pool, new IsConsistent());
    }

    static public Set<Grid8> constructUniqueGrids(Collection<Num3> pool) {
        return constructGrids(pool, new IsConsistentAndUnique());
    }

    /************************************************************************/

    /**
     * Class providing a function that takes a Grid argument and returns
     * either true or false.
     */
    public static interface Predicate {
        /**
         * Checks a grid for some condition.
         * @param g   The grid to check.
         * @return    True or false, depending on the condition.
         */
        public boolean check(Grid8 g);
    }
    
    public static class IsConsistent implements Predicate {
        /* (non-Javadoc)
         * @see org.gljiva.trPuzzleCorner.p2006nd3.Grid.Predicate#check(org.gljiva.trPuzzleCorner.p2006nd3.Grid)
         */
        public boolean check(Grid8 g) {
            return g.isConsistent();
        }
    }
    
    public static class IsConsistentAndUnique implements Predicate {
        /* (non-Javadoc)
         * @see org.gljiva.trPuzzleCorner.p2006nd3.Grid.Predicate#check(org.gljiva.trPuzzleCorner.p2006nd3.Grid)
         */
        public boolean check(Grid8 g) {
            return g.isConsistent() && g.areNum3Unique();
        }
    }
}
