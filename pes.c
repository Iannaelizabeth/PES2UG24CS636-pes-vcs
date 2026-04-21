#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void init_repo() {
    mkdir(".pes", 0777);
    mkdir(".pes/objects", 0777);
    mkdir(".pes/refs", 0777);
    mkdir(".pes/refs/heads", 0777);

    FILE *f;

    f = fopen(".pes/HEAD", "w");
    fprintf(f, "refs/heads/main\n");
    fclose(f);

    f = fopen(".pes/refs/heads/main", "w");
    fclose(f);

    f = fopen(".pes/index", "w");
    fclose(f);

    printf("Repository structure created\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./pes <command>\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0) {
        init_repo();
    } else {
        printf("Unknown command\n");
    }

    return 0;
}
