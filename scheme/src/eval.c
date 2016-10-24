
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
object nil;





/**** Fonctions utilitaires environnement ****/

object ajout_env (object p)
{
    object o = liste_env;
    while(o->this.pair.cdr!=nil && o->this.pair.cdr!=NULL)
    {
        o = (liste_env)->this.pair.cdr;
    }
    p->this.pair.cdr = NULL;
    o->this.pair.cdr = p ;
    return liste_env;
}


uint ajout_binding (object variable, object valeur)
{
    DEBUG_MSG("on entre dans ajout_binding");

    object binding_ajout = make_pair();
    binding_ajout->this.pair.car = make_liste() ;
    strcpy(binding_ajout->this.pair.car->this.liste.nom,variable->this.symbol);
    binding_ajout->this.pair.car->this.liste.val = valeur;



    object tmp_binding;
    object binding = liste_env->this.pair.car;

    if(binding == NULL || binding == nil)
    {
        DEBUG_MSG("on n'a pas encore de binding -> création");
        binding = binding_ajout;
        binding->this.pair.cdr = nil;
        liste_env->this.pair.car = binding;
        return 1;
    }

    tmp_binding = make_pair();


    tmp_binding->this.pair.cdr = binding_ajout;
    binding_ajout->this.pair.cdr = binding;
    liste_env->this.pair.car = binding_ajout;
    DEBUG_MSG("%s",liste_env->this.pair.car->this.pair.car->this.liste.nom);
    return 1;
}


object valeur_symb (string nom)
{
    if (chercher_symb(nom)==0)
    {
        WARNING_MSG("La variable %s n'a pas été définie",nom);
        return NULL;
    }
    object env = liste_env;
    object binding = env->this.pair.car;
    while(env != NULL && env != nil)
    {
        while(binding!=NULL && binding != nil)
        {
            object valeur_binding = binding->this.pair.car;
            if(strcmp(valeur_binding->this.liste.nom, nom)==0)
            {
                return valeur_binding->this.liste.val;
            }
            binding = binding->this.pair.cdr;
        }
        env = (liste_env)->this.pair.cdr;
        binding = env->this.pair.car;
    }
    WARNING_MSG("Le programme n'a pas pu trouver la variable %s dans les environnements",nom);
    return NULL;
}

uint chercher_symb (string nom)
{
    DEBUG_MSG("entree dans chercher_symb");
    object env = liste_env;
    if(liste_env->this.pair.car == NULL)
        return 0;
    object binding = NULL;
    while(env != NULL)
    {
        DEBUG_MSG("premier palier");
        binding = env->this.pair.car;
        while(binding != nil)
        {
            DEBUG_MSG("deuxieme palier");
            object valeur_binding = binding->this.pair.car;
            DEBUG_MSG("on a un valeur_binding : %s",valeur_binding->this.liste.nom);
            if(strcmp(valeur_binding->this.liste.nom, nom)==0)
            {
                DEBUG_MSG("on trouve un correspondance");
                return 1;
            }
            binding = binding->this.pair.cdr;
            DEBUG_MSG("on change binding, %s",binding->this.pair.car->this.liste.nom);
            if(binding == nil)
            DEBUG_MSG("wtf c'est bien nil");
        }
        DEBUG_MSG("on sort du deuxieme palier");
        env = env->this.pair.cdr;
        DEBUG_MSG("on change d'env");

    }
    DEBUG_MSG("on a rien trouvé");
    return 0;
}


void afficher_env (void)
{
    DEBUG_MSG(" on entre dans afficher_env");
    object env = liste_env;
    object binding = NULL;
    while(env != NULL)
    {
        binding = env->this.pair.car;
        while(binding != nil)
        {
            object valeur_binding = binding->this.pair.car;
            if(valeur_binding->this.liste.val->type == SFS_NUMBER)
                DEBUG_MSG("variable : %s, valeur: %d",valeur_binding->this.liste.nom,valeur_binding->this.liste.val->this.number.this.integer);
            if(valeur_binding->this.liste.val->type == SFS_SYMBOL)
                DEBUG_MSG("variable : %s, valeur: %s",valeur_binding->this.liste.nom,valeur_binding->this.liste.val->this.symbol);
            binding = binding->this.pair.cdr;
        }
        env = (liste_env)->this.pair.cdr;
    }
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
    DEBUG_MSG("on entre dans define");
    uint i = chercher_symb(variable->this.symbol);
    if (i == 1)
        return set(variable,valeur);
    if(variable->type != SFS_SYMBOL)
    {
        WARNING_MSG("on ne peut pas définir une nouvelle valeur pour un nombre");
        return NULL;
    }
    ajout_binding(variable,valeur);
    return variable;
}


/*** forme set! ***/

object set (object variable, object valeur)
{
    DEBUG_MSG("entree dans set");
    uint i = chercher_symb(variable->this.symbol);
    DEBUG_MSG("chercher_symb = %d",i);
    if(i==0)
    {
        WARNING_MSG("La variable %s n'est pas initialisée : utilisez define",variable->this.symbol);
        return NULL;
    }
    else
    {
        object env = liste_env;
        object binding = NULL;
        while(env != NULL)
        {
            binding = env->this.pair.car;
            while(binding != nil)
            {
                object valeur_binding = binding->this.pair.car;
                if(strcmp(valeur_binding->this.liste.nom, variable->this.symbol)==0)
                {
                    binding->this.pair.car->this.liste.val = valeur ;
                }
                binding = binding->this.pair.cdr;
            }
            env = (liste_env)->this.pair.cdr;
        }
    }
    afficher_env();
    return variable;
}

uint is_form(string forme, object input)
{
    if (input->type == SFS_NUMBER || input->type == SFS_STRING || input->type == SFS_CHARACTER || input->type == SFS_BOOLEAN || input->type == SFS_CHAR_SPECIAL)
        return 0 ;
    if(strcmp(input->this.pair.car->this.symbol,forme)==0)
        return 1;
    else return 0;
}

object evaluer_predicat (object input,string signe)
{
    num a, b;
    object car = input->this.pair.cdr->this.pair.car;
    DEBUG_MSG("car :%d",car->this.number.this.integer);
    if(car->type==SFS_NUMBER)
    {
        DEBUG_MSG("on prend bien a = int");
        a = car->this.number;
    }
    if(car->type == SFS_SYMBOL)
    {
        if(chercher_symb(car->this.symbol)==0)
        {
            WARNING_MSG("le symbole %s que vous cherchez à évaluer n'a pas été défini",car->this.symbol);
            return NULL;
        }
        else
        {
            object o = valeur_symb(car->this.symbol);
            if(o->type == SFS_NUMBER)
                a = o->this.number;
            else
            {
                WARNING_MSG("vous cherchez à comparer quelque chose qui n'est pas un nombre");
                return NULL;
            }
        }
    }
    if(car->type != SFS_NUMBER && car->type != SFS_SYMBOL)
    {
        WARNING_MSG("vous cherchez à comparer des éléments qui ne sont ni des nombres, ou des symboles définis");
        return NULL;
    }

    object cadr = input->this.pair.cdr->this.pair.cdr->this.pair.car;
    if(cadr->type == SFS_NUMBER)
    {
        b = cadr->this.number;
        DEBUG_MSG("on prend bien b = int");
    }
    if(cadr->type == SFS_SYMBOL)
    {
        if(chercher_symb(cadr->this.symbol)==0)
        {
            WARNING_MSG("le symbole %s que vous cherchez à évaluer n'a pas été défini",cadr->this.symbol);
            return NULL;
        }
        else
        {
            object o = valeur_symb(cadr->this.symbol);
            if(o->type == SFS_NUMBER)
            b = o->this.number;
            else
            {
                WARNING_MSG("vous cherchez à comparer quelque chose qui n'est pas un nombre");
                return NULL;
            }
        }
    }
    if(cadr->type != SFS_NUMBER && cadr->type != SFS_SYMBOL)
    {
        WARNING_MSG("vous cherchez à comparer des éléments qui ne sont ni des nombres, ou des symboles définis");
        return NULL;
    }
    if(strcmp(signe,"+")==0)
    {
        uint sum = a.this.integer+b.this.integer;
        object s = make_integer(sum);
        return s;
    }
    if(strcmp(signe,"-")==0)
    {
        uint dif = a.this.integer-b.this.integer;
        object d = make_integer(dif);
        return d;
    }
    if(strcmp(signe,">")==0)
    {
        if(a.this.integer>b.this.integer)
            return boolean_true;
        else return boolean_false;
    }
    if(strcmp(signe,"<")==0)
    {
        if(a.this.integer<b.this.integer)
            return boolean_true;
        else return boolean_false;
    }
    if(strcmp(signe,"=")==0)
    {
        if(a.this.integer==b.this.integer)
            return boolean_true;
        else return boolean_false;
    }
    if(strcmp(signe,"!=")==0)
    {
        if(a.this.integer!=b.this.integer)
            return boolean_true;
        else return boolean_false;
    }
    return input;
}



object sfs_eval( object input )
{
    restart:
    DEBUG_MSG("\n");
    DEBUG_MSG("on entre dans eval");
    if(input->this.pair.cdr == nil)
        DEBUG_MSG("input cdr = nil ");
    /*if(input->type == SFS_NUMBER || input->type == SFS_STRING || input->type == SFS_CHARACTER || input->type == SFS_BOOLEAN || input->type == SFS_CHAR_SPECIAL)
        return input;*/

    if(input->type == SFS_SYMBOL)
    {
        uint i = chercher_symb(input->this.symbol);
        if(i==1)
        {
            return valeur_symb(input->this.symbol);
        }
        else return input;
    }

    object o = NULL;
    if(input->type == SFS_PAIR)
    {
        DEBUG_MSG("input = pair");
        DEBUG_MSG("input car type = %d",input->this.pair.car->type);
        o = input->this.pair.car;

        if(o->type == SFS_BOOLEAN)
        {
            if(strcmp(o->this.symbol,"#t")==0)
            {
                DEBUG_MSG("boolean_true");
                return boolean_true;
            }
            if(strcmp(o->this.symbol,"#f")==0)
            {
                DEBUG_MSG("boolean_false");
                return boolean_false;
            }
        }

        if(o->type == SFS_NUMBER)
        {

            return o;
        }

        if(o->type == SFS_SYMBOL)
        {
            DEBUG_MSG("on a un car symbol : %s",o->this.symbol);


            if(is_form("define",input)==1)
            {
                return define(input->this.pair.cdr->this.pair.cdr,input->this.pair.cdr->this.pair.cdr->this.pair.car);
            }

            if(is_form("set!",input)==1)
            {
                return set(input->this.pair.cdr->this.pair.cdr,input->this.pair.cdr->this.pair.cdr->this.pair.car);
            }
            if(is_form("quote",input)==1)
            {
                return (input->this.pair.cdr->this.pair.car);
            }
            if(is_form("if",input))
            {
                DEBUG_MSG("on a un if");
                input = input->this.pair.cdr;
                if(boolean_true==sfs_eval(input))
                {
                    DEBUG_MSG("on a un if vrai");
                    DEBUG_MSG("then : %d",input->this.pair.car->this.number.this.integer);
                    return sfs_eval(input->this.pair.cdr->this.pair.car);
                }
                else
                {
                    DEBUG_MSG("on a if faux");
                    if (input->this.pair.cdr->this.pair.cdr->this.pair.car != nil)
                    {
                        input = input->this.pair.cdr->this.pair.cdr->this.pair.car;
                        DEBUG_MSG("on change input");
                        goto restart;
                    }
                    else
                    return boolean_false;
                }
            }
            if(is_form("and",input))
            {
                if(boolean_false==sfs_eval(input->this.pair.cdr))
                {
                    return boolean_false;
                }
                else
                {
                    DEBUG_MSG("(and #true...)");
                    input = input->this.pair.cdr;
                    while(input != nil)
                    {
                        if(boolean_false==sfs_eval(input->this.pair.cdr))
                        return boolean_false;
                        else input = input->this.pair.cdr;
                    }
                    return boolean_true;
                }
            }
            if(is_form("or",input))
            {
                if(boolean_true==sfs_eval(input->this.pair.cdr))
                {
                    return boolean_true;
                }
                else
                {
                    input = input->this.pair.cdr;
                    while(input != nil)
                    {
                        DEBUG_MSG("(or #f ...)");
                        if(boolean_true==sfs_eval(input->this.pair.cdr))
                        return boolean_true;
                        else input = input->this.pair.cdr;
                    }
                    return boolean_false;
                }
            }

            else
            {
                DEBUG_MSG("on a un symbol mais pas de forme connue");
                string signe;
                if(strcmp(o->this.symbol,"+")==0)
                {
                    strcpy(signe ,"+");
                    return evaluer_predicat (input,signe);
                }
                if(strcmp(o->this.symbol,"-")==0)
                {
                    strcpy(signe ,"-");
                    return evaluer_predicat(input,signe);
                }
                if(strcmp(o->this.symbol,">")==0)
                {
                    strcpy(signe ,">");
                    return evaluer_predicat(input,signe);
                }
                if(strcmp(o->this.symbol,"<")==0)
                {
                    strcpy(signe ,"<");
                    return evaluer_predicat(input,signe);
                }
                if(strcmp(o->this.symbol,"=")==0)
                {
                    strcpy(signe ,"=");
                    return evaluer_predicat(input,signe);
                }
                if(strcmp(o->this.symbol,"!=")==0)
                {
                    strcpy(signe ,"!=");
                    return evaluer_predicat(input,signe);
                }
                else return o;
            }
        }


    }
    return input;
}
