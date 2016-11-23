
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "read.h"






void flip( uint *i ) {

    if ( *i == FALSE ) {
        *i = TRUE;
    }
    else {
        *i = FALSE;
    }
}

/*
 * @fn char* first_usefull_char(char* line)
 *
 * @brief retourne un pointeur sur le premier caractere utile dans line
 * ou NULL si line ne contient que des espaces et des commentaires
 */
char* first_usefull_char(char* line) {

    int i=0;
    if (line == NULL) {
        return NULL;
    }
    i = 0;
    /* on saute les espaces */
    while(line[i] != '\0' && isspace(line[i])) {
        i++;
    }
    /* si fin de ligne => ligne inutile */
    if(line[i] == '\0') {
        return NULL;
    }
    /* si premier caractere non espace est ';' => ligne inutile */
    if(line[i] == ';') {
        return NULL;
    }
    return line + i; /* ligne utile */
}

/**
 * @fn uint  sfs_get_sexpr( char *input, FILE *fp )
 *
 * @brief extrait la prochaine S-Expression dans le flux fp et la stocke dans input
 * (meme si elle est repartie sur plusieurs lignes)
 * @param fp (FILE *) flux d'entree (ouvert en ecriture, mode texte)
 * @param input (char *) chaine allouee de taille BIGSTRING, dans laquelle la S-Expression sera stockée
 *
 * @return S_OK si une S-Expression apparemment valide a ete trouvee
 * @return S_KO si on n'a pas trouve de S-Expression valide
 * @return S_END si fin de fichier atteinte sans avoir lu de caractere utile.
 *
 * sfs_get_sexpr commence par lire une ligne dans fp,
 * puis compte le nombre de parentheses ouvrantes et fermantes sur la ligne.
 * Les parentheses dans des chaines et les caracteres Scheme #\( et #\)
 * ne sont pas comptes.
 *
 * Si le compte devient zéro et que
 *        - la ligne est fini, la fonction retourne S_OK
 * 				- la ligne n'est pas fini la fonction retourne S_KO
 *
 * S'il y a plus de parentheses fermantes qu'ouvrantes,
 * la fonction retourne S_KO.
 * Les commentaires et espaces qui figurent a la fin de chaque ligne
 * sont remplacés par un espace.
 * Les espaces qui figurent a la fin de la S-Expression (y compris '\n')
 * sont supprimés.
 *
 * Attention : cette fonction refuse les S-Expression multiple sur une seule ligne. Ainsi :
 *    a b c
 *    (qqchose) (autrechose)
 *    (qqchose) 78
 * seront considereees comme des erreurs
 * et la fonction retournera S_KO
 *
 * @pre fp doit etre prealablement ouvert en lecture
 * @pre input doit etre prealablement alloue en memoire, de taille BIGSTRING
 */

typedef enum {
    NOTHING,        /* rien n'a ete trouve encore.. */
    STRING_ATOME,   /* la premiere trouvee dans la ligne semble etre un atome */
    BASIC_ATOME,    /* la premiere trouvee dans la ligne semble etre d'une chaine */
    S_EXPR_PARENTHESIS, /* la premiere trouvee dans la ligne semble etre une expression parenthesee */
    FINISHED        /* on a trouve une S-Expr bien formee */
} EXPRESSION_TYPE_T;

uint  sfs_get_sexpr( char *input, FILE *fp ) {
    int       parlevel = 0;
    uint      in_string = FALSE;
    uint      s = 0;
    char      k[BIGSTRING];
    char      *ret = NULL;
    char      *chunk = NULL;
    uint      i = 0;
    string    sfs_prompt;

    EXPRESSION_TYPE_T typeOfExpressionFound = NOTHING;

    parlevel = 0;
    memset( input, '\0', BIGSTRING );

    do {
        ret = NULL;
        chunk=k;
        memset( chunk, '\0', BIGSTRING );

        /* si en mode interactif*/
        if ( stdin == fp ) {
            uint nspaces = 2*parlevel;

            init_string( sfs_prompt );

            /* le prompt indique le niveau de parenthese
               et decale la prochaine entrée en fonction
               de ce niveau (un peu à la python)*/
            sprintf( sfs_prompt, "SFS:%u > ", parlevel );

            for ( i= 0; i< nspaces; i++ ) {
                sfs_prompt[strlen(sfs_prompt)] = ' ';
            }

            /* si sur plusieurs lignes, le \n équivaut à un espace*/
            if (nspaces>0) {
                input[strlen(input)+1] = '\0';
                input[strlen(input)] = ' ';
            }

            /*saisie de la prochaine ligne à ajouter dans l'input*/
            printf("%s",sfs_prompt);
            ret = fgets( chunk, BIGSTRING, fp );
            if (ret && chunk[strlen(chunk)-1] == '\n') chunk[strlen(chunk)-1] = '\0';

        }
        /*si en mode fichier*/
        else {
            ret = fgets( chunk, BIGSTRING, fp );

            if ( NULL == ret ) {
                /* fin de fichier...*/
                if ( parlevel != 0 ) {
                    WARNING_MSG( "Parse error: missing ')'" );
                    return S_KO;
                }
                return S_END;
            }

            if (strlen(chunk) == BIGSTRING-1
                    && chunk[BIGSTRING-1] != '\n'
                    && !feof(fp)) {
                WARNING_MSG( "Too long line for this interpreter!" );
                return S_KO;
            }
        }

        /* si la ligne est inutile
        	=> on va directement à la prochaine iteration */
        if (first_usefull_char(chunk) == NULL) {
            continue;
        }


        s = strlen( chunk );

        if ( s > 0 ) {
            if (strlen(input) + s > BIGSTRING-1 ) {
                WARNING_MSG( "Too long a S-expression for this interpreter!" );
                return S_KO;
            }

            for ( i = 0; i< strlen(chunk); i++ ) {
                /* si la fin de la ligne chunk est inutile,
                   on ajoute un espace dans input et on sort de la boucle*/
                if ( in_string == FALSE && first_usefull_char(chunk + i) == NULL ) {
                    chunk[i]='\0';
                    input[strlen(input)] = ' ';
                    break;
                }


                switch(chunk[i]) {
                case '(':
                    if (in_string == FALSE
                            && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) ) {
                        parlevel++;
                        typeOfExpressionFound = S_EXPR_PARENTHESIS;
                    }
                    break;
                case ')':
                    if ( in_string == FALSE
                            && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) ) {
                        parlevel--;
                        if (parlevel == 0 ) {
                            typeOfExpressionFound = FINISHED;
                        }
                        if ( parlevel < 0 ) {
                            WARNING_MSG( "Parse error : cannot start with ')'" );
                            return S_KO;
                        }
                    }
                    break;
                case '"':
                    if ( i<2 || chunk[i-1] != '\\' ) {
                        if ( in_string == FALSE ) {
                            if(typeOfExpressionFound == BASIC_ATOME) {
                                WARNING_MSG("Parse error: invalid string after atom : '%s'", chunk+i);
                                return S_KO;
                            }
                            in_string = TRUE;
                            if(typeOfExpressionFound != S_EXPR_PARENTHESIS) {
                                typeOfExpressionFound = STRING_ATOME;
                            }
                        }
                        else {
                            in_string = FALSE;
                            if(typeOfExpressionFound == STRING_ATOME) {
                                typeOfExpressionFound = FINISHED;
                            }
                        }
                    }
                    break;
                default:
                    if(in_string == FALSE) {
                        if(isspace(chunk[i])) {
                            if(typeOfExpressionFound == BASIC_ATOME) {
                                typeOfExpressionFound = FINISHED;
                            }
                        } else if(typeOfExpressionFound != S_EXPR_PARENTHESIS) {
                            typeOfExpressionFound = BASIC_ATOME;
                        }
                    }
                    break;
                }


                if(typeOfExpressionFound == FINISHED) {
                    char *first_useful = first_usefull_char(chunk + i + 1);
                    if( first_useful != NULL) {
                        if(*first_useful == ')' ) {
                            WARNING_MSG( "Parse error: too many closing parenthesis')'" );
                        }
                        else {
                            WARNING_MSG("Parse error: invalid trailing chars after S-Expr : '%s'", chunk+i);
                        }
                        return S_KO;
                    }
                }

                /* recopie char par char*/
                input[strlen(input)] = chunk[i];
            }
            if(in_string == TRUE) {
                WARNING_MSG( "Parse error: non terminated string on line %s", chunk );
                return S_KO;
            }
        }


        if ( parlevel > 0 && fp != stdin ) {
            if ( feof( fp ) ) {
                WARNING_MSG( "Parse error: missing ')'" );
                return S_KO;
            }

            if (input[strlen(input)-1] == '\n') input[strlen(input)-1] = ' ';
        }
    } while ( parlevel > 0 );

    /* Suppression des espaces restant a la fin de l'expression, notamment le dernier '\n' */
    while (isspace(input[strlen(input)-1])) input[strlen(input)-1] = '\0';

    return S_OK;
}
/*


************* TEST DES CARACTERES ****************/

int test_integer (char* charactere)	/*teste si le premier caractère est un chiffre et retourne 1 */
{
	if(charactere[0] ==  '0'|| charactere[0] == '1' || charactere[0] == '2' ||charactere[0] ==  '3' || charactere[0] == '4' ||charactere[0] ==  '5' ||charactere[0] ==  '6' || charactere[0] == '7' || charactere[0] == '8' ||charactere[0] == '9' )
	return 1;
	else return 0;
}

int test_signed_integer (char*input, uint *i) /* teste si la chaine commence par un signe ou un entier, et retourne 1 si oui, -1 sinon*/
{
	if(test_integer(&input[*i])==1)
		return 1;
	if(input[*i]=='+'||input[*i]== '-')
	{
		if(test_integer(&input[*i+1]) == 1)
			return 1;
	}
	return -1 ;
}

int test_character (char* input, uint *here)	/*teste les différents cas de caracteres*/
{
	uint i = *here;

	if(input[i]== '#' )/* on a un caractère pour '#\'*/
	{
		if(input[i+1]=='\\')
		{
			i = i+2;
			if(strncmp(input+i,"newline",6)==0)	/*si on a #\newline test_character renvoie 2*/
				return 2;
			if( strncmp(&input[i],"space",5)==0) /*si on a #\space test_character renvoie 3*/
				return 3;

			if(input[i+1] != 0 && input[i+1] != 32 && input[i+1] != ')')	/*si on a #\ mais un caractere plus long que 1, on renvoie -1 -> symbole */
			{
				DEBUG_MSG("le caractere est trop long\n");
				return -1;
			}
		}
		else
		{
			if(input[i+1]=='t') /* si le caractere est un booleen, on renvoie 4*/
				return 4;
			if(input[i+1]=='f') /* si le caractere est un booleen, on renvoie 5*/
				return 5;
		}
		return 1; /* si le caractere n'est pas trop long, on renvoie 1*/

	}
	return 0;
}

int test_string (char* input, uint *here) /* test si on a des guillemets et renvoie 1 // NE GÈRE PAS si on ne finit pas la chaine par un ' */
{
	if(input[*here]=='\'' || input[*here]=='\"')
	{
		uint i = *here + 1;
		while(input[i]!='\'' && input[i]!='\"')
		{
			if(input[i]<32 || input[i]==')' || input[i]=='(')
			{
                i = next_char(input, &i);
                if(input[i]!='\"' && input[i]!='\'')
            	   return 0;
            }
			i++;
		}
		return 1;

	}
	return 0;
}

uint taille_int (uint integer)	/* renvoie la taille d'un entier (nb de characteres) */
{
    if (integer == 0)
        return 1;
	uint n = log10(integer)+1;
	return n;
}


uint taille_string(string chaine)
{
	int i = 0;
	while(chaine[i]!='\0') i++;
	return i;
}




/*****************FONCTIONS DE CONVERSTION*************/
int string_to_integer(char *input, uint *here)
{
	*here = next_char(input,here);
	uint indice = *here;
	string tmp_chaine;
	int integer = 0;
	strcpy(tmp_chaine,&input[*here]);

	if(input[indice]=='+'|| input[indice]=='-') indice ++;
	while(input[indice] != '\0' && input[indice]!= 32 && input[indice]!= 41 )
	{
		if(test_integer(&input[indice])==0)
		{
			return -1;
		}
		indice++;
	}
	integer = atoi(tmp_chaine);

	return integer;
}





object input_to_string (char* input, uint *here)
{
	uint i = *here;
	if(input[i]=='\''|| input[i]=='\"') i++;

	string tmp_chaine;
	string chaine;
	strcpy(tmp_chaine,&input[i]) ;

	for( ; input[i]!='\"' && input[i]!='\'' ; i++)
	{
		DEBUG_MSG("%c",input[i]);
	}
	strncpy(chaine,tmp_chaine, i-*here-1);
    object string = make_string(chaine);
	return  string;
}




object input_to_symbol (char* input, uint *here)
{
	uint i = *here;

	string tmp_chaine;
	string * chaine = malloc(sizeof(chaine));
    strcpy(*chaine,"\0");
	strcpy(tmp_chaine,&input[i]) ;

	while(input[i]!= 0 && input[i]!= 41 && input[i]>32 && input[i]!=32)
	{
		i++;
	}
	strncpy(*chaine,tmp_chaine, i-*here);
    DEBUG_MSG("%s",*chaine);
    object symbol = make_symbol(*chaine);
	return symbol;
}




/******* FONCTIONS CONS, CAR ET CDR **********/

object cons_pair (object car, object cdr)
{
	DEBUG_MSG("entree dans cons_pair");
    object o = make_pair() ;
    object p = make_pair() ;
    p->this.pair.car = cdr;
	p->this.pair.cdr = nil;
    o->this.pair.car = car;
    o->this.pair.cdr = p;
    return o ;
}



/**************     READ      *****************/




object sfs_read_atom( char *input, uint *here)
{
	uint test = 0 ;
	*here = next_char(input,here);

	while(*here<strlen(input))
	{
		if(test_signed_integer(input,here) == 1)
		{
			int integer = string_to_integer(input,here);  /*test si la suite est tj un chiffre*/
			if(integer<0)
			{
				*here += taille_int(abs(integer))+1;
				return make_integer(integer);
			}
			else
			{
				if(input[*here]=='+')
					*here += taille_int(integer)+1;
				else *here += taille_int(integer);
				return make_integer(integer);
			}
		}

		test = test_character(input,here);
		if(test ==1)
		{
			char caractere = input[*here+2];
			*here += 3;
			return make_character(caractere);
		}
		if(test ==2)
		{
			*here += 9;
			string chaine = "#\\newline";
			return make_character_special(chaine);
		}
		if(test ==3)
		{
			*here += 7;
			string chaine = "#\\space";
			return make_character_special(chaine);
		}
		if (test ==4)
		{
			*here += 2;
			return make_boolean(TRUE);
		}
		if (test ==5)
		{
			*here += 2;
			return make_boolean(FALSE);
		}

		if(test == -1)
		{
			DEBUG_MSG("sfs_read_atom : on lit un caractere trop long");
		}

		if(test_string(input,here)==1)
		{
            uint* string_here = here;
            object string = input_to_string(input, string_here);
			*here += taille_string(string->this.string)+2;
			return string;
		}

		if(input[*here] > 32 && *here < strlen(input))
		{
			if(input[*here]=='\'')
			{
				*here += 1;
				return cons_pair(make_symbol("quote"),sfs_read(input,here));
			}
            uint *symb_here = here;
            object o = input_to_symbol(input,symb_here);
            *here+= taille_string(o->this.symbol);
			return o;
		}
	}
	return NULL ;
}





object sfs_read_pair (char* stream, uint *i)

{

	*i = next_char(stream,i);
	if(stream[*i]== ')' || stream[*i] < 31)
	{
		(*i)++;
		return nil;
	}
	*i = next_char(stream,i);

	if(stream[*i]<31) return nil;

	object pair = make_pair();

	*i = next_char(stream,i);
	pair->this.pair.car = sfs_read(stream, i);
	pair->this.pair.cdr = sfs_read_pair(stream,i);
	return pair;
}

uint next_char (char*input, uint *i)
{
	while(input[*i]==32 || input[*i]== 8 || input[*i]==9 )

	{
		if(*i<strlen(input))

		(*i)++;
	}
	return *i;
}




object sfs_read( char *input, uint *here )
{
	*here = next_char(input,here);
	{
		if(input[*here]<33)
			(*here)++;
			*here = next_char(input,here);

		if(*here<strlen(input) && input[*here]!=41)
		{
			if ( input[*here] == '(' )
			{
				if ( input[(*here)+1] == ')' )
				{
					*here += 2;
					return nil;
				}
				else
				{
					(*here)++;
					*here = next_char(input,here);

					return sfs_read_pair( input, here  );
					DEBUG_MSG("? \n");
				}
			}
			else
			{
				*here = next_char(input,here);

				return sfs_read_atom( input, here );
			}
		}
	}
	return NULL ;
}
