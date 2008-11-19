/**
 * Run a generalized operation on each tree node (possibly with pruning).
 */
package org.gljiva.util.tree.op;

import java.util.List;
import java.util.ArrayList;
import org.gljiva.util.tree.ITreeNode;
import org.gljiva.util.tree.ITreeChildren;
import org.gljiva.util.tree.op.ITreeOp;

/**
 * @author bert
 *
 */
public abstract class ForEach {
    public static <D, R> R apply(ITreeNode<D> node, ITreeOp<D, R> op) {
        op.start();
        R ret = recurse(node, op);
        op.end();
        return ret;
    }

    public static <D, R> List<R> apply(ITreeChildren<D> children,
            ITreeOp<D, R> op) {
        op.start();
        List<R> ret = recurse(children, op);
        op.end();
        return ret;
    }
    
    protected static <D, R> R recurse(ITreeNode<D> node, ITreeOp<D, R> op) {
        R ret = op.enterNode(node);
        if (ret != null) {
            return ret;
        }

        List<R> childRet = recurse(node.getChildren(), op);
        ret = op.exitNode(node, childRet);
        return ret;
    }

    protected static <D, R> List<R> recurse(ITreeChildren<D> children,
            ITreeOp<D, R> op) {
        List<ITreeNode<D>> childList = children.getList();
        List<R> ret = new ArrayList<R>(childList.size());
        for (int i = 0;  i < childList.size();  ++i) {
            ret.set(i, recurse(childList.get(i), op));
        }
        return ret;
    }
}
