Introduction
============

INR is a program that constructs finite state automata and transducers
from generalized rational expressions. Although originally developed
with to support research into approximate string matching [@Johnson83]
it has proved useful for a number of other purposes.

The program operates in a traditional read/evaluate/print mode, that is,
it repeatedly reads an expression from the input stream, computes the
automaton required, and displays a result. For example, if we wish to
find the minimized automaton over the alphabet $\{a,b\}$ that recognizes
words that contain at least one "$a$" and at least one "$b$". This can
be described using the extended regular expression indicating the
intersection of words containing $a$ and words containing $b$:
$$\{a,b\}^*a\{a,b\}^* \wedge \{a,b\}^*b\{a,b\}^*.$$ This expression can
be presented to INR in the following form:

>     {a,b}* a {a,b}* & {a,b}* b {a,b}*;

INR will print back

>     DFA MIN States: 5      Trans: 9      Tapes: 1  Strg: 1 K
>
>     (START) a 2
>     (START) b 3
>     2 a 2
>     2 b 4
>     3 a 4
>     3 b 3
>     4 -| (FINAL)
>     4 a 4
>     4 b 4

indicating that a minimized deterministic one-tape finite automaton has
been computed with 5 states and 9 transitions. It requires 1 Kbyte (1024
bytes) of memory in the representation used by INR. After this the
transitions of the automaton are presented in the form

> *source-state label target-state*

so that

>     3 a 4

indicates a transition from state `3` to state `2` which reads an $a$.
The start state is denoted by the special state name `(START)` and final
states are indicated by a transition on end-marker (`-|`) to the dummy
state `(FINAL)`. In the above example

>     4 -| (FINAL)

indicates that `4` is a final state.

If we want to enumerate some of the words recognized by this automaton
we can specify the following command to INR:

>     {a,b}* a{a,b}* & {a,b}* b {a,b}*:;

The beginning of the result typed would then be:

>     a b 
>     b a 
>     a a b 
>     a b a 
>     a b b 
>     b a a 
>     b a b 
>     b b a 
>     a a a b 
>     a a b a 
>     a a b b 
>     a b a a 
>     a b a b 
>     a b b a 
>     a b b b 
>     b a a a 
>      ...

The enumerator prints the first few words of the infinite sequence
terminating when it reaches a preset limit.

Another interactive use for INR involves testing whether two regular
sets are the same. For example consider the set of all words over the
alphabet $\{a,b\}^*$ that are not either all $a$'s or all $b$'s. This
can be denoted using the extended regular expression
$$\{a,b\}^* - ( \{a\}^* \cup \{b\}^* ).$$ This can be indicated to INR
as:

>     {a,b}* - ( a* | b* );

To check that these are the same sets, we can compute the symmetric
difference of the two regular languages and verify that is empty. The
symmetric difference (exclusive or) operator for INR is the symbol `!`.

>     ( {a,b}* a {a,b}* & {a,b}* b {a,b}* )
>       ! ( {a,b}* - ( a* | b* ) );

This yields the result

>     Empty Automaton

indicating that the symmetric difference is empty, that is, that the two
indicated languages are identical.

In the first example a generalized regular expression was entered and an
automaton was printed back. Instead of this, the automaton could have
been written to a file "temp" in the form shown above:

>     {a,b}* a {a,b}* & {a,b}* b {a,b}* :pr temp;

Alternatively, it could be written to the file "temp" in a condensed
format which usually requires about half of the space and can be loaded
more quickly.

>     {a,b}* a {a,b}* & {a,b}* b {a,b}* :save temp;

It is also possible to assign it to an internal variable named "temp":

>     temp = {a,b}* a {a,b}* & {a,b}* b {a,b}*;

In each of these cases it is then available for computation in further
expressions allowing for a considerable saving in typing. The value can
be retrieved from the file temp in either `:pr` form or `:save` form by
means of the sub-expression `:load temp`. The value from a variable can
be retrieved from an internal variable by simply using the variable name
itself (if that name has not been used as a transition label).

INR is very often run in a non-interactive (batch) mode using variables
to build up larger and larger automata followed by a `:pr` or a `:save`
to store the result into a file. For large automata this is the
preferred approach since INR provides no mechanisn for recalling past
expressions or for editing them.
