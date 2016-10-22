
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


void ajout_binding (object variable, object valeur)
{
    DEBUG_MSG("on entre dans ajout_binding");

    object binding_ajout = make_pair();
    binding_ajout->this.pair.car = make_liste() ;
    strcpy(binding_ajout->this.pair.car->this.liste.nom,variable->this.symbol);
    DEBUG_MSG("%s",binding_ajout->this.pair.car->this.liste.nom);
    binding_ajout->this.pair.car->this.liste.val = valeur;
    DEBUG_MSG("on cree et remplie binding_ajout");


    object tmp_binding;
    object binding = liste_env->this.pair.car;
    DEBUG_MSG("utilisation de liste_env");


    tmp_binding = make_pair();

    DEBUG_MSG("on va jusqu'a la fin des binding");

    tmp_binding->this.pair.cdr = binding_ajout;
    binding_ajout->this.pair.cdr = binding;
    liste_env->this.pair.car = binding_ajout;
    DEBUG_MSG("%s",liste_env->this.pair.car->this.pair.car->this.liste.nom);
}


object valeur_symb (string nom)
{
    if (chercher_symb(nom)==0) return nil;
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
    return nil;
}

uint chercher_symb (string nom)
{
    object env = liste_env;
    object binding = env->this.pair.car;
    while(env != NULL && env != nil)
    {
        while(binding!=NULL && binding != nil)
        {
            object valeur_binding = binding->this.pair.car;
            if(strcmp(valeur_binding->this.liste.nom, nom)==0)
            {
                return 1;
            }
            binding = binding->this.pair.cdr;
        }
        env = (liste_env)->this.pair.cdr;
        binding = env->this.pair.car;
    }
    return 0;
}


void afficher_env (void)
{
    DEBUG_MSG(" on entre dans afficher_env");
    if((liste_env)->this.pair.car != NULL)
    {
        object binding = liste_env->this.pair.car;
        DEBUG_MSG("binding %s",binding->this.pair.car->this.liste.nom);
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
    ajout_binding(variable,valeur);
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
        object env = liste_env;
        object binding = env->this.pair.car;
        while(env != NULL && env != nil)
        {
            while(binding!=NULL && binding != nil)
            {
                object valeur_binding = binding->this.pair.car;
                if(strcmp(valeur_binding->this.liste.nom, variable->this.symbol)==0)
                {
                    binding->this.pair.car->this.liste.val = valeur ;
                }
                binding = binding->this.pair.cdr;
            }
            env = (liste_env)->this.pair.cdr;
            binding = env->this.pair.car;
        }
    }
    return variable;
}

uint is_form(string forme, object input)
{
    DEBUG_MSG("dans is_form : input : %s",input->this.pair.car->this.symbol);
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
            printf("erreur\n");
            return nil;
        }
        else
        {
            object o = valeur_symb(car->this.symbol);
            if(o->type == SFS_NUMBER)
                a = o->this.number;
            else
            {
                printf("erreur\n");
                return nil;
            }
        }
    }
    if(car->type != SFS_NUMBER && car->type != SFS_SYMBOL)
    {
        printf("erreur\n");
        return nil;
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
            printf("erreur\n");
            return nil;
        }
        else
        {
            object o = valeur_symb(cadr->this.symbol);
            if(o->type == SFS_NUMBER)
            b = o->this.number;
            else
            {
                printf("erreur\n");
                return nil;
            }
        }
    }
    if(cadr->type != SFS_NUMBER && cadr->type != SFS_SYMBOL)
    {
        printf("erreur\n");
        return nil;
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
    return nil;
}



object sfs_eval( object input )
{

    DEBUG_MSG("\n");
    afficher_env();
    DEBUG_MSG("on entre dans eval, variable : %s",input->this.pair.car->this.symbol);
    /*if(input->type == SFS_NUMBER || input->type == SFS_STRING || input->type == SFS_CHARACTER || input->type == SFS_BOOLEAN || input->type == SFS_CHAR_SPECIAL)
        return input;*/

    object o = input->this.pair.car;

    if(o->type == SFS_SYMBOL)
    {
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
    }

    if(is_form("define",input)==1)
    {
        object variable = input->this.pair.cdr->this.pair.car;
        object valeur = input->this.pair.cdr->this.pair.cdr->this.pair.car;
        DEBUG_MSG("variable : %s, valeur : %s",variable->this.symbol,valeur->this.symbol);
        return define(variable,valeur);
    }

    if(is_form("set!",input)==1)
    {
        return set(input->this.pair.cdr->this.pair.car,input->this.pair.cdr->this.pair.cdr->this.pair.car);
    }
    if(is_form("quote",input)==1)
    {
        return (input->this.pair.cdr->this.pair.car);
    }
    if(is_form("if",input))
    {
        if(true==eval(input->this.pair.cdr->this.pair.car))
            return eval(caddr(input));
            input = goto restart ;
    }
    return input;
}
