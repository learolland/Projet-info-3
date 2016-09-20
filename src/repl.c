
/**
 * @file repl.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:13:02 2012
 * @brief Main REPL for SFS.
 *
 * Main REPL for SFS.
 */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "src/object.h"
#include "src/read.h"
#include "src/eval.h"
#include "src/print.h"



object nil;

void init_interpreter ( void ) 
{

    nil      = make_nil();

}

int main ( int argc, char *argv[] ) 
{
    char* chaine = malloc(sizeof(chaine));
    *chaine = "(152)";
    uint *here = 1;
    object t = sfs_read_atom(chaine,here);
    
    exit( EXIT_SUCCESS );
}
