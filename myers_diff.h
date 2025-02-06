#ifndef __MYERS_DIFF__
#define __MYERS_DIFF__

// Diff result
typedef struct {
    char type; // ' ' => equal, '+' => insert, '-' => delete
    char text;
} Diff_t;

Diff_t* myers_diff(char *a, int len_a, char *b, int len_b, int *diff_count);

#endif