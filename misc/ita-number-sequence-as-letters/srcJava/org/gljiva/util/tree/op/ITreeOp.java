/**
 * A generalized operation on each tree node (possibly with pruning).
 */
package org.gljiva.util.tree.op;

import java.util.List;

import org.gljiva.util.tree.ITreeNode;

/**
 * @author bert
 *
 * @param <D>  The data type associated with the tree's nodes.
 * @param <R>  The return type for this operation.
 */
public interface ITreeOp<D, R> {
    /**
     * This method is called when "entering" a node.
     * It may return either null (to continue processing the children) or
     * a non-null value (to "prune" the tree and return early).<p>
     *
     * If this method returns null, that indicates that the processing should
     * continue with each of the children, after which the
     * {@code exitNode()} method will be called.<p>
     *
     * If this method returns a non-null value, child nodes will not be
     * processed, and {@code exitNode()}
     * will not be called.  The value returned will be taken as the
     * operation's return value.<p>
     * 
     * @param node  The current tree node.
     * @return      Null, or the early-return value for the operation on the
     *              current node.
     */
    public R enterNode(ITreeNode<D> node);

    /**
     * This method is called when "exiting" a node.
     * Its return value is the operation's return value for this node.  If
     * {@code enterNode} returns non-null, this method will never be called
     * (nor will any of the methods on this node's children).<p>
     * 
     * For symmetry with {@code enterNode()}, returning null from this method
     * is discouraged (but not prohibited).<p>
     * 
     * @param node              The current tree node.
     * @param childReturnValues The list of return values from each of this
     *                          node's children.
     * @return                  The value for the operation on the current node.
     */
    public R exitNode(ITreeNode<D> node, List<R> childReturnValues);


    /**
     * Called at the start of a tree traversal.
     */
    public void start();

    /**
     * Called at the end of a tree traversal.
     */
    public void end();
}