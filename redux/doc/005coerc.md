Coercing and Displaying Operators
=================================

Coercing Operators
------------------

Internally *INR* operates on the following laziness principle. Each
automaton can be in one a number of modes depending on how much work has
been done on it. At one extreme, the automaton is a collection of
transitions appearing any order, and possibly with duplicate transtions
or inaccessible states. At the other extreme, the automaton is a
minimized deterministic automaton with its transitions sorted and
unduplicated. Each operation then automatically coerces its arguments to
the required form by calling the appropriate routine and sets the mode
of its result value to a appropriate value. For example, union takes two
sets of transitions and after renaming combines them into one set. On
the other hand, intersection requires that its operands be minimized
deterministic automata and produces as a result a not necessarily
minimized deterministic automaton. For debugging and education purposes,
the coercing operator can be explicitly used:

> :nfa
>
> :   Sort and unduplicate transitions.
>
> :trim
>
> :   Remove states that are unreachable. (Reduce)
>
> :lameq
>
> :   Remove lambda equivalent states.
>
> :lamcm
>
> :   Combine lambda implied states.
>
> :closed
>
> :   Form lambda closure.
>
> :dfa
>
> :   Form deterministic machine. (Subsets Construction)
>
> :min
>
> :   Minimize the DFA.

Thus `<expression> :trim;` will print the trim (reduced) automaton
corresponding to the expression. Note that coercing is one way and that
some operators are smart about promoting their results.

Displaying Operators
--------------------

The following operators cause some form of printing:

> :pr
>
> :   Print in readable format.
>
> :save
>
> :   Save in condensed format.
>
> :report
>
> :   Write a one line report.
>
> :enum
>
> :   Enumerate words.
>
> :card
>
> :   Count number of words and print.
>
> :length
>
> :   Print length of shortest word.

The operation `:pr` may be followed by a filename in which case the
automaton is printed into that file. The operation `:save` must be
followed by a filename indicating the desired file. The operation
`:enum` may be followed by a positive number indicating the limit on
printing desired. The number bounds the length of the longest word (in
tokens) and the number of tokens to be printed. Thus 1000 means that no
word longer than 1000 characters will be printed and that after 1000
tokens have been displayed no further words will be started.

Input Operator
--------------

To cause an automaton to be loaded the operator `:read` followed by a
filename can be used. This will load a file in either `:pr` or `:save`
format.
