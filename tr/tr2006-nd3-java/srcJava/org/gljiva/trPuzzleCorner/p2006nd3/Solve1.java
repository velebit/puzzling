/**
 * Copyright (c) 2006, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd3;

import java.util.Collection;

/**
 * Technology Review's Puzzle Corner problems:
 * solution for Nov/Dec problem 3 (2006).
 *
 * @author bert
 */
public class Solve1 {

    /**
     * @param args
     */
    public static void main(String[] args) {

        if (false) {
            System.out.println(new Rect3());
            System.out.println(new Grid());
            System.out.println();
        }

        if (false)
            Util.printList(Num3Pool.SQUARES);
        if (false)
            Util.printListSorted(Num3Pool.SQUARES);
        if (false)
            Util.printList(Num3Pool.SQUARES_BIDI);
        if (false)
            Util.printListSorted(Num3Pool.SQUARES_BIDI);
        if (true) {
            for (Num3 n : Num3Pool.SQUARES) {
                if (n.flipped().equals(n))
                    System.out.println(n);
            }        
            System.out.println();
        }
        if (true) {
            System.out.println("# SQUARES:           "
                    + Num3Pool.SQUARES.size());
            System.out.println("# SQUARES_BIDI:      "
                    + Num3Pool.SQUARES_BIDI.size());
        }
        if (false) {
            System.out.println("# Rect3s(S):         "
                    + Rect3.constructPossibleRect3s(Num3Pool.SQUARES).size());
            System.out.println("# Rect3s(SB):        "
                    + Rect3.constructPossibleRect3s(Num3Pool.SQUARES_BIDI).size());
            System.out.println("# unique Rect3s(S):  "
                    + Rect3.constructUniqueRect3s(Num3Pool.SQUARES).size());
            System.out.println("# unique Rect3s(SB): "
                    + Rect3.constructUniqueRect3s(Num3Pool.SQUARES_BIDI).size());
        }

        if (false) {
            Collection<Rect3> rect3s =
                Rect3.constructUniqueRect3s(Num3Pool.SQUARES);
            for (Rect3 r : rect3s) {
                System.out.println(r.toString());
                System.out.println();
            }
            System.out.println("total rect3s: " + rect3s.size());
        }

        if (true) {
            Collection<Grid> grids =
                Grid.constructUniqueGrids(Num3Pool.SQUARES);
            if (true && grids.size() < 10) {
                System.out.println("===========");
                for (Grid g : grids) {
                    System.out.println(g.toString());
                    if (true) {
                        System.out.println("-----------");
                        for (Num3 n : g.getNum3List()) {
                            System.out.println(n.describeSquare());
                        }
                    }
                    System.out.println("===========");
                }
            }
            System.out.println("total S,u grids:   " + grids.size());
        }
        
        if (true) {
            Collection<Grid> grids =
                Grid.constructUniqueGrids(Num3Pool.SQUARES_BIDI);
            if (true && grids.size() < 10) {
                System.out.println("===========");
                for (Grid g : grids) {
                    System.out.println(g.toString());
                    if (true) {
                        System.out.println("-----------");
                        for (Num3 n : g.getNum3List()) {
                            System.out.println(n.describeSquare());
                        }
                    }
                    System.out.println("===========");
                }
            }
            System.out.println("total SB,u grids:  " + grids.size());
        }
        
        if (true) {
            Collection<Grid> grids =
                Grid.constructPossibleGrids(Num3Pool.SQUARES);
            if (true && grids.size() < 10) {
                System.out.println("===========");
                for (Grid g : grids) {
                    System.out.println(g.toString());
                    if (true) {
                        System.out.println("-----------");
                        for (Num3 n : g.getNum3List()) {
                            System.out.println(n.describeSquare());
                        }
                    }
                    System.out.println("===========");
                }
            }
            System.out.println("total S,!u grids:  " + grids.size());
        }
        
        if (true) {
            long count = Grid.countGrids(Num3Pool.SQUARES_BIDI,
                    new Grid.IsConsistent());
            System.out.println("total SB,!u grids: " + count);
        }
        
        if (false) {
            Collection<Grid8> grid8s =
                Grid8.constructUniqueGrids(Num3Pool.SQUARES);
            if (true && grid8s.size() < 100) {
                System.out.println("=======");
                for (Grid8 g8 : grid8s) {
                    System.out.println(g8.toString());
                    if (false) {
                        System.out.println("-------");
                        for (Num3 n : g8.getNum3List()) {
                            System.out.println(n.describeSquare());
                        }
                    }
                    System.out.println("=======");
                }
            }
            System.out.println("total grid8s: " + grid8s.size());
        }
    }
}
