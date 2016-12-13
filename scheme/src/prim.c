

#include "math.h"
#include "eval.h"
#include "prim.h"



/*** PRIMITIVES ***/

void creer_primitives(void)
{
    define(make_symbol("+"),make_primitive(plus_p),liste_env);
    define(make_symbol("-"),make_primitive(moins_p),liste_env);
    define(make_symbol(">"),make_primitive(sup_p),liste_env);
    define(make_symbol("<"),make_primitive(inf_p),liste_env);
    define(make_symbol("*"),make_primitive(mult_p),liste_env);
    define(make_symbol("quotient"),make_primitive(quotient_p),liste_env);
    define(make_symbol("remainder"),make_primitive(remainder_p),liste_env);
    define(make_symbol("="),make_primitive(egal_p),liste_env);

    define(make_symbol("boolean?"),make_primitive(boolean_p),liste_env);
    define(make_symbol("char?"),make_primitive(char_p),liste_env);
    define(make_symbol("symbol?"),make_primitive(symbol_p),liste_env);
    define(make_symbol("integer?"),make_primitive(integer_p),liste_env);
    define(make_symbol("string?"),make_primitive(string_p),liste_env);
    define(make_symbol("pair?"),make_primitive(pair_p),liste_env);
    define(make_symbol("nil?"),make_primitive(nil_p),liste_env);
    define(make_symbol("null?"),make_primitive(null_p),liste_env);
    define(make_symbol("procedure?"),make_primitive(procedure_p),liste_env);

    define(make_symbol("char->integer"),make_primitive(char_integer_p),liste_env);
    define(make_symbol("integer->char"),make_primitive(integer_char_p),liste_env);
    define(make_symbol("number->string"),make_primitive(number_string_p),liste_env);
    define(make_symbol("string->number"),make_primitive(string_number_p),liste_env);
    define(make_symbol("symbol->string"),make_primitive(symbol_string_p),liste_env);
    define(make_symbol("string->symbol"),make_primitive(string_symbol_p),liste_env);

    define(make_symbol("cons"),make_primitive(cons_p),liste_env);
    define(make_symbol("car"),make_primitive(car_p),liste_env);
    define(make_symbol("cdr"),make_primitive(cdr_p),liste_env);
    define(make_symbol("list"),make_primitive(liste_p),liste_env);

    define(make_symbol("set-car!"),make_primitive(set_car_p),liste_env);
    define(make_symbol("set-cdr!"),make_primitive(set_cdr_p),liste_env);
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




object plus_p (object nums)
{
    DEBUG_MSG("on va faire un plus");
    uint somme = 0;
    object l = nums;
    for (;l != nil && l != NULL; l = l->this.pair.cdr)
    {
        if(l->this.pair.car->type != SFS_NUMBER)
        {
            return pb_type;
        }
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
        if(l->this.pair.car->type != SFS_NUMBER)
        {
            return pb_type;
        }
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
        return arg_plus;
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
        return arg_plus;
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
        if(l->this.pair.car->type != SFS_NUMBER)
        {
            return pb_type;
        }
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
        DEBUG_MSG("cdr null");
        if(nums->this.pair.car->type != SFS_NUMBER)
        {
            return pb_type;
        }
        quotient = 1/quotient;
        DEBUG_MSG("l = %lf",quotient);
    }
    for (;l != nil && l != NULL; l = l->this.pair.cdr)
    {
        if(l->this.pair.car->type != SFS_NUMBER)
        {
            return pb_type;
        }
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
        return arg_plus;

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
        return pb_type;

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
        return pb_type;

    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
        return arg_plus;

    uint integer = (int)l->this.character ;
    object o_integer = make_integer(integer);
    return o_integer;
}
object integer_char_p (object arg)
{
    object l = arg->this.pair.car;
    if(l->type != SFS_NUMBER)
        return pb_type;

    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
        return arg_plus;

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
        return pb_type;

    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
        return arg_plus;

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
        return pb_type;

    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
        return arg_plus;

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
        return pb_type;

    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
        return arg_plus;

    string chaine;
    strcpy(chaine ,l->this.symbol);
    object o_string = make_string(chaine);
    return o_string;
}
object string_symbol_p (object arg)
{
    object l = arg->this.pair.car;
    if(l->type != SFS_STRING)
        return pb_type;

    if(arg->this.pair.cdr != NULL  && arg->this.pair.cdr != nil)
        return arg_plus;

    string chaine;
    strcpy(chaine ,l->this.string);
    object o_symbol = make_symbol(chaine);
    return o_symbol;
}

object boolean_p (object arg)
{

    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
        return arg_plus;

    if(arg->this.pair.car->type == SFS_BOOLEAN)
        return boolean_true;
    return boolean_false;
}
object char_p (object arg)
{
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
        return arg_plus;

    if(arg->this.pair.car->type == SFS_CHARACTER)
        return boolean_true;
    return boolean_false;
}
object symbol_p (object arg)
{
    DEBUG_MSG("type %d",arg->this.pair.car->type);
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
        return arg_plus;

    if(arg->this.pair.car->type == SFS_SYMBOL)
        return boolean_true;
    return boolean_false;
}
object integer_p (object arg)
{
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
        return arg_plus;

    if(arg->this.pair.car->type == SFS_NUMBER)
        return boolean_true;
    return boolean_false;
}
object string_p (object arg)
{
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
        return arg_plus;

    if(arg->this.pair.car->type == SFS_STRING)
        return boolean_true;
    return boolean_false;
}
object pair_p (object arg)
{
    DEBUG_MSG("type %d",arg->this.pair.car->type);
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
        return arg_plus;

    if(arg->this.pair.car->type == SFS_PAIR)
        return boolean_true;
    return boolean_false;
}
object nil_p (object arg)
{
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
        return arg_plus;

    if(arg->this.pair.car->type == SFS_NIL)
        return boolean_true;
    return boolean_false;
}
object null_p (object arg)
{
    DEBUG_MSG("null_p");
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
        return arg_plus;

    if(arg->this.pair.car == NULL || arg == NULL)
        return boolean_true;
    return boolean_false;
}

object cons_p (object arg)
{
    DEBUG_MSG("coucou");
    object l = arg;
    object o = make_pair();
    while(l!= NULL && l!= nil)
    {
        /*if(l->this.pair.car->type != SFS_PAIR)
        {
            WARNING_MSG("erreur, utilisez plutôt list ");
            return NULL;
        }*/    /*o = ajout_queue(o,sfs_eval(l->this.pair.car));*/

        {
            object l_bis = l;
            DEBUG_MSG("l_bis : %d",l_bis->type);
            while (l->this.pair.car->type == SFS_PAIR)
            {
                DEBUG_MSG("while");
                l_bis = l->this.pair.car;
                l = l->this.pair.car;
            }
            DEBUG_MSG("l_bis car %d",l_bis->this.pair.car->type);
            o = ajout_queue(o,sfs_eval(l_bis->this.pair.car,liste_env));

        }
        DEBUG_MSG("%d",l->this.pair.car->this.number.this.integer);
        l = l->this.pair.cdr;
    }
    return o;

}
object car_p (object arg)
{
    object l = arg->this.pair.car;
    DEBUG_MSG("type = %d",l->type);
    if(l->type != SFS_PAIR)
        {
            DEBUG_MSG("oula ça bug");

            return l;
        }
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
    if(l == NULL || l == nil)
        return cdr_pb;
    return l;
}
object liste_p (object arg)
{
    object l = arg;
    object liste = make_pair();
    while(l!=NULL && l!=nil)
    {
        liste = ajout_queue(liste,sfs_eval(l->this.pair.car,liste_env));
        l = l->this.pair.cdr;
    }
    return liste;
}
object set_car_p (object arg)
{
    object l = arg;
    object var = l->this.pair.car;
    DEBUG_MSG("type = %d",var->type);
    if(var->type != SFS_PAIR)
        return var_non_def;

    if(l->this.pair.cdr == NULL || l->this.pair.cdr== nil)
        return arg_moins;

    object new_car = l->this.pair.cdr->this.pair.car;
    DEBUG_MSG("var: %s",var->this.symbol);
    if(var == NULL)
        return var_non_def;

    var->this.pair.car = new_car;
    return var;
}
object set_cdr_p (object arg)
{
    object l = arg;
    object var = l->this.pair.car;
    if(var->type != SFS_PAIR)
        return var_non_def;

    if(l->this.pair.cdr == NULL || l->this.pair.cdr == nil)
        return arg_moins;

    object new_cdr = l->this.pair.cdr->this.pair.car;
    if(var == NULL)
        return var_non_def;

    var->this.pair.cdr = new_cdr;
    return var;
}

object procedure_p (object arg)
{
    if(arg->this.pair.cdr != NULL && arg->this.pair.cdr != nil)
        return arg_plus;

    if(arg->this.pair.car->type == SFS_COMPOUND)
        return boolean_true;
    return boolean_false;
}
