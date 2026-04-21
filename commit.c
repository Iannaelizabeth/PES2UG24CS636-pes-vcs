// commit.c (Commit 2)

#include "commit.h"
#include "tree.h"
#include "index.h"
#include "pes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int create_commit(const char *message, ObjectID *commit_id) {
    ObjectID tree_id;

    if (tree_from_index(&tree_id) != 0) {
        return -1;
    }

    printf("Tree created\n");

    return 0;
}
