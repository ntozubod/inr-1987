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

T2_OBJECT T2_create( )
{
    T2_OBJECT T2;
    T2 = (T2_OBJECT) Salloc( sizeof(struct T2_desc) );
    T2-> T2_int = Tn_create();
    T2-> T2_ext = Tn_create();

    return( T2 );
}

void T2_destroy( T2_OBJECT T2 )
{
    if ( T2 == NULL ) return;

    Tn_destroy( T2-> T2_int );
    Tn_destroy( T2-> T2_ext );
    Sfree( (char *) T2 );
}

int T2_member( T2_OBJECT T2, char *name, int length )
{
    return( Tn_member( T2-> T2_int, name, length ) );
}

int T2_insert( T2_OBJECT T2, char *name, int length )
{
    return( Tn_insert( T2-> T2_int, name, length ) );
}

char *T2_name( T2_OBJECT T2, int i )
{
    return( Tn_name( T2-> T2_int, i ) );
}

int T2_length( T2_OBJECT T2, int i )
{
    return( Tn_length( T2-> T2_int, i ) );
}

P_OBJECT T2_Pstr( T2_OBJECT T2, int i )
{
    return( Tn_Pstr( T2-> T2_int, i ) );
}

char *T2_name_pr( T2_OBJECT T2, int i )
{
    T2_sync( T2 );
    return( Tn_name( T2-> T2_ext, i ) );
}

int T2_length_pr( T2_OBJECT T2, int i )
{
    T2_sync( T2 );
    return( Tn_length( T2-> T2_ext, i ) );
}

P_OBJECT T2_Pstr_pr( T2_OBJECT T2, int i )
{
    T2_sync( T2 );
    return( Tn_Pstr( T2-> T2_ext, i ) );
}

void T2_stats()
{
    Tn_stats();
}

int valid_utf8_at( char *s, int i, int l ) {
    int c1 = s[ i ];
    int type, c2, c3, c4;

         if ( ( c1 & 0x80 ) == 0x00 ) { type = 0; }
    else if ( ( c1 & 0xc0 ) == 0x80 ) { type = 1; }
    else if ( ( c1 & 0xe0 ) == 0xc0 ) { type = 2; }
    else if ( ( c1 & 0xf0 ) == 0xe0 ) { type = 3; }
    else if ( ( c1 & 0xf8 ) == 0xf0 ) { type = 4; }
    else { type = 5; }

    switch ( type ) {

    case 2:
        if ( i + 1 >= l ) { return( 0 ); }
        c2 = s[ i + 1 ];
        if (    ( c2 & 0xc0 ) == 0x80 ) {
            return( 2 );
        }
        break;

    case 3:
        if ( i + 2 >= l ) { return( 0 ); }
        c2 = s[ i + 1 ];
        c3 = s[ i + 2 ];
        if (    ( c2 & 0xc0 ) == 0x80
             && ( c3 & 0xc0 ) == 0x80 ) {
            return( 3 );
        }
        break;

    case 4:
        if ( i + 3 >= l ) { return( 0 ); }
        c2 = s[ i + 1 ];
        c3 = s[ i + 2 ];
        c4 = s[ i + 3 ];
        if (    ( c2 & 0xc0 ) == 0x80
             && ( c3 & 0xc0 ) == 0x80
             && ( c4 & 0xc0 ) == 0x80 ) {
            return( 4 );
        }
        break;

    case 0: case 1: case 5:
        break;
    }
    return( 0 );
}

void T2_sync( T2_OBJECT T2 )
{
    char hexmap[17] = "0123456789ABCDEF";
    char *pr_str, *cstr;
    int i, j, k, length, next_ch, ii, pr_str_length;
    int j2, utf8_len;

    Tn_OBJECT TT       = T2-> T2_int;
    Tn_OBJECT TT_print = T2-> T2_ext;

    pr_str = Salloc( 100 );
    pr_str_length = Ssize( pr_str );
    for( i = TT_print-> Tn_n; i < TT-> Tn_n; ++i ) {
        length = Tn_length( TT, i );
        if ( pr_str_length < length * 4 + 1 ) {
            pr_str = Srealloc( pr_str, length * 4 + 1 );
        }
        cstr = Tn_name( TT, i );
        k = 0;
        for( j = 0; j < length; ++j ) {
            next_ch = cstr[ j ];
            utf8_len = valid_utf8_at( cstr, j, length );
            if ( utf8_len > 0 ) {
                pr_str[ k++ ] = next_ch;
                for ( j2 = 1; j2 < utf8_len; ++j2 ) {
                    next_ch = cstr[ ++j ];
                    pr_str[ k++ ] = next_ch;
                }
            } else if ( next_ch == ' ' ) {
                pr_str[ k++ ] = '\\';
                pr_str[ k++ ] = '_';
            } else if ( next_ch == '\t' ) {
                pr_str[ k++ ] = '\\';
                pr_str[ k++ ] = 't';
            } else if ( next_ch == '\n' ) {
                pr_str[ k++ ] = '\\';
                pr_str[ k++ ] = 'n';
            } else if ( next_ch == '\\' ) {
                pr_str[ k++ ] = '\\';
                pr_str[ k++ ] = '\\';
            } else if ( next_ch == '.' ) {
                pr_str[ k++ ] = '\\';
                pr_str[ k++ ] = '.';
            } else if ( isprint( next_ch ) ) {
                pr_str[ k++ ] = next_ch;
            } else {
                pr_str[ k++ ] = '\\';
                pr_str[ k++ ] = 'x';
                pr_str[ k++ ] = hexmap[ ( next_ch >> 4 ) & 0xf ];
                pr_str[ k++ ] = hexmap[ next_ch & 0xf ];
            }
        }
        pr_str[ k ] = '\0';
        ii = Tn_insert( TT_print, pr_str, k );
        assert( i == ii );
    }
    Sfree( pr_str );
}
