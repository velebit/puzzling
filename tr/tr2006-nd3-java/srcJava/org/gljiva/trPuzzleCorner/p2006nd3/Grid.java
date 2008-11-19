package org.gljiva.trPuzzleCorner.p2006nd3;


import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.Collection;
import java.util.Collections;

/**
 * A crossword-like grid of 3-digit numbers, as specified in problem.
 * 
 * @author bert
 */
public class Grid
implements Cloneable {

    public static final String STRING_LSEP = "\n";
    public static final String STRING_CTR  = " ";

    protected Num3  leftV  = null;
    protected Num3  leftH  = null;
    protected Rect3 leftR  = new Rect3();
    protected Num3  cntrH  = null;
    protected Rect3 rightR = new Rect3();
    protected Num3  rightH = null;
    protected Num3  rightV = null;
    
    /************************************************************************/

    protected static final int SLOT_LV  =  0;
    protected static final int SLOT_LH  =  1;
    protected static final int SLOT_LRL =  2;
    protected static final int SLOT_LRT =  3;
    protected static final int SLOT_LRB =  4;
    protected static final int SLOT_LRR =  5;
    protected static final int SLOT_C   =  6;
    protected static final int SLOT_RRL =  7;
    protected static final int SLOT_RRT =  8;
    protected static final int SLOT_RRB =  9;
    protected static final int SLOT_RRR = 10;
    protected static final int SLOT_RH  = 11;
    protected static final int SLOT_RV  = 12;

    protected static final int N_SLOTS  = 13;

    /************************************************************************/

    /**
     * Default constructor isn't needed for now.
     */
    //public Grid() {}    

    /* (non-Javadoc)
     * @see java.lang.Object#clone()
     */
    @Override
    public Grid clone() {
        try {
            Grid g2 = (Grid)super.clone();
            g2.leftR  = leftR.clone();
            g2.rightR = rightR.clone();
            return g2;
        } catch (CloneNotSupportedException cns) {
            throw new RuntimeException(cns);
        }
    }

    /************************************************************************/

    public static String[] toStrings(Grid grid) {
        // Broken: may not show all available data for partially empty grids.
        final String csep  = STRING_CTR;

        Num3  leftV   = (grid == null ? null : grid.leftV);
        Num3  leftH   = (grid == null ? null : grid.leftH);
        Rect3 leftR   = (grid == null ? null : grid.leftR);
        Num3  cntrH   = (grid == null ? null : grid.cntrH);
        Rect3 rightR  = (grid == null ? null : grid.rightR);
        Num3  rightH  = (grid == null ? null : grid.rightH);
        Num3  rightV  = (grid == null ? null : grid.rightV);
        
        String[] leftRLines  = Rect3.toStrings(leftR);
        String[] rightRLines = Rect3.toStrings(rightR);

        String line1 = Num3.d1(leftV) + csep + leftRLines[0] + csep
            + rightRLines[0] + csep + Num3.d1(rightV);
        String line2 = Num3.d2(leftV) + Num3.d2(leftH) + leftRLines[1]
            + Num3.d2(cntrH) + rightRLines[1] + Num3.d2(rightH) + Num3.d2(rightV);
        String line3 = Num3.d3(leftV) + csep + leftRLines[2] + csep
            + rightRLines[2] + csep + Num3.d3(rightV);
        String lines[] = { line1, line2, line3 };
        return lines;
    }
    
    public static String toString(Grid grid) {
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
        if (leftV == null || leftH == null || leftR == null || cntrH == null
                || rightR == null || rightH == null || rightV == null)
            return false;
        if (! leftR.isFilled() || ! rightR.isFilled())
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
        list.addAll(leftR.getNum3List());
        if (cntrH != null)
            list.add(cntrH);
        list.addAll(rightR.getNum3List());
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
        return (areNum3sConsistent()
                && leftR.isConsistent() && rightR.isConsistent());
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
        if (leftH != null && leftR != null && leftR.left != null
                && leftH.d3 != leftR.left.d2)
            return false;
        if (leftR != null && leftR.right != null && cntrH != null
                && leftR.right.d2 != cntrH.d1)
            return false;
        if (cntrH != null && rightR != null && rightR.left != null
                && cntrH.d3 != rightR.left.d2)
            return false;
        if (rightR != null && rightR.right != null && rightH != null
                && rightR.right.d2 != rightH.d1)
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
        case SLOT_LRL:    leftR.left   = val;  break;
        case SLOT_LRT:    leftR.top    = val;  break;
        case SLOT_LRB:    leftR.bot    = val;  break;
        case SLOT_LRR:    leftR.right  = val;  break;
        case SLOT_C:      cntrH = val;         break;
        case SLOT_RRL:    rightR.left  = val;  break;
        case SLOT_RRT:    rightR.top   = val;  break;
        case SLOT_RRB:    rightR.bot   = val;  break;
        case SLOT_RRR:    rightR.right = val;  break;
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
        case SLOT_LRL:    return leftR.left;
        case SLOT_LRT:    return leftR.top;
        case SLOT_LRB:    return leftR.bot;
        case SLOT_LRR:    return leftR.right;
        case SLOT_C:      return cntrH;
        case SLOT_RRL:    return rightR.left;
        case SLOT_RRT:    return rightR.top;
        case SLOT_RRB:    return rightR.bot;
        case SLOT_RRR:    return rightR.right;
        case SLOT_RH:     return rightH;
        case SLOT_RV:     return rightV;
        default:
            assert(false);
        }
        return null;
    }

    /************************************************************************/

    static private Set<Grid> fillInSlot(int slot, Set<Grid> prev,
            Collection<Num3> num3s, Predicate checkValid) {

        Set<Grid> next = new HashSet<Grid>();
        for (Grid gprev : prev) {
            for (Num3 n : num3s) {
                Grid gnext = gprev.clone();
                gnext.setSlot(slot, n);
                if (! checkValid.check(gnext))
                    continue;
                            
                next.add(gnext);
            }
        }
        return next;
    }
    
    @SuppressWarnings("unused")
    static private Set<Grid> fillInNextSlot(Set<Grid> prev,
            Collection<Num3> num3s, Predicate checkValid) {

        if (prev.size() == 0)
            return null;

        Grid pfirst = prev.iterator().next();
        assert(pfirst != null);

        for (int i = 0;  i < N_SLOTS;  ++i) {
            if (pfirst.getSlot(i) != null)
                continue;            
            return fillInSlot(i, prev, num3s, checkValid);
        }

        return null;
    }
    
    @SuppressWarnings("unused")
    static private Set<Grid> fillInSmallestSlot(Set<Grid> prev,
            Collection<Num3> num3s, Predicate checkValid) {

        if (prev.size() == 0)
            return null;

        Grid pfirst = prev.iterator().next();
        assert(pfirst != null);

        Set<Grid> min = null;
        for (int i = 0;  i < N_SLOTS;  ++i) {
            if (pfirst.getSlot(i) != null)
                continue;            
            Set<Grid> next = fillInSlot(i, prev, num3s, checkValid);
            if ((min == null) || (min.size() > next.size()))
                min = next;
        }
        if (min != null)
            System.out.println("  sz " + min.size());
        return min;
    }
    
    static public Set<Grid> constructGrids(Collection<Num3> num3s,
            Predicate checkValid) {

        Grid top = new Grid();
        assert(checkValid.check(top));

        Set<Grid> wset = Collections.singleton(top);
        while (true) {
            Set<Grid> wnext = fillInNextSlot(wset, num3s, checkValid);
            if (wnext != null) {
                wset = wnext;
            } else {
                // Done!  Result is in "wset"  
                break;
            }
        }

        return wset;
    }

    static public Set<Grid> constructPossibleGrids(Collection<Num3> pool) {
        return constructGrids(pool, new IsConsistent());
    }

    static public Set<Grid> constructUniqueGrids(Collection<Num3> pool) {
        return constructGrids(pool, new IsConsistentAndUnique());
    }

    /************************************************************************/

    static public long countGrids(Collection<Num3> num3s,
            Predicate checkValid) {

        Grid top = new Grid();
        assert(checkValid.check(top));
        return countGridsRecurse(Collections.singleton(top),
                num3s, checkValid);
    }

    static private long countGridsRecurse(Set<Grid> wset,
            Collection<Num3> num3s,
            Predicate checkValid) {
        
        while (true) {
            Set<Grid> wnext = fillInNextSlot(wset, num3s, checkValid);
            if (wnext == null) {
                return wset.size();
            }
            
            if (wnext.size() > 1000) {
                // break up and add...
                long sum = 0;
                for (Grid g : wnext) {
                    sum += countGridsRecurse(Collections.singleton(g),
                            num3s, checkValid);
                }
                return sum;
            }
            
            // keep looping
            wset = wnext;
        }
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
        public boolean check(Grid g);
    }
    
    public static class IsConsistent implements Predicate {
        /* (non-Javadoc)
         * @see org.gljiva.trPuzzleCorner.p2006nd3.Grid.Predicate#check(org.gljiva.trPuzzleCorner.p2006nd3.Grid)
         */
        public boolean check(Grid g) {
            return g.isConsistent();
        }
    }
    
    public static class IsConsistentAndUnique implements Predicate {
        /* (non-Javadoc)
         * @see org.gljiva.trPuzzleCorner.p2006nd3.Grid.Predicate#check(org.gljiva.trPuzzleCorner.p2006nd3.Grid)
         */
        public boolean check(Grid g) {
            return g.isConsistent() && g.areNum3Unique();
        }
    }
}
