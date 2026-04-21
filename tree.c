// tree.c (Commit 5 FINAL)

#include "tree.h"
#include "index.h"
#include "pes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_entries(const void *a, const void *b) {
    IndexEntry *ea = (IndexEntry *)a;
    IndexEntry *eb = (IndexEntry *)b;
    return strcmp(ea->path, eb->path);
}

int tree_from_index(Index *index, ObjectID *tree_id) {
    if (!index || index->count == 0) return -1;

    qsort(index->entries, index->count, sizeof(IndexEntry), compare_entries);

    char buffer[8192];
    int offset = 0;

    for (int i = 0; i < index->count; i++) {
        IndexEntry *e = &index->entries[i];

        char *name = strrchr(e->path, '/');
        if (name) name++;
        else name = e->path;

        offset += snprintf(buffer + offset, sizeof(buffer) - offset,
                           "%o %s", e->mode, name);

        buffer[offset++] = '\0';

        memcpy(buffer + offset, e->id.hash, HASH_SIZE);
        offset += HASH_SIZE;
    }

    object_write(OBJ_TREE, buffer, offset, tree_id);
    return 0;
}
