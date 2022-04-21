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
#include "y.tab.h"

int ch = ' ';
char    token[512];
int in_string = 0;

int yylex()
{
    int li, d, lflag, in_comment;

    fflush( fpout );
    if ( in_string ) {
        ch = getc( fpin );
        if ( ch == '\'' ) {
            ch = getc( fpin );
            if ( ch != '\'' ) {
                in_string = 0;
                return( RPAREN );
            }
        }
        if ( ch == '\\' ) {
            ch = getc( fpin );
            switch( ch ) {
            case 'n': ch = '\n'; break;
            case 't': ch = '\t'; break;
            case '_': ch = ' '; break;
            case 'x':
                d = getc( fpin );
                if ( d >= '0' && d <= '9' ) d = d - '0';
                else if ( d >= 'a' && d <= 'f' ) d = d - 'a' + 10;
                else if ( d >= 'A' && d <= 'F' ) d = d - 'A' + 10;
                else Error( "Unexpected Hex digit" );
                ch = d << 4;
                d = getc( fpin );
                if ( d >= '0' && d <= '9' ) d = d - '0';
                else if ( d >= 'a' && d <= 'f' ) d = d - 'a' + 10;
                else if ( d >= 'A' && d <= 'F' ) d = d - 'A' + 10;
                else Error( "Unexpected Hex digit" );
                ch += d;
            }
        }
        if ( ch == EOF ) Error( "End of file in string" );
        yylval.up = P_create( T2_length( TT2, ch + 2 ),
                              T2_name( TT2, ch + 2 ) );
        return( PNAME );
    }
    in_comment = 0;
    while( ch == ' ' || ch == '\t' || ch == '\n' || ch == '#'
            || in_comment ) {
        if( in_comment && !isatty(fileno(fpout)) ) putc( ch, fpout );
        if ( ch == '#'  ) in_comment = 1;
        if ( ch == '\n' ) in_comment = 0;
        if ( ch == EOF  ) Error( "End of file in comment" );
        ch = getc( fpin );
    }
    if ( ch == EOF ) return( 0 );
    d = ch;
    ch = ' ';
    switch( d ) {
    case '!':       return( EXCLAM );
/*  case '"':       not used    */
/*  case '#':       COMMENT     */
    case '$':       return( DOLLAR );
    case '%':       return( PERCENT );
    case '&':       return( AMPERSAND );
    case '\'':      in_string = 1; return( LPAREN );

    case '(':       return( LPAREN );
    case ')':       return( RPAREN );
    case '*':       return( STAR );
    case '+':       return( PLUS );
    case ',':       return( COMMA );
    case '-':       return( MINUS );
/*  case '.':       ALPHANUMERIC    */
    case '/':       return( SLASH );

    case ':':       return( COLON );
    case ';':       return( SEMI );
/*  case '<':       not used    */
    case '=':       return( EQUAL );
/*  case '>':       not used    */
    case '?':       return( QUESTION );

    case '@':       return( AT );

    case '[':       return( LBRACK );
    case '\\':      return( BSLASH );
    case ']':       return( RBRACK );
    case '^':       return( CIRCUMFLEX );
/*  case '_':       ALPHANUMERIC    */

/*  case '`':       TOKEN QUOTE */

    case '{':       return( LBRACE );
    case '|':       return( VBAR );
    case '}':       return( RBRACE );
/*  case '~':       not used    */

    case '"': case '<': case '>': case '~':
        fprintf( fpout, "Reserved character: %c\n", d );
        return( d );
    }
    li = 0;
    ch = d;
    lflag = 1;
    if ( ch == '`' ) {
        ch = getc( fpin );
        while ( ch != EOF ) {
            if ( ch == '`' ) {
                ch = getc( fpin );
                if ( ch != '`' ) break;
            }
            if ( ch == '\\' ) {
                ch = getc( fpin );
                switch( ch ) {
                case 'n': ch = '\n'; break;
                case 't': ch = '\t'; break;
                case '_': ch = ' '; break;
                case 'x':
                    d = getc( fpin );
                    if ( d >= '0' && d <= '9' ) d = d - '0';
                    else if ( d >= 'a' && d <= 'f' ) d = d - 'a' + 10;
                    else if ( d >= 'A' && d <= 'F' ) d = d - 'A' + 10;
                    else Error( "Unexpected Hex digit" );
                    ch = d << 4;
                    d = getc( fpin );
                    if ( d >= '0' && d <= '9' ) d = d - '0';
                    else if ( d >= 'a' && d <= 'f' ) d = d - 'a' + 10;
                    else if ( d >= 'A' && d <= 'F' ) d = d - 'A' + 10;
                    else Error( "Unexpected Hex digit" );
                    ch += d;
                }
            }
            token[ li++ ] = ch;
            ch = getc( fpin );
        }
        if ( li == 0 ) return( CIRCUMFLEX );
        token[ li ] = 0;
        yylval.up = P_create( li, token );
        return( LNAME );
    } else {
        while ( lflag && ch != EOF ) {
            token[ li++ ] = ch;
            ch = getc( fpin );

            if ( li!=2 || token[1]!='.' || !isdigit(token[0]) ) {
                lflag = 0;
                switch( ch ) {
                case '.': case '_':

                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':

                case 'A': case 'B': case 'C': case 'D': case 'E':
                case 'F': case 'G': case 'H': case 'I': case 'J':
                case 'K': case 'L': case 'M': case 'N': case 'O':
                case 'P': case 'Q': case 'R': case 'S': case 'T':
                case 'U': case 'V': case 'W': case 'X': case 'Y':
                case 'Z':

                case 'a': case 'b': case 'c': case 'd': case 'e':
                case 'f': case 'g': case 'h': case 'i': case 'j':
                case 'k': case 'l': case 'm': case 'n': case 'o':
                case 'p': case 'q': case 'r': case 's': case 't':
                case 'u': case 'v': case 'w': case 'x': case 'y':
                case 'z':
                    lflag = 1; break;
                }
            }

        }
    }
    token[ li ] = 0;
    yylval.up = P_create( li, token );
    return( PNAME );
}

void yyerror( char *str )
{
    fprintf( fpout, "*** %s ***\n", str );
}
