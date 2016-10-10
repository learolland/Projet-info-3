
/**
 * @file read.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:30:11 2012
 * @brief Read routine for SFS.
 *
 * Read routine for SFS.
 */

#ifndef _READ_H_
#define _READ_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include "object.h"

enum {S_OK, S_KO, S_END};

uint   sfs_get_sexpr( string input, FILE *fp );
void init(void);

int test_integer (char* charactere);

int string_to_integer( char *input, uint *here );

object read (char*input);

object sfs_read( char *input, uint *here );
object sfs_read_atom( char *input, uint *here );
object sfs_read_pair( char *stream, uint *i );


uint next_char (char*input, uint *i);


object cons (char*stream,uint*i);

#ifdef __cplusplus
}
#endif

#endif /* _READ_H_ */