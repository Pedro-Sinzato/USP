#ifndef READER_H
#define READER_H

#include "estruturas_dados/stack/stack.h"
#include "leitor_argumentos/argument_reader.h"
#include "estruturas_dados/item.h"

extern bool *reader(STACK *stack,ARGUMENTS *arg, DATATYPE tipos, int n_campos);

#endif
