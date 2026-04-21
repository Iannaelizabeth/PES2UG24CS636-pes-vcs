#include "index.h"
#include "tree.h"
#include "pes.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

void cmd_init() {
    mkdir(".pes", 0755);
    mkdir(".pes/objects", 0755);

    FILE *f = fopen(".pes/index", "w");
    if (f) fclose(f);

    printf("Initialized empty PES repository\n");
}

void cmd_add(int argc, char *argv[]) {
    Index index;
    index_load(&index);

    for (int i = 2; i < argc; i++) {
        index_add(&index, argv[i]);
    }

    index_save(&index);
}

void cmd_status() {
    Index index;
    index_load(&index);

    printf("Staged files:\n");
    for (int i = 0; i < index.count; i++) {
        printf("%s\n", index.entries[i].path);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./pes <command>\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0) {
        cmd_init();
    }
    else if (strcmp(argv[1], "add") == 0) {
        cmd_add(argc, argv);
    }
    else if (strcmp(argv[1], "status") == 0) {
        cmd_status();
    }
    else {
        printf("Unknown command\n");
    }

    return 0;
}
