#include <stdio.h>
#include <stdlib.h>

#include "myers_diff.h"

// Path history
typedef struct Path_s{
    struct Path_s *next;
    int xs;
    int ys;
    int xt;
    int yt;
} Path_t;

static
Path_t *path_new(int xs, int ys, int xt, int yt) {
    Path_t *p_node = (Path_t*)calloc(1, sizeof(Path_t));
    p_node->xs = xs;
    p_node->ys = ys;
    p_node->xt = xt;
    p_node->yt = yt;
    return p_node;
}

static
void path_free(Path_t *p_node) {
    if (!p_node) {
        return;
    }
    
    free(p_node);
}

static
void path_prepend(Path_t **head, Path_t *p_node) {
    if (!head || !p_node) {
        return;
    }
    
    p_node->next = *head;
    *head = p_node;
}

// Myers diff core
Diff_t* myers_diff(char *a, int len_a, char *b, int len_b, int *diff_count)
{
    int max_d = len_a + len_b; // Possible longest edit distance
    int *v = (int *)calloc((2 * max_d + 1), sizeof(int));
    Path_t *head = NULL;
    Diff_t *diffs = NULL;
    int max_diff = 0;
    int d = 0;

    for (d = 0; d <= max_d; d++) {
        int k = 0;
        // printf("D: %d\n", d);
        
        for (k = -d; k <= d; k += 2) {
            int down = k == -d || (k != d && v[k - 1 + max_d] < v[k + 1 + max_d]);
            int prev_k = k + (down ? 1 : -1);
            int xs = v[max_d + prev_k];
            int ys = xs - prev_k;
            int xt = down ? xs : xs + 1;
            int yt = xt - k;
            Path_t *p_node = NULL;
            
            // printf("k: %d, ", k);
            
            while (xt < len_a && yt < len_b && a[xt] == b[yt]) {
                xt++;
                yt++;
            }
            
            v[max_d + k] = xt;
            
            // Append Path
            p_node = path_new(xs, ys, xt, yt);
            if (!p_node) {
                goto END;
            }
            path_prepend(&head, p_node);
            
            // printf("%d, (%d, %d) -> (%d, %d)\n", prev_k, xs, ys, xt, yt);
            
            if (xt >= len_a && yt >= len_b) {
                goto FOUND;
            }
        }
    }

FOUND:
    // traceback Path
    max_diff = (len_a + len_b + d) / 2;
    *diff_count = 0;
    diffs = (Diff_t *)calloc(max_diff, sizeof(Diff_t));

    if (head) {
        int cur_x = len_a;
        int cur_y = len_b;
        Path_t *tmp = NULL;
        while ((tmp = head->next) != NULL) {
            if (head->xt == cur_x && head->yt == cur_y) {
                int xd = cur_x - head->xs;
                int yd = cur_y - head->ys;
                cur_x = head->xs;
                cur_y = head->ys;
                
                while (xd > 0 && yd > 0) {
                    xd--;
                    yd--;
                    (*diff_count)++;
                    diffs[max_diff - *diff_count].type = ' ';
                    diffs[max_diff - *diff_count].text = a[cur_x + xd];
                    // printf("%d (' ', '%c')\n", *diff_count, a[cur_x + xd]);
                }
                if (xd + yd) {
                    (*diff_count)++;
                    if (xd == 1) {
                        diffs[max_diff - *diff_count].type = '-';
                        diffs[max_diff - *diff_count].text = a[cur_x];
                        // printf("%d ('-', '%c')\n", *diff_count, a[cur_x]);
                    }
                    else if (yd == 1) {
                        diffs[max_diff - *diff_count].type = '+';
                        diffs[max_diff - *diff_count].text = b[cur_y];
                        // printf("%d ('+', '%c')\n", *diff_count, b[cur_y]);
                    }
                    else {
                        printf("Unexpect result!");
                        free(diffs);
                        diffs = NULL;
                        goto END;
                    }
                }
                // last walk
            }
            
            path_free(head);
            head = tmp;
        }
        path_free(head);
        head = NULL;
        
        if (cur_x != cur_y) {
            printf("Unexpect result!");
            free(diffs);
            diffs = NULL;
            goto END;
        }
        
        if (cur_x > 0) {
            int i = 0;
            for (i = cur_x; i > 0; i--) {
                (*diff_count)++;
                diffs[max_diff - *diff_count].type = ' ';
                diffs[max_diff - *diff_count].text = a[i - 1];
                // printf("%d (' ', '%c')\n", *diff_count, a[i - 1]);
            } 
        }
    }
    
    if ((*diff_count) != max_diff) {
        printf("unexpected diff count: %d, expect %d\n", *diff_count, max_diff);
    }
    
END:
    if (head) {
        Path_t *tmp = NULL;
        while ((tmp = head->next) != NULL) {
            path_free(head);
            head = tmp;
        }
        path_free(head);
        head = NULL;
    }
    free(v);
    return diffs;
}