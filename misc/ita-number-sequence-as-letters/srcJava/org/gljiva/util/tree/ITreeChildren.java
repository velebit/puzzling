package org.gljiva.util.tree;

import java.util.List;

/**
 * The "children" part of a general (non-binary) tree structure.
 * 
 * We split up the nodes and the children so an implementation can have, for
 * example, a ready-made collection of children ready to be stuck into one or
 * more nodes as needed.  Of course, an implementation doesn't have to do it
 * that way; for example, the same class could implement ITreeNode and
 * ITreeChildren. 
 * 
 * @author bert
 *
 * @param <D>  The data type associated with each child node.
 */
public interface ITreeChildren<D> {

    /**
     * @return
     */
    public List<ITreeNode<D>> getList();
}
