/**
 * 
 */
package org.gljiva.util.tree;

import java.util.ArrayList;
import java.util.List;
import java.util.Collection;
import java.util.Arrays;

/**
 * @author bert
 *
 */
public class TreeNode<D> {
    private final D                 m_data;
    private final List<TreeNode<D>> m_children;

    public TreeNode(D data, Collection<TreeNode<D>> children) {
        m_data     = data;
        m_children = new ArrayList<TreeNode<D>>(children);
    }

    public TreeNode(D data, TreeNode<D>... children) {
        m_data     = data;
        m_children = new ArrayList<TreeNode<D>>(Arrays.asList(children));
    }

    public D getData() {
        return m_data;
    }

    public Collection<TreeNode<D>> getChildren() {
        return m_children;
    }
}
