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

static SHORT *Tn_hashpos = 0;
static int Tn_calls = 0;
static int Tn_probes = 0;
static int Tn_fail = 0;

Tn_OBJECT Tn_create( )
{
    Tn_OBJECT Tn;
    Tn = (Tn_OBJECT) Salloc( sizeof(struct Tn_desc) );
    Tn-> Type = Tn_Object;
    Tn-> Tn_n  = 0;
    Tn-> Tn_lname = 0;
    Tn-> Tn_lhash = 1;
    Tn-> Tn_name = 0;
    Tn-> Tn_lens = 0;
    Tn-> Tn_hash = s_alloc( 1 );
    Tn-> Tn_hash[ 0 ] = MAXSHORT;
    return( Tn );
}

void Tn_destroy( Tn_OBJECT Tn )
{
    char **p, **pl;
    if ( Tn == NULL ) return;
    p = Tn-> Tn_name;
    pl = p + Tn-> Tn_n;
    while ( p < pl ) Sfree( (char *) *p++ );
    Sfree( (char *) Tn-> Tn_name );
    Sfree( (char *) Tn-> Tn_lens );
    Sfree( (char *) Tn-> Tn_hash );
    Sfree( (char *) Tn );
}

int Tn_member( Tn_OBJECT Tn, char *name, int length )
{
    int h, i;
    char *na;
    SHORT *p;
    assert( name[ length ] == '\0' );
    ++Tn_calls;
    h = 0;
    for ( i = 0; i < length; ++i ) {
        h = ( ( h + (unsigned) name[ i ] ) * 16807 ) & 017777777777;
    }
    p  = Tn-> Tn_hash + h % Tn-> Tn_lhash;
    while ( *p < MAXSHORT ) {
        ++Tn_probes;
        if ( length == Tn-> Tn_lens[ *p ] ) {
            na = Tn-> Tn_name[ *p ];
            for ( i = 0; i < length; ++i ) {
                if ( na[ i ] != name[ i ] ) break;
            }
            if ( i == length ) return( *p );
        }
        if ( --p < Tn-> Tn_hash )
            p = Tn-> Tn_hash + Tn-> Tn_lhash - 1;
    }
    ++Tn_fail;
    Tn_hashpos = p;
    return( -1 );
}

Tn_OBJECT Tn_grow( Tn_OBJECT Tn, int lname )
{
    SHORT *p, *pl;
    char **q, **ql;
    int i;
    int len;
    if ( lname < 15 ) lname = 15;
    if ( lname <= Tn-> Tn_lname ) return( Tn );
    Sfree( (char *) Tn-> Tn_hash );
    Tn-> Tn_name =
        (char **) Srealloc( (char *) Tn-> Tn_name,
                            lname * sizeof(char *) );
    Tn-> Tn_lens =
        (int *) Srealloc( (char *) Tn-> Tn_lens,
                            lname * sizeof(int) );
    Tn-> Tn_lname = Ssize( (char *) Tn-> Tn_name ) / sizeof(char *);
    if ( Tn-> Tn_lname > MAXSHORT ) Tn-> Tn_lname = MAXSHORT;
    Tn-> Tn_hash = s_alloc( 2 * Tn-> Tn_lname );
    Tn-> Tn_lhash = Ssize( (char *) Tn-> Tn_hash ) / sizeof(SHORT);
    p = Tn-> Tn_hash;
    pl = p + Tn-> Tn_lhash;
    while ( p < pl ) *p++ = MAXSHORT;
    q = Tn-> Tn_name;
    ql = q + Tn-> Tn_n;
    i = 0;
    while ( q < ql ) {
        len = strlen( *q );
        if ( Tn_member( Tn, *q++, len ) != (-1) )
            Error( "Tn_grow: BOTCH" );
        *Tn_hashpos = i++;
    }
    return( Tn );
}

int Tn_insert( Tn_OBJECT Tn, char *name, int length )
{
    int i;
    char *na;
    assert( name[ length ] == '\0' );
    if ( Tn-> Tn_n >= Tn-> Tn_lname ) {
        if ( Tn-> Tn_n >= MAXSHORT )
            Error( "Tn_insert: Table FULL" );
        Tn = Tn_grow( Tn, 2 * Tn-> Tn_lname );
    }
    if ( (i = Tn_member( Tn, name, length )) >= 0 ) return( i );
    na = Salloc( length + 1 );
    for( i = 0; i < length; ++i ) {
        na[ i ] = name[ i ];
    }
    na[ length ] = '\0';
    Tn-> Tn_name[ Tn-> Tn_n ] = na;
    Tn-> Tn_lens[ Tn-> Tn_n ] = length;
    return( *Tn_hashpos = Tn-> Tn_n++ );
}

char *Tn_name( Tn_OBJECT Tn, int i )
{
    if ( i >= 0 && i < Tn-> Tn_n ) return( Tn-> Tn_name[ i ] );
    else    return( NULL );
}

int Tn_length( Tn_OBJECT Tn, int i )
{
    if ( i >= 0 && i < Tn-> Tn_n )
            return( Tn-> Tn_lens[ i ] );
    else    return( -1 );
}

void Tn_stats()
{
    fprintf( fpout, "(Tn) Calls:%7d  Probes:%7d  Unsuccessful:%7d\n",
             Tn_calls, Tn_probes, Tn_fail );
}
