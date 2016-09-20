#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/object.h"
#include "include/read.h"
#include "include/eval.h"
#include "include/print.h"


int main ( int argc, char *argv[] ) 
{
    char *chaine = malloc(sizeof(*chaine));
    scanf("%s",chaine);
    printf("main : %s",chaine);
    uint* here = NULL;
    *here = 1;
    printf(" %d\n",*here);
    object t = NULL;
    t = sfs_read_atom(chaine,here);
    printf("%d\n",t->type);
    
   /*exit( EXIT_SUCCESS );*/
}
