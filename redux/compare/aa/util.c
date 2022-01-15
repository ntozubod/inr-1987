#include "O.h"
#include <stdio.h>
#include <stdlib.h>
extern FILE * fpout ;
void Warning ( char * s )
{
  fprintf ( fpout, "*** %s ***\n", s ) ;
}
void Error ( char * s )
{
  fprintf ( fpout, "*** %s ***\n", s ) ;
  fflush ( fpout ) ;
  exit ( 1 ) ;
}
SHORT * s_alloc ( int n )
{
  return ( ( SHORT * ) Salloc ( n * sizeof ( SHORT ) ) ) ;
}
int * i_alloc ( int n )
{
  return ( ( int * ) Salloc ( n * sizeof ( int ) ) ) ;
}
void pr_time_diff ( )
{
  return ;
}
