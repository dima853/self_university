## 13.1 Properties of Red-Black Trees

A Red-Black Tree is a self-balancing Binary Search Tree (BST) with a simple but powerful addition: each node has an **extra bit for color**, which can be either **RED** or **BLACK**.

The colors are not the goal; they are a **tool**. The specific color of any single node isn't what's important. What matters is that by following a set of five rules regarding these colors, the tree can guarantee efficient performance. You can recolor nodes in different ways—the tree itself chooses a valid coloring during balancing operations. The core principle is:

> **Colors are like paint on a fence—you can repaint it, as long as the fence remains structurally sound.**

The five rules that create this "sound structure" are:

1.  **Every node is either red or black.**
2.  **The root is always black.**
3.  **All leaves (NIL) are black.**
4.  **If a node is red, then both its children must be black.** (No two red nodes in a row).
5.  **For every node, all simple paths from it to descendant leaves contain the same number of black nodes.** (Equal "Black Height").

---

### 1. Basic Node Structure

Each node in a Red-Black Tree contains the following attributes to maintain its structure and color:

```
+-------------------+
|      Node         |
|  +-------------+  |
|  |    color    |  |  // RED or BLACK
|  |     key     |  |  // The data value
|  | left | right|  |  // Pointers to children
|  |    p        |  |  // Pointer to parent
|  +-------------+  |
+-------------------+
    /        \
   /          \
LEFT         RIGHT
CHILD        CHILD
```

---

### 2. Visualizing the Five Properties

Let's examine a valid Red-Black Tree to see these rules in action. NIL leaves are shown as black squares (■).

```
        11(B)
        /    \
       /      \
     2(R)      14(B)
    /   \       /   \
  1(B)  7(B)   ■    15(R)
 /  \   /  \        /   \
■    ■ 5(R) ■      ■     ■
        / \
       ■   ■
```

**Now, let's verify all five properties against this diagram:**

*   **✅ Property 1: Every node is either red or black.**
    *   Visually represented by `(B)` for Black and `(R)` for Red.

*   **✅ Property 2: The root is black.**
    *   The root node `11` is black.

*   **✅ Property 3: Every leaf (NIL) is black.**
    *   All `NIL` pointers are represented as black squares (■).

*   **✅ Property 4: Red nodes have black children.**
    *   Let's check the red nodes:
        *   Node `2(R)`: Its children are `1(B)` and `7(B)`.
        *   Node `5(R)`: Its children are both `NIL` (black).
        *   Node `15(R)`: Its children are both `NIL` (black).

*   **✅ Property 5: All paths from a node to descendant leaves contain the same number of black nodes.**
    *   **From the root (11):**
        *   Path 11(B) → 2(R) → 1(B) → NIL: **2 Black Nodes** (11, 1)
        *   Path 11(B) → 2(R) → 7(B) → 5(R) → NIL: **2 Black Nodes** (11, 7)
        *   Path 11(B) → 14(B) → NIL: **2 Black Nodes** (11, 14)
        *   Path 11(B) → 14(B) → 15(R) → NIL: **2 Black Nodes** (11, 14)
    *   All paths have the same black height.

![A diagram of a red-black tree showing internal nodes and NIL leaves.](image.png)

---

### 3. How This Ensures Balance

Property 5 is the key to guaranteeing balance. It ensures that no path from the root to a leaf is more than **twice as long** as any other.

*   The **shortest possible path** is mostly black (e.g., 11(B) → 14(B) → NIL).
*   The **longest possible path** alternates red and black (e.g., 11(B) → 2(R) → 7(B) → 5(R) → NIL).

Because of Property 4 (no two reds in a row), you cannot add more red nodes to a path without also adding black nodes. This constraint keeps the longest path within twice the length of the shortest.

**Result:** The tree's height is always **O(log n)**, which ensures that search, insertion, and deletion operations are efficient.

### Summary of Constraints

| Property | Description                    | Visual Guarantee                                                            |
| :------- | :----------------------------- | :-------------------------------------------------------------------------- |
| **1**    | Node color is Red or Black.    | `(R)` or `(B)` on every node.                                               |
| **2**    | The root is Black.             | The top node is `(B)`.                                                      |
| **3**    | All leaves (NIL) are Black.    | All `NIL` pointers are black (■).                                           |
| **4**    | Red nodes have Black children. | No two `(R)` nodes are directly connected vertically.                       |
| **5**    | Equal black count to leaves.   | For any node, counting `(B)` nodes down to any `NIL` gives the same number. |

---

### The Sentinel (`T.nil`): A Technical Convenience

**The Problem:** In code, handling regular nodes that have `parent`, `left`, and `right` pointers differently from NIL leaves (which have none) complicates the algorithms.

**The Solution:** Use a single, shared **sentinel node**, called `T.nil`, to represent all NILs.

**Before:**
```
    26
    / \
  17  41
 / \  / \
NIL NIL NIL NIL  // Each NIL is separate, with no parent.
```

**After:**
```
       26
      /  \
    17    41
   / \    / \
  ▼   ▼  ▼   ▼  // All pointers to NIL now point to the single T.nil object.
      \
       T.nil (BLACK)
```

![A diagram showing how all NIL pointers are replaced by pointers to a single sentinel node.](image-1.png)

**`T.nil` is a "universal soldier":**
*   Its color is always **BLACK** (satisfying Property 3).
*   It has all the same attributes as a normal node (like `parent`), simplifying code logic.
*   It saves memory (one object instead of many NILs).

---

### Understanding Black-Height

**Definition:** The **black-height** of a node `x` is the number of **black nodes** on any simple path from `x` down to a leaf (NIL), **not including** `x` itself.

**Example:**
```
    26(B)  // Not counted!
    /    \
  17(R)   41(B)  // Count black nodes from here down.
```

From node 26 to any leaf:
*   Path 26 → 17(R) → 14(B) → NIL: **1 Black Node** (14)
*   Path 26 → 41(B) → 30(R) → NIL: **1 Black Node** (41)

**The Key Insight:** Because of Property 5, this black-height is **identical** for all paths from a given node, which is what guarantees the tree remains balanced.

![A diagram of a complex red-black tree with black-heights labeled for each node.](image-2.png)

---

### Conclusion

1.  **Colors are a Tool:** The specific coloring is a means to an end (balance), not the end itself.
2.  **The Sentinel (`T.nil`)** is a technical trick to simplify the code.
3.  **Black-Height** is the core metric that, when kept consistent across all paths, ensures the tree's height is logarithmic.