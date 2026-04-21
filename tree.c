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

int tree_from_index(ObjectID *id_out) {
    Index index;

    if (index_load(&index) != 0) {
        return -1;
    }

    if (index.count == 0) {
        return -1;
    }

    qsort(index.entries, index.count, sizeof(IndexEntry), compare_entries);

    char buffer[8192];
    int offset = 0;

    for (int i = 0; i < index.count; i++) {
        IndexEntry *e = &index.entries[i];

        const char *name = strrchr(e->path, '/');
        if (name) name++;
        else name = e->path;

        offset += snprintf(buffer + offset, sizeof(buffer) - offset,
                           "%o %s", e->mode, name);

        buffer[offset++] = '\0';

        memcpy(buffer + offset, e->hash.hash, HASH_SIZE);
        offset += HASH_SIZE;
    }

    object_write(OBJ_TREE, buffer, offset, id_out);

    return 0;
}
