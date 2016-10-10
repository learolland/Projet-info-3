
/**
 * @file object.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:02:57 2012
 * @brief Object definitions for SFS.
 *
 * Object definitions for SFS.
 */

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

object make_nil( void ) 			/*make_empty_list ??*/
{

    object t = make_object( SFS_NIL );

    t->this.special = t;

    return t;
}


object make_boolean( uint true_false  )
{

    object t = make_object(SFS_BOOLEAN);
    DEBUG_MSG("on lit un booléen\n");
    if(true_false == FALSE)
    {
        string chaine = "#f";
        strcpy(t->this.string,chaine);
        printf("BOOLEN / %s\n",chaine);
    }
    if (true_false == TRUE )
    {
        string chaine = "#t";
        printf("BOOLEN / %s\n",chaine);

        strcpy(t->this.string,chaine);

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
    DEBUG_MSG("make_pair\n");
    object t = make_object(SFS_PAIR);
    t->this.pair.car = NULL;			/*pas suuuuuure*/
    t->this.pair.cdr = NULL;

    return t;
}





void afficher_objet (object t)/*pour un entier poue l'instant*/
{
    if(t->type == 0)
    printf("                type : %d, valeur : %d\n",t->type,t->this.number.this.integer);
    if(t->type == 1)
    printf("                type : %d, valeur : %c\n",t->type,t->this.character);
    if(t->type == 2)
    printf("                type : %d, valeur : %s\n",t->type,t->this.string);
    if(t->type == 6)
    printf("                type : %d, valeur : %s\n",t->type,t->this.symbol);

}