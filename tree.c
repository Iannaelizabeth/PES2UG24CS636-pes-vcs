// tree.c (Commit 2)

#include "tree.h"
#include "index.h"
#include "pes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tree_from_index(Index *index, ObjectID *tree_id) {
    if (!index || index->count == 0) return -1;

    char buffer[4096];
    int offset = 0;

    for (int i = 0; i < index->count; i++) {
        IndexEntry *e = &index->entries[i];

        offset += sprintf(buffer + offset, "%o %s", e->mode, e->path);
        buffer[offset++] = '\0';

        memcpy(buffer + offset, e->id.hash, HASH_SIZE);
        offset += HASH_SIZE;
    }

    object_write(OBJ_TREE, buffer, offset, tree_id);
    return 0;
}
