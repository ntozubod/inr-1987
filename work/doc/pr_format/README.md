# A improved display format for INR

## Introduction

INR from the earliest days needed to be able to read in and to display
finite automata.
The output format was intended to be human readable, at least in the
cases where the number of transitions was relatively few.
Large automata almost always contain enough complexity that their structure
is usually not obvious at all.

On input this format was intended to be extremely forgiving, accepting as
state names anything that did not invalidate proper parsing.
In practice, this meant that new lines, tabs and blanks needed to be escaped
as '\n', '\t', and '\_', respectively.
Null bytes ('\x00') were treated as string terminators and probably
inconsistently handled.

Transition labels had the same rules as state names except that '.' was
treated as a separator between a tape number and the symbol part of the
label.
Complex rules were applied to distinguish the usage of '.' as a separator
from its usage as a part of symbol.
This was a result of original evolution of the code as multi-tape automata
were introduced.

The 'magic state names' were used to identify the start state (assumed to
be unique) and all of the final states.

Two 'magic labels' were used to identify empty word transitions (lambda or
epsilon transitions), end-markers in the case of deterministic multi-tape
transducers, and as a part of the indication of which states are to be
considered as final.
The number of tapes was inferred from the usage pattern.

The :pr format was intended to by quite readable and to present automata
as they were usually considered as a collection of transitions,
each made up of a 'from state', a 'transition label', and a 'to state'.
The emphasis was on readability and this is the format displayed when INR
is used interactively.
