/*
 * Copyright (c) 1985, J Howard Johnson, University of Waterloo.
 * Copyright (c) 2022, J Howard Johnson
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

A_OBJECT A_pr( A_OBJECT A, char *file, Tn_OBJECT Tn_Sigma )
{
    int t;
    A_row *p, *pz;

    if ( A == NULL || Tn_Sigma == NULL ) return( A );
    if ( file != NULL ) {
        if ( strcmp( file, "devnull" ) == 0 ) return( A );
        else fp = fopen( file, "w" );
    } else {
        fp = fpout;
        if ( fp == NULL ) fp = stdin;
    }
    if ( fp == NULL ) {
        Warning( "Cannot open file" );
        return( A );
    }
    pz = A-> A_t + A-> A_nrows;
    for( p = A-> A_t; p < pz; p++ ) {
        if ( ( t = p-> A_a ) == START ) fprintf( fp, "(START) " );
        else if ( t == FINAL )          fprintf( fp, "(FINAL) " );
        else                            fprintf( fp, "%d ", t );
        if ( ( t = p-> A_b ) <= 1 || A-> A_nT == 1 ) {
            fprintf( fp, Tn_pr_name( Tn_Sigma, t ) );
        } else {
            fprintf( fp, "%1d.", t % A-> A_nT );
            fprintf( fp, Tn_pr_name( Tn_Sigma, t / A-> A_nT ) );
        }
        if ( ( t = p-> A_c ) == START ) fprintf( fp, " (START)\n" );
        else if ( t == FINAL )          fprintf( fp, " (FINAL)\n" );
        else                            fprintf( fp, " %d\n", t );
    }
    if ( file != NULL ) {
        fclose( fp );
    } else  if ( fflush( stdout ) == EOF ) Error( "A_store: fflush" );
    return( A );
}

A_OBJECT A_load_pr( char *file, Tn_OBJECT Tn_Sigma )
{
    int from, symb, to, tape, ntapes, i;
    int c;
    A_OBJECT A;
    Tn_OBJECT TQ;
    char *t;
    char *buffer;
    A_row *p;

    buffer = Salloc( 100 );

    if ( file != NULL )             fp = fopen( file, "r" );
    else if ( fpin != NULL )        fp = fpin;
    else                            fp = stdin;
    if ( fp == NULL ) {
        Warning( "File does not exist" );
        return( NULL );
    }
    A = A_create();
    if ( Tn_Sigma == NULL
            || Tn_insert( Tn_Sigma, "^^", 2 ) != 0
            || Tn_insert( Tn_Sigma, "-|", 2 ) != 1 )
                Error( "A_load: BOTCH 1" );

    A-> A_nT = ntapes = 1;
    c = getc( fp );

    TQ = Tn_create();
    if ( Tn_insert( TQ, "(START)", 7 ) != START
      || Tn_insert( TQ, "(FINAL)", 7 ) != FINAL )
        Error( "A_load: BOTCH 2" );
    while ( c != EOF ) {
        if ( (t = get_name()) == NULL ) {
            A_destroy( A );
            Tn_destroy( TQ );
            return( NULL );
        }
        from = Tn_insert( TQ, t, strlen( t ) );
        if ( (t = get_name()) == NULL ) {
            A_destroy( A );
            Tn_destroy( TQ );
            return( NULL );
        }
        if ( t[1] == '.' && t[0] >= '0' && t[0] <= '9' ) {
            tape = t[0] - '0';
            if ( tape >= ntapes ) {
                for( p = A-> A_t + A-> A_nrows;
                        --p >= A-> A_t; ) {
                    i = p-> A_b;
                    if ( i > 1 )
                        i = i / ntapes * (tape+1)
                            + i % ntapes;
                    p-> A_b = i;
                }
                A-> A_nT = ntapes = tape + 1;
            }
            symb = Tn_insert( Tn_Sigma, t + 2, strlen( t + 2) );
            if ( symb == 1 && ntapes > 1 ) A-> A_ems = 1;
            symb = symb * ntapes + tape;
        } else {
            symb = Tn_insert( Tn_Sigma, t, strlen( t ) );
            if ( symb == 1 && ntapes > 1 ) A-> A_ems = 1;
            if ( symb != 1 ) symb *= ntapes;
        }
        if ( (t = get_name()) == NULL ) {
            A_destroy( A );
            Tn_destroy( TQ );
            return( NULL );
        }
        to   = Tn_insert( TQ, t, strlen( t ) );
        A = A_add( A, from, symb, to );
        if ( !get_nl() ) {
            A_destroy( A );
            Tn_destroy( TQ );
            return( NULL );
        }
    }
    if ( file != NULL ) fclose( fp );
    Tn_destroy( TQ );
    return( A_close( A_rename( A, (SHORT *) NULL ) ) );
}
