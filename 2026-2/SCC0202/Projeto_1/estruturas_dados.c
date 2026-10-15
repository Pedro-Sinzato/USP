#include <stdlib.h>
#include <stdbool.h>

#include "estruturas_dados.h"
#include "estruturas_dados/stack/stack.h"

//Basicamento implementando C++ manualmente em C
//Ver Vtable

ED *create_ed(ED_TYPES type)
{
    switch (type) {
        case PILHA: return (ED *)create_stack();
        default:    return NULL;
    }
}
void destroy_ed(ED **self)
{
    if(self == NULL || *self == NULL)
    {
        return;
    }
    (*self)->ed_vtable->destroy_ed(self);
    *self = NULL;
}
