#ifndef _KDNODE_H
#define _KDNODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifdef KDTREE_IMPLEMENTATION
inline float dist(kdNode_t *a, kdNode_t *b)
{
    #if DIM == 2
        return (a->x[0] - b->x[0]) + (a->x[1] - b->x[1]);
    #elif DIM == 3
        return (a->x[0] - b->x[0]) + (a->x[1] - b->x[1]) + (a->x[2] - b->x[2]);
    #endif
}

inline void swap(kdNode_t *x, kdNode_t *y)
{
    float tmp[DIM];
    memcpy(tmp,  x->x, sizeof(tmp));
    memcpy(x->x, y->x, sizeof(tmp));
    memcpy(y->x, tmp,  sizeof(tmp));
}


kdNode_t *find_median(kdNode_t *start, kdNode_t *end, int idx)
{
    if (end <= start)
        return NULL;
    if (end == start + 1)
        return start;

    kdNode_t *p, *store, *md = start + (end - start) / 2;
    float pivot;
    while (1) {
        pivot = md->x[idx];

        swap(md, end - 1);
        for (store = p = start; p < end; p++) {
            if (p->x[idx] < pivot) {
                if (p != store)
                    swap(p, store);
                store++;
            }
        }
        swap(store, end - 1);

        /* median has duplicate values */
        if (store->x[idx] == md->x[idx])
            return md;

        if (store > md)
            end = store;
        else
            start = store;
    }
}

kdNode_t *make_tree(kdNode_t *t, int len, int i, int dim)
{
    kdNode_t *n;

    if (!len) return 0;

    if ((n = find_median(t, t + len, i))) {
        i = (i + 1) % dim;
        n->left  = make_tree(t, n - t, i, dim);
        n->right = make_tree(n + 1, t + len - (n + 1), i, dim);
    }
    return n;
}

void nearest(kdNode_t *root, kdNode_t *nd, int i, int dim, kdNode_t **best, double *best_dist)
{
    double d, dx, dx2;

    if (!root) return;
    d = dist(root, nd);
    dx = root->x[i] - nd->x[i];
    dx2 = dx * dx;

    visited++;

    if (!*best || d < *best_dist) {
        *best_dist = d;
        *best = root;
    }

    /* if chance of exact match is high */
    if (!*best_dist)
        return;

    if (++i >= dim)
        i = 0;

    nearest(dx > 0 ? root->left : root->right, nd, i, dim, best, best_dist);
    if (dx2 >= *best_dist)
        return;
    nearest(dx > 0 ? root->right : root->left, nd, i, dim, best, best_dist);
}
#endif // KDTREE_IMPLEMENTATION

#endif // _KDNODE_H
