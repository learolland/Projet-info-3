
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
object nil;


void init_environment (void)
{
    *liste_env = make_pair();
     nil = make_nil();

}



/**** Fonctions utilitaires environnement ****/

void ajout_env (object *liste_env, object p)
{
    object o = *liste_env;
    while(o->this.pair.cdr!=nil && o->this.pair.cdr!=NULL)
    {
        o = (*liste_env)->this.pair.cdr;
    }
    p->this.pair.cdr = NULL;
    o->this.pair.cdr = p ;
}


void ajout_binding (object variable, object valeur, uint indice_env)
{
    object env = *liste_env;
    uint i = indice_env;
    while(i!=0)
    {
        env = (*liste_env)->this.pair.cdr;
        i--;
    }

    object binding = env->this.pair.car;
    while(binding->this.pair.cdr != NULL && binding->this.pair.cdr != nil)
    {
        binding = binding->this.pair.cdr;
    }
    object binding_ajout = make_pair();
    binding_ajout->this.pair.car = make_liste() ;
    strcpy(binding_ajout->this.pair.car->this.liste->nom,variable->this.symbol);
    binding_ajout->this.pair.car->this.liste->val = valeur;
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
            object valeur_binding = binding->this.pair.car;
            if(strcmp(valeur_binding->this.liste->nom, nom)==0)
            {
                return 1;
            }
            binding = binding->this.pair.cdr;
        }
        env = (*liste_env)->this.pair.cdr;
        binding = env->this.pair.car;
    }
    return 0;
}





object cons(object car, object cdr)
{
    object o = make_pair() ;
    object p = make_pair() ;
    p->this.pair.car = cdr;
    p->this.pair.cdr = nil;
    o->this.pair.car = car;
    o->this.pair.cdr = p;
    return o ;
}




/*** forme define ****/
object define (object variable, object valeur)
{
    ajout_binding(variable,valeur,0);
    return variable;
}


/*** forme set! ***/

object set (object variable, object valeur)
{
    uint i = chercher_symb(variable->this.symbol);
    if(i==0)
    {
        define(variable,valeur);
    }
    else
    {
        object env = *liste_env;
        object binding = env->this.pair.car;
        while(env != NULL && env != nil)
        {
            while(binding!=NULL && binding != nil)
            {
                object valeur_binding = binding->this.pair.car;
                if(strcmp(valeur_binding->this.liste->nom, variable->this.symbol)==0)
                {
                    binding->this.pair.car->this.liste->val = valeur ;
                }
                binding = binding->this.pair.cdr;
            }
            env = (*liste_env)->this.pair.cdr;
            binding = env->this.pair.car;
        }
    }
    return variable;
}

uint is_form(string forme, object input)
{
    if (input->type == SFS_NUMBER || input->type == SFS_STRING || input->type == SFS_CHARACTER || input->type == SFS_BOOLEAN || input->type == SFS_CHAR_SPECIAL)
        return 0 ;
    if(strcmp(input->this.symbol,forme))
        return 1;
    else return 0;
}




object sfs_eval( object input )
{
    
    if(input->type == SFS_NUMBER || input->type == SFS_STRING || input->type == SFS_CHARACTER || input->type == SFS_BOOLEAN || input->type == SFS_CHAR_SPECIAL)
        return input;

    if(is_form("quote",input))
    {
        return (input->this.pair.cdr->this.pair.car);
    }
    /*if(is_form("if",input))
    {
        if(true==eval(cadr(input)))
            return eval(caddr(input));
            input = goto restart ;
    }*/
    return input;
}
