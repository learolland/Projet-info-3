
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
    /*if(valeur->type == SFS_SYMBOL)
    {
        if(chercher_symb(valeur->this.symbol)==1)
        valeur = valeur_symb(valeur->this.symbol);
    }*/
    object binding_ajout = make_pair();
    binding_ajout->this.pair.car = make_liste() ;
    strcpy(binding_ajout->this.pair.car->this.liste.nom,variable->this.symbol);
    binding_ajout->this.pair.car->this.liste.val = sfs_eval(valeur);



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
    object env = liste_env;
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

object ajout_queue (object liste, object car)
{
    DEBUG_MSG("dans ajout_queue");
    if (liste->this.pair.car==nil || liste->this.pair.car == NULL)
    {
        DEBUG_MSG("ajout_tete simple");
        liste->this.pair.car = car ;
        DEBUG_MSG("on ajoute le car");
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
object define (object variable, object valeur)
{
    DEBUG_MSG("on entre dans define");
    uint i = chercher_symb(variable->this.symbol);
    if (i == 1)
        return set(variable,valeur);
    if(variable->type != SFS_SYMBOL)
    {
        WARNING_MSG("on ne peut pas définir une nouvelle valeur pour un nombre %d",variable->type);
        return NULL;
    }
    ajout_binding(variable,valeur);
    return variable;
}

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

                            binding->this.pair.car->this.liste.val = sfs_eval(valeur);


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
    if(strcmp(input->this.symbol,forme)==0)
        return 1;
    else return 0;
}



/*** PRIMITIVES ***/

object plus_p (object nums)
{
    DEBUG_MSG("on va faire un plus");
    uint somme = 0;
    object l = nums;
    while (l->this.pair.cdr != nil && l->this.pair.cdr != nil)
    {
        DEBUG_MSG("on entre dans le while");
        somme += l->this.pair.car->this.number.this.integer;
        l = l->this.pair.cdr;
    }
    object o_somme = make_integer(somme);
    return o_somme;
}


/********** EVALUATION ***********/
object evaluer_arg (object liste)
{
    DEBUG_MSG("evaluation des arguments");
    object liste_arg = make_pair();
    object ptr_liste = liste ;
    DEBUG_MSG("car de la liste : %d",ptr_liste->this.pair.car->this.number.this.integer);
    while(ptr_liste != NULL && ptr_liste != nil)
    {
        DEBUG_MSG("dans le for de evaluer_arg");
        liste_arg = ajout_queue(liste_arg,sfs_eval(ptr_liste->this.pair.car));
        ptr_liste = ptr_liste->this.pair.cdr;
        DEBUG_MSG("on change le ptr_liste");
    }
    DEBUG_MSG("on sort du while\n");
    return liste_arg;
}

object evaluer_prim (object input)
{
    string signe;
    strcpy(signe,input->this.symbol);
    if(strcmp(signe,"+")==0)
    {
        DEBUG_MSG("on a un +");
        return plus_p;
    }
    return NULL;
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


    if(input->type == SFS_SYMBOL)
    {
        object p = valeur_symb(input->this.symbol);
        if(p!= NULL) return p;
        else return input;
    }

    object o = NULL;
    object input_bis = NULL;
    if(input->type == SFS_PAIR)
    {
        DEBUG_MSG("input = pair");
        DEBUG_MSG("input car type = %d",input->this.pair.car->type);

        o = input->this.pair.car;
        input_bis = input;

        object test_input = input ;
        while(test_input->this.pair.car->type == SFS_PAIR)
        {
            DEBUG_MSG("on a une pair imbriquée");
            o = test_input->this.pair.car->this.pair.car;
            input_bis = test_input->this.pair.car;
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

            return o;
        }

        if(o->type == SFS_SYMBOL)
        {
            DEBUG_MSG("on a un car symbol : %s",o->this.symbol);
            if(is_form("define",o)==1)
            {
                return define(input_bis->this.pair.cdr->this.pair.car,input_bis->this.pair.cdr->this.pair.cdr->this.pair.car);
            }
            if(is_form("set!",o)==1)
            {
                return set(input_bis->this.pair.cdr->this.pair.car,input_bis->this.pair.cdr->this.pair.cdr->this.pair.car);
            }
            if(is_form("quote",o)==1)
            {
                return (input_bis->this.pair.cdr->this.pair.car);
            }

            if(is_form("if",o))
            {
                DEBUG_MSG("on a un if");
                object consequence = input->this.pair.cdr->this.pair.cdr;
                object alternative = input->this.pair.cdr->this.pair.cdr->this.pair.cdr;
                /*DEBUG_MSG("consequence = %s, type : %d",consequence->this.pair.car->this.string, consequence->this.pair.car->type);
                DEBUG_MSG("alternative = %s",alternative->this.pair.car->this.string);*/

                DEBUG_MSG("input car : %s",input->this.pair.car->this.symbol);
                object input_if = input->this.pair.cdr;

                if(strcmp(sfs_eval(input_if)->this.symbol,boolean_true->this.symbol)==0)
                {
                    DEBUG_MSG("on a un if vrai");
                    DEBUG_MSG("then : %d",input->this.pair.cdr->this.pair.cdr->this.pair.car->type);
                    return sfs_eval(consequence);
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
                        return sfs_eval(consequence);
                    }
                }
            }
            if(is_form("and",o))
            {
                DEBUG_MSG("on a un and");
                object input_and = input_bis;
                while(input_and != nil)
                {
                    DEBUG_MSG("(and ...)");
                    if(strcmp(boolean_false->this.symbol,sfs_eval(input_and->this.pair.cdr)->this.symbol)==0)
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
                    if(strcmp(boolean_true->this.symbol,sfs_eval(input_or->this.pair.cdr)->this.symbol)==0)
                        return boolean_true;
                    else input_or = input_or->this.pair.cdr;
                }
                return boolean_false;
            }

            else
            {
                if(chercher_symb(o->this.symbol)==1)
                {
                    return valeur_symb(o->this.symbol);
                }

                object p = make_primitive(evaluer_prim(o)) ;
                DEBUG_MSG("on a créé la prim");
                object liste_arg = evaluer_arg(input_bis->this.pair.cdr);
                DEBUG_MSG("on a une liste_arg");
                return liste_arg;
                return (p->this.prim.fonction)(liste_arg);
            /*    if(strcmp(o->this.symbol,"+")==0)
                {
                    strcpy(signe ,"+");
                    object liste_arg = evaluer_arg(input->this.pair.cdr);

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
                else
                {
                    DEBUG_MSG("on retourne %s",o->this.symbol);
                    return o;
                }*/
            }
        }


    }
    return input;
}
