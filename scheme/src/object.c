

#include "object.h"
#include "mem.h"
#include "basic.h"
#include <stdlib.h>




object make_object( uint type )
{
    object t = sfs_malloc( sizeof( *t ) );

    t->type = type;
    return t;
}

object make_primitive (object (*p)(object))
{
    object o = make_object(SFS_PRIM);
    o->this.prim.fonction = p;
    return o;
}

object make_liste (void)
{
    object p = make_object(SFS_LIST);
    strcpy(p->this.liste.nom,"\0");
    p->this.liste.val = NULL;
    return p;
}

object make_nil( void ) 			/*make_empty_list ??*/
{
    object t = make_object( SFS_NIL );
    t->this.special = t;
    return t;
}


object make_boolean( uint true_false  )
{

    object t = make_object(SFS_BOOLEAN);
    if(true_false == FALSE)
    {
        string chaine = "#f";
        strcpy(t->this.symbol,chaine);
    }
    if (true_false == TRUE )
    {
        string chaine = "#t";
        strcpy(t->this.symbol,chaine);

    }

    return t;
}


object make_symbol( string symbol )
{
    object t = make_object(SFS_SYMBOL);
    strcpy(t->this.symbol,symbol);

    return t;
}


object make_integer(uint integer)
{
    object t = make_object(SFS_NUMBER);
    t->this.number.numtype = NUM_INTEGER;
    t->this.number.this.integer = (int)integer;
    return t;
}


object make_character( char character)
{
    object t = make_object(SFS_CHARACTER);
    t->this.character = character;

    return t;
}

object make_character_special(string chaine)
{
    object t = make_object(SFS_CHAR_SPECIAL);
    strcpy(t->this.string,chaine);
    return t;
}


object make_string( string chaine)
{
    object t = make_object(SFS_STRING);
    strcpy(t->this.string,chaine);

    return t;
}


object make_pair(void)
{
    object t = make_object(SFS_PAIR);
    t->this.pair.car = NULL;
    t->this.pair.cdr = NULL;
    return t;
}
