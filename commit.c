// commit.c (Commit 4)

#include "commit.h"
#include "tree.h"
#include "index.h"
#include "pes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int create_commit(const char *message, ObjectID *commit_id) {
    ObjectID tree_id;
    tree_from_index(&tree_id);

    char tree_hex[65];
    hash_to_hex(&tree_id, tree_hex);

    char buffer[1024];

    int len = snprintf(buffer, sizeof(buffer),
        "tree %s\n\n%s\n",
        tree_hex,
        message
    );

    object_write(OBJ_COMMIT, buffer, len, commit_id);

    printf("Commit object written\n");

    return 0;
}
