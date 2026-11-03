#include <stdio.h>
#include <stdlib.h>

#include "search.h"

typedef struct
{
    const QUERY *query;
    SIM **sims;       /* referências aos SIMs da QUERY */
    int n_sims;
    bool use_accept;
    double accept;

    ITEM **found;
    size_t n_found;
    size_t capacity;
    size_t n_scanned;
    bool ok;
} SEARCH_CTX;

static bool search_visit(const ITEM *item, void *ctx_)
{
    SEARCH_CTX *ctx = ctx_;
    ctx->n_scanned++;

    if (!apply_filters(item, ctx->query))
    {
        return true;
    }

    if (ctx->use_accept && total_similarity(ctx->sims, ctx->n_sims, item) < ctx->accept)
    {
        return true;
    }

    if (ctx->n_found == ctx->capacity)
    {
        size_t nova = (ctx->capacity == 0) ? 64 : ctx->capacity * 2;
        ITEM **novo = realloc(ctx->found, nova * sizeof(ITEM *));
        if (novo == NULL)
        {
            ctx->ok = false;
            return false;
        }
        ctx->found = novo;
        ctx->capacity = nova;
    }

    ctx->found[ctx->n_found++] = (ITEM *)item;
    return true;
}

bool search_ed(const ED *base, const QUERY *query, ITEM ***found, size_t *n_found, size_t *n_scanned)
{
    if (found != NULL)
    {
        *found = NULL;
    }
    if (n_found != NULL)
    {
        *n_found = 0;
    }
    if (n_scanned != NULL)
    {
        *n_scanned = 0;
    }
    if (base == NULL || query == NULL || found == NULL || n_found == NULL)
    {
        return false;
    }

    SEARCH_CTX ctx = {0};
    ctx.query = query;
    ctx.ok = true;
    ctx.n_sims = query_n_sims(query);

    /* Sem SIM os filtros bastam e o ACCEPT não tem o que avaliar. */
    if (ctx.n_sims > 0 && query_has_accept(query))
    {
        ctx.sims = calloc((size_t)ctx.n_sims, sizeof(SIM *));
        if (ctx.sims == NULL)
        {
            return false;
        }
        for (int i = 0; i < ctx.n_sims; i++)
        {
            ctx.sims[i] = query_get_sim(query, i);
        }
        ctx.use_accept = true;
        ctx.accept = query_accept(query);
    }

    for_each_ed(base, search_visit, &ctx);

    free(ctx.sims);

    if (!ctx.ok)
    {
        free(ctx.found);
        return false;
    }

    *found = ctx.found;
    *n_found = ctx.n_found;
    if (n_scanned != NULL)
    {
        *n_scanned = ctx.n_scanned;
    }
    return true;
}

bool insert_found(ED *result, ITEM *const *found, size_t n_found)
{
    if (result == NULL || (found == NULL && n_found > 0))
    {
        return false;
    }

    for (size_t i = 0; i < n_found; i++)
    {
        ITEM *copia = item_clone(found[i]);
        if (copia == NULL)
        {
            return false;
        }
        if (!insert_ed(result, copia))
        {
            delete_item(&copia);
            return false;
        }
    }
    return true;
}
