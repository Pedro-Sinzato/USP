#include <stddef.h>
#include "avl.h"

static int height(NODE *p)
{
    return p == NULL ? 0 : p->info.height;
}

static int balance(NODE *p)
{
    return p == NULL ? 0 : height(p->left) - height(p->right);
}

static void update_height(NODE *p)
{
    int hl = height(p->left);
    int hr = height(p->right);

    p->info.height = 1 + (hl > hr ? hl : hr);
}

static NODE *rotate_right(NODE *y)
{
    NODE *x = y->left;

    y->left = x->right;
    x->right = y;

    update_height(y); /* y desceu: atualiza primeiro */
    update_height(x);

    return x;
}

static NODE *rotate_left(NODE *x)
{
    NODE *y = x->right;

    x->right = y->left;
    y->left = x;

    update_height(x);
    update_height(y);

    return y;
}

void avl_init(NODE *p)
{
    p->info.height = 1;
}

NODE *avl_rebalance(NODE *h)
{
    int fb;

    update_height(h);
    fb = balance(h);

    if(fb > 1)
    {
        if(balance(h->left) < 0)            /* caso LR */
        {
            h->left = rotate_left(h->left);
        }
        return rotate_right(h);             /* LL (ou LR depois da rotação) */
    }

    if(fb < -1)
    {
        if(balance(h->right) > 0)           /* caso RL */
        {
            h->right = rotate_right(h->right);
        }
        return rotate_left(h);              /* RR (ou RL depois da rotação) */
    }

    return h;
}
