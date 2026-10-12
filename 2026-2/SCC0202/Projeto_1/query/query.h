#ifndef QUERY_H
#define QUERY_H

#include <stdbool.h>
#include "campo.h"

typedef struct query QUERY;

typedef enum ordem { ORDEM_ASC, ORDEM_DESC } ORDEM;

extern QUERY *query_reader(const char *query_name);
extern bool delete_query(QUERY *query);
extern void print_query(const QUERY *query);

/* ACCEPT */
extern bool   query_has_accept(const QUERY *query);
extern double query_accept(const QUERY *query);
/* ORDER */
extern bool  query_has_order(const QUERY *query);
extern bool  query_order_by_similarity(const QUERY *query);
extern CAMPO query_order_campo(const QUERY *query);
extern ORDEM query_order_dir(const QUERY *query);
/* GROUP */
extern int   query_n_group(const QUERY *query);
extern CAMPO query_group(const QUERY *query, int i);   // i = 0 é o nível mais externo
/* CROSS */
extern bool  query_has_cross(const QUERY *query);
extern CAMPO query_cross(const QUERY *query, int i);   // i = 0 ou 1

#endif
