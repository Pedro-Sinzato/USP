#ifndef ITEM_H
#define ITEM_H


typedef enum datatype
{
    ITEM_INT,
    ITEM_DOUBLE,
    ITEM_CHAR,
    ITEM_PCHAR,
    ITEM_BOOL
}DATATYPE;

typedef struct item ITEM;

extern ITEM **create_item(int n_fields,DATATYPE types[]);
extern bool delete_item(ITEM **item);

#endif
