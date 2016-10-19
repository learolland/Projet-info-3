
/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"





/********* ENVIRONNEMENT **********/
object *liste_env ;

void init_environment (void)
{
    *liste_env = make_pair();

}



/**** Fonctions utilitaires environnement ****/

void ajout_env (object *liste_env, object p)
{
    object o = *liste_env;
    while(o->this.pair.cdr!=nil && o->this.pair.cdr!=NULL)
    {
        o = *liste_env->this.pair.cdr;
    }
    p->this.pair.cdr = NULL;
    o->this.pair.cdr = p ;
}


void ajout_binding (object valeur, uint indice_env)
{
    object env = *liste_env;
    uint i = indice_env;
    while(i!=0)
    {
        env = *liste_env->this.pair.cdr;
        i--;
    }

    object binding = env->this.pair.car;
    while(binding->this.pair.cdr != NULL && binding->this.pair.cdr != nil)
    {
        binding = binding->this.pair.cdr;
    }
    binding_ajout = make_pair();
    binding_ajout->this.pair.car = valeur ;
    binding->this.pair.cdr = binding_ajout;
}



uint chercher_symb (string nom)
{
    object env = *liste_env;
    object binding = env->this.pair.car;
    while(env != NULL && env != nil)
    {
        while(binding!=NULL && binding != nil)
        {
            object valeur = binding->this.pair.car;
            if(strcmp(valeur->this.liste.nom, nom)==0)
            {
                return 1;
            }
            binding = binding->this.pair.cdr;
        }
        env = *liste_env->this.pair.cdr:
        binding = env->this.pair.car;
    }
    return 0;
}








/*** forme set! ***/
object set! (object variable)
{

}




object sfs_eval( object input ) {

    return input;
}
