/**
 * Wrap some generalized operation so that it gets run once per each unique
 * node.  (In other words, 
 */
package org.gljiva.util.tree.op;

import java.util.List;
import java.util.Map;
import java.util.HashMap;

import org.gljiva.util.tree.ITreeChildren;
import org.gljiva.util.tree.ITreeNode;

/**
 * @author bert
 */
/**
 * @author bert
 *
 * @param <D>
 * @param <R>
 */
public class RunOpOncePerNode<D, R> implements ITreeOp<D, R> {

    protected final ITreeOp<D, R>                  m_op;
    protected final Map<ITreeNode<D>, R>           m_nodeReturnValues;
    protected final Map<ITreeChildren<D>, List<R>> m_childReturnValues;
    protected final boolean                        m_autoClean;

    /**
     * @param op
     */
    public RunOpOncePerNode(ITreeOp<D, R> op) {
        this(op, false);
    }

    /**
     * @param op
     * @param autoClean
     */
    public RunOpOncePerNode(ITreeOp<D, R> op, boolean autoClean) {
        m_op = op;
        m_nodeReturnValues  = new HashMap<ITreeNode<D>, R>();
        m_childReturnValues = new HashMap<ITreeChildren<D>, List<R>>();
        m_autoClean = autoClean;
    }

    /* (non-Javadoc)
     * @see org.gljiva.util.tree.ITreeOp#beforeChildren(org.gljiva.util.tree.ITreeNode)
     */
    public R enterNode(ITreeNode<D> node) {
        // If the return value for this node is cached, return that.
        if (m_nodeReturnValues.containsKey(node)) {
            return m_nodeReturnValues.get(node);
        }

        // Otherwise, we need to actually enter the node.
        R ret = m_op.enterNode(node);
        // If the return value is non-null, cache it and return it.
        if (ret != null) {
            m_nodeReturnValues.put(node, ret);
            return ret;
        }

        // If the return values for the children are cached, use those.
        ITreeChildren<D> children = node.getChildren();
        if (m_childReturnValues.containsKey(children)) {
            ret = m_op.exitNode(node, m_childReturnValues.get(children));
            m_nodeReturnValues.put(node, ret);
            return ret;
        }

        // We tried all possible shortcuts and failed.
        return null;
    }

    /* (non-Javadoc)
     * @see org.gljiva.util.tree.ITreeOp#afterChildren(org.gljiva.util.tree.ITreeNode, java.util.List)
     */
    public R exitNode(ITreeNode<D> node, List<R> childReturnValues) {
        ITreeChildren<D> children = node.getChildren();

        // There should be no cached values.
        assert(! m_nodeReturnValues.containsKey(node));
        assert(! m_childReturnValues.containsKey(children));

        // Cache the return value list for the children...
        m_childReturnValues.put(children, childReturnValues);

        // ... and the return value from the op on this node.
        R ret = m_op.exitNode(node, childReturnValues);
        if (ret != null) {
            m_nodeReturnValues.put(node, ret);
        }
        return ret;
    }

    public void start() {
        if (m_autoClean) {
            m_nodeReturnValues.clear();
            m_childReturnValues.clear();
        }
    }

    public void end() {
    }
}
