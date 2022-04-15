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

B_OBJECT B_create_B4( )
{
    return( (B_OBJECT) B4_create() );
}

B_OBJECT B_create_B4i( )
{
    return( (B_OBJECT) B4i_create() );
}

void B_destroy( B_OBJECT B )
{
    if ( B != NULL ) {
        switch( B-> Type ) {
        case B4_Object:
            B4_destroy( (B4_OBJECT) B );
            break;
        case B4i_Object:
            B4i_destroy( (B4i_OBJECT) B );
            break;
        }
    }
}

B_OBJECT B_set_trans( B_OBJECT B,
    SHORT from, SHORT symb, T2_OBJECT T2_Sigma )
{
    assert( B != NULL );

    switch( B-> Type ) {
    case B4_Object:
        B4_set_trans( (B4_OBJECT) B, from, symb, T2_Sigma );
        break;
    case B4i_Object:
        B4i_set_trans( (B4i_OBJECT) B, from, symb, T2_Sigma );
        break;
    }
    return( B );
}

void B_print_trans( B_OBJECT B, T2_OBJECT T2_Sigma )
{
    assert( B != NULL );

    switch( B-> Type ) {
    case B4_Object:
        B4_print_trans( (B4_OBJECT) B, T2_Sigma );
        break;
    case B4i_Object:
        B4i_print_trans( (B4i_OBJECT) B, T2_Sigma );
        break;
    }
}
