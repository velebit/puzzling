/**
 * Copyright (c) 2007, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd2;

/**
 * An abstract base class for rules that use constraints on some linear
 * combination of the parameters.
 *  
 * @author bert
 */
public abstract class SumBasedRule extends LinearCombination
implements Rule {

    private Rationale rationale;

    /**
     * Construct a SumBasedRule with the specified parameter weights and a
     * rationale.
     * 
     * @param Ka is the value for the weight for a.
     * @param Kb is the value for the weight for b.
     * @param Kc is the value for the weight for c.
     * @param rat is the RuleRationale for this rule (or null).
     */
    public SumBasedRule(int Ka, int Kb, int Kc, Rationale rat) {
        super(Ka, Kb, Kc);
        rationale = rat;
        canonicalize();
    }

    /**
     * Convert the rule into a canonical form where:
     * <ul>
     * <li>the constants in the rule have no common divisor greater than 1, and
     * <li>the first non-zero constant is always positive.
     * </ul>
     */
    protected void canonicalize() {
        int gcd = Util.gcd(k_a, k_b, k_c);
        if (gcd == 0) {
            return;
        }
    
        // Make the leftmost non-zero coefficient be positive.
        if (k_a < 0) {
            gcd = -gcd;
        } else if (k_a == 0) {
            if (k_b < 0) {
                gcd = -gcd;
            } else if (k_b == 0) {
                if (k_c < 0) {
                    gcd = -gcd;
                }
            }
        }
    
        k_a = k_a / gcd;
        k_b = k_b / gcd;
        k_c = k_c / gcd;
    }

    // -----------------------------------------------------------------------


    public Rationale getRationale() {
        return rationale;
    }

    public void setRationale(Rationale rationaleVal) {
        rationale = rationaleVal;
    }

    // -----------------------------------------------------------------------

    public abstract Rule invert();

    // -----------------------------------------------------------------------

    protected Integer solutionValueForA(int b, int c) {
        int residual = k_b * b + k_c * c;
        if ((k_a != 0) && ((residual % k_a) == 0)) {
            return -(residual / k_a);
        } else {
            return null;
        }
    }

    protected Integer solutionValueForB(int a, int c) {
        int residual = k_a * a + k_c * c;
        if ((k_b != 0) && ((residual % k_b) == 0)) {
            return -(residual / k_b);
        } else {
            return null;
        }
    }

    protected Integer solutionValueForC(int a, int b) {
        int residual = k_a * a + k_b * b;
        if ((k_c != 0) && ((residual % k_c) == 0)) {
            return -(residual / k_c);
        } else {
            return null;
        }
    }

    // -----------------------------------------------------------------------

    protected void eliminateA(SumBasedRule other) {
        // If either rule has a zero k_a, there's no point in doing this.
        assert(this.k_a != 0);
        assert(other.k_a != 0);

        /* We want to construct a scaled sum
         *   [Ka', Kb', Kc'] = s1*[Ka1, Kb1, Kc1] + s2*[Ka2, Kb2, Kc2]
         * so that the value of Ka' is zero.  The simplest solution is
         * s1 = Ka2, s2 = -Ka1.
         */
        int s1 =  other.k_a;
        int s2 = -this.k_a;
        k_a = s1 * this.k_a + s2 * other.k_a; 
        k_b = s1 * this.k_b + s2 * other.k_b; 
        k_c = s1 * this.k_c + s2 * other.k_c; 

        // Make the result canonical again.
        canonicalize();
    }

    protected void eliminateB(SumBasedRule other) {
        // If either rule has a zero k_b, there's no point in doing this.
        assert(this.k_b != 0);
        assert(other.k_b != 0);

        /* We want to construct a scaled sum
         *   [Ka', Kb', Kc'] = s1*[Ka1, Kb1, Kc1] + s2*[Ka2, Kb2, Kc2]
         * so that the value of Kb' is zero.  The simplest solution is
         * s1 = Kb2, s2 = -Kb1.
         */
        int s1 =  other.k_b;
        int s2 = -this.k_b;
        k_a = s1 * this.k_a + s2 * other.k_a; 
        k_b = s1 * this.k_b + s2 * other.k_b; 
        k_c = s1 * this.k_c + s2 * other.k_c; 

        // Make the result canonical again.
        canonicalize();
    }

    protected void eliminateC(SumBasedRule other) {
        // If either rule has a zero k_c, there's no point in doing this.
        assert(this.k_c != 0);
        assert(other.k_c != 0);

        /* We want to construct a scaled sum
         *   [Ka', Kb', Kc'] = s1*[Ka1, Kb1, Kc1] + s2*[Ka2, Kb2, Kc2]
         * so that the value of Kc' is zero.  The simplest solution is
         * s1 = Kc2, s2 = -Kc1.
         */
        int s1 =  other.k_c;
        int s2 = -this.k_c;
        k_a = s1 * this.k_a + s2 * other.k_a; 
        k_b = s1 * this.k_b + s2 * other.k_b; 
        k_c = s1 * this.k_c + s2 * other.k_c; 

        // Make the result canonical again.
        canonicalize();
    }

    // -----------------------------------------------------------------------

    /**
     * @return the separator to use for stringification (e.g. " != ").
     */
    protected abstract String stringifySeparator();

    /* (non-Javadoc)
     * @see org.gljiva.trPuzzleCorner.p2006nd2.LinearCombination#toString()
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("rule: ");
    
        if (k_a > 0) {
            if (k_a != 1) {
                sb.append(k_a);
                sb.append("*a");
            } else {
                sb.append("a");
            }
        }
        if (k_b > 0) {
            if (k_a > 0) {
                sb.append(" + ");
            }
            if (k_b != 1) {
                sb.append(k_b);
                sb.append("*b");
            } else {
                sb.append("b");
            }
        }
        if (k_c > 0) {
            if ((k_a > 0) || (k_b > 0)) {
                sb.append(" + ");
            }
            if (k_c != 1) {
                sb.append(k_c);
                sb.append("*c");
            } else {
                sb.append("c");
            }
        }
        if (! ((k_a > 0) || (k_b > 0) || (k_c > 0))) {
            sb.append("0");
        }
    
        sb.append(stringifySeparator());
    
        if (k_a < 0) {
            if (k_a != -1) {
                sb.append(-k_a);
                sb.append("*a");
            } else {
                sb.append("a");
            }
        }
        if (k_b < 0) {
            if (k_a < 0) {
                sb.append(" + ");
            }
            if (k_b != -1) {
                sb.append(-k_b);
                sb.append("*b");
            } else {
                sb.append("b");
            }
        }
        if (k_c < 0) {
            if ((k_a < 0) || (k_b < 0)) {
                sb.append(" + ");
            }
            if (k_c != -1) {
                sb.append(-k_c);
                sb.append("*c");
            } else {
                sb.append("c");
            }
        }
        if (! ((k_a < 0) || (k_b < 0) || (k_c < 0))) {
            sb.append("0");
        }
    
        return sb.toString();
    }
}