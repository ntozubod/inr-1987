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

A_OBJECT AB_comp( A_OBJECT A1, B_OBJECT B2, T2_OBJECT T2_Sigma )
{
    A_OBJECT A;
    R_OBJECT R1;
    R_OBJECT R2;
    int current, cur_a, cur_b, cur_c, next_c, s1, t1, nt, lt;
    int state1, state2, i, iz;
    SHORT output1, output2;
    A_row *p1, *p1z;
    R_row *cur_st;

    if ( ! ( A1-> A_ems ) ) {
        if ( A1-> A_ems ) A1 = A_deems( A1 );
    }

    A1 = A_min( A1 );
    nt = A1-> A_nT;

    A = A_create();
    A-> A_nT = nt;
    lt = nt - 1;
    R1 = R_create();
    R2 = R_create();

    if ( R_insert( R1, START, START ) != 0 )
        Error( "AB_comp: BOTCH 1a" );
    if ( R_insert( R2, 0, 0 ) != START )
        Error( "AB_comp: BOTCH 1b" );

    if ( R_insert( R1, FINAL, FINAL ) != 1 )
        Error( "AB_comp: BOTCH 2a" );
    if ( R_insert( R2, 1, 0 ) != FINAL )
        Error( "AB_comp: BOTCH 2b" );

    for( current = 0; current < R1-> R_n; current++ ) {
        cur_st = R_rec( R1, current );
        cur_a = cur_st-> R_a;
        cur_b = cur_st-> R_b;
        cur_c = R_insert( R2, current, 0 );

        p1  = A1-> A_p[ cur_a ];
        p1z = A1-> A_p[ cur_a + 1 ];

        while( p1 < p1z ) {
            if ( p1-> A_b == 1 ) {  /* force an endmarker on last tape */
                assert( p1-> A_c == 1 );
                s1 = 1;
                t1 = lt;
            } else {
                s1 = ( p1-> A_b ) / nt;
                t1 = ( p1-> A_b ) % nt;
            }
            if ( t1 < lt ) {
                next_c =
                    R_insert( R2, R_insert( R1, p1-> A_c, cur_b ), 0 );
                A = A_add( A, cur_c, p1-> A_b, next_c );
            } else {
                B2 = B_set_trans( B2, cur_b, s1, T2_Sigma );
                if ( B2-> B_to != MAXSHORT ) {
                    next_c =
                        R_insert( R2,
                            R_insert( R1, p1-> A_c, B2-> B_to ), 0 );
                    output1 = B2-> B_output[ 0 ];
                    if ( output1 == MAXSHORT ) {
                        A = A_add( A, cur_c, 0, next_c );
                    } else {
                        iz = Ssize( (char *) B2-> B_output ) / sizeof(SHORT);
                        state1 = cur_c;
                        i = 1;
                        output2 = B2-> B_output[ i ];
                        while ( output2 != MAXSHORT ) {
                            assert( i < iz );
                            state2 = R_insert( R2, current, i );
                            A = A_add( A, state1, output1 * nt + lt, state2 );
                            state1 = state2;
                            output1 = output2;
                            ++i;
                            output2 = B2-> B_output[ i ];
                        }
                        if ( output1 == 1 ) {
                            A = A_add( A, state1, 1, next_c );
                        } else {
                            A = A_add( A, state1, output1 * nt + lt, next_c );
                        }
                    }
                }
            }
            ++p1;
        }
    }
    A_destroy( A1 );
    B_destroy( B2 );
    R_destroy( R1 );
    R_destroy( R2 );
    return( A );
}
