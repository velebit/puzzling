package org.gljiva.util.tree;

import java.util.List;
import java.util.ArrayList;

public class Trees {

    public static interface IDataCopyPolicy {
        public <D> D make(D orig);
    }

    public static final IDataCopyPolicy DATA_SHARED =
        new IDataCopyPolicy() {
        public <D> D make(D orig) {
            return orig;
        }
    };

    
    public static interface IChildrenCopyPolicy {
        public <D> ITreeChildren<D> make(List<ITreeNode<D>> children);
    }

    public static final IChildrenCopyPolicy CHILDREN_MUTABLE =
        new IChildrenCopyPolicy() {
        public <D> ITreeChildren<D> make(List<ITreeNode<D>> children) {
            return new MutableChildList<D>(children);
        }
    };

    public static final IChildrenCopyPolicy CHILDREN_IMMUTABLE =
        new IChildrenCopyPolicy() {
        public <D> ITreeChildren<D> make(List<ITreeNode<D>> children) {
            return new ImmutableChildList<D>(children);
        }
    };


    public static <D> ITreeNode<D> flatDeepCopy(ITreeNode<D> node,
            IDataCopyPolicy dcp, IChildrenCopyPolicy ccp) {
        D                data     = node.getData();
        ITreeChildren<D> children = node.getChildren();
        if (data != null) {
            data = dcp.make(data);
        }
        if (children != null) {
            children = flatDeepCopy(children, dcp, ccp);
        }
        return new TreeNode<D>(data, children);
    }

    public static <D> ITreeChildren<D> flatDeepCopy(ITreeChildren<D> children,
            IDataCopyPolicy dcp, IChildrenCopyPolicy ccp) {
        List<ITreeNode<D>> list = children.getList();
        assert(list != null);
        list = new ArrayList<ITreeNode<D>>(list);
        for (int i = 0;  i < list.size();  ++i) {
            list.set(i, flatDeepCopy(list.get(i), dcp, ccp));
        }
        return ccp.make(list);
    }
}
