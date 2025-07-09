public class DisjointSet {
    private int[] parent;
    private int[] rank; // For union by rank optimization

    // Constructor to initialize Disjoint Set
    public DisjointSet(int size) {
        parent = new int[size];
        rank = new int[size];

        // Initially, each element is its own parent
        for (int i = 0; i < size; i++) {
            parent[i] = i;
            rank[i] = 0; // Initial rank is 0
        }
    }

    // Find with path compression
    public int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Path compression
        }
        return parent[x];
    }

    // Union by rank
    public void union(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) return; // Already in same set

        // Attach smaller rank tree under root of higher rank tree
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            // If ranks are same, choose one as root and increment its rank
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }

    // Check if two elements are in the same set
    public boolean isConnected(int x, int y) {
        return find(x) == find(y);
    }
}