/*
 * Copyright (c) 1987, J Howard Johnson, University of Waterloo.
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

int disp_flag;

int do_n_i( char *op )
{
    int i;
    if ( !strcmp("alph",op) ) {
        fprintf( fpout, "\n" );
        for( i = 2; i < TT-> T_n; i++ ) {
            fprintf( fpout, "%s", T_name( TT, i ) );
            fprintf( fpout, " " );
        }
        fprintf( fpout, "\n" );
    } else if ( !strcmp("free",op) ) {
        fprintf( fpout, "\n" );
        (void) Sarena();
        fprintf( fpout, "\n" );
    } else if ( !strcmp("list",op) ) {
        fprintf( fpout, "\n" );
        for( i = 0; i < TAlist-> T_n; i++ ) {
            fprintf( fpout, "%s  ", pad20(
                         T_name( TAlist, i ) ) );
            (void) A_rept( Alist[ i ] );
        }
        fprintf( fpout, "\n" );
    } else if ( !strcmp("noreport",op) ) A_report = 0;
    else if ( !strcmp("pr",op) ) {
        for( i = 1; i < TAlist-> T_n; i++ )
            if ( Alist[ i ]-> A_nrows > 0 )
                (void) A_store( Alist[ i ],
                                T_name( TAlist, i ), TT );
    } else if ( !strcmp("report",op) ) A_report = 1;
    else if ( !strcmp("save",op) ) {
        for( i = 1; i < TAlist-> T_n; i++ )
            if ( Alist[ i ]-> A_nrows > 0 )
                Alist[ i ] = A_save( Alist[ i ],
                                     T_name( TAlist, i ), TT );
    } else if ( !strcmp("time",op) ) pr_time_diff();
    else if( !strcmp("help",op) )

        fprintf( fpout, "\n\
Basic Help\n\
\n\
To terminate session type       :bye;\n\
\n\
To get additional help type\n\
\n\
     :help ops;          (Priority and meaning of basic operators)\n\
     :help colonops;     (Colon operators)\n\
     :help ioops;        (IO operators)\n\
     :help stats;        (Form of statements)\n\n" );

    else if (  !strcmp("done",op)
               || !strcmp("quit",op)
               || !strcmp("stop",op)
               || !strcmp("bye",op) ) return 1;
    else Warning( "Unknown function" );
    return 0;
}

A_OBJECT do_an_a( A_OBJECT A, char *op )
{
    int i, num;
    disp_flag = 0;
    if ( !strcmp("pr",op) ) {
        (void) A_rept( A );
        fprintf( fpout, "\n" );
        A = A_store( A, (char *) NULL, TT );
        fprintf( fpout, "\n" );
    } else if ( !strcmp("prsseq",op) ) {
        fprintf( fpout, "\n" );
        A = A_prsseq( A, (char *) NULL, TT );
        fprintf( fpout, "\n" );
    } else if ( !strcmp("acomp",op) ) {
        if ( A_report ) fprintf( fpout, "(acomp)\n" );
        Atemp = A_star( A_alph( A_copy( A ) ) );
        A = A_differ( Atemp, A );
        disp_flag = 2;
    } else if ( !strcmp("alph",op) ) {
        if ( A_report ) fprintf( fpout, "(alph)\n" );
        A = A_alph( A );
        disp_flag = 2;
    } else if ( !strcmp("card",op) ) {
        i = A_card( A );
        if ( i < 0 ) fprintf( fpout, "Infinite\n" );
        else {
            fprintf( fpout, "%d word", i );
            if ( i != 1 ) fprintf( fpout, "s" );
            fprintf( fpout, "\n" );
        }
    } else if ( !strcmp("closed",op) ) {
        A = A_clsure( A );
        disp_flag = 1;
    } else if ( !strcmp("clsseq",op) ) {
        A = A_clsseq( A );
        disp_flag = 2;
    } else if ( !strcmp("comp",op) ) {
        if ( A_report ) fprintf( fpout, "(comp)\n" );
        if ( (i = T_member( TAlist, "SIGMA" )) >= 0 ) {
            Atemp = A_star( A_alph( A_copy( Alist[i] ) ) );
            A = A_differ( Atemp, A );
        } else  fprintf( fpout,
                             "Error in comp: SIGMA not defined\n" );
        disp_flag = 2;
    } else if ( !strcmp("deecho",op) ) {
        A = A_deecho( A,
                      T_insert(TT,"ECHO") * A-> A_nT + 1,
                      T_insert(TT,"NOECHO") * A-> A_nT + 1 );
        disp_flag = 2;
    } else if ( !strcmp("dfa",op) ) {
        A = A_subs( A );
        disp_flag = 1;
    } else if ( !strcmp("dfamin",op) ) {
        A = A_min( A );
        disp_flag = 1;
    } else if ( !strcmp("enum",op) ) {
        A = A_enum( A, TT, 1000 );
    } else if ( !strcmp("limit",op) ) {
        if ( A_report ) fprintf( fpout, "(limit)\n" );
        if ( A-> A_nT != 2 )
            Error( "Wrong number of tapes (limit)" );

        Atemp = A_differ(
                    A_retape(
                        A_copy( A ),
                        A_letter( 0, T_insert( TT, "1" ) ),
                        TT
                    ),
                    A_retape(
                        A_copy( A ),
                        A_letter( 0, T_insert( TT, "0" ) ),
                        TT
                    )
                );

        A = A_join( A, Atemp );
        disp_flag = 2;
    } else if ( !strcmp("lamcm",op) ) {
        A = A_lamcm( A );
        disp_flag = 1;
    } else if ( !strcmp("lameq",op) ) {
        A = A_lameq( A );
        disp_flag = 1;
    } else if ( !strcmp("lenmin",op) ) {
        A = A_lenmin( A );
        disp_flag = 2;
    } else if ( !strcmp("min",op) ) {
        A = A_min( A );
        disp_flag = 1;
    } else if ( !strcmp("length",op) ) {
        i = A_minlen( A );
        if ( i < 0 ) fprintf( fpout, "Empty Set\n" );
        else {
            fprintf( fpout, "%d letter", i );
            if ( i != 1 ) fprintf( fpout, "s" );
            fprintf( fpout, "\n" );
        }
    } else if ( !strcmp("nfa",op) ) {
        A = A_close( A );
        disp_flag = 1;
    } else if ( !strcmp("pref",op) ) {
        if ( A_report ) fprintf( fpout, "(pref)\n" );
        A = A_pref( A );
        disp_flag = 2;
    } else if ( !strcmp("report",op) ) {
        A = A_rept( A );
    } else if ( !strcmp("rev",op) ) {
        if ( A_report ) fprintf( fpout, "(rev)\n" );
        A = A_rev( A );
        disp_flag = 2;
    } else if ( !strcmp("sseq",op) ) {
        A = A_sseq( A );
        disp_flag = 2;
    } else if ( !strcmp("LMsseq",op) ) {
        A = A_LMsseq( A );
        disp_flag = 2;
    } else if ( !strcmp("GMsseq",op) ) {
        A = A_GMsseq( A );
        disp_flag = 2;
    } else if ( !strcmp("stems",op) ) {
        A_prstems( A, TT, 0 );
    } else if ( !strcmp("suff",op) ) {
        if ( A_report ) fprintf( fpout, "(suff)\n" );
        A = A_suff( A );
        disp_flag = 2;
    } else if ( !strcmp("time",op) ) {
        pr_time_diff();
        disp_flag = 2;
    } else if ( !strcmp("trim",op) ) {
        A = A_trim( A );
        disp_flag = 1;
    } else if ( !strcmp("update",op) ) {
        A = A_open( A );
        disp_flag = 1;
    } else if ( (num = tonum(op)) >= 0 ) {
        if ( A_report ) fprintf( fpout, "(pow)\n" );
        A = A_catpow( A, num );
        disp_flag = 2;
    } else  Warning( "Unknown function" );
    return A;
}

A_OBJECT do_ann_a( A_OBJECT A, char *op, char *arg )
{
    int num;
    disp_flag = 0;
    if ( !strcmp("enum",op) && (num = tonum(arg)) >= 0)
        A = A_enum( A, TT, num );
    else if ( !strcmp("save",op) )
        A = A_save( A, arg, TT );
    else if ( !strcmp("pr",op) )
        A = A_store( A, arg, TT );
    else if ( !strcmp("prsseq",op) )
        A = A_prsseq( A, arg, TT );
    else if ( !strcmp("stems",op) )
        A_prstems( A, TT, tonum(arg) );
    else if ( !strcmp("surgery",op) ) {
        num = tonum(arg);
        if ( num < 2 || num >= A-> A_nrows )
            Warning( "Illegal surgery" );
        else {
            A = A_open( A );
            A = A_add( A, num,
                       T_insert(TT,"INCISION") * A-> A_nT,
                       num );
        }
    } else Warning( "Unknown function" );
    return A;
}

A_OBJECT do_nn_a( char *op, char *arg )
{
    int i;
    A_OBJECT A;
    disp_flag = 0;
    A = NULL;
    if ( !strcmp("get",op) ) {
        if ( (i = T_member( TAlist, arg )) >= 0 )
            A = A_copy( Alist[i] );
        disp_flag = 1;
    } else if ( !strcmp("read",op) || !strcmp("load",op) ) {
        A = A_load( arg, TT );
        disp_flag = 1;
    } else if ( !strcmp("readwords",op) ) {
        A = A_lwds( arg, TT );
        disp_flag = 1;
    } else if ( !strcmp("save",op) ) {
        if ( (i = T_member( TAlist, arg )) >= 0 )
            A = A_save( A_copy( Alist[i] ), arg, TT );
        else fprintf( fpout, "Warning: %s undefined\n", arg );
    } else if ( !strcmp("pr",op) ) {
        if ( (i = T_member( TAlist, arg )) >= 0 )
            A = A_store( Alist[i], arg, TT );
        else fprintf( fpout, "Warning: %s undefined\n", arg );
    } else if ( !strcmp("help",op) ) {
        if ( !strcmp("ops",arg) )

            fprintf( fpout, "\n\
Operations by priority (highest to lowest):\n\
\n\
+   *   ?               postfix operators for 1 or more, 0 or more, 0 or 1\n\
<concatenation>         no explicit operator\n\
\\   /                   left factor, right factor\n\
&   -                   intersection, set difference\n\
|   !   ||  !!          union, exclusive or, elseor, shuffle\n\
$                       project\n\
@   @@                  composition, join\n\
<all colon operators>   see :help colonops; for details\n\
,                       Cartesian product within (), union within {}\n\
\n\
All operators associate from left to right.\n\
Parentheses may be used to indicate a specific order of evaluation.\n\
{,,,} is a set constructor.\n\
(,,,) is a tuple construtor.\n\
[   ] is the tape-shifting operator\n\
'   ' is a string of single letter tokens.\n\
`   ` is a token containing arbitrary symbols.\n\
^     indicates the empty word (or an explicit concatenation operator).\n\n" );

        else if ( !strcmp("colonops",arg) )

            fprintf( fpout, "\n\
Colon operations (postfix operators at lowest priority)\n\
\n\
Transformation Operators               Displaying Operators\n\
:acomp      Active complement          :card       Print cardinality\n\
:alph       Active alphabet            :enum       Enumerate language\n\
:clsseq     Subsequential closure      :length     Display min word length\n\
:comp       Complement w.r.t. SIGMA*   :pr         Display automaton\n\
:lenmin     Words of min length        :prsseq     Subsequential display\n\
:pref       Set of prefixes            :report     Display report line\n\
:rev        Reverse                    :stems #    Print tape # stems\n\
:sseq       Subsequential transducer\n\
:LMsseq     LM Subsequential transducer\n\
:GMsseq     GM Subsequential transducer\n\
:suff       Set of suffixes            Coercing operators\n\
:<number>   Concatenation power        :update :nfa :trim :lameq\n\
:(<number>) Composition power          :lamcm :closed :dfa :dfamin\n\
\n\
:enum may take an optional argument to specify the quantity of output.\n\n" );

        else if ( !strcmp("ioops",arg) )

            fprintf( fpout, "\n\
IO operations\n\
\n\
:pr <filename>        Postfix operator to display automaton into a file\n\
:save <filename>      Postfix operator to save automaton in compressed form\n\
:load <filename>      Operator without left argument to get value from a file\n\
:readwords <filename> Operator with no argument to load a word file\n\
\n\
:get <variable>       Operator with no arguments to get value from a variable\n\
\n\
<var> = :load;        Short for <var> = :load <var>;\n\
:save <var>;          Short for <var> :save <var>;\n\n" );

        else if ( !strcmp("stats",arg) )

            fprintf( fpout, "\n\
Statement forms\n\
\n\
:bye;           Terminate session\n\
\n\
:alph;          Display token symbol table\n\
:free;          Display storage management report\n\
:list;          Display report lines for variables\n\
:noreport;      Turn off debug reporting\n\
:pr;            Save each variable into a file with the variable as file name\n\
:report;        Turn on debug reporting\n\
:save;          Save in compressed form all variables into files\n\
\n\
<var> = <exp>;  Assign value of expression to variable\n\
<exp>;          Compute and display expression\n\
<exp>:;         Compute and enumerate words in expression\n\
\n\
The action of <exp>; depends on the last operator evaluated:\n\
Coercing Operator:  display the value using :pr implicitly\n\
Printing Operator:  do not perform additional display\n\
Other:              coerce to DFAMIN and display using :pr\n\n" );

        else if ( !strcmp("w",arg) )

            fprintf( fpout, "\n\
(from GPLv3: see COPYING or http://www.gnu.org/licenses/gpl-3.0.html)\n\
  15. Disclaimer of Warranty.\n\
\n\
  THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\n\
APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT\n\
HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY\n\
OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,\n\
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n\
PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM\n\
IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF\n\
ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\n\
\n" );

        else if ( !strcmp("c",arg) )

            fprintf( fpout, "\n\
(from GPLv3: see COPYING or http://www.gnu.org/licenses/gpl-3.0.html)\n\
  2. Basic Permissions.\n\
\n\
  All rights granted under this License are granted for the term of\n\
copyright on the Program, and are irrevocable provided the stated\n\
conditions are met.  This License explicitly affirms your unlimited\n\
permission to run the unmodified Program.  The output from running a\n\
covered work is covered by this License only if the output, given its\n\
content, constitutes a covered work.  This License acknowledges your\n\
rights of fair use or other equivalent, as provided by copyright law.\n\
\n\
  You may make, run and propagate covered works that you do not\n\
convey, without conditions so long as your license otherwise remains\n\
in force.  You may convey covered works to others for the sole purpose\n\
of having them make modifications exclusively for you, or provide you\n\
with facilities for running those works, provided that you comply with\n\
the terms of this License in conveying all material for which you do\n\
not control copyright.  Those thus making or running the covered works\n\
for you must do so exclusively on your behalf, under your direction\n\
and control, on terms that prohibit them from making any copies of\n\
your copyrighted material outside their relationship with you.\n\
\n\
  Conveying under any other circumstances is permitted solely under\n\
the conditions stated below.  Sublicensing is not allowed; section 10\n\
makes it unnecessary.\n\
\n" );

        else fprintf( fpout, "Unknown help request" );
    } else Warning( "Unknown function" );
    return A;
}
