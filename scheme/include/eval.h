
/**
 * @file eval.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:10:38 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#ifndef _EVAL_H_
#define _EVAL_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "object.h"

uint chercher_symb (string nom);
object valeur_symb (string nom);
uint is_form(string forme, object input);
object sfs_eval( object input);
object set(object variable, object valeur);
extern object liste_env;
extern object nil;
extern object boolean_false;
extern object boolean_true;

#ifdef __cplusplus
}
#endif

#endif /* _EVAL_H_ */
