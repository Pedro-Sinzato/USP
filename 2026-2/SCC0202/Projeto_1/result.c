#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "leitor_argumentos/argument_reader.h"
#include "estruturas_dados/item.h"

bool save_struct(ARGUMENTS *arguments, void**fields, DATATYPE dataypes[30])
{
    FILE *result = fopen(get_result(arguments), "w");

    for(int i = 0; i < 30; i++)
    {
        if(dataypes[i] == ITEM_INT)
        {
            fprintf(result,"%d,", (int)fields[i]); //Tá certo?
        }
        else if(dataypes[i] == ITEM_DOUBLE)
        {
            fprintf(result, "%.3lf", (double)fields[i]); //Provávelmente não
        }
        else if(dataypes[i] == ITEM_CHAR)
        {
            fprintf(result, "%c,",(char)fields[i]);
        }
        fprintf(result, "\n");
    }

    fclose(result);
    result = NULL;

    return true;
}
