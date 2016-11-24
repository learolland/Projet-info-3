
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
    object binding_ajout = make_pair();
    binding_ajout->this.pair.car = make_liste() ;
    strcpy(binding_ajout->this.pair.car->this.liste.nom,variable->this.symbol);
    if(valeur->type == SFS_PRIM)
    {
        binding_ajout->this.pair.car->this.liste.val = valeur;

    }
    else binding_ajout->this.pair.car->this.liste.val = sfs_eval(valeur);



    object tmp_binding;
    object binding = liste_env->this.pair.car;

    if(binding == NULL || binding == nil)
    {
        binding = binding_ajout;
        binding->this.pair.cdr = nil;
        liste_env->this.pair.car = binding;
        return 1;
    }

    tmp_binding = make_pair();


    tmp_binding->this.pair.cdr = binding_ajout;
    binding_ajout->this.pair.cdr = binding;
    liste_env->this.pair.car = binding_ajout;
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
            /*DEBUG_MSG("on sort du deuxieme palier");*/
            binding = binding->this.pair.cdr;
        }
        /*DEBUG_MSG("on sort du premier palier");*/
        env = (liste_env)->this.pair.cdr;
        if(env!= NULL && env != nil) binding = env->this.pair.car;
    }
    WARNING_MSG("il n'y a pas de symbol %s dans l'environnement",nom);
    return NULL;
}

uint chercher_symb (string nom)
{
    object env = liste_env;
    if(liste_env->this.pair.car == NULL)
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

/*object cons(object car, object cdr)
{
    object o = make_pair() ;
    object p = make_pair() ;
    p->this.pair.car = cdr;
    p->this.pair.cdr = nil;
    o->this.pair.car = car;
    o->this.pair.cdr = p;
    return o ;
}*/

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
object define (object variable, object valeur)
{
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

double partie_entiere (double nombre)
{
    double seuil = 0;
    while (seuil <= nombre)
        seuil ++;

    double comp = seuil-0.5;

    DEBUG_MSG("seuil : %lf",seuil);
    if(comp==nombre)
    {
        DEBUG_MSG("0,5");
        return (uint)seuil;
    }
    if(comp<=nombre)
    {
        DEBUG_MSG("dif : %lf",nombre-seuil);
        return (uint)seuil;
    }
    else
    {
        DEBUG_MSG("dif : %lf",nombre-(seuil+0.5));
        return (uint)seuil-1;
    }
}

/*** PRIMITIVES ***/

object plus_p (object nums)
{
    DEBUG_MSG("on va faire un plus");
    uint somme = 0;
    object l = nums;
    for (;l != nil && l != NULL; l = l->this.pair.cdr)
    {
        DEBUG_MSG("on entre dans le while, %d",l->this.pair.car->this.number.this.integer);
        somme += l->this.pair.car->this.number.this.integer;
        DEBUG_MSG("l = %d",somme);
    }
    DEBUG_MSG("on sort du while ");
    object o_somme = make_integer(somme);
    DEBUG_MSG("objet final : %d",o_somme->this.number.this.integer);
    return o_somme;
}

object moins_p (object nums)
{
    uint diff = nums->this.pair.car->this.number.this.integer;
    object l = nums->this.pair.cdr;

    for (;l != nil && l != NULL; l = l->this.pair.cdr)
    {
        DEBUG_MSG("on entre dans le while, %d",l->this.pair.car->this.number.this.integer);
        diff -= l->this.pair.car->this.number.this.integer;
        DEBUG_MSG("l = %d",diff);
    }
    DEBUG_MSG("on sort du while ");
    object o_diff = make_integer(diff);
    DEBUG_MSG("objet final : %d",o_diff->this.number.this.integer);
    return o_diff;
}

object sup_p (object nums)
{
    object l = nums ;
    if(l->this.pair.cdr->this.pair.cdr != NULL && l->this.pair.cdr->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    if(l->this.pair.car->this.number.this.integer > l->this.pair.cdr->this.pair.car->this.number.this.integer)
        return boolean_true ;
    else return boolean_false;
}

object inf_p (object nums)
{
    object l = nums ;
    if(l->this.pair.cdr->this.pair.cdr != NULL && l->this.pair.cdr->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    if(l->this.pair.car->this.number.this.integer < l->this.pair.cdr->this.pair.car->this.number.this.integer)
        return boolean_true ;
    else return boolean_false;
}

object mult_p (object nums)
{
    object l = nums ;
    uint mult = 1;
    for (;l != nil && l != NULL; l = l->this.pair.cdr)
    {
        DEBUG_MSG("on entre dans le while, %d",l->this.pair.car->this.number.this.integer);
        mult = mult *  l->this.pair.car->this.number.this.integer;
        DEBUG_MSG("l = %d",mult);
    }
    object o_mult = make_integer(mult);
    return o_mult;
}

object quotient_p (object nums)
{
    double quotient = nums->this.pair.car->this.number.this.integer;
    object l = nums->this.pair.cdr ;
    if(l == NULL || l == nil)
    {
        quotient = 1/quotient;
        DEBUG_MSG("l = %lf",quotient);
    }
    for (;l != nil && l != NULL; l = l->this.pair.cdr)
    {
        quotient = quotient /  l->this.pair.car->this.number.this.integer;
        DEBUG_MSG("l = %lf",quotient);
    }
    uint int_quotient = partie_entiere(quotient);
    DEBUG_MSG("int : %d, double : %lf",int_quotient,quotient);
    object o_quotient = make_integer(int_quotient);
    return o_quotient;
    /*double nb = nums->this.pair.car->this.number.this.integer;
    object l = nums->this.pair.cdr ;
    double dividende = l->this.pair.car->this.number.this.integer;
    object quotient = NULL;
    for (;l != nil && l != NULL; l = l->this.pair.cdr)
    {
        quotient = partie_entiere(nb/dividende);
        DEBUG_MSG("l = %lf",quotient);
    }*/
}

object remainder_p (object nums)
{
    double nb = nums->this.pair.car->this.number.this.integer;
    object l = nums->this.pair.cdr ;
    double dividende = l->this.pair.car->this.number.this.integer;
    if(l->this.pair.cdr != NULL && l->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }

    double quotient = nb/dividende;
    double seuil = 0;
    while(seuil < fabs(quotient)-1) seuil++;
    DEBUG_MSG("seuil = %lf, quotient %lf, fabs(dividende) : %lf", seuil,quotient,fabs(dividende));
    uint reste = nb - seuil*fabs(dividende);
    if (reste == dividende) reste = 0;
     object o_reste = make_integer(reste);
    return o_reste;
}

object egal_p (object nums)
{
    object l = nums;
    if(l->this.pair.car->type != SFS_NUMBER)
    {
        WARNING_MSG("attention, les arguments ne sont pas des nombres");
        return NULL;
    }
    uint i = l->this.pair.car->this.number.this.integer;
    l = l->this.pair.cdr;
    while(l != nil && l != NULL)
    {
        if(l->this.pair.car->this.number.this.integer != i)
            return boolean_false;
        l = l->this.pair.cdr;
    }
    return boolean_true;
}



object char_integer_p (object arg)
{
    object l = arg->this.pair.car;
    if(l->type != SFS_CHARACTER)
    {
        WARNING_MSG("il ne s'agit pas d'un caractere");
        return NULL;
    }
    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    uint integer = (int)l->this.character ;
    object o_integer = make_integer(integer);
    return o_integer;
}

object integer_char_p (object arg)
{
    object l = arg->this.pair.car;
    if(l->type != SFS_NUMBER)
    {
        WARNING_MSG("il ne s'agit pas d'un entier");
        return NULL;
    }
    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    uint integer = l->this.number.this.integer;
    string string_integer ;
    sprintf(string_integer,"%d",integer);

    char character = string_integer[0];
    object o_character = make_character(character);
    return o_character;
}

object number_string_p (object arg)
{
    object l = arg->this.pair.car;
    if(l->type != SFS_NUMBER)
    {
        WARNING_MSG("il ne s'agit pas d'un entier");
        return NULL;
    }
    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    uint integer = l->this.number.this.integer;
    string string_integer ;
    sprintf(string_integer,"%d",integer);

    object o_string = make_string(string_integer);
    return o_string;
}

object string_number_p (object arg)
{
    object l = arg->this.pair.car;
    if(l->type != SFS_STRING)
    {
        WARNING_MSG("il ne s'agit pas d'un entier");
        return NULL;
    }
    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    string chaine;
    strcpy(chaine ,l->this.string);
    uint number = atoi(chaine);
    DEBUG_MSG("number : %d, %s",number,chaine);
    object o_number = make_integer(number);
    return o_number;
}

object symbol_string_p (object arg)
{
    object l = arg->this.pair.car;
    if(l->type != SFS_SYMBOL)
    {
        WARNING_MSG("il ne s'agit pas d'un symbol");
        return NULL;
    }
    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    string chaine;
    strcpy(chaine ,l->this.symbol);
    object o_string = make_string(chaine);
    return o_string;
}

object string_symbol_p (object arg)
{
    object l = arg->this.pair.car;
    if(l->type != SFS_STRING)
    {
        WARNING_MSG("il ne s'agit pas d'une chaine");
        return NULL;
    }
    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    string chaine;
    strcpy(chaine ,l->this.string);
    object o_symbol = make_string(chaine);
    return o_symbol;
}





object boolean_p (object arg)
{

    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    if(arg->this.pair.car->type == SFS_BOOLEAN)
        return boolean_true;
    return boolean_false;
}

object char_p (object arg)
{
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    if(arg->this.pair.car->type == SFS_CHARACTER)
        return boolean_true;
    return boolean_false;
}

object symbol_p (object arg)
{
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    if(arg->this.pair.car->type == SFS_SYMBOL)
        return boolean_true;
    return boolean_false;
}

object integer_p (object arg)
{
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    if(arg->this.pair.car->type == SFS_NUMBER)
        return boolean_true;
    return boolean_false;
}

object string_p (object arg)
{
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    if(arg->this.pair.car->type == SFS_STRING)
        return boolean_true;
    return boolean_false;
}

object pair_p (object arg)
{
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    if(arg->this.pair.car->type == SFS_PAIR)
        return boolean_true;
    return boolean_false;
}

object nil_p (object arg)
{
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    if(arg->this.pair.car->type == SFS_NIL)
        return boolean_true;
    return boolean_false;
}

object null_p (object arg)
{
    DEBUG_MSG("null_p");
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
    {
        WARNING_MSG("il y a trop d'arguments");
        return NULL;
    }
    if(arg->this.pair.car == NULL || arg == NULL)
        return boolean_true;
    return boolean_false;
}





object cons_p (object arg)
{
    object l = arg;
    object o = make_pair();
    while(l!= NULL && l!= nil)
    {
        if(l->this.pair.car->type != SFS_PAIR)
            o = ajout_queue(o,sfs_eval(l->this.pair.car));
        else
        {
            object l_bis = l->this.pair.car;
            while (l->this.pair.car->type == SFS_PAIR)
            {
                l_bis = l->this.pair.car;
                l = l->this.pair.car;
            }
            o = ajout_queue(o,sfs_eval(l_bis->this.pair.car));

        }
        DEBUG_MSG("%d",l->this.pair.car->this.number.this.integer);
        l = l->this.pair.cdr;
    }
    return o;

}

object car_p (object arg)
{
    object l = arg->this.pair.car;
    DEBUG_MSG("type = %d",l->this.pair.car->type);
    if(l->this.pair.car->type != SFS_PAIR)
        return l->this.pair.car;
    else
    {
        object l_bis = l->this.pair.car;
        while (l->this.pair.car->type == SFS_PAIR)
        {
            l_bis = l->this.pair.car;
            l = l->this.pair.car;
        }
        return l_bis;
    }
}

object cdr_p (object arg)
{
    object l = arg->this.pair.car->this.pair.cdr ;

    return l;
}


/********** EVALUATION ***********/
object evaluer_arg (object liste)
{
    DEBUG_MSG("evaluation des arguments");
    object liste_arg = make_pair();
    object ptr_liste = liste ;
    uint i = 0;
    /*DEBUG_MSG("car de la liste : %d",ptr_liste->this.pair.car->this.number.this.integer);*/
    while(ptr_liste != NULL && ptr_liste != nil)
    {
        liste_arg = ajout_queue(liste_arg,sfs_eval(ptr_liste->this.pair.car));
        ptr_liste = ptr_liste->this.pair.cdr;
        i++;
    }
    return liste_arg;
}


void creer_primitives(void)
{
    define(make_symbol("+"),make_primitive(plus_p));
    define(make_symbol("-"),make_primitive(moins_p));
    define(make_symbol(">"),make_primitive(sup_p));
    define(make_symbol("<"),make_primitive(inf_p));
    define(make_symbol("*"),make_primitive(mult_p));
    define(make_symbol("quotient"),make_primitive(quotient_p));
    define(make_symbol("remainder"),make_primitive(remainder_p));
    define(make_symbol("="),make_primitive(egal_p));

    define(make_symbol("boolean?"),make_primitive(boolean_p));
    define(make_symbol("char?"),make_primitive(char_p));
    define(make_symbol("symbol?"),make_primitive(symbol_p));
    define(make_symbol("integer?"),make_primitive(integer_p));
    define(make_symbol("string?"),make_primitive(string_p));
    define(make_symbol("pair?"),make_primitive(pair_p));
    define(make_symbol("nil?"),make_primitive(nil_p));
    define(make_symbol("null?"),make_primitive(null_p));

    define(make_symbol("char->integer"),make_primitive(char_integer_p));
    define(make_symbol("integer->char"),make_primitive(integer_char_p));
    define(make_symbol("number->string"),make_primitive(number_string_p));
    define(make_symbol("string->number"),make_primitive(string_number_p));
    define(make_symbol("symbol->string"),make_primitive(symbol_string_p));
    define(make_symbol("string->symbol"),make_primitive(string_symbol_p));

    define(make_symbol("cons"),make_primitive(cons_p));
    define(make_symbol("car"),make_primitive(car_p));
    define(make_symbol("cdr"),make_primitive(cdr_p));

}



object sfs_eval( object input )
{
    restart:

    DEBUG_MSG("EVAL");
    if(input->type == SFS_SYMBOL)
    {
        DEBUG_MSG("on a un symbol");
        object p = valeur_symb(input->this.symbol);
        if(p!= NULL) return p;
        else return input;
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
                return define(input_bis->this.pair.cdr->this.pair.car,input_bis->this.pair.cdr->this.pair.cdr->this.pair.car);
            }
            if(is_form("set!",o)==1)
                return set(input_bis->this.pair.cdr->this.pair.car,input_bis->this.pair.cdr->this.pair.cdr->this.pair.car);
            if(is_form("quote",o)==1)
                return (input_bis->this.pair.cdr->this.pair.car);
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
                alternative = NULL;
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
                object p = valeur_symb(o->this.symbol) ;
                if (p!=NULL)
                {
                    DEBUG_MSG("on a créé la prim p type = %d",p->type);
                    object liste_arg = evaluer_arg(input_bis->this.pair.cdr);
                    DEBUG_MSG("on a une liste_arg");
                    /*{
                        object tmp_liste = liste_arg ;
                        while(tmp_liste!= NULL && tmp_liste != nil)
                        {
                            if(tmp_liste->this.pair.car->type == SFS_NUMBER)
                            DEBUG_MSG("affichage : %d",tmp_liste->this.pair.car->this.number.this.integer);
                            else DEBUG_MSG("affichage : %s, type %d",tmp_liste->this.pair.car->this.symbol,tmp_liste->this.pair.car->type);

                            tmp_liste = tmp_liste->this.pair.cdr;
                        }
                    }*/
                    return (p->this.prim.fonction)(liste_arg);
                }
                else return o;
            }
        }

    }
    return input;
}
