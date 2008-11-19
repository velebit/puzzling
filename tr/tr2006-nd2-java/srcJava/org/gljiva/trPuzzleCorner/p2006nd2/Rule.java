/**
 * Copyright (c) 2007, Albert Dvornik <bert@alum.mit.edu>
 */
package org.gljiva.trPuzzleCorner.p2006nd2;

/**
 * A rule claims something about the relationship of values of three numbers.
 *  
 * @author bert
 */
public interface Rule {
    /**
     * @param a  The value of the first parameter.
     * @param b  The value of the second parameter.
     * @param c  The value of the third parameter.
     * @return   True if the rule is satisfied by these parameters,
     *           false otherwise. 
     */
    public boolean isSatisfied(int a, int b, int c);

    /**
     * @return  True if the rule will be satisfied by any set of
     *          (positive integer) parameters.
     */
    public boolean isAlwaysSatisfied();

    /**
     * @return  True if the rule will <b>never</b> be satisfied by any set of
     *          (positive integer) parameters.
     */
    public boolean isNeverSatisfied();

    /**
     * @return  A descriptive string representation of this rule.
     */
    public String  toString();

    /**
     * Set the rationale for this rule.
     * 
     * @param rationaleVal  The new rationale.
     */
    public abstract void setRationale(Rationale rationaleVal);

    /**
     * @return  The rationale for this rule.
     */
    public abstract Rationale getRationale();
}
