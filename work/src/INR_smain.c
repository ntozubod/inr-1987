/*
 * Copyright (c) 1985, J Howard Johnson, University of Waterloo.
 *
 * This software was developed while I was a student and, later, professor
 * at the University of Waterloo.  It has only minimal enhancements and bug
 * fixes from later than August 1988.  It was released under the GPLv3
 * licence on July 26, 2010.
 *                 -- J Howard Johnson ( j.howard.johnson *at* gmail.com )
 *
 * This file is part of INR.
 *
 *   INR is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   INR is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with INR.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "O.h"

A_OBJECT    A, Atemp;
Tn_OBJECT   TAlist;
A_OBJECT    * Alist;

T2_OBJECT   TT2;

void TT2_init( )
{
    int result, ti;
    char tstr[3];

    TT2 = T2_create();

    result = T2_insert( TT2, "^^", 2 );
    assert( result == 0 );
    result = T2_insert( TT2, "-|", 2 );
    assert( result == 1 );
    for( ti = 0; ti < 256; ti++ ) {
        tstr[ 0 ] = ti;
        tstr[ 1 ] = '\0';
        result = T2_insert( TT2, tstr, 1 );
        assert( result == ti + 2 );
    }
    result = T2_insert( TT2, "__", 2 );
    assert( result == 258 );
}

char * pad20( char *s )
{
    static char tmp[ 41 ];
    if ( strlen( s ) >= 20 ) return( s );
    else {
        (void) strcpy( tmp, s );
        (void) strcat( tmp, " -------------------" );
        tmp[ 20 ] = 0;
        return( tmp );
    }
}

#define PROMT   if(isatty(fileno(fpin))&&isatty(fileno(fpout)))printf("--* ");

char *copyof( char *str )
{
    return( strcpy( Salloc( strlen( str ) + 1 ), str ) );
}

char Notice[]
   = "Copyright (c) 1985, 1988, J Howard Johnson, University of Waterloo";

int smain( int argc, char *argv[] )
{
    int result;
    char file_in[50], file_out[50], rpt_out[50];

    fpin  = stdin;
    fpout = stdout;

    if( argc > 3) {
        printf( "Usage: inr [ input_file ] [ output_file ]\n" );
        exit (1) ;
    }

    if( argc > 2 ) {
        strcpy( file_out, argv[2] ) ;
        fpout = fopen( file_out, "w" );
    }

    if( argc > 1 ) {
        strcpy( file_in, argv[1] ) ;
        fpin = fopen( file_in, "r" );
    }

    if (fpin == 0 || fpout == 0 ) {
        if( fpin )   strcpy( rpt_out, file_out );
        else if (fpout ) strcpy( rpt_out, file_in );
        else {
            strcpy( rpt_out, file_in );
            strcat( rpt_out, ", " );
            strcat( rpt_out, file_out );
        }
        printf ( "Problem with %s file(s) opens. -- aborting\n", rpt_out) ;
        exit (1) ;
    }

    if ( isatty(fileno(fpout)) ) {

fprintf( fpout, "\n" );
fprintf( fpout, "       II  N     N  RRRRRR  " );
fprintf( fpout, "   x I N R   Version %s\n", Version );
fprintf( fpout, "       II  N N   N  R    RR " );
fprintf( fpout, "             modified  %s\n", Date );
fprintf( fpout, "x  x   II  N  N  N  RRRRRR  " );
fprintf( fpout, "   Copyright (C) 1988-2022 J Howard Johnson\n" );
fprintf( fpout, " xx    II  N   N N  R    R  " );
fprintf( fpout, "\n" );
fprintf( fpout, "x  x   II  N    NN  R     R " );
fprintf( fpout, "                     (For help type   `:help;')\n" );
fprintf( fpout, "This program comes with ABSOLUTELY NO WARRANTY; " );
fprintf( fpout, "for details type `:help w;'.\n" );
fprintf( fpout, "This is free software, and you are welcome " );
fprintf( fpout, "to redistribute it under certain\n" );
fprintf( fpout, "conditions; type `:help c;' for details.\n" );
fprintf( fpout, "\n" );

    } else {
        fprintf ( fpout, "x I N R -- V %s, modified %s\n", Version, Date ) ;
        fprintf ( fpout, "Copyright (C) 1988-2022 J Howard Johnson\n" ) ;
        fprintf ( fpout, "Distributed under GPLv3 (see COPYING)\n" ) ;
        if ( fpin != stdin )
            fprintf( fpout, "  (Source file: %s)", file_in ) ;
        fprintf( fpout, "\n\n\n" );
    }

    TT2_init();

    Alist = (A_OBJECT *) Salloc( 100 * sizeof( A_OBJECT ) );
    TAlist = Tn_create();
    result = Tn_insert( TAlist, "_Last_", 6 );
    assert( result == 0 );
    Alist[ 0 ] = A_create();

    pr_time_diff();
    PROMT
    if ( yyparse() != 0 )
        Error( "yyparse returned unexpectedly" );
    T2_destroy( TT2 );
    Tn_destroy( TAlist );
    if ( A_report ) {
        fprintf( fpout, "\n" );
        Tn_stats();
        V_stats();
        R_stats();
        U_stats();
    }
    exit( 0 );
}

int tonum( char *p )
{
    int acum, c;
    acum = 0;
    while ( ( c = *p++ ) ) {
        if ( c < '0' || c > '9' ) return( -1 );
        acum = acum * 10 + c - '0';
    }
    return( acum );
}
