/**
 * Copyright (c) 2007, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd2;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;


/**
 * Technology Review's Puzzle Corner problems:
 * solution for Nov/Dec problem 2 (2006).
 *
 * @author bert
 */
public class ThreeHatPeople {

    // Whether we know the numbers on the three hats, and if so, what they are.
    protected boolean haveNumbers;
    protected int a;
    protected int b;
    protected int c;

    // The three people with said hats
    protected Person personA = new Person.A();
    protected Person personB = new Person.B();
    protected Person personC = new Person.C();

    // The commonly known set of "rules" based on guess history
    protected Collection<SumIsNotZeroRule> currentRules = Arrays.asList(
                new SumIsNotZeroRule(1, 0, 0, new NotZeroRationale("A")),
                new SumIsNotZeroRule(0, 1, 0, new NotZeroRationale("B")),
                new SumIsNotZeroRule(0, 0, 1, new NotZeroRationale("C")));

    // Rules as of just before the last person's turn.
    protected Collection<SumIsNotZeroRule> lastRules = null;

    // Rules added during the last person's turn.
    protected Collection<SumIsNotZeroRule> newRules = null;

    // Keep track of whether this object is "done", i.e. whether more solving
    // steps are expected/allowed.
    protected boolean done = false;

    // If someone has guessed, keep it here, so we know we're all "used up"
    protected Turn whoGuessed = null;

    // How many rounds to bother trying for
    protected Turn turnLimit = null;

    // -----------------------------------------------------------------------
    
    public static void debug(String msg) {
        //System.out.println(msg);
    }

    // -----------------------------------------------------------------------

    public ThreeHatPeople(int aVal, int bVal, int cVal) {
        haveNumbers = true;
        a = aVal;
        b = bVal;
        c = cVal;
    }

    public ThreeHatPeople() {
        haveNumbers = false;
        a = -999;
        b = -999;
        c = -999;
    }

    public Turn getTurnLimit() {
        return turnLimit;
    }

    public void setTurnLimit(Turn turnLimitVal) {
        turnLimit = turnLimitVal;
    }

    public Turn getWhoGuessed() {
        return whoGuessed;
    }

    public List<SumIsZeroRule> getRulesForGuess() {
        List<SumIsZeroRule> rules = new ArrayList<SumIsZeroRule>();
        for (SumIsNotZeroRule r : newRules) {
            ChainNoRationale rationale = (ChainNoRationale) r.getRationale();
            SumIsZeroRule i = r.invert();
            i.setRationale(new ChainYesRationale(rationale));
            rules.add(i);
        }
        return rules;
    }

    public Collection<SumIsNotZeroRule> getRulesBeforeGuess() {
        return lastRules;
    }

    // -----------------------------------------------------------------------

    protected boolean handleOneTurn(int round, Person person) {
        assert(done == false);

        // Remember the previous rules list
        lastRules = currentRules;

        // Generate the new rules
        newRules = person.newRulesAfterNoGuess(round, lastRules);
        debug("--- new rules from person " + person.getId() + " ---");
        for (Rule rule : newRules) {
            debug(rule.toString());            
        }

        // Add the new rules
        currentRules = combineRules(lastRules, newRules);

        // Check if the person has enough information to guess.
        if (haveNumbers && person.canGuess(a, b, c, lastRules)) {
            whoGuessed = new Turn(round, person.getId());
            done = true;
        }

        // Check if we have reached the desired point to stop at
        if (turnLimit != null) {
            assert(round <= turnLimit.round);
            if ((round == turnLimit.round) &&
                    (person.getId().equals(turnLimit.who))) {
                done = true;
            }
        }

        return done;
    }

    public void execute() {
        int r;
        for (r = 1; ;  ++r) {
            debug("*** entering round " + r + " ***");
            handleOneTurn(r, personA);
            if (done) {
                return;
            }
            handleOneTurn(r, personB);
            if (done) {
                return;
            }
            handleOneTurn(r, personC);
            if (done) {
                return;
            }
        }
    }
    
    // -----------------------------------------------------------------------

    public static Turn findWhoGuessed(int aVal, int bVal, int cVal) {
        ThreeHatPeople obj = new ThreeHatPeople(aVal, bVal, cVal);
        obj.execute();
        return obj.getWhoGuessed();
    }

    public static Turn findWhoGuessed(int aVal, int bVal, int cVal,
            Turn limit) {
        ThreeHatPeople obj = new ThreeHatPeople(aVal, bVal, cVal);
        obj.setTurnLimit(limit);
        obj.execute();
        return obj.getWhoGuessed();
    }

    // -----------------------------------------------------------------------

    protected static List<SumIsNotZeroRule> combineRules(
            Collection<SumIsNotZeroRule> list1,
            Collection<SumIsNotZeroRule> list2) {
        List<SumIsNotZeroRule> ret = new ArrayList<SumIsNotZeroRule>(list1);
        ret.addAll(list2);
        return ret;
    }
}
