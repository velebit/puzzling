/**
 * Copyright (c) 2007, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd2;

/**
 * @author bert
 */
public class SumIsZeroRule extends SumBasedRule implements Rule {

    public SumIsZeroRule(int Ka, int Kb, int Kc, Rationale rat) {
        super(Ka, Kb, Kc, rat);
    }

    public SumIsZeroRule(int Ka, int Kb, int Kc) {
        this(Ka, Kb, Kc, null);
    }

    // -----------------------------------------------------------------------

    @Override
    public Rule invert() {
        return new SumIsNotZeroRule(k_a, k_b, k_c);
    }

    private SumIsZeroRule makeCopy() {
        //XXX handle rationale?
        return new SumIsZeroRule(k_a, k_b, k_c);
    }

    // -----------------------------------------------------------------------

    public Integer findRequiredA(int b, int c) {
        return solutionValueForA(b, c);
    }

    public Integer findRequiredB(int a, int c) {
        return solutionValueForB(a, c);
    }

    public Integer findRequiredC(int a, int b) {
        return solutionValueForC(a, b);
    }

    // -----------------------------------------------------------------------

    public SumIsZeroRule newRuleWithoutA(SumIsZeroRule whichSumRule) {
        SumIsZeroRule res = makeCopy();
        res.eliminateA(whichSumRule);
        return res;
    }

    public SumIsZeroRule newRuleWithoutB(SumIsZeroRule whichSumRule) {
        SumIsZeroRule res = makeCopy();
        res.eliminateB(whichSumRule);
        return res;
    }

    public SumIsZeroRule newRuleWithoutC(SumIsZeroRule whichSumRule) {
        SumIsZeroRule res = makeCopy();
        res.eliminateC(whichSumRule);
        return res;
    }

    // -- implement Rule: ----------------------------------------------------

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.Rule#isSatisfied(int, int, int)
     */
    public boolean isSatisfied(int a, int b, int c) {
        return (value(a, b, c) == 0);
    }

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.Rule#isAlwaysSatisfied()
     */
    public boolean isAlwaysSatisfied() {
        return valueIsAlwaysZero();
    }

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.Rule#isNeverSatisfied()
     */
    public boolean isNeverSatisfied() {
        return valueIsAlwaysPositive() || valueIsAlwaysNegative();
    }

    // -- from SumBasedRule: -----------------------------------------

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.SumBasedRule#stringifySeparator()
     */
    @Override
    protected String stringifySeparator() {
        return " == ";
    }
}
