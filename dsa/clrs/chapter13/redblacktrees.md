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

---
---
---

## 📌 Lemma 13.1 Statement

> A red-black tree with `n` internal nodes has height at most `2 lg(n + 1)`.

In mathematical terms, if we let `h` be the height of the tree:
$$h \leq 2 \log_2(n + 1)$$
This guarantees the tree remains approximately balanced, as its height grows only logarithmically with the number of nodes. 
> Just the boundaries

---

## 🧠 Part 1: The Core Claim & Induction Setup

The proof strategy is to first prove a stronger claim about the structure of the tree.

**Claim:** The subtree rooted at any node `x` contains at least `2^{bh(x)} - 1` (**This is the MINIMUM number of nodes in a subtree rooted in x**) internal nodes.

Where `bh(x)` is the **black-height** of node `x`—the number of black nodes on any path from `x` to a leaf (not including `x` itself).

> In simple terms: "If you have a lot of black nodes on the paths, then there should be a lot of common nodes" 

### **THE TREE IN FIGURE 13.1:**
```text
        26(B) bh=2
        /         \
      17(R)       41(B) bh=1
     /    \       /    \
   14(B)  21(B)  30(R)  47(R)
   bh=1   bh=1   bh=1   bh=1
  /  \    /  \   /  \   /  \
 ■    ■  ■    ■ ■    ■ ■    ■
bh=0     bh=0    bh=0   bh=0
```

### Proof by Induction on the Height of `x`

#### **Base Case: Height of `x` is 0**
If the height of `x` is 0, then `x` must be a leaf (`T.nil`).
*   The black-height of a leaf (`T.nil`) is 0: `bh(x) = 0`.
*   The number of internal nodes in a subtree rooted at a leaf is 0.

Let's check the claim:
$$ 2^{bh(x)} - 1 = 2^0 - 1 = 1 - 1 = 0 $$
The claim holds, as `0 >= 0`.

#### **Inductive Step: Height of `x` is > 0**
We assume the claim is true for all nodes with a height less than `x`'s height.

Node `x` is an internal node with a positive height, so it has two children. Each child can be an internal node or `T.nil`.

**Crucial Observation:** The black-height of a child depends on its color relative to `x`.
*   If a child is **BLACK**, its black-height is `bh(x) - 1`.
    *   Why? The path goes through this black child, which *is* counted in its own black-height, so we have `bh(x) - 1` remaining black nodes to reach the original count.
*   If a child is **RED**, its black-height is `bh(x)`.
    *   Why? The path goes through this red child, which is *not* counted in its own black-height, so we still need all `bh(x)` black nodes.

Since the height of each child is less than the height of `x`, we can apply the **inductive hypothesis** to them.

*   **Minimum nodes in left child's subtree:** `2^{bh(child_{left})} - 1`
*   **Minimum nodes in right child's subtree:** `2^{bh(child_{right})} - 1`

A child's black-height is *at least* `bh(x) - 1` (this is the worst case for our count, which happens if the child is black). Therefore, each child's subtree has at least:
$$ 2^{bh(x) - 1} - 1 \text{ internal nodes} $$

Now, the total number of internal nodes in the subtree rooted at `x` is:
\[ \text{(Nodes in left subtree)} + \text{(Nodes in right subtree)} + \text{(Node x itself)} \]
\[ \geq \left[2^{bh(x)-1} - 1\right] + \left[2^{bh(x)-1} - 1\right] + 1 \]
\[ = 2 \cdot \left(2^{bh(x)-1} - 1\right) + 1 \]
\[ = 2^{bh(x)} - 2 + 1 \]
\[ = 2^{bh(x)} - 1 \]

This completes the inductive step and proves the core claim. ✅

---

## 🔗 Part 2: Connecting to the Tree Height

Now we link this structural claim to the overall height `h` of the tree.

Let `h` be the height of the entire red-black tree.

From **Property 4** (a red node must have black children), we know that on any path from the root to a leaf, **at least half** of the nodes (excluding the root itself) must be black.

Let `bh(root)` be the black-height of the root.
*   The longest path from the root to a leaf has length `h`.
*   At least half of these `h` nodes (not counting the root) are black.
*   Therefore, the number of black nodes on this path, which is `bh(root)`, must be at least `h/2`:

$$ bh(root) \geq \lceil h/2 \rceil $$
For simplicity in the proof, we use:
$$ bh(root) \geq h/2 $$

Now, we apply our previously proven claim to the **root** of the entire tree. The whole tree has `n` internal nodes and is rooted at the root, so:
$$ n \geq 2^{bh(root)} - 1 $$
Substituting `bh(root) >= h/2`:
$$ n \geq 2^{h/2} - 1 $$

---

## 📐 Part 3: The Final Algebraic Manipulation

We now solve for `h` in terms of `n`.

Start with:
$$ n \geq 2^{h/2} - 1 $$

Add 1 to both sides:
$$ n + 1 \geq 2^{h/2} $$

Take the logarithm base-2 of both sides:
$$ \log_2(n + 1) \geq \log_2(2^{h/2}) $$
$$ \log_2(n + 1) \geq h/2 $$

Multiply both sides by 2 to solve for `h`:
$$ 2 \log_2(n + 1) \geq h $$

Or, equivalently:
$$ h \leq 2 \log_2(n + 1) $$

**This is exactly what we set out to prove.** 🎉

---
## 💡 Corollaries of the Lemma

As indicated in the text, this lemma is fundamental. Since the height `h` of a red-black tree is `O(log n)`, all basic operations on dynamic sets performed in time proportional to the height in a BST are now performed in `O(log n)` time on a red-black tree.

* `SEARCH`, `MINIMUM`, `MAXIMUM`, `SUCCESSOR`, `PREDECESSOR` all have `O(log n)` time.

The text also notes an important point: although the standard BST operations `INSERT` and `DELETE` run in `O(log n)` time *for* a red-black tree, they do not preserve the red-black tree properties. The remainder of the chapter details how to modify these operations to preserve the properties while achieving `O(log n)` time complexity.

---
---
## 🔄 13.2 Rotations

**Problem:** The TREE-INSERT and TREE-DELETE operations in a red-black tree can disrupt its balance. A mechanism is needed to restore the structure without losing the binary search property.

**Solution:** Rotations are local operations that change the pointer structure but preserve the binary search property.

### 🎯 What is a rotation?

A rotation is an operation that "twists" parent-child relationships, changing their hierarchy but preserving the key order.

> **Analogy:** Imagine the tree as a mobile. If you pull one branch and "rotate" it, the overall structure changes, but all elements remain in their relative order.

### Two types of rotations

![alt text](image-3.png)

**Key observations:**
- **Left rotation** makes the right child the parent
- **Right rotation** makes the left child the parent
- Both rotations are **mutually inverse**
- Running time: **O(1)** – only a constant number of pointers are changed

### 💻 Pseudocode LEFT-ROTATE

```pseudocode
LEFT-ROTATE(T, x)
1 y = x.right // set y
2 x.right = y.left // turn y's left subtree into x's right subtree
3 if y.left ≠ T.nil
4 y.left.p = x // update the parent of y's left subtree
5 y.p = x.p // move x's parent to y
6 if x.p == T.nil
7 T.root = y // if x was the root, then y is now the root
8 elseif x == x.p.left
9 x.p.left = y // if x was a left child
10 else x.p.right = y // if x was a right child
11 y.left = x // make x a left child of y
12 x.p = y // update x's parent
```

### Rotation visualization (Fig. 13.3)

![alt text](image-4.png)

**What changed:**
- `y` (18) became the new root of the subtree
- `x` (11) became the left child of `y`

### 🔄 Symmetrical RIGHT-ROTATE pseudocode

```pseudocode
RIGHT-ROTATE(T, y)
1 x = y.left // set x
2 y.left = x.right // turn x's right subtree into y's left subtree
3 if x.right ≠ T.nil
4 x.right.p = y // update the parent of x's right subtree
5 x.p = y.p // move y's parent to x
6 if y.p == T.nil
7 T.root = x // if y was the root, then x is the root
8 elseif y == y.p.left
9 y.p.left = x // if y was a left child
10 else y.p.right = x // if y was a right child
11 x.right = y // move y to be a right child of x
12 y.p = x // update y's parent
```

### Key properties of rotations

1. **Preservation of BST property:** The key order remains unchanged
2. **Locality:** Only a small fragment of the tree is changed.
3. **Reversibility:** LEFT-ROTATE and RIGHT-ROTATE operations are mutually inverse.
4. **Efficiency:** O(1) time, constant memory.

### 💡 Why is this needed in red-black trees?

Rotations are used in combination with recoloring to:
- **Restore property 4** (no two consecutive red nodes)
- **Preserve property 5** (same black height)
- **Balance the tree** after insertion/deletion

---

### Conclusion

**Rotations are a fundamental building block** for all balancing operations in red-black trees. They allow "restructuring" the tree while preserving its search properties but improving its balance.

> **As in construction:** You can move a beam (rotate) to redistribute the load, but the overall house structure (BST property) remains unchanged.


---
---
## 🔴 13.3 Insertion

**Main idea:** Insertion into a red-black tree consists of two stages:
1. **Standard BST insertion** + coloring the new node **red**
2. **Fixing violations** using recoloring and rotations

### 🎯 Why is the new node always RED?

> **Exercise 13.3-1:** Why not color the new node black?

**Answer:** If the new node `z` is made black, we **guarantee a violation of property 5** (equal black height)! All paths through this node will have +1 black node. Fixing this is more difficult than a potential violation of property 4 (two reds in a row).

### 💻 Pseudocode RB-INSERT

```pseudocode
RB-INSERT(T, z)
1   x = T.root                    // start from the root
2   y = T.nil                     // y will be z's parent
3   while x ≠ T.nil               // descend to a leaf
4       y = x
5       if z.key < x.key
6           x = x.left
7       else x = x.right
8   z.p = y                       // found parent for z
9   if y == T.nil                 // tree was empty
10      T.root = z
11  elseif z.key < y.key
12      y.left = z
13  else y.right = z
14  z.left = T.nil               // initialize children
15  z.right = T.nil
16  z.color = RED                // KEY MOMENT: color it red
17  RB-INSERT-FIXUP(T, z)        // fix possible violations
```

### 🔧 RB-INSERT-FIXUP: Fixing violations

**Which properties can be violated?**
- ✅ **Property 1** (colors): remains
- ✅ **Property 3** (leaves are black): remains
- ✅ **Property 5** (black height): remains
- ❗ **Property 2** (root is black): violated if `z` became the root
- ❗ **Property 4** (no two reds in a row): violated if `z`'s parent is red

### 🎨 Three cases of fixing

#### **Case 1: Uncle is RED**
```
    black(B)
    /       \
 red(R)  red(R) ← uncle y
   /  \       /   \
  ... z(R)   ...   ...
```

**Solution:** Recolor parent and uncle to black, grandparent to red, move `z` up to grandparent level.

```pseudocode
if y.color == RED:           // case 1
    z.p.color = BLACK
    y.color = BLACK
    z.p.p.color = RED
    z = z.p.p               // move z up 2 levels
```

#### **Case 2: Uncle is BLACK, z is a right child**
```
    black(B)
    /       \
 red(R)  black(B) ← uncle y
   /  \
  ...  z(R) ← right child
```

**Solution:** Left rotation around the parent, turning it into case 3.

```pseudocode
if z == z.p.right:          // case 2
    z = z.p
    LEFT-ROTATE(T, z)
// proceed to case 3...
```

#### **Case 3: Uncle is BLACK, z is a left child**
```
    black(B)
    /       \
 red(R)  black(B) ← uncle y
   /  \
 z(R) ... ← left child
```

**Solution:** Recoloring + right rotation around the grandparent.

```pseudocode
z.p.color = BLACK          // case 3
z.p.p.color = RED
RIGHT-ROTATE(T, z.p.p)
```

### 📊 Visual example (Figure 13.4)

![alt text](image-5.png)

          ┌─────────────────────────────────────────────────────────────────┐
          │                 FIGURE 13.4 - RB-INSERT-FIXUP                   │
          └─────────────────────────────────────────────────────────────────┘

(a) █ Node z after insertion. Since both z and its parent z.p are red,
a violation of PROPERTY 4 occurs. Since z's uncle y is RED,
CASE 1 in the code is applied. The grandparent of z z.p.p must be BLACK,
and its blackness is passed down one level to z's parent and uncle.
Once the pointer z is moved two levels up the tree,
the tree shown in (b) is obtained.

(b) █ Again z and its parent are red, but this time z's uncle y is BLACK. Since z is the RIGHT child of z.p, CASE 2 is applied.

(c) █ Performing a LEFT rotation results in the tree shown in (c).
Now z is the LEFT child of its parent.

(d) █ CASE 3 is applied. Recoloring and a right rotation yield tree (d), which is a VALID RED-BLACK TREE.


### 🔄 Full RB-INSERT-FIXUP pseudocode

```pseudocode
RB-INSERT-FIXUP(T, z)
1   while z.p.color == RED
2       if z.p == z.p.p.left           // parent is a left child
3           y = z.p.p.right            // uncle
4           if y.color == RED          // CASE 1
5               z.p.color = BLACK
6               y.color = BLACK
7               z.p.p.color = RED
8               z = z.p.p
9           else
10              if z == z.p.right      // CASE 2
11                  z = z.p
12                  LEFT-ROTATE(T, z)
13              z.p.color = BLACK      // CASE 3
14              z.p.p.color = RED
15              RIGHT-ROTATE(T, z.p.p)
16       else                          // symmetrically for a right child
17           // ... same cases with left/right swapped
18   T.root.color = BLACK              // ensure black root
```

---


![alt text](image-6.png)

### **Case 1: Uncle is RED**

**Problem:**
Node `z` and its parent `z.p` are both RED → violation of **Property 4** (two reds in a row)

**🎯 Condition for Case 1:**
- Uncle `y` (parent's sibling) is also **RED**

**🔄 Solution: "Pushing blackness down"**

**Steps:**
1. `z.p.color = BLACK`    ← parent becomes BLACK
2. `y.color = BLACK`      ← uncle becomes BLACK
3. `z.p.p.color = RED`    ← grandparent becomes RED
4. `z = z.p.p`           ← move pointer to grandparent

**✅ What is fixed:**
- **Property 4**: no more violation between `z` and parent
- **Property 5**: black height is preserved - we just "redistributed" blackness

**What next:**
- `z` now points to the grandparent (red)
- If grandparent became the root → violation of Property 2 (we'll fix later)
- If grandparent has a red parent → new violation of Property 4 (repeat the cycle!)

**Features of Case 1:**
- NO rotations - only recoloring
- `z` moves up 2 levels
- Can create a new violation higher up the tree
- The cycle continues with the new `z`
---

![alt text](image-7.png)

🔄 **Cases 2 and 3: Uncle is BLACK**

**Problem:**
`z` and parent `z.p` are both RED → violation of **Property 4**

**Condition:**
Uncle `y` is **BLACK**

---

### Case 2: `z` is a RIGHT child

**🔄 Transformation:**
- Left rotation around the parent
- Case 2 → Case 3

**Code:**
```pseudocode
z = z.p
LEFT-ROTATE(T, z)
// proceed to Case 3...
```

---

### Case 3: `z` is a LEFT child

**Final solution:**
1. `z.p.color = BLACK`       ← parent → BLACK
2. `z.p.p.color = RED`       ← grandparent → RED
3. `RIGHT-ROTATE(T, z.p.p)`  ← right rotation around grandparent

**✅ Result:**
- **Property 4** restored - no two reds in a row
- **Property 5** preserved - black height unchanged
- Cycle **TERMINATES** - parent is now black

---

### Key points:

**Case 2 → Case 3:**
- Just prepares the structure
- Left rotation aligns `z` as a left child

**Case 3 - FINAL:**
- Recoloring + right rotation
- Completely eliminates the violation
- Cycle terminates on the next iteration

**Result:** The tree becomes a valid red-black tree again!

---
---

# 🔄 **13.4 Deletion - Deletion from a Red-Black Tree**

**🎯 Main idea:** Deletion is more complex than insertion, but also works in **O(log n)**

---

### 🔧 **RB-TRANSPLANT - Modified replacement**

```pseudocode
RB-TRANSPLANT(T, u, v)
1   if u.p == T.nil          // u is the root
2       T.root = v
3   elseif u == u.p.left     // u is a left child
4       u.p.left = v
5   else                     // u is a right child
6       u.p.right = v
7   v.p = u.p               // ALWAYS update v's parent
```

**Differences from regular TRANSPLANT:**
- Works with `T.nil` instead of `NIL`
- Always updates `v.p` (even if `v = T.nil`)

---

### 🗑️ **RB-DELETE - Main deletion procedure**

```pseudocode
RB-DELETE(T, z)
1   y = z                    // y - node to be deleted
2   y-original-color = y.color
3   if z.left == T.nil       // no left child
4       x = z.right
5       RB-TRANSPLANT(T, z, z.right)
6   elseif z.right == T.nil  // no right child
7       x = z.left
8       RB-TRANSPLANT(T, z, z.left)
9   else                     // has both children
10      y = TREE-MINIMUM(z.right)  // successor
11      y-original-color = y.color
12      x = y.right
13      if y != z.right      // successor is not the direct right child
14          RB-TRANSPLANT(T, y, y.right)
15          y.right = z.right
16          y.right.p = y
17      RB-TRANSPLANT(T, z, y)
18      y.left = z.left
19      y.left.p = y
20      y.color = z.color
21  if y-original-color == BLACK
22      RB-DELETE-FIXUP(T, x)  // fix violations
```

---

### 🎨 **What is "extra blackness"?**

**Problem:** When deleting a BLACK node `y`, **Property 5** (black height) is violated

**Solution concept:**
- Node `x` receives **"extra blackness"**
- `x` becomes either:
  - **"Doubly black"** (black + extra blackness)
  - **"Red-black"** (red + extra blackness)

**Goal of RB-DELETE-FIXUP:** Get rid of the extra blackness!

---

### 🔧 **RB-DELETE-FIXUP - 4 cases of fixing**

**Case 1: Sibling `w` is RED**
```pseudocode
// Sibling w is red → make it black
w.color = BLACK
x.p.color = RED
LEFT-ROTATE(T, x.p)
w = x.p.right
// Proceed to Case 2, 3 or 4
```

**Case 2: Sibling `w` is BLACK, both children of `w` are BLACK**
```pseudocode
// Remove blackness from w, move extra blackness up
w.color = RED
x = x.p  // Move x one level up
```

**Case 3: Sibling `w` is BLACK, left child of `w` is RED, right is BLACK**
```pseudocode
// Preparation for Case 4
w.left.color = BLACK
w.color = RED
RIGHT-ROTATE(T, w)
w = x.p.right
// Proceed to Case 4
```

**Case 4: Sibling `w` is BLACK, right child of `w` is RED**
```pseudocode
// FINAL solution - remove extra blackness!
w.color = x.p.color
x.p.color = BLACK
w.right.color = BLACK
LEFT-ROTATE(T, x.p)
x = T.root  // Terminate the cycle!
```

![alt text](image-8.png)
---

### ⏱ **Complexity analysis**

- **RB-DELETE:** O(log n) - search + transplantation
- **RB-DELETE-FIXUP:** O(log n) - moving up the tree
- **Rotations:** no more than 3 rotations per deletion

**Total:** O(log n) time, O(1) additional memory

---

### 🎯 **Key points:**

1. **Only deletion of a BLACK node** requires fixes
2. **Concept of "extra blackness"** - an elegant solution for Property 5
3. **4 cases** cover all possible configurations
4. **Case 1 → Case 2/3/4** - preparatory
5. **Case 4** - the only one that completely removes extra blackness

**Result:** The tree remains a valid red-black tree after deletion! 🎉

---
---
# 🔴 **Persistent Red-Black Trees** (Problem 13-1)

## **Problem of persistent structures**
Need to maintain **all versions** of the set during changes, but copying the entire tree every time is too expensive!

## **Solution for regular BST**
**Idea:** Copy only the changed nodes on the path from the root!

**Example from Figure 13.8:**
![alt text](image-9.png)

### 📝 **Answers to subpoints:**

**a) Which nodes change?**
- All nodes on the path from the root to the inserted/deleted node
- Their immediate children affected by the change

**b) PERSISTENT-TREE-INSERT:**
```pseudocode
PERSISTENT-TREE-INSERT(T, z)
1   if T.root == NIL
2       new_root = COPY-NODE(z)
3       return new tree with root new_root
4   new_root = COPY-NODE(T.root)
5   x = new_root
6   y = NIL
7   while x != NIL
8       y = x
9       if z.key < x.key
10          x = COPY-NODE(x.left)  // copy while descending!
11          y.left = x
12      else
13          x = COPY-NODE(x.right)
14          y.right = x
15   // Insert z as a child of y
16   if z.key < y.key
17       y.left = z
18   else
19       y.right = z
20   return new tree with root new_root
```

**c) Time and memory: O(h)**
- Copy only h nodes on the path
- Memory: O(h) new nodes
- Time: O(h) operations

**d) With parent pointers - O(n) in the worst case**
When changing a node's parent, you need to copy all its descendants!

**e) Solution with red-black trees: O(log n)**
Use persistent RBT - guaranteed height O(log n) ⇒ O(log n) time and memory per operation!

---

# 🔗 **Join Operation** (Problem 13-2)

## **Join task**
Given two RBTs T₁ and T₂ and a key x, where all keys in T₁ < x < all keys in T₂. Merge into one RBT.

## 🛠 **Solution:**

**a) Supporting black-height:**
- Can be computed on the fly while descending the tree
- No need to store separately!

**b) Finding a black node in T₁ with black-height = T₂.bh:**
- Descend along right children, maintaining current black-height
- Time: O(log n)

**c) Quick replacement: O(1)**
```pseudocode
// T_y - subtree with root y
new_node = Node(x)
new_node.left = y
new_node.right = T₂.root
y.parent = new_node
T₂.root.parent = new_node
```

**d) Color for x:**
- Make x **RED** (preserve Properties 1,3,5)
- Fix Properties 2,4 in O(log n) using RB-INSERT-FIXUP

**e) Symmetrical case:** When T₁.bh ≥ T₂.bh

**f) Total time: O(log n)** - limited by tree heights

---

# **AVL Trees** (Problem 13-3)

## 🎯 **Height balancing**
For each node: $$|height(left) - height(right)| ≤ 1$$

**a) Proof of O(log n) height:**
- Minimum number of nodes in an AVL tree of height h: N(h) ≥ Fₕ (Fibonacci numbers)
- Fₕ grows exponentially ⇒ h = O(log n)

**b) BALANCE(x) procedure:**
Use **4 rotation types** as in RBT:
- Left-Left case: one right rotation
- Right-Right case: one left rotation
- Left-Right case: left + right rotation
- Right-Left case: right + left rotation

**c) AVL-INSERT:**
1. Standard BST insertion
2. Move up, recalculating heights
3. If an unbalanced node is found - apply BALANCE

**d) Analysis: O(log n) time, O(log n) rotations**
- Height O(log n) ⇒ O(log n) steps up
- Maximum O(1) rotations per level

---

## 🏆 **Structure comparison:**

| Structure           | Guarantee               | Operation Complexity   |
| ------------------- | ----------------------- | ---------------------- |
| **Red-Black Trees** | height ≤ 2log(n+1)      | O(log n)               |
| **AVL Trees**       | strict height balance   | O(log n)               |
| **Persistent RBT**  | all versions + O(log n) | O(log n) per operation |