#include "code.h"

void init_lsm_tree(LSMTree *tree)
{
    tree->memtable.pairs = malloc(MEMTABLE_SIZE * sizeof(KeyValuePair));
    if (!tree->memtable.pairs)
    {
        perror("Failed to allocate memtable");
        exit(EXIT_FAILURE);
    }

    tree->memtable.size = 0;
    tree->memtable.capacity = MEMTABLE_SIZE;
    tree->sstable_count = 0;

    for (int i = 0; i < MAX_SSTABLES; i++)
    {
        tree->sstables[i].pairs = NULL;
        tree->sstables[i].size = 0;

        /*
        int snprintf(char *str, size_t size, const char *format, ...); (stdio)
        The snprintf function in C is part of the stdio.h standard library and is
        used for formatted output to a character array (buffer) with a specified
        size limit. It is a safer alternative to sprintf because it prevents buffer
        overflows by ensuring that no more than a specified number of characters
        are written to the destination buffer.
        */
        snprintf(tree->sstables[i].filename, sizeof(tree->sstables[i].filename), "sstable%d.dat", i);
    }

    printf("LSM-tree initialized with memtable capacity %d\n", MEMTABLE_SIZE);
}

void free_lsm_tree(LSMTree *tree)
{
    free(tree->memtable.pairs);

    for (int i = 0; i < tree->sstable_count; i++)
    {
        free(tree->sstables[i].pairs);
    }

    printf("LSM-tree resources freed\n");
}

int compare_keys(const void *a, const void *b)
{
    const KeyValuePair *pair_a = (const KeyValuePair *)a;
    const KeyValuePair *pair_b = (const KeyValuePair *)b;

    /*
    int strcmp(const char *s1, const char *s2);
    int strncmp(const char s1[.n], const char s2[.n], size_t n);

    The strcmp() function compares the two strings s1 and s2.  The
       locale is not taken into account (for a locale-aware comparison,
       see strcoll(3)).  The comparison is done using unsigned
       characters.
    */
    return strcmp(pair_a->key, pair_b->key);
}

void lsm_put(LSMTree *tree, const char *key, const char *value)
{
    if (tree->memtable.size >= tree->memtable.capacity)
    {
        printf("Memtable full, flushing to SSTable...\n");
        flush_memtable_to_sstable(tree); // Dumping data to disk
    }

    KeyValuePair new_pair;

    /*
    char *strncpy(char *dest, const char *src, size_t n);
    */
    strncpy(new_pair.key, key, KEY_SIZE - 1);
    new_pair.key[KEY_SIZE - 1] = '\0'; // safe
    strncpy(new_pair.value, value, VALUE_SIZE - 1);
    new_pair.value[VALUE_SIZE - 1] = '\0';

    tree->memtable.pairs[tree->memtable.size] = new_pair;
    tree->memtable.size++;

    printf("Put: key='%s', value='%s'\n", key, value);
}

void flush_memtable_to_sstable(LSMTree *tree)
{
    if (tree->memtable.size == 0)
    {
        printf("Memtable is empty, nothing to flush\n");
        return;
    }

    if (tree->sstable_count >= MAX_SSTABLES)
    {
        printf("Max SSTables reached, compacting...\n");
        compact_sstables(tree);
    }

    /*
    void qsort(void base[.size * .n], size_t n, size_t size,
                  typeof(int (const void [.size], const void [.size]))
                      *compar);
       void qsort_r(void base[.size * .n], size_t n, size_t size,
                  typeof(int (const void [.size], const void [.size], void *))
                      *compar,
                  void *arg);
    */
    qsort(tree->memtable.pairs, tree->memtable.size, sizeof(KeyValuePair), compare_keys);

    SSTable *new_sstable = &tree->sstables[tree->sstable_count];

    new_sstable->pairs = malloc(tree->memtable.size * sizeof(KeyValuePair));
    if (!new_sstable->pairs)
    {
        perror("Failed to allocate SSTable");
        exit(EXIT_FAILURE);
    }
    new_sstable->size = tree->memtable.size;

    for (int i = 0; i < tree->memtable.size; i++)
    {
        strncpy(new_sstable->pairs[i].key, tree->memtable.pairs[i].key, KEY_SIZE);
        strncpy(new_sstable->pairs[i].value, tree->memtable.pairs[i].value, VALUE_SIZE);
    }

    save_sstable_to_disk(new_sstable);

    tree->sstable_count++;
    tree->memtable.size = 0;
}

void save_sstable_to_disk(SSTable *sstable)
{
    /*
    FILE *fopen(const char *restrict pathname, const char *restrict mode);
    FILE *fdopen(int fd, const char *mode);
    FILE *freopen(const char *restrict pathname, const char *restrict mode,
                    FILE *restrict stream);
    */
    FILE *file = fopen(sstable->filename, "wb");
    if (!file)
    {
        perror("Failed to open SSTable file for writing");
        exit(EXIT_FAILURE);
    }

    /*
    size_t fread(void *ptr, size_t size, size_t nmembFILE *" stream );

    size_t fwrite(const void *ptr, size_t size, size_t nmemb,
    FILE *stream);
    */
    fwrite(&sstable->size, sizeof(int), 1, file);
    fwrite(sstable->pairs, sizeof(KeyValuePair), sstable->size, file);

    fclose(file);

    printf("SSTable saved to %s (%d entries)\n", sstable->filename, sstable->size);
}

void load_sstable_from_disk(SSTable *sstable, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("Failed to open SSTable file for reading");
        exit(EXIT_FAILURE);
    }

    /*
        size_t fread(void ptr[restrict .size * .n],
                    size_t size, size_t n,
                    FILE *restrict stream);
        size_t fwrite(const void ptr[restrict .size * .n],
                    size_t size, size_t n,
                    FILE *restrict stream);

        the best practices
        uint32_t size;  // fixed size
        fread(&size, sizeof(uint32_t), 1, file);
    */
    fread(&sstable->size, sizeof(int), 1, file);

    /*
    void *malloc(size_t size);
       void free(void *_Nullable ptr);
       void *calloc(size_t n, size_t size);
       void *realloc(void *_Nullable ptr, size_t size);
       void *reallocarray(void *_Nullable ptr, size_t n, size_t size);
    */
    sstable->pairs = malloc(sstable->size * sizeof(KeyValuePair));
    if (!sstable->pairs)
    {
        perror("Failed to allocate SSTable");
        exit(EXIT_FAILURE);
    }

    //  sstable->size - quatity
    fread(sstable->pairs, sizeof(KeyValuePair), sstable->size, file);

    strncpy(sstable->filename, filename, sizeof(sstable->filename) - 1);

    fclose(file);

    printf("SSTable loaded from %s (%d entries)\n", filename, sstable->size);
}

char *lsm_get(LSMTree *tree, const char *key)
{
    for (int i = tree->memtable.size - 1; i >= 0; i--)
    {
        if (strcmp(tree->memtable.pairs[i].key, key) == 0)
        {
            printf("Found in memtable: key='%s', value='%s'\n",
                   key, tree->memtable.pairs[i].value);
            /*
            char *strdup(const char *s);

            char *strndup(const char s[.n], size_t n);
            char *strdupa(const char *s);
            char *strndupa(const char s[.n], size_t n)

            The strdup() function returns a pointer to a new string which is a
            duplicate of the string s.  Memory for the new string is obtained
            with malloc(3), and can be freed with free(3).
            */
            return strdup(tree->memtable.pairs[i].value); // return copy
        }
    }

    for (int i = tree->sstable_count - 1; i >= 0; i--)
    {
        SSTable *sstable = &tree->sstables[i];
        int left = 0;
        int right = sstable->size - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            /*
            int strcmp(const char *s1, const char *s2);
            int strncmp(const char s1[.n], const char s2[.n], size_t n);
            */
            int cmp = strcmp(sstable->pairs[mid].key, key);

            if (cmp == 0)
            { // finded key
                printf("Found in SSTable %d: key='%s', value='%s'\n",
                       i, key, sstable->pairs[mid].value);
                return strdup(sstable->pairs[mid].value);
            }
            else if (cmp < 0)
            { // The key you are looking for is bigger
                left = mid + 1;
            }
            else
            { // The key you are looking for is less
                right = mid - 1;
            }
        }
    }

    printf("Key not found: '%s'\n", key);
    return NULL;
}

void compact_sstables(LSMTree *tree)
{
    if (tree->sstable_count < 2)
    {
        printf("Not enough SSTables to compact (%d)\n", tree->sstable_count);
        return;
    }

    printf("Compacting %d SSTables...\n", tree->sstable_count);

    int total_pairs = 0;
    for (int i = 0; i < tree->sstable_count; i++)
    {
        total_pairs += tree->sstables[i].size;
    }

    KeyValuePair *all_pairs = malloc(total_pairs * sizeof(KeyValuePair));
    if (!all_pairs)
    {
        perror("Failed to allocate memory for compaction");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int i = 0; i < tree->sstable_count; i++)
    {
        for (int j = 0; j < tree->sstables[i].size; j++)
        {
            all_pairs[index++] = tree->sstables[i].pairs[j];
        }
    }

    qsort(all_pairs, total_pairs, sizeof(KeyValuePair), compare_keys);

    // delete dublicates
    int unique_count = 0;
    for (int i = 0; i < total_pairs; i++)
    {
        if (i == 0 || strcmp(all_pairs[i].key, all_pairs[i - 1].key) != 0)
        {
            all_pairs[unique_count++] = all_pairs[i];
        }
    }

    // Freeing up the memory of the old SSTables
    for (int i = 0; i < tree->sstable_count; i++)
    {
        free(tree->sstables[i].pairs);
        tree->sstables[i].pairs = NULL;
        tree->sstables[i].size = 0;
    }

    // Creating a new compact SSTable
    tree->sstables[0].pairs = malloc(unique_count * sizeof(KeyValuePair));
    if (!tree->sstables[0].pairs)
    {
        perror("Failed to allocate memory for compacted SSTable");
        exit(EXIT_FAILURE);
    }

    memcpy(tree->sstables[0].pairs, all_pairs, unique_count * sizeof(KeyValuePair));
    tree->sstables[0].size = unique_count;
    tree->sstable_count = 1;

    save_sstable_to_disk(&tree->sstables[0]);

    free(all_pairs);

    printf("Compaction complete. %d unique pairs in new SSTable\n", unique_count);
}

int main()
{
    LSMTree tree;
    init_lsm_tree(&tree);

    lsm_put(&tree, "name", "Alice");
    lsm_put(&tree, "age", "30");
    lsm_put(&tree, "city", "New York");
    lsm_put(&tree, "job", "Engineer");
    lsm_put(&tree, "language", "C");

    lsm_put(&tree, "hobby", "Hiking");

    char *value;

    value = lsm_get(&tree, "name");
    if (value)
    {
        printf("Retrieved: name = %s\n", value);
        free(value);
    }

    free_lsm_tree(&tree);
    return 0;
}