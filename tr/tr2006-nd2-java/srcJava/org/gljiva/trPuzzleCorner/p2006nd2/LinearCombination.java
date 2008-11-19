package org.gljiva.trPuzzleCorner.p2006nd2;

public class LinearCombination {

    /**
     * The weight of the first variable (a) in the linear combination.
     */
    protected int k_a;
    /**
     * The weight of the second variable (b) in the linear combination.
     */
    protected int k_b;
    /**
     * The weight of the third variable (c) in the linear combination.
     */
    protected int k_c;

    // -- constructors: -----------------------------------------------------

    /**
     * @param Ka is the value for the weight for a.
     * @param Kb is the value for the weight for b.
     * @param Kc is the value for the weight for c.
     */
    public LinearCombination(int Ka, int Kb, int Kc) {
        k_a = Ka;
        k_b = Kb;
        k_c = Kc;
    }

    // -- getters: ----------------------------------------------------------

    /**
     * @return k_a, weight of a.
     */
    public int getKa() {
        return k_a;
    }

    /**
     * @return k_b, weight of b.
     */
    public int getKb() {
        return k_b;
    }

    /**
     * @return k_c, weight of c.
     */
    public int getKc() {
        return k_c;
    }

    // ----------------------------------------------------------------------

    /**
     * Calculates the value of the linear combination.
     * @param a The first variable.
     * @param b The second variable.
     * @param c The third variable.
     * @return the weighted sum, k_a * a + k_b * b + k_c * c.
     */
    public int value(int a, int b, int c) {
        return (k_a * a + k_b * b + k_c * c);
    }

    // ----------------------------------------------------------------------

    /**
     * @return true if the value is always zero for any three values of the
     *      variables. 
     */
    public boolean valueIsAlwaysZero() {
        return ((k_a == 0) && (k_b == 0) && (k_c == 0));
    }

    /**
     * @return true if the value is always positive for any three positive
     *      values of the variables.
     */
    public boolean valueIsAlwaysPositive() {
        return ((k_a >= 0) && (k_b >= 0) && (k_c >= 0) &&
                ((k_a + k_b + k_c) != 0));
    }

    /**
     * @return true if the value is always negative for any three positive
     *      values of the variables.
     */
    public boolean valueIsAlwaysNegative() {
        return ((k_a <= 0) && (k_b <= 0) && (k_c <= 0) &&
                ((k_a + k_b + k_c) != 0));
    }

    // ----------------------------------------------------------------------

    /* (non-Javadoc)
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("expr: ");

        sb.append(k_a);
        sb.append("*a + ");
        sb.append(k_b);
        sb.append("*b + ");
        sb.append(k_c);
        sb.append("*c");

        return sb.toString();
    }

    /* (non-Javadoc)
     * @see java.lang.Object#equals(java.lang.Object)
     */
    @Override
    public boolean equals(Object obj) {
        // Must belong to the same class (derivation is not allowed):
        if ((obj == null) || (! this.getClass().equals(obj.getClass()))) {
            return false;
        }
        LinearCombination lcObj = (LinearCombination) obj;
        return ((k_a == lcObj.k_a) && (k_b == lcObj.k_b) && (k_c == lcObj.k_c));
    }
}