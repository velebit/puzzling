/**
 * A general (not binary!) tree structure: the node part.
 * 
 * We split up the nodes and the children so an implementation can have, for
 * example, a ready-made collection of children ready to be stuck into one or
 * more nodes as needed.  Of course, an implementation doesn't have to do it
 * that way; for example, the same class could implement ITreeNode and
 * ITreeChildren.
 */
package org.gljiva.util.tree;

/**
 * @author bert
 *
 * @param <D>
 */
public interface ITreeNode<D> {
    /**
     * Get the data object associated with this node.
     * @return the associated data object.
     */
    public D                getData();

    /**
     * Get the ITree object containing the children of this node.
     * @return
     */
    public ITreeChildren<D> getChildren();
}
