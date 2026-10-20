#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "reader.h"
#include "estruturas_dados/item.h"
#include "leitor_argumentos/argument_reader.h"
#include "estruturas_dados/stack/stack.h"

extern bool *reader(STACK *stack,ARGUMENTS *arg, DATATYPE tipos, int n_campos)
{
    FILE *database = fopen(get_input(arg), "r");
    if(database == NULL)
    {
        return false;
    }

    char line[1024];
    while (fgets(line, sizeof(line), database))
    {
        //Remove \n se tiver
        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, ",");

        int idx = 0;
        ITEM *new_item = create_item(n_campos, &tipos);
        while(token != NULL)
        {
            item_set(new_item, token, idx);
            token = strtok(NULL, ",");
            idx++;
        }
        stack_insert(stack, new_item);
    }

    fclose(database);
    return true;
}
