
/**
 * @file object.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 17:49:46 2012
 * @brief Object definition for SFS.
 *
 * Object definition for SFS.
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "number.h"
#include "basic.h"


typedef struct object_t {

    uint type;

    union {

        num              number;
        char             character;
        string           string;
        string           symbol;

        struct pair_t {
            struct object_t *car;
            struct object_t *cdr;
        } pair;
        
         struct liste_t
        {
            string nom;
            object val;
        }*liste ;

        struct object_t *special;

    } this;

} *object;



object make_liste (void);

object make_object( uint type );
object make_nil( void );
object make_boolean( uint true_false  );
object make_symbol( string symbol );
object make_integer( uint integer);
object make_character( char character);
object make_string( string chaine);
object make_pair(void);
object make_character_special(string chaine);


void afficher_objet(object t);


#define SFS_NUMBER       0x00
#define SFS_CHARACTER    0x01
#define SFS_STRING       0x02
#define SFS_PAIR         0x03
#define SFS_NIL          0x04
#define SFS_BOOLEAN      0x05
#define SFS_SYMBOL       0x06
#define SFS_CHAR_SPECIAL 0x07
#define SFS_LIST         0x08

/*extern object nil;*/

#ifdef __cplusplus
}
#endif

#endif /* _OBJECT_H_ */
