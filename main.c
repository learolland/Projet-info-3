#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "include/mem.h"
#include "include/read.h"
#include "include/eval.h"
#include "include/print.h"


int main ( int argc, char *argv[] )
{
    char *chaine = malloc(sizeof(*chaine));
    scanf("%s",chaine);

    uint* here = malloc(sizeof(*here));
    *here = 0;

    object t = sfs_malloc(sizeof(*t));
    t = read(chaine);
    afficher_objet(t);
    printf("fin \n");

   /*exit( EXIT_SUCCESS );*/
}
