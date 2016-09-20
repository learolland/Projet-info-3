
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


#include "include/object.h"
#include "include/read.h"
#include "include/eval.h"
#include "include/print.h"



/*object nil;

void init_interpreter ( void ) 
{

    nil      = make_nil();

}*/

int main ( int argc, char *argv[] ) 
{
    char *chaine = malloc(sizeof(*chaine));
    *chaine = "512";
    /*string tmp = "(152)";
    strcpy(*chaine,tmp);*/
    printf("main : %s\n",chaine);
    uint here = 1;
    object t ;
    t = sfs_read_atom(&chaine,&here);
    
   /*exit( EXIT_SUCCESS );*/
}
