/**
 * Copyright (c) 2007, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd2;

/**
 * @author bert
 */
public class SumIsNotZeroRule extends SumBasedRule implements Rule {

    public SumIsNotZeroRule(int Ka, int Kb, int Kc, Rationale rat) {
        super(Ka, Kb, Kc, rat);
    }

    public SumIsNotZeroRule(int Ka, int Kb, int Kc) {
        this(Ka, Kb, Kc, null);
    }

    // -----------------------------------------------------------------------

    @Override
    public SumIsZeroRule invert() {
        return new SumIsZeroRule(k_a, k_b, k_c);
    }

    private SumIsNotZeroRule makeCopy() {
        //XXX handle rationale?
        return new SumIsNotZeroRule(k_a, k_b, k_c);
    }

    // -----------------------------------------------------------------------

    public Integer findDisallowedA(int b, int c) {
        return solutionValueForA(b, c);
    }

    public Integer findDisallowedB(int a, int c) {
        return solutionValueForB(a, c);
    }

    public Integer findDisallowedC(int a, int b) {
        return solutionValueForC(a, b);
    }

    // -----------------------------------------------------------------------

    public SumIsNotZeroRule newRuleWithoutA(SumIsZeroRule whichSumRule) {
        SumIsNotZeroRule res = makeCopy();
        res.eliminateA(whichSumRule);
        return res;
    }

    public SumIsNotZeroRule newRuleWithoutB(SumIsZeroRule whichSumRule) {
        SumIsNotZeroRule res = makeCopy();
        res.eliminateB(whichSumRule);
        return res;
    }

    public SumIsNotZeroRule newRuleWithoutC(SumIsZeroRule whichSumRule) {
        SumIsNotZeroRule res = makeCopy();
        res.eliminateC(whichSumRule);
        return res;
    }

    // -- implement Rule: ----------------------------------------------------

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.Rule#isSatisfied(int, int, int)
     */
    public boolean isSatisfied(int a, int b, int c) {
        return (value(a, b, c) != 0);
    }

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.Rule#isAlwaysSatisfied()
     */
    public boolean isAlwaysSatisfied() {
        return valueIsAlwaysPositive() || valueIsAlwaysNegative();
    }

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.Rule#isNeverSatisfied()
     */
    public boolean isNeverSatisfied() {
        return valueIsAlwaysZero();
    }

    // -- from SumBasedRule: -----------------------------------------

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.SumBasedRule#stringifySeparator()
     */
    @Override
    protected String stringifySeparator() {
        return " != ";
    }
}
