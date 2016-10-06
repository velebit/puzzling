package org.gljiva.trPuzzleCorner.p2006nd3;

import java.util.Collections;
import java.util.List;
import java.util.ArrayList;

/**
 * Sets of 3-digit numbers.
 * @author bert
 */
interface Num3Pool {
    /**
     * All 3-digit squares.
     */
    @SuppressWarnings("serial")
    public static final List<Num3> SQUARES =
        Collections.unmodifiableList(new ListImpl() {
            @Override
            public void init() {
                for (int i = 10;  i <= 31;  ++i) {
                    add(new Num3(i*i));
                }  
            }
        });

    /**
     * All 3-digit squares, plus their backwards counterparts (e.g. 144, 441).
     */
    @SuppressWarnings("serial")
    public static final List<Num3> SQUARES_BIDI =
        Collections.unmodifiableList(new ListImpl() {
            @Override
            public void init() {
                addAll(Num3Pool.SQUARES);
                for (Num3 num : Num3Pool.SQUARES) {
                    Num3 rev = num.flipped();
                    // skip reverse squares that are also squares!
                    if (! Util.isSquare(rev.val) || (rev.val < 100)) {
                        add(rev);
                    }
                }
            }
        });
   
    @SuppressWarnings("serial")
    public static abstract class ListImpl extends ArrayList<Num3>
    {
        public ListImpl() {
            super();
            init();
        }
        public abstract void init();
    }
}