A parser for basic set operations
Examples of valid statements:

{}
{1}
{1,2}
{1}U{1,2}
({1}U{2})^({2}U{3})
({12,3,4}^{2,9})U({2,1,9}^{3,4,5})

Implementation Notes
 * The parse table is implemented with a linear-probing HashMap. 
 * The parse tree is implemented as a LMC-RS tree to allow for any
   number of children
 * The stack is a linked-list-based stack, and is O(1) in all operations.
 * I provided a nice abstraction type called Grammar (defined in grammar.h)
   that is just a collection of productions.

