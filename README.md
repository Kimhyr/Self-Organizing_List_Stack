# Symbol Tree

## Implementation

The symbol tree is a tree with each node having a parent, children (a circular
linked list), a prior and next node (it's siblings), and a bias (for
organizing). The newest node is saved, and, if the newest node is a sub of a
super node (a node for a procedure/function), the super node of the newest node
is saved.

Nodes are organized by the greatest bias at first.

When a node is destroyed, all it's children are destroyed too. Super nodes do
not get destroyed unless, if there are no more symbols left, they are never used.
