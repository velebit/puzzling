/**
 * Copyright (c) 2007, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd2;

import java.util.Collection;

public class ThreeHatSolutions {

    public static void main(String[] args) {
        //bruteForceGuess();
        //ruleBasedSolution();

        explainWhyAll();

    }

    // -----------------------------------------------------------------------

    protected static void bruteForceGuess() {
        int numFound = 0;
        for (int i = 1;  i < 60;  ++i) {
            if (tryAndPrintGuess(i, 60-i, 60)) {
                ++numFound;
            }
        }
        for (int i = 1;  i < 200;  ++i) {
            if (tryAndPrintGuess(i, 60+i, 60)) {
                ++numFound;                
            }
            if (tryAndPrintGuess(60+i, i, 60)) {
                ++numFound;
            }
        }
        System.out.println("total: " + numFound + " solutions");
    }

    private static boolean tryAndPrintGuess(int a, int b, int c) {
        Turn res = ThreeHatPeople.findWhoGuessed(a, b, c,
                new Turn(4, Turn.WHO_A));
        if ((res != null) && (res.round == 3) && (res.who.equals(Turn.WHO_C))) {
            System.out.println("found solution: " + a + "," + b + "," + c);
            return true;
        }
        return false;
    }

    // -----------------------------------------------------------------------

    protected static void ruleBasedSolution() {
        ThreeHatPeople obj = new ThreeHatPeople();
        obj.setTurnLimit(new Turn(3, Turn.WHO_C));
        obj.execute();
        Collection<SumIsZeroRule> guessRules = obj.getRulesForGuess();
        Collection<SumIsNotZeroRule> oldRules = obj.getRulesBeforeGuess();

        Collection<SumIsZeroRule> waysToAdd = Person.getWaysToAdd();

        int numFound = 0;
        for (SumIsZeroRule gRule : guessRules) {
            for (SumIsZeroRule wRule : waysToAdd) {
                // Find a and b (if any) that satisfy both of these rules
                SumIsZeroRule acRule = gRule.newRuleWithoutB(wRule);
                assert(! acRule.isAlwaysSatisfied());
                if (acRule.isNeverSatisfied()) {
                    continue;
                }
                Integer ao = acRule.findRequiredA(0, 60);
                if (ao == null) {
                    continue;
                }
                Integer bo = wRule.findRequiredB(ao, 60);
                assert(bo != null);

                // Verify that the new a and b don't match any *other* rules
                if (! valuesSatisfyRules(ao, bo, 60, oldRules)) {
                    continue;
                }

                boolean ok = tryAndPrintGuess(ao, bo, 60);
                assert(ok);
                ++numFound;
            }
        }
        System.out.println("total: " + numFound + " solutions");
    }

    private static boolean valuesSatisfyRules(int a, int b, int c,
            Collection<? extends Rule> rules) {
        for (Rule r : rules) {
            if (! r.isSatisfied(a, b, c)) {
                return false;
            }
        }
        return true;
    }

    // -----------------------------------------------------------------------

    protected static void explainWhyAll() {
        int[] list = { 10, 16, 18, 25 };
        for (int v : list) {
            explainWhy(v, 60-v, 60);
            explainWhy(60-v, v, 60);
        }
    }

    private static void explainWhy(int a, int b, int c) {
        ThreeHatPeople obj = new ThreeHatPeople(a, b, c);
        obj.execute();
        Collection<SumIsZeroRule> guessRules = obj.getRulesForGuess();
        //Collection<SumIsNotZeroRule> oldRules = obj.getRulesBeforeGuess();

        SumIsZeroRule relevantRule = null;
        for (SumIsZeroRule gRule : guessRules) {
            if (gRule.isSatisfied(a, b, c)) {
                relevantRule = gRule;
                break;
            }
        }
        assert(relevantRule != null);

        Rationale rat = relevantRule.getRationale();
        assert(rat != null);

        System.out.println("***** A: " + a + ", B: " + b + ", C: " + c +
                " *****");
        System.out.print(rat.explain(a, b, c, ""));
        System.out.println();
    }
}
