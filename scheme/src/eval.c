
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
    if(strcmp(cible,"car")==0)
    {
        return input->this.pair.car;
    }
    if(strcmp(cible,"cdr")==0)
    {
        return input->this.pair.cdr;
    }
    if(strcmp(cible,"cadr")==0)
    {
        return input->this.pair.cdr->this.pair.car;
    }
    if(strcmp(cible,"caddr")==0)
    {
        return input->this.pair.cdr->this.pair.cdr->this.pair.car;
    }
    if(strcmp(cible,"caar")==0)
    {
        return input->this.pair.car->this.pair.car;
    }
    if(strcmp(cible,"cddr")==0)
    {
        return input->this.pair.cdr->this.pair.cdr;
    }
    if(strcmp(cible,"cdddr")==0)
    {
        return input->this.pair.cdr->this.pair.cdr->this.pair.cdr;
    }
    return NULL;

}

uint get_uint (object o)
{
    return o->this.number.this.integer;
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
    DEBUG_MSG("evaluation des parametres");
    object liste_arg = make_pair();
    object ptr_liste = liste ;
    uint i = 0;

    while(ptr_liste != NULL && ptr_liste != nil)
    {
        liste_arg = ajout_queue(liste_arg,ptr_liste->this.pair.car);
        ptr_liste = ptr_liste->this.pair.cdr;
        i++;
    }
    DEBUG_MSG("on a une liste de parametres");
    return liste_arg;
}




/******************** EVAL *******************/

object sfs_eval( object input, object envt)
{
    restart:

    DEBUG_MSG("EVAL");

    /****** EVALUATION TYPE HORS PAIR ********/
    if(input == NULL) return input_vide;

    if(input->type == SFS_SYMBOL)
    {
        DEBUG_MSG("on a un symbol");
        object p = valeur_symb(input->this.symbol,envt);
        if(p!= NULL) return p;
        else
            return var_non_def;
    }

    if(input->type == SFS_PROBLEM)
        return input;

    if(input->type == SFS_NUMBER)
        return input;


    /******************* TYPE PAIR *********************/
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
            o = get("caar",test_input);
            input_bis = test_input->this.pair.car;
            test_input = test_input->this.pair.car;
        }

        if(o->type == SFS_PROBLEM)  return o;

        if(o->type == SFS_BOOLEAN)
        {
            if(strcmp(o->this.symbol,"#t")==0) return boolean_true;
            if(strcmp(o->this.symbol,"#f")==0) return boolean_false;
        }

        if(o->type == SFS_NUMBER || o->type == SFS_CHAR_SPECIAL || o->type == SFS_CHARACTER || o->type == SFS_STRING) return o;

        /*** SYMBOL DANS PAIR ***/
        if(o->type == SFS_SYMBOL)
        {

            /**************** FORMES ****************/
            if(is_form("define",o)==1)
            {
                DEBUG_MSG("valeur : %s, type %d",get("cadr",input)->this.symbol,get("cadr",input)->type);
                define(get("cadr",input_bis),get("caddr",input_bis),envt);
                return NULL;
            }
            if(is_form("set!",o)==1)
            {
                set(get("cadr",input_bis),get("caddr",input_bis),envt);
                return NULL;
            }
            if(is_form("quote",o)==1)
                return (get("cadr",input_bis));
            if(is_form("if",o))
            {
                DEBUG_MSG("on a un if");
                object consequence = get("cddr",input);
                object alternative = get("cdddr",input);
                DEBUG_MSG("input car : %s",input->this.pair.car->this.symbol);
                object input_if = input->this.pair.cdr;

                if(strcmp(sfs_eval(input_if,envt)->this.symbol,boolean_true->this.symbol)==0)
                {
                    DEBUG_MSG("on a un if vrai");
                    DEBUG_MSG("then : %d",get("caddr",input)->type);
                    return sfs_eval(consequence,envt);
                }
                else
                {
                    DEBUG_MSG("on a if faux");
                    if (get("cdddr",input)!= nil)
                    {
                        input = get("cdddr",input);
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

            /************************ LAMBDA ********************/
            if(is_form("lambda",o))
            {
                object agregat = make_compound();

                DEBUG_MSG("on va evaluer les données");
                agregat->this.compound.parms = evaluer_parms(get("cadr",input_bis));
                agregat->this.compound.body = get("caddr",input_bis);
                /*agregat->this.compound.envt = ajout_env();*/

                /* probleme ici */
                if(is_form("lambda",input->this.pair.car))
                {
                    return agregat;
                }
                else if(get("cdr",input) == NULL || get("cdr",input) == nil)
                {
                    DEBUG_MSG("On n'a pas d'argument immédiat");
                    return agregat;
                }
                DEBUG_MSG("on a un agregat");
                object envt_lambda = ajout_env();

                DEBUG_MSG("on prend les arguments du compound");
                object arg = agregat->this.compound.parms;
                object valeur_arg = input->this.pair.cdr;

                while(arg != NULL && arg != nil)
                {
                    DEBUG_MSG("on defini l'argument %s comme celui donné, %d",arg->this.pair.car->this.symbol,get_uint(get("cadr",input)));
                    define(arg->this.pair.car,sfs_eval(valeur_arg->this.pair.car,envt_lambda),envt_lambda);
                    DEBUG_MSG("%s,%d",arg->this.pair.car->this.symbol, get_uint(valeur_symb(arg->this.pair.car->this.symbol,envt_lambda)));
                    arg = arg->this.pair.cdr;
                    valeur_arg = valeur_arg->this.pair.cdr;
                }
                return sfs_eval(agregat->this.compound.body,envt_lambda);

            }
            if(is_form("begin",o))
            {
                object liste_begin = evaluer_arg(input_bis->this.pair.cdr,envt);
                while(liste_begin->this.pair.cdr != NULL && liste_begin->this.pair.cdr !=nil)
                    liste_begin = liste_begin->this.pair.cdr;
                return liste_begin->this.pair.car;
            }


            /************************ PRIMITIVES ***************************/
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
                        return (p->this.prim.fonction)(liste_arg);
                    }
                    if(p->type == SFS_COMPOUND)
                    {

                        DEBUG_MSG("on a un agregat");
                        object envt_c = ajout_env();

                        DEBUG_MSG("on prend les arguments du compound");
                        object arg_f = p->this.compound.parms;
                        object valeur_arg_f = input->this.pair.cdr;
                        while(arg_f != NULL && arg_f != nil)
                        {
                            DEBUG_MSG("arg : %s",arg_f->this.pair.car->this.symbol);

                            DEBUG_MSG("on defini l'argument comme celui donné, %d",get_uint(valeur_arg_f->this.pair.car));
                            define(arg_f->this.pair.car,sfs_eval(get("car",valeur_arg_f),envt_c),envt_c);
                            DEBUG_MSG("%s,%d",arg_f->this.pair.car->this.symbol, get_uint(valeur_symb(arg_f->this.pair.car->this.symbol,envt_c)));
                            arg_f = arg_f->this.pair.cdr;
                            valeur_arg_f = valeur_arg_f->this.pair.cdr;
                        }

                        return sfs_eval(p->this.compound.body,envt_c);
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
