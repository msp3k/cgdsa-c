/** \mainpage [C]ool and [G]roovy [D]ata [S]tructures and [A]lgorithms in [C]
 *
 * GitHub Repository: https://github.com/msp3k/cgdsa-c/tree/master
 *
 * \section rbtree_t Double-Linked List
 * A linked data structure that consists of a set of sequentially linked nodes
 * linked together in a bidirectional linear list.
 *
 * <TABLE>
 * <TR><TH> Property </TH><TH> Average Case </TH><TH> Worst Case </TH></TR>
 * <TR><TD> Space </TD><TD> O(n) </TD><TD> O(n) </TD></TR>
 * <TR><TD> Traversal </TD><TD> O(n) </TD><TD> O(n) </TD></TR>
 * <TR><TD> Insertion/Deletion From Beginning/End </TD><TD> O(1) </TD><TD> O(1) </TD></TR>
 * <TR><TD> Search </TD><TD> O(n) </TD><TD> O(n) </TD></TR>
 * <TR><TD> Insertion/Deletion From Middle </TD><TD> Search Time + O(1)
 * </TD><TD> Search Time + O(1) </TD></TR>
 * </TABLE>
 *
 * Files: https://github.com/msp3k/cgdsa-c/tree/master/./double-linked-list
 *
 * \section dllist_t Red-Black Tree
 * A self-balancing binary search tree.  Balance is preserved by associating a
 * color with each node of the tree in a way that satisfies certain properties
 * which constrain how unbalanced the tree can become.  Balancing is not as
 * rigorously enforced as with other algorithms such as the AVL tree, but the
 * tradeoff is that rebalancing is slightly faster while lookups are slightly
 * slower.
 *
 * <TABLE>
 * <TR><TH> Property </TH><TH> Average Case </TH><TH> Worst Case </TH></TR>
 * <TR><TD> Space </TD><TD> O(n) </TD><TD> O(n) </TD></TR>
 * <TR><TD> Search </TD><TD> O(log n) </TD><TD> O(log n) </TD></TR>
 * <TR><TD> Insert </TD><TD> O(log n) </TD><TD> O(log n) </TD></TR>
 * <TR><TD> Delete </TD><TD> O(log n) </TD><TD> O(log n) </TD></TR>
 * </TABLE>
 *
 * Files: https://github.com/msp3k/cgdsa-c/tree/master/./red-black-tree
 *
 * \section tree_t Tree
 * A simple binary tree with no rebalancing.
 *
 * <TABLE>
 * <TR><TH> Property </TH><TH> Average Case </TH><TH> Worst Case </TH></TR>
 * <TR><TD> Space </TD><TD> O(n) </TD><TD> O(n) </TD></TR>
 * <TR><TD> Search </TD><TD> O(log n) </TD><TD> O(n) </TD></TR>
 * <TR><TD> Insert </TD><TD> O(log n) </TD><TD> O(n) </TD></TR>
 * <TR><TD> Delete </TD><TD> O(log n) </TD><TD> O(n) </TD></TR>
 * </TABLE>
 *
 * Files: https://github.com/msp3k/cgdsa-c/tree/master/./tree
 *
 */
