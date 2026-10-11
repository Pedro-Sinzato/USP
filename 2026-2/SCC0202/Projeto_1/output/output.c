#include <stdio.h>
#include <stdbool.h>

#include "output.h"
#include "../leitor_argumentos/argument_reader.h"

extern bool output_result(ARGUMENTS *arguments)
{
    FILE *result = NULL;
    result = fopen(get_result(arguments), "wb");

    if(result == NULL)
    {
        return false;
    }
    //Ler arquivo.
    return true;
}
