
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "read.h"
#include "print.h"


/* Globals */
object * nil;
object * boolean_true;
object * boolean_false;





void sfs_print_atom( object p )
{
	if (p->type != SFS_NIL)
	{
		if(p->type == 0)
		{
			printf("%d ",p->this.number.this.integer );
			return;
		}
		if(p->type == 1)
		{
			printf("%c ",p->this.character);
			return;
		}
		if(p->type == 2)
		{
			printf("%s ",p->this.string);
			return;
		}
		if(p->type == 4)
		{
			printf("() ");
			return;
		}
		if(p->type == 5)
		{
			printf("booléen (à gérer) ");
			return;
		}
		if(p->type == 6)
		{
			printf("%s ",p->this.symbol);
			return;
		}
	}

    return;
}

void sfs_print_pair( object o )
{
    while (o!= NULL && o->this.pair.car !=NULL)
    {
		/*printf("type suivant : %d\n", o->this.pair.cdr->this.pair.car->type);

		if(o->this.pair.cdr->type == 3)*/
		sfs_print(o->this.pair.car);
		o = o->this.pair.cdr ;
    }

    return;
}

void sfs_print( object o )
{
    if ( SFS_PAIR == o->type )
    {
		printf("(");
        sfs_print_pair( o );
		printf(" )");
    }
    else
    {
        sfs_print_atom( o );
    }


}
