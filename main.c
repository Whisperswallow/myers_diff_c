#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myers_diff.h"

int main() {
    // char a[] = "BABD";
    // char b[] = "CABE";
    char a[] = "Selection";
    char b[] = "Setting";
    int len_a = strlen(a);
    int len_b = strlen(b);
    int diff_count;

    Diff_t *diffs = myers_diff(a, len_a, b, len_b, &diff_count);
    if (diffs) {
        for (int i = 0; i < diff_count; i++) {
            printf("%c %c\n", diffs[i].type, diffs[i].text);
        }
    
        free(diffs);
    }
    else {
        printf("Myers diff error!\n");
    }
    return 0;
}