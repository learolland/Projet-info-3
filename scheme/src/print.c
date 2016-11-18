
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "read.h"
#include "print.h"








void sfs_print_atom( object p )
{

		if(p->type == SFS_NUMBER)
		{
			printf("%d",p->this.number.this.integer );
			return;
		}
		if(p->type == SFS_CHARACTER)
		{
			printf("#\\%c",p->this.character);
			return;
		}
		if(p->type == SFS_STRING)
		{
			printf("\"%s\"",p->this.string);
			return;
		}
		if(p->type == SFS_NIL)
		{
			printf("()");
			return;
		}
		if(p->type == SFS_BOOLEAN)
		{
			printf("%s",p->this.string);
			return;
		}
		if(p->type == SFS_SYMBOL)
		{
			printf("%s",p->this.symbol);
			return;
		}
		if(p->type == SFS_CHAR_SPECIAL)
		{
			printf("%s",p->this.string);
			return;
		}
    return;
}

void sfs_print_pair( object o )
{
    if (o!= NULL  && o->this.pair.car != nil)
    {

		/*if(o->this.pair.cdr->type == 3)*/
		DEBUG_MSG("this.pair.car->type = %d",o->this.pair.car->type);

		sfs_print(o->this.pair.car);
		DEBUG_MSG("retour");

		if(o->this.pair.cdr != nil && o->this.pair.cdr != NULL)
		{
			o = o->this.pair.cdr ;
			DEBUG_MSG("2 :this.pair.car->type = %d",o->this.pair.car->type);

			if(o->this.pair.car->type!=SFS_NIL)
			{
				printf(" ");
				DEBUG_MSG("o : %d",o->this.pair.car->this.number.this.integer);
				sfs_print_pair(o);
			}
		}
	}

    return;
}

void sfs_print( object o )
{
	DEBUG_MSG("type : %d",o->type);
    if ( SFS_PAIR == o->type )
    {
		printf("(");
        sfs_print_pair( o );
		printf(")");
    }
    else
    {
        sfs_print_atom( o );
    }

}
