
/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"
#include "math.h"
#include "prim.h"


/**** Fonctions utilitaires environnement ****/
object ajout_env (void)
{
    object o = make_pair();
    o->this.pair.cdr = liste_env;
    return o;

    /*object o = liste_env;
    while(o->this.pair.cdr!=nil && o->this.pair.cdr!=NULL)
    {
        o = (liste_env)->this.pair.cdr;
    }
    p->this.pair.cdr = NULL;
    o->this.pair.cdr = p ;
    return liste_env;*/
}

uint ajout_binding (object variable, object valeur, object envt)
{
    object binding_ajout = make_pair();
    binding_ajout->this.pair.car = make_liste() ;
    strcpy(binding_ajout->this.pair.car->this.liste.nom,variable->this.symbol);

    if(valeur->type == SFS_PRIM)
    {
        binding_ajout->this.pair.car->this.liste.val = valeur;

    }
    else binding_ajout->this.pair.car->this.liste.val = sfs_eval(valeur,envt);



    object tmp_binding;
    object binding = envt->this.pair.car;

    if(binding == NULL || binding == nil)
    {
        binding = binding_ajout;
        binding->this.pair.cdr = nil;
        envt->this.pair.car = binding;
        return 1;
    }

    tmp_binding = make_pair();


    tmp_binding->this.pair.cdr = binding_ajout;
    binding_ajout->this.pair.cdr = binding;
    envt->this.pair.car = binding_ajout;
    return 1;
}

object valeur_symb (string nom, object envt)
{
    object env = envt;
    object binding = env->this.pair.car;
    while(env != NULL && env != nil)
    {
        while(binding!=NULL && binding != nil)
        {
            object valeur_binding = binding->this.pair.car;
            if(strcmp(valeur_binding->this.liste.nom, nom)==0)
            {
                DEBUG_MSG("on trouve une valeur de type : %d",valeur_binding->this.liste.val->type);
                return valeur_binding->this.liste.val;
            }
            /*DEBUG_MSG("on sort du deuxieme palier");*/
            binding = binding->this.pair.cdr;
        }
        /*DEBUG_MSG("on sort du premier palier");*/
        env = envt->this.pair.cdr;
        if(env!= NULL && env != nil) binding = env->this.pair.car;
    }
    return var_non_def;
}

uint chercher_symb (string nom, object envt)
{
    object env = envt;
    if(envt->this.pair.car == NULL)
        return 0;
    object binding = NULL;
    while(env != NULL)
    {
        binding = env->this.pair.car;
        while(binding != nil)
        {
            object valeur_binding = binding->this.pair.car;
            if(strcmp(valeur_binding->this.liste.nom, nom)==0)
            {
                return 1;
            }
            binding = binding->this.pair.cdr;
        }
        env = env->this.pair.cdr;
    }
    return 0;
}


object ajout_queue (object liste, object car)
{
    /*DEBUG_MSG("dans ajout_queue");*/
    if (liste->this.pair.car==nil || liste->this.pair.car == NULL)
    {
        /*DEBUG_MSG("ajout_tete simple");*/
        liste->this.pair.car = car ;
        /*DEBUG_MSG("on ajoute le car");*/
        return liste ;
    }

    object p,a;
    for (a = liste; a->this.pair.cdr != NULL && a->this.pair.cdr !=  nil ; a = a->this.pair.cdr);

    p = make_pair();
    p->this.pair.car = car;
    a->this.pair.cdr = p;
    return liste;
}


        /*** FORMES ****/


void define (object variable, object valeur, object envt)
{
    uint i = chercher_symb(variable->this.symbol,envt);
    if (i == 1)
        set(variable,valeur,envt);
    if(variable->type != SFS_SYMBOL)
    {
        WARNING_MSG("on ne peut pas définir une nouvelle valeur pour un nombre %d",variable->type);
    }
    ajout_binding(variable,valeur,envt);
}

void set (object variable, object valeur, object envt)
{
    uint i = chercher_symb(variable->this.symbol,envt);
    DEBUG_MSG("chercher_symb = %d",i);
    if(i==0)
    {
        WARNING_MSG("La variable %s n'est pas initialisée : utilisez define",variable->this.symbol);
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
                    binding->this.pair.car->this.liste.val = sfs_eval(valeur,envt);
                }
                binding = binding->this.pair.cdr;
            }
            env = (liste_env)->this.pair.cdr;
        }
    }
}

uint is_form(string forme, object input)
{
    if (input->type == SFS_NUMBER || input->type == SFS_STRING || input->type == SFS_CHARACTER || input->type == SFS_BOOLEAN || input->type == SFS_CHAR_SPECIAL)
        return 0 ;
    if(strcmp(input->this.symbol,forme)==0)
        return 1;
    else return 0;
}

object get (string cible, object input)
{
    if(strcmp(cible,car)==0)
    {
        return input->this.pair.car;
    }
    if(strcmp(cible,cdr)==0)
    {
        return input->this.pair.cdr;
    }
    if(strcmp(cible,cadr)==0)
    {
        return input->this.pair.cdr->this.pair.car;
    }
    if(strcmp(cible,caddr)==0)
    {
        return input->this.pair.cdr->this.pair.cdr->this.pair.car;
    }
    if(strcmp(cible,caar)==0)
    {
        return input->this.pair.car->this.pair.car;
    }

}


/********** EVALUATION ***********/


object evaluer_arg (object liste, object envt)
{
    DEBUG_MSG("evaluation des arguments");
    object liste_arg = make_pair();
    object ptr_liste = liste ;
    uint i = 0;
    /*DEBUG_MSG("car de la liste : %d",ptr_liste->this.pair.car->this.number.this.integer);*/
    while(ptr_liste != NULL && ptr_liste != nil)
    {
        liste_arg = ajout_queue(liste_arg,sfs_eval(ptr_liste->this.pair.car,envt));
        ptr_liste = ptr_liste->this.pair.cdr;
        i++;
    }
    return liste_arg;
}

object evaluer_parms (object liste)
{
    DEBUG_MSG("evaluation des arguments");
    object liste_arg = make_pair();
    object ptr_liste = liste ;
    uint i = 0;
    /*DEBUG_MSG("car de la liste : %d",ptr_liste->this.pair.car->this.number.this.integer);*/
    while(ptr_liste != NULL && ptr_liste != nil)
    {
        liste_arg = ajout_queue(liste_arg,ptr_liste->this.pair.car);
        ptr_liste = ptr_liste->this.pair.cdr;
        i++;
    }
    return liste_arg;
}


object sfs_eval( object input, object envt)
{
    restart:

    DEBUG_MSG("EVAL");
    if(input->type == SFS_SYMBOL)
    {
        DEBUG_MSG("on a un symbol");
        object p = valeur_symb(input->this.symbol,envt);
        if(p!= NULL)
        {
            return p;
        }
        else
        {
            return var_non_def;
        }
    }
    if(input->type == SFS_PROBLEM)
    {
        DEBUG_MSG("syntax_problem");
        return input;
    }
    if(input->type == SFS_NUMBER)
    {
        DEBUG_MSG("on trouve un nb");
        return input;
    }
    object o = NULL;
    object input_bis = NULL;

    if(input->type == SFS_PAIR)
    {
        o = input->this.pair.car;
        input_bis = input;

        object test_input = input ;
        while(test_input->this.pair.car->type == SFS_PAIR)
        {
            DEBUG_MSG("on a une pair imbriquée");
            o = test_input->this.pair.car->this.pair.car;
            input_bis = test_input->this.pair.car;
            test_input = test_input->this.pair.car;
        }

        if(o->type == SFS_COMPOUND)
        {
            return o;
        }

        if(o->type == SFS_PROBLEM)
        {
            DEBUG_MSG("syntax_problem");
            return o;
        }


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

        if(o->type == SFS_NUMBER || o->type == SFS_CHAR_SPECIAL || o->type == SFS_CHARACTER || o->type == SFS_STRING)
        {
            return input;
        }

        if(o->type == SFS_SYMBOL)
        {

            if(is_form("define",o)==1)
            {
                DEBUG_MSG("valeur : %s, type %d",input->this.pair.cdr->this.pair.car->this.symbol,input->this.pair.cdr->this.pair.car->type);
                define(input_bis->this.pair.cdr->this.pair.car,input_bis->this.pair.cdr->this.pair.cdr->this.pair.car,envt);
                return NULL;
            }
            if(is_form("set!",o)==1)
            {
                set(input_bis->this.pair.cdr->this.pair.car,input_bis->this.pair.cdr->this.pair.cdr->this.pair.car,envt);
                return NULL;
            }
            if(is_form("quote",o)==1)
                return (input_bis->this.pair.cdr->this.pair.car);
            if(is_form("if",o))
            {
                DEBUG_MSG("on a un if");
                object consequence = input->this.pair.cdr->this.pair.cdr;
                object alternative = input->this.pair.cdr->this.pair.cdr->this.pair.cdr;
                DEBUG_MSG("input car : %s",input->this.pair.car->this.symbol);
                object input_if = input->this.pair.cdr;

                if(strcmp(sfs_eval(input_if,envt)->this.symbol,boolean_true->this.symbol)==0)
                {
                    DEBUG_MSG("on a un if vrai");
                    DEBUG_MSG("then : %d",input->this.pair.cdr->this.pair.cdr->this.pair.car->type);
                    return sfs_eval(consequence,envt);
                }
                else
                {
                    DEBUG_MSG("on a if faux");
                    if (input->this.pair.cdr->this.pair.cdr->this.pair.cdr!= nil)
                    {
                        input = input->this.pair.cdr->this.pair.cdr->this.pair.cdr;
                        DEBUG_MSG("on change input");
                        goto restart;
                    }
                    else
                    {
                        WARNING_MSG("il manque une conséquence");
                        return sfs_eval(consequence,envt);
                    }
                }
                alternative = NULL;
            }
            if(is_form("and",o))
            {
                DEBUG_MSG("on a un and");
                object input_and = input_bis;
                while(input_and != nil)
                {
                    DEBUG_MSG("(and ...)");
                    if(strcmp(boolean_false->this.symbol,sfs_eval(input_and->this.pair.cdr,envt)->this.symbol)==0)
                        return boolean_false;
                    else input_and = input_and->this.pair.cdr;
                }
                DEBUG_MSG("toutes les variables sont vraies");
                return boolean_true;

            }
            if(is_form("or",o))
            {
                object input_or = input_bis;
                DEBUG_MSG("o type = %d", o->type);
                while(input_or != nil)
                {
                    DEBUG_MSG("(or ...)");
                    if(strcmp(boolean_true->this.symbol,sfs_eval(input_or->this.pair.cdr,envt)->this.symbol)==0)
                        return boolean_true;
                    else input_or = input_or->this.pair.cdr;
                }
                return boolean_false;
            }
            if(is_form("lambda",o))
            {
                object agregat = make_compound();

                agregat->this.compound.parms = evaluer_parms (input->this.pair.cdr->this.pair.car);
                agregat->this.compound.body = input->this.pair.cdr->this.pair.cdr->this.pair.car;
                agregat->this.compound.envt = ajout_env();
                return agregat;
            }
            if(is_form("begin",o))
            {
                object liste_begin = evaluer_arg(input_bis->this.pair.cdr,envt);
                while(liste_begin->this.pair.cdr != NULL && liste_begin->this.pair.cdr !=nil)
                    liste_begin = liste_begin->this.pair.cdr;
                return liste_begin->this.pair.car;
            }

            else
            {
                object p = valeur_symb(o->this.symbol,envt) ;
                if (p!=NULL)
                {
                    if(p->type == SFS_PRIM)
                    {
                        DEBUG_MSG("on a créé la prim p type = %d",p->type);
                        object liste_arg = evaluer_arg(input_bis->this.pair.cdr,envt);
                        DEBUG_MSG("on a une liste_arg");
                        /*
                        object tmp_liste = liste_arg ;
                        while(tmp_liste!= NULL && tmp_liste != nil)
                        {
                        if(tmp_liste->this.pair.car->type == SFS_NUMBER)
                        DEBUG_MSG("affichage : %d",tmp_liste->this.pair.car->this.number.this.integer);
                        else DEBUG_MSG("affichage : %s, type %d",tmp_liste->this.pair.car->this.symbol,tmp_liste->this.pair.car->type);

                        tmp_liste = tmp_liste->this.pair.cdr;
                        }*/

                        DEBUG_MSG("on est la");
                        return (p->this.prim.fonction)(liste_arg);
                    }
                    if(p->type == SFS_COMPOUND)
                    {
                        DEBUG_MSG("on a un agregat");
                        object envt_c = ajout_env();
                        DEBUG_MSG("ajout d'un envt");

                        DEBUG_MSG("on prend les arguments du compound");
                        object arg = p->this.compound.parms;
                        DEBUG_MSG("arg : %s",arg->this.pair.car->this.symbol);

                        DEBUG_MSG("on defini l'argument comme celui donné, %d",input_bis->this.pair.cdr->this.pair.car->this.number.this.integer);
                        define(arg->this.pair.car,sfs_eval(input_bis->this.pair.cdr->this.pair.car,envt_c),envt_c);
                        DEBUG_MSG("%s,%d",arg->this.pair.car->this.symbol, valeur_symb(arg->this.pair.car->this.symbol,envt_c)->this.number.this.integer);

                        object resultat = sfs_eval(p->this.compound.body,envt_c);
                        return resultat;
                    }
                    return o;
                }
            }
        }
        else
        {
            return o;
        }

    }
    return input;
}
