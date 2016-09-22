#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/object.h"
#include "include/mem.h"
#include "include/read.h"
#include "include/eval.h"
#include "include/print.h"


int main ( int argc, char *argv[] ) 
{
    char *chaine = malloc(sizeof(*chaine));
   /* scanf("%s",chaine);*/
   
    uint* here = malloc(sizeof(*here));
    *here = 0;
    printf(" %d\n",*here);
    object t = sfs_malloc(sizeof(*t));
    t = sfs_read_atom(chaine,here);
    printf("fin\n");
    
   /*exit( EXIT_SUCCESS );*/
}
