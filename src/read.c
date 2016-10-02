
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "read.h"


/* Globals */
object  nil;
object  boolean_true;
object  boolean_false;
object root ;
uint nb_maillon = 0;

void init (void)
{
	DEBUG_MSG("init\n");
	boolean_true = make_boolean(TRUE);
	boolean_false = make_boolean(FALSE);
	root = make_pair();

    nil = make_nil();
}


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

int test_integer (char* charactere)
{
	/*DEBUG_MSG("entree dans test_integer\n");*/
	if(charactere[0] ==  '0'|| charactere[0] == '1' || charactere[0] == '2' ||charactere[0] ==  '3' || charactere[0] == '4' ||charactere[0] ==  '5' ||charactere[0] ==  '6' || charactere[0] == '7' || charactere[0] == '8' ||charactere[0] == '9' )
	return 1;
	else return 0;
}

int test_character (char* input, uint *here)
{
	DEBUG_MSG("entree dans test_character\n");
	uint i = *here;

	if(input[i]== '#' && input[i+1]=='\\')
	{
		i = i+2;
		DEBUG_MSG("dans test integer : i : %d, input :%s\n",i,input+i);
		if(strncmp(input+i,"newline",6)==0)
		{
			return 2;
		}
		if( strcmp(&input[i],"space")==0)
			return 3;
		if(input[i+1] == 0 || input[i+1] == 32 || input[i+1] == 41)
		{
			DEBUG_MSG("le caractere est trop long\n");
			return -1;
		}
		if(input[i]=='t')
		{
			DEBUG_MSG("boolen t\n");
			return 4;
		}
		if(input[i]=='f')
		{
			DEBUG_MSG("boolen t\n");

			return 5;
		}
		return 1;

	}
}

int test_string (char* input, uint *here)
{
	if(input[*here]=='\'')
		return 1;
	return 0;
}

uint taille_int (uint integer)
{
	uint n = log10(integer)+1;
	return n;
}

/*****************FONCTIONS DE CONVERSTION*************/
uint string_to_integer(char *input, uint *here)
{
	DEBUG_MSG("                entree dans string_to_integer\n");
	uint indice = *here;
	string tmp_chaine;
	int integer = 0;
	strcpy(tmp_chaine,&input[*here]);
	indice++;
	while(input[indice] != '\0' && input[indice]!= 32 && input[indice]!= 41 )
	{
		if(test_integer(&input[indice])==0)
		{
			DEBUG_MSG("                ce n'est pas un entier\n");
			return -1;
		}
		indice++;
	}
	integer = atoi(tmp_chaine);
	return integer;
}





char * input_to_string (char* input, uint *here)
{
	uint i = *here + 1;
	DEBUG_MSG("début de la chaine : %d\n",i);
	string tmp_chaine;
	strcpy(tmp_chaine,&input[i]) ;
	DEBUG_MSG("                copie de input dans tmp_chaine : %s\n",tmp_chaine );
	char* chaine = malloc(sizeof(*chaine));
	for( ; input[i]!=39 ; i++)
	{
		DEBUG_MSG("%c\n",input[i]);
	}
	DEBUG_MSG("                fin de la chaine, i = %d\n",i);
	strncpy(chaine,tmp_chaine, i-*here-1);
		DEBUG_MSG("                     chaine  = %s , taille : %lu\n",chaine,strlen(chaine));
	/*DEBUG_MSG("                 on copie tmp dans chaine %s jusqu'à i- here+1 : %d\n",chaine,i-*here-1);*/
	return  chaine;
}




char* input_to_symbol (char* input, uint *here)
{
	uint i = *here;
	DEBUG_MSG("début de input_to_symbol : input[i] = %c\n",input[i]);

	string tmp_chaine;
	strcpy(tmp_chaine,&input[i]) ;
	char* chaine = malloc(sizeof(*chaine));
	while(input[i]!= 32 && input[i]!= 41)
	{
		DEBUG_MSG("input[i] dans symbol : %c\n",input[i]);
		i++;
	}
	strncpy(chaine,tmp_chaine, i-*here);
	DEBUG_MSG("symbol : %s, taille du symbole %lu\n",chaine, strlen(chaine));
	return chaine;
}



/**************     READ      *****************/

object sfs_read_atom( char *input, uint *here)
{
	DEBUG_MSG("            atom :%c, size : %lu, chaine : %s \n", input[*here],strlen(input),input);
	uint i = *here;
	uint test = 0 ;
	if(input[*here]<33)
		(*here)++;
	while(*here<strlen(input))
	{
		if(test_integer(&input[i]) == 1)
		{
			DEBUG_MSG("            sfs_read_atom : on lit un entier\n");
			uint integer = string_to_integer(input,here);  /*test si la suite est tj un chiffre*/
			if(integer!=-1)
			{
				*here += taille_int(integer);
				DEBUG_MSG("            *here = %d, taille integer : %d\n",*here,taille_int(integer));
				return make_integer(integer);
			}
		}

		test = test_character(input,here);
		if(test ==1)
		{
			DEBUG_MSG("            sfs_read_atom : on lit un caractere\n");
			char caractere = input[*here+2];
			*here += 3;
			DEBUG_MSG("            *here = %d\n",*here);
			return make_character(caractere);
		}
		if(test ==2)
		{
			DEBUG_MSG("            sfs_read_atom : on lit un saut a la ligne\n");
			DEBUG_MSG("            *here = %d\n",*here);
			*here += 9;
			string chaine = "newline";
			return make_character_special(chaine);
		}
		if(test ==3)
		{
			DEBUG_MSG("            sfs_read_atom : on lit un espace\n");
			DEBUG_MSG("            *here = %d\n",*here);
			*here += 7;
			string chaine = "space";
			return make_character_special(chaine);
		}
		if (test ==4)
		{
			DEBUG_MSG("boolean_true\n");
			*here += 3;
			return make_boolean(TRUE);
		}
		if (test ==5)
		{
			*here += 3;
			return make_boolean(FALSE);
		}

		if(test == -1)
		{
			DEBUG_MSG("            sfs_read_atom : on lit un caractere trop long\n");
		}

		if(test_string(input,here)==1)
		{
			DEBUG_MSG("            sfs_read_atom : on lit une chaine\n");
			char *chaine = malloc(sizeof(chaine));
			chaine = input_to_string(input,here);
			*here += strlen(chaine)+2;
			DEBUG_MSG("            chaine : %s taille de la chaine : %lu -> here = %d\n", chaine, strlen(chaine),*here);
			DEBUG_MSG("            *here = %d, input : %c\n",*here, input[*here]);
			return make_string(chaine);

		}

		if(input[i] > 32 && *here < strlen(input))
		{
			DEBUG_MSG("            sfs_read_atom : on ne lit aucun des cas precedents -> symbole \n");
			char * chaine_symbol = malloc(sizeof(chaine_symbol));
			chaine_symbol = input_to_symbol(input, here);
			DEBUG_MSG("here = %d taille : %lu\n",*here, sizeof(chaine_symbol));
			*here += sizeof(chaine_symbol);
			DEBUG_MSG("            *here = %d\n",*here);
			return make_symbol(input);
		}

		/*object atom = NULL;*/
	}
}





object cons (char*stream,uint*i)
{
	object t = make_pair();
	t->this.pair.car = sfs_read(stream,i);
	return t;
}
/*

	object p ;
	while(stream[*i]!=41 && stream[*i]>32 && *i<strlen(stream))
	{
		p = make_pair();
		p->this.pair.car = sfs_read(stream,i);
		p->this.pair.cdr = cons(stream,i);
	}
	return p ;
}*/


object sfs_read_pair (char* stream, uint *i)

{
	/*DEBUG_MSG("entrée dans read_pair\n");
	while(stream[*i]!=41 && stream[*i]>31 && *i<strlen(stream))
	{
	if(stream[*i]==32) (*i)++;
	DEBUG_MSG("STREAM : %c ou %d, i : %d\n",stream[*i],stream[*i], *i);

	object pair = cons(stream,i);
	if(stream[*i]!=41 && stream[*i]>31 && *i<strlen(stream))
	{
	while(stream[*i]==32) (*i)++;

	DEBUG_MSG("STREAM SUIVANT: %c ou %d , i : %d\n",stream[*i],stream[*i],*i);
	if(stream[*i]==41) pair->this.pair.cdr = nil;
	else
	pair->this.pair.cdr = sfs_read_pair(stream,i);
}
else pair->this.pair.cdr = nil ;

return pair;
}
return NULL;*/
	*i = next_char(stream,i);
	if(stream[*i]== ')' || stream[*i] < 31)
	{
		(*i)++;
		DEBUG_MSG("on va retourner nil\n");
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
	while(input[*i]==32)

	{
		if(*i<strlen(input))

		(*i)++;
	}
	return *i;
}
/*


object compiler (object t, char* input, uint *here)
{
	DEBUG_MSG("    on entre dans compiler %d\n",input[*here]);

	if(input[*here]!=0 && input[*here]!= 41)
	{
		if(input[*here]==32 || input[*here]==0)
			(*here)++;

		t = cons(sfs_read(input,here),t);
		t = compiler(t,input,here);
	}
	return t;
}
*/


/*
object make_root (char*input, uint*i)
{
	DEBUG_MSG("entree dans make_root, %c\n",input[*i]);
	object pair;

	while (input[*i]!=41)
	{
		pair = make_pair();
		root = pair;
		pair->this.pair.car = sfs_read(input,i);
		if(input[*i]!= 41)
		pair->this.pair.cdr = make_root(input,i);
		else return pair;
	}
	DEBUG_MSG("1\n");

	return pair;
}

object make_liste (char *input, uint*i)
{
	object pair;
	while (input[*i]!=41)
	{
		pair = make_pair();
		pair->this.pair.car = sfs_read(input,i);
		pair->this.pair.cdr = make_liste(input,i);

	}
	return pair;
}

object sfs_read_pair (char* input, uint *i, uint nb_maillon)
{
	object pair;
	DEBUG_MSG("entree dans sfs_read_pair\n");
	while(input[*i]!=41)
	{
		DEBUG_MSG("entree dans sfs_read_pair, maillon = %d\n", nb_maillon);
		(nb_maillon)++;
		if (nb_maillon == 1)
		{
			nb_maillon++;
			DEBUG_MSG("if nb_maillon = 1\n");
			pair = make_root(input,i);
			return pair;

		}
		else
		{
			while(input[*i]!=41)
			{
				pair = make_liste(input,i);
				DEBUG_MSG("on a une liste interieure finie\n");
			}
		}
		nb_maillon = 0;
		root->this.pair.cdr = sfs_read_pair(input,i,nb_maillon);
		if(input[*i]==41 && *i+1 >=strlen(input))
			return pair;
	}
	DEBUG_MSG("3\n");

	return pair;
}
*/


object sfs_read( char *input, uint *here )
{
	if(input[*here]<33)
		(*here)++;
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
				return sfs_read_pair( input, here  );
				DEBUG_MSG("? \n");
			}
		}
		else
		{
			return sfs_read_atom( input, here );
		}
	}
	DEBUG_MSG("fin de sfs_read\n");
}
