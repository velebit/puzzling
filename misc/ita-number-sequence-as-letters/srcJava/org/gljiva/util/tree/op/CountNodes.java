package org.gljiva.util.tree.op;

import java.util.List;

import org.gljiva.util.tree.ITreeNode;
import org.gljiva.util.tree.ITreeChildren;

public class CountNodes {
    protected int m_uniqueNodes = 0;
    protected int m_logicalNodes = 0;

    public <D> CountNodes(ITreeNode<D> rootNode) {
        Op<D> countOp = new Op<D>();
        ITreeOp<D, Integer> op1 = new RunOpOncePerNode<D, Integer>(countOp);
        m_logicalNodes = ForEach.apply(rootNode, op1);
        m_uniqueNodes = countOp.getVisitedNodes();
    }

    public <D> CountNodes(ITreeChildren<D> rootChildren) {
        Op<D> countOp = new Op<D>();
        ITreeOp<D, Integer> onceOp = new RunOpOncePerNode<D, Integer>(countOp);
        m_logicalNodes = Op.sum( ForEach.apply(rootChildren, onceOp) );
        m_uniqueNodes = countOp.getVisitedNodes();
    }

    public int getUniqueNodes() {
        return m_uniqueNodes;
    }
    public int getLogicalNodes() {
        return m_logicalNodes;
    }

    public static class Op<D> implements ITreeOp<D, Integer> {
        protected int m_visited = 0;

        public Integer enterNode(ITreeNode<D> node) {
            return null;   // no early return
        }

        public static int sum(List<Integer> values) {
            int ret = 0;
            // The loop uses auto-unboxing
            for (int i : values) {
                ret += i;
            }
            return ret;
        }

        public Integer exitNode(ITreeNode<D> node,
                List<Integer> childReturnValues) {
            ++m_visited;
            // This node counts as 1, then we add children if any.
            // The return uses autoboxing.
            return 1 + sum(childReturnValues);
        }

        public void start() {
            m_visited = 0;
        }
    
        public void end() {
        }
    
        public int getVisitedNodes() {
            return m_visited;
        }
    }
}