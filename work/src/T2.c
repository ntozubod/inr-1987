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

int valid_utf8_at( char *s, int i, int l, int *cp_ref ) {
    int c1 = s[ i ];
    int type, c2, c3, c4, cp;

    *cp_ref = -1;

         if ( ( c1 & 0x80 ) == 0x00 ) { type = 0; }
    else if ( ( c1 & 0xc0 ) == 0x80 ) { type = 1; }
    else if ( ( c1 & 0xe0 ) == 0xc0 ) { type = 2; }
    else if ( ( c1 & 0xf0 ) == 0xe0 ) { type = 3; }
    else if ( ( c1 & 0xf8 ) == 0xf0 ) { type = 4; }
    else { type = 5; }

    switch ( type ) {

    case 0:
        cp = c1;
        *cp_ref = cp;
        return( 1 );

    case 2:
        if ( i + 1 >= l ) { return( 0 ); }
        c2 = s[ i + 1 ];
        cp = ( ( c1 & 0x1f ) << 6 )
           +   ( c2 & 0x3f );
        if ( ( c2 & 0xc0 ) == 0x80
          &&   cp > 0x7f ) {
            *cp_ref = cp;
            return( 2 );
        }
        break;

    case 3:
        if ( i + 2 >= l ) { return( 0 ); }
        c2 = s[ i + 1 ];
        c3 = s[ i + 2 ];
        cp = ( ( c1 & 0x0f ) << 12 )
           + ( ( c2 & 0x3f ) <<  6 )
           +   ( c3 & 0x3f );
        if (    ( c2 & 0xc0 ) == 0x80
             && ( c3 & 0xc0 ) == 0x80
             && (    ( 0x03ff < cp && cp < 0xd800 )
                  || ( 0xdfff < cp ) ) ) {
            *cp_ref = cp;
            return( 3 );
        }
        break;

    case 4:
        if ( i + 3 >= l ) { return( 0 ); }
        c2 = s[ i + 1 ];
        c3 = s[ i + 2 ];
        c4 = s[ i + 3 ];
        cp = ( ( c1 & 0x0f ) << 18 )
           + ( ( c2 & 0x3f ) << 12 )
           + ( ( c3 & 0x3f ) <<  6 )
           +   ( c4 & 0x3f );
        if (    ( c2 & 0xc0 ) == 0x80
             && ( c3 & 0xc0 ) == 0x80
             && ( c4 & 0xc0 ) == 0x80
             &&   0xffff < cp && cp <= 0x10ffff ) {
            *cp_ref = cp;
            return( 4 );
        }
        break;

    case 1: case 5:
        break;
    }
    return( 0 );
}

int printable_unicode( int cp ) {
/* Exclude all control, spacing, and non-characters */
    if ( (  0x0000 <= cp && cp <    0x0021 )
      || (  0x007F <= cp && cp <    0x00A1 )
      || (  0x00AD <= cp && cp <    0x00AE )
      || (  0x0600 <= cp && cp <    0x0606 )
      || (  0x061C <= cp && cp <    0x061D )
      || (  0x06DD <= cp && cp <    0x06DE )
      || (  0x070F <= cp && cp <    0x0710 )
      || (  0x0890 <= cp && cp <    0x0892 )
      || (  0x08E2 <= cp && cp <    0x08E3 )
      || (  0x1680 <= cp && cp <    0x1681 )
      || (  0x180E <= cp && cp <    0x180F )
      || (  0x2000 <= cp && cp <    0x2010 )
      || (  0x2028 <= cp && cp <    0x2030 )
      || (  0x205F <= cp && cp <    0x2065 )
      || (  0x2066 <= cp && cp <    0x2070 )
      || (  0x3000 <= cp && cp <    0x3001 )
      || (  0xD800 <= cp && cp <    0xF900 )
      || (  0xFDD0 <= cp && cp <    0xFDF0 )
      || (  0xFEFF <= cp && cp <    0xFF00 )
      || (  0xFFF9 <= cp && cp <    0xFFFC )
      || (  0xFFFE <= cp && cp <   0x10000 )
      || ( 0x110BD <= cp && cp <   0x110BE )
      || ( 0x110CD <= cp && cp <   0x110CE )
      || ( 0x13430 <= cp && cp <   0x13439 )
      || ( 0x1BCA0 <= cp && cp <   0x1BCA4 )
      || ( 0x1D173 <= cp && cp <   0x1D17B )
      || ( 0x1FFFE <= cp && cp <   0x20000 )
      || ( 0x2FFFE <= cp && cp <   0x30000 )
      || ( 0x3FFFE <= cp && cp <   0x40000 )
      || ( 0x4FFFE <= cp && cp <   0x50000 )
      || ( 0x5FFFE <= cp && cp <   0x60000 )
      || ( 0x6FFFE <= cp && cp <   0x70000 )
      || ( 0x7FFFE <= cp && cp <   0x80000 )
      || ( 0x8FFFE <= cp && cp <   0x90000 )
      || ( 0x9FFFE <= cp && cp <   0xA0000 )
      || ( 0xAFFFE <= cp && cp <   0xB0000 )
      || ( 0xBFFFE <= cp && cp <   0xC0000 )
      || ( 0xCFFFE <= cp && cp <   0xD0000 )
      || ( 0xDFFFE <= cp && cp <   0xE0000 )
      || ( 0xE0001 <= cp && cp <   0xE0002 )
      || ( 0xE0020 <= cp && cp <   0xE0080 )
      || ( 0xEFFFE <= cp && cp <= 0x10FFFF ) ) {
        return( 0 );
    }
    return( 1 );
}

void T2_sync( T2_OBJECT T2 )
{
    char hexmap[17] = "0123456789ABCDEF";
    char *pr_str, *cstr;
    int i, j, k, length, next_ch, ii, pr_str_length;
    int j2, utf8_len, cp;

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
            utf8_len = valid_utf8_at( cstr, j, length, &cp );
            if ( utf8_len > 0 && printable_unicode( cp ) ) {
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
