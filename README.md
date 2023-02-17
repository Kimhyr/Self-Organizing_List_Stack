# Symbol Tree

## Implementation

The symbol tree is a tree with each node having a parent, children, and
a prior and next node (it's siblings). A current tree node is saved, and a
super node (usually a node for a symbol of a
procedure/struct/namespace/...) of the current tree node is saved.

Nodes are organized based off of the type of a symbol. In parsers, symbols for
things inside of procedures/functions are organized by the newest symbol first,
while symbols outside of them are organized by the most used first.

When a node is destroyed, all it's children are destroyed too. Super nodes do
not get destroyed unless they are never used when there are no more symbols
left.
