/**
 * Copyright (c) 2007, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd2;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

/**
 * Handling for a person with a hat.  Must be subclassed to implement the
 * differences between A, B and C.
 * 
 * @author bert
 */
public abstract class Person {

    private final static Collection<SumIsZeroRule> waysToAdd =
        Collections.unmodifiableList(Arrays.asList(
                new SumIsZeroRule(-1, 1, 1),
                new SumIsZeroRule(1, -1, 1),
                new SumIsZeroRule(1, 1, -1)));

    public static Collection<SumIsZeroRule> getWaysToAdd() {
        return waysToAdd;
    }

    public abstract String getId();

    public abstract boolean appliesToMe(SumIsNotZeroRule sourceRule);

    public abstract SumIsNotZeroRule newRuleWithoutMe(
            SumIsNotZeroRule sourceRule, SumIsZeroRule sumRule);

    protected abstract Integer findRequired(SumIsZeroRule rule,
            int a, int b, int c);

    protected abstract Integer findDisallowed(SumIsNotZeroRule rule,
            int a, int b, int c);

    public abstract int   getMyNumber(int a, int b, int c);
    public abstract int[] getOtherNumbers(int a, int b, int c);
    public abstract int[] substituteMyNumber(int newVal, int a, int b, int c);

    protected void debug(String msg) {
        ThreeHatPeople.debug(msg);
    }

    public Collection<SumIsNotZeroRule> newRulesAfterNoGuess(int round,
            Collection<SumIsNotZeroRule> oldRules) {
        Turn now = new Turn(round, getId());

        List<SumIsNotZeroRule> rules = new ArrayList<SumIsNotZeroRule>();
        for (SumIsNotZeroRule sourceRule : oldRules) {
            if (appliesToMe(sourceRule)) {
                for (SumIsZeroRule sumRule : waysToAdd) {

                    SumIsNotZeroRule newRule =
                        newRuleWithoutMe(sourceRule, sumRule);
                    assert(! newRule.isNeverSatisfied());

                    if (! newRule.isAlwaysSatisfied() && 
                            ! oldRules.contains(newRule) &&
                            ! rules.contains(newRule)) {
                        newRule.setRationale(
                                new ChainNoRationale(sourceRule, this, now));
                        rules.add(newRule);
                    }
                }
            }
        }
        return rules;
    }

    public boolean canGuess(int a, int b, int c,
            Collection<SumIsNotZeroRule> currentRules) {
        Integer myGuess = null;
    
        int[] guesses = possibleGuesses(a, b, c);
        assert(guesses.length == 3);
    
        for (SumIsNotZeroRule rule : currentRules) {
            Integer antiGuess = findDisallowed(rule, a, b, c);
            if (antiGuess == null) {
                // No anti-guess from this rule, keep going.
            } else if (antiGuess == guesses[2]) {
                assert(antiGuess > 0);
                if (myGuess == null) {
                    debug("Guess can't be #2 (" + antiGuess + "), " +
                            "must be #1 (" + guesses[1] + ").");
                    myGuess = guesses[1];
                } else {
                    assert(myGuess == guesses[1]);
                }
            } else if (antiGuess == guesses[1]) {
                assert(antiGuess >= 0);
                if (myGuess == null) {
                    debug("Guess can't be #1 (" + antiGuess + "), " +
                            "must be #2 (" + guesses[2] + ").");
                    myGuess = guesses[2];
                } else {
                    assert(myGuess == guesses[2]);
                }
            } else if (antiGuess == guesses[0]) {
                assert(antiGuess < 0);
                debug("Guess can't be #0 (" + antiGuess + "), " +
                        "but that doesn't change anything.");
            }
        }
        if (myGuess == null) {
            return false;
        } else {
            assert(myGuess == getMyNumber(a, b, c));
            return true;
        }
    }

    protected int[] possibleGuesses(int a, int b, int c) {
        int[] guesses = new int[waysToAdd.size()];
        int gi = 0;
        for (SumIsZeroRule sumRule : waysToAdd) {
            guesses[gi] = findRequired(sumRule, a, b, c);
            ++gi;
        }
        Arrays.sort(guesses);
        return guesses;
    }

    /**
     * Specific handling for person "A".
     * 
     * @author bert
     */
    public static class A extends Person {

        @Override
        public String getId() {
            return "A";
        }

        @Override
        public boolean appliesToMe(SumIsNotZeroRule sourceRule) {
            return sourceRule.getKa() != 0;
        }

        @Override
        public SumIsNotZeroRule newRuleWithoutMe(SumIsNotZeroRule sourceRule,
                SumIsZeroRule sumRule) {
            return sourceRule.newRuleWithoutA(sumRule);
        }

        @Override
        protected Integer findDisallowed(SumIsNotZeroRule rule,
                int aDummy, int b, int c) {
            return rule.findDisallowedA(b, c);
        }

        @Override
        protected Integer findRequired(SumIsZeroRule rule,
                int aDummy, int b, int c) {
            return rule.findRequiredA(b, c);
        }

        @Override
        public int getMyNumber(int a, int bDummy, int cDummy) {
            return a;
        }

        @Override
        public int[] getOtherNumbers(int aDummy, int b, int c) {
            int[] other = { b, c };
            return other;
        }

        @Override
        public int[] substituteMyNumber(int newVal, int aDummy, int b, int c) {
            int[] subst = { newVal, b, c };
            return subst;
        }
    }

    /**
     * Specific handling for person "B".
     * 
     * @author bert
     */
    public static class B extends Person {

        @Override
        public String getId() {
            return "B";
        }

        @Override
        public boolean appliesToMe(SumIsNotZeroRule sourceRule) {
            return sourceRule.getKb() != 0;
        }

        @Override
        public SumIsNotZeroRule newRuleWithoutMe(SumIsNotZeroRule sourceRule,
                SumIsZeroRule sumRule) {
            return sourceRule.newRuleWithoutB(sumRule);
        }

        @Override
        protected Integer findDisallowed(SumIsNotZeroRule rule,
                int a, int bDummy, int c) {
            return rule.findDisallowedB(a, c);
        }

        @Override
        protected Integer findRequired(SumIsZeroRule rule,
                int a, int bDummy, int c) {
            return rule.findRequiredB(a, c);
        }

        @Override
        public int getMyNumber(int aDummy, int b, int cDummy) {
            return b;
        }

        @Override
        public int[] getOtherNumbers(int a, int bDummy, int c) {
            int[] other = { a, c };
            return other;
        }

        @Override
        public int[] substituteMyNumber(int newVal, int a, int bDummy, int c) {
            int[] subst = { a, newVal, c };
            return subst;
        }
    }

    /**
     * Specific handling for person "C".
     * 
     * @author bert
     */
    public static class C extends Person {

        @Override
        public String getId() {
            return "C";
        }

        @Override
        public boolean appliesToMe(SumIsNotZeroRule sourceRule) {
            return sourceRule.getKc() != 0;
        }

        @Override
        public SumIsNotZeroRule newRuleWithoutMe(SumIsNotZeroRule sourceRule,
                SumIsZeroRule sumRule) {
            return sourceRule.newRuleWithoutC(sumRule);
        }

        @Override
        protected Integer findDisallowed(SumIsNotZeroRule rule,
                int a, int b, int cDummy) {
            return rule.findDisallowedC(a, b);
        }

        @Override
        protected Integer findRequired(SumIsZeroRule rule,
                int a, int b, int cDummy) {
            return rule.findRequiredC(a, b);
        }

        @Override
        public int getMyNumber(int aDummy, int bDummy, int c) {
            return c;
        }

        @Override
        public int[] getOtherNumbers(int a, int b, int cDummy) {
            int[] other = { a, b };
            return other;
        }

        @Override
        public int[] substituteMyNumber(int newVal, int a, int b, int cDummy) {
            int[] subst = { a, b, newVal };
            return subst;
        }
    }

}