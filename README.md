This this sucks.

pls go away it's embarsing

# Self-Organizing List Stack

A DSA for a compiler's [symbol table](https://en.wikipedia.org/wiki/Symbol_table).

The DSA is really just a version of a [self-organizing
list](https://en.wikipedia.org/wiki/Self-organizing_list) except each list
(node) can stack on top of each other to represent scopes.

* An enter's worst case is $O(2n)$, and the best case is $O(n)$.
* A look-up's worst case is $O(n)$, and the best case is $O(1)$.
