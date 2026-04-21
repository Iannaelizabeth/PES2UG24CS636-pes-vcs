// commit.c (Commit 5 FINAL)

#include "commit.h"
#include "tree.h"
#include "index.h"
#include "pes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int create_commit(const char *message, ObjectID *commit_id) {
    ObjectID tree_id;
    if (tree_from_index(&tree_id) != 0) return -1;

    char tree_hex[65];
    hash_to_hex(&tree_id, tree_hex);

    char buffer[1024];

    int len = snprintf(buffer, sizeof(buffer),
        "tree %s\n\n%s\n",
        tree_hex,
        message
    );

    if (object_write(OBJ_COMMIT, buffer, len, commit_id) != 0) {
        return -1;
    }

    // update HEAD
    char commit_hex[65];
    hash_to_hex(commit_id, commit_hex);

    FILE *f = fopen(".pes/HEAD", "w");
    if (f) {
        fprintf(f, "%s\n", commit_hex);
        fclose(f);
    }

    printf("Committed: %s\n", commit_hex);

    return 0;
}
