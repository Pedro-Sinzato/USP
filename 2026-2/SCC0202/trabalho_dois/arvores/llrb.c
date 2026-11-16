#include <stddef.h>
#include "llrb.h"

static int is_red(NODE *p)
{
    return p != NULL && p->info.color == RED;
}

static NODE *rotate_left(NODE *h)
{
    NODE *x = h->right;

    h->right = x->left;
    x->left = h;
    x->info.color = h->info.color;
    h->info.color = RED;

    return x;
}

static NODE *rotate_right(NODE *h)
{
    NODE *x = h->left;

    h->left = x->right;
    x->right = h;
    x->info.color = h->info.color;
    h->info.color = RED;

    return x;
}

static void color_flip(NODE *h)
{
    h->info.color = !h->info.color;
    h->left->info.color = !h->left->info.color;
    h->right->info.color = !h->right->info.color;
}

void llrb_init(NODE *p)
{
    p->info.color = RED;
}

void llrb_split(NODE *h)
{
    if(is_red(h->left) && is_red(h->right))
    {
        color_flip(h);
    }
}

NODE *llrb_fixup(NODE *h)
{
    if(is_red(h->right) && !is_red(h->left))
    {
        h = rotate_left(h);
    }
    if(is_red(h->left) && is_red(h->left->left))
    {
        h = rotate_right(h);
    }
    return h;
}

NODE *llrb_blacken(NODE *root)
{
    if(root != NULL)
    {
        root->info.color = BLACK;
    }
    return root;
}
