/**
 * 
 */
package org.gljiva.util.tree;

import java.util.*;

/**
 * An implementation of ITreeChildren whose child list is immutable.
 * 
 * @author bert
 *
 * @param <D>
 */
public class ImmutableChildList<D> {

    /**
     * 
     */
    protected ImmutableChildList() {        
    }

    /**
     * @param list
     */
    public ImmutableChildList(Collection<ITreeNode<D>> children) {
    }

    /**
     * @param list
     */
    public ImmutableChildList(ITreeNode<D>... children) {
    }

    /**
     * Copy constructor with deep copy semantics.
     * Always uses TreeNode.
     * @param orig
     */
    public ImmutableChildList(ITreeChildren<D> orig) {
         List<ITreeNode<D>> children = orig.getList();
         int numChildren = children.size();
         for (int i = 0;  i < numChildren;  ++i) {
         }
     }
}
