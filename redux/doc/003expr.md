Generalized Regular Expressions
===============================

This section outlines the method for building expressions which define
regular languages. This corresponds to the one-tape automaton case for
INR. In these situations, INR will compute a minimized deterministic
finite automaton (DFA MIN) and display the result. More mathematically
inclined readers will notice an anomolous use of set notation that is
common in the formal language theory literature and continued here to
avoid pedantry and cumbersome notational problems. A string containing
one letter is ambiguously written as the letter and a set containing one
element is ambiguously written as its element. With this understanding
`a` $\equiv$ `{a}` $\equiv$ `{'a'}`.

Token
-----

A token is a primitive element. It is represented by a symbol that has
not been assigned a value as a variable and denotes the automaton that
accepts that token and then quits.

`abc;`\
or `` `abc`; ``

    (START) abc 2
    2 -| (FINAL)

Variable
--------

A variable is a symbol that has been previously been assigned a value
using an assignment statement.

`var = abc; var;`

    (START) abc 2
    2 -| (FINAL)

Variables should use names that are clearly distinguishable from tokens.
For example, they should be at least two letters in length to avoid
conflict with token names.

String
------

A string is a possibly empty sequence of tokens and denotes the
automaton that recognizes only that sequence. Strings are indicated by
writing the tokens in the sequence in which they occur. They normally
require spaces between adjacent tokens because of the lexical rules for
distinguishing symbols. Thus `a b c` indicates a string with three
one-letter tokens whereas `abc` indicates a single token.

Since one-letter tokens are very useful to denote the printable
characters of the ASCII (or EBCDIC) character set an alternative
notation is provided to indicate strings of one-letter tokens. Any such
string can be written without blanks but preceded and followed by single
forward quotes ('):

`a b c;`\
or `'abc';`

    (START) a 2
    2 b 3
    3 c 4
    4 -| (FINAL)

Any sequence of characters including operators may be used within a
string with the following exceptions: `\'` indicates a single forward
quote character, `\\` indicates a single backslash character, `\n`
indicates a newline character, and `\t` indicates a tab character. Note
that strings of one character are a good way of entering single letter
tokens that are otherwise treated as operators or white space by the
lexical scanner. Thus the token containing exactly one blank can be
specified using the notation `' '`, newline can be specified by `'\n'`,
and the plus character by `'+'`. Although backquotes also work in these
cases, a greater uniformity of notation can be achieved if many strings
also occur.

Empty String
------------

The empty string is indicated by `^`. This symbol is used because of its
vague similariy to the Greek letter $\Lambda$ that is often used for
this purpose. Note that in EBCDIC this character has a graphic
representation of $\not c$. Alternative representations is `()` (empty
parentheses) or `''` (empty forward quotes).

`^;`\
or `();`\
or `'';`

    (START) -| (FINAL)

Finite Set of Strings
---------------------

Finite sets of strings are indicated using the standard brace/comma
notation. Thus `{'ab','aa','a'}` denotes an automaton recognizing a
three strings `'ab'`, `'aa'` and `'a'`.

`{'ab','aa','a'};`

    (START) a 2
    2 -| (FINAL)
    2 a 3
    2 b 3
    3 -| (FINAL)

Empty Set
---------

The empty set is indicated by `{}`.

`{};`

    Empty Automaton

Alternation (Set Union)
-----------------------

The operator for alternation (OR) is expressed using the vertical bar
`|`.

`{'abc'}|{'def'};`

    (START) a 2
    (START) d 3
    2 b 4
    3 e 5
    4 c 6
    5 f 6
    6 -| (FINAL)

Note that the concatenations are done before the alternation since
concatenation has a higher priority than alternation. Parentheses may be
used to enforce a different order of evaluation:

`a b (c | d) e f;`\
or `a b c e f | a b d e f;`

    (START) a 2
    2 b 3
    3 c 4
    3 d 4
    4 e 5
    5 f 6
    6 -| (FINAL)

Note also that the brace-comma set notation is always equivalent to
parentheses with `|`. This departs slightly from standard set theory but
is a natural generalization in this situation. Thus the above automata
can also be denoted by `a b { c, d } e f`.

Concatenation
-------------

Concatenation is represented by adjacency. That is, there is no explicit
concatenation operator although `^` can be used if desired.

`{ a, b } { c, d e };`\
or `{ a, b }^{ c, d e };`\
or `{'ac','ade','bc','bde'};`

    (START) a 2
    (START) b 2
    2 c 3
    2 d 4
    3 -| (FINAL)
    4 e 3

Note that strings, as discussed before, are just the concatenation of
their constituent tokens.

Kleene Plus and Star
--------------------

The concatenation closure operators (Kleene $+$ and Kleene $*$) are
represented by the corresponding unary postfix operator.

`(a b)+;`\
or `'ab'+;`

    (START) a 2
    2 b 3
    3 -| (FINAL)
    3 a 2

`(a b)*;`\
or `'ab'*;`

    (START) -| (FINAL)
    (START) a 2
    2 b (START)

The difference is that the second form includes the empty word. Note
that parentheses are used to group together the argument to + and \*.
This is because these two operators are considered to have higher
priority than any of the others.

`a b*;`\
or `a (b*);`

    (START) a 2
    2 -| (FINAL)
    2 b 2

Optional
--------

The operator indicating zero or one occurrences of a set is denoted by
`?`. This can be used to identify optional components.

`a b?;`

    (START) a 2
    2 -| (FINAL)
    2 b 3
    3 -| (FINAL)

The operators `+`, `*`, and `?` are computed from left to right.

Concatenation Power
-------------------

To express the concatenation of a set with itself exactly $k$ times the
expression may be followed by a `:` and the value $k$. Thus `R1 :2` is
the same as `R1 R1` and `R1 :3` the same as `R1 R1 R1`.

`{a,b} :2;`

    (START) a 2
    (START) b 2
    2 a 3
    2 b 3
    3 -| (FINAL)

To express the concatenation of a set with itself $k$ or fewer times the
notation `R1? :`$k$ can be used. To express the concatenation of a set
with itself more than $k$ times the set difference can be used:
`R1* - (R1? :`$k$`)`.

Concatenation Quotient
----------------------

If concatenation is interpreted as a multiplication of strings so that
`'abc'` $\cdot$ `'def'` $=$ `'abcdef'` we can define a right quotient
operator $/$ as `'abcdef'` / `'def'` $=$ `'abc'` and a left quotient
operator $\backslash$ as `'abc'` $\backslash$ `'abcdef'` $=$ `'def'`.
The (left or right) quotient of sets is defined as the union of their
elementwise quotients.

`'ab' \ 'abcdef' / 'ef';`\
or `'cd';`

    (START) c 2
    2 d 3
    3 -| (FINAL)

Set Intersection
----------------

The automaton recognizing the intersection of two regular languages is
indicated by the `&` operator.

`{a,b}* & b {a,c}*;`\
or `b a*;`

    (START) b 2
    2 -| (FINAL)
    2 a 2

Set Difference
--------------

The operator `-` indicates the difference of two two regular languages.

`{a,b}* - b a*;`\
or `(^ | a | b a* b) {a,b}*;`

    (START) -| (FINAL)
    (START) a 2
    (START) b 3
    2 -| (FINAL)
    2 a 2
    2 b 2
    3 a 3
    3 b 2

Symmetric Difference (Exclusive Or)
-----------------------------------

The symmetric difference of two languages is the set of words that are
in one of the languages but not the other and will be indicated by the
`!` operator. Thus $R_1 ! R_2 \equiv (R_1-R_2) \cup (R_2-R_1)$.

`{a,b}* ! {a,c}*;`\
or `a* ( b {a,b}* | c {a,c}* );`

    (START) a (START)
    (START) b 2
    (START) c 3
    2 -| (FINAL)
    2 a 2
    2 b 2
    3 -| (FINAL)
    3 a 3
    3 c 3

Note that the symmetric difference of a language with itself is the
empty set. Thus this operation can be used to check the equivalence of
two regular languages.

Shuffle
-------

The shuffle of two words is the set of words formed by alternatively
taking some letters from each of the words. Thus the shuffle of the
words `'ab'` and `'cd'` is the set
`{'abcd','acbd','acdb','cabd','cadb','cdab'}`. The shuffle of two sets
of words is the union of the shuffles of all pairs of words such that
one word is taken from each set. The shuffle operator is indicated by
the symbol `!!` which mildly suggests the symbol $\amalg$ that is often
used for this purpose.

`'ab' !! 'cd';`\
or `{'abcd','acbd','acdb',`\
`  'cabd','cadb','cdab'};`

    (START) a 2
    (START) c 3
    2 b 4
    2 c 5
    3 a 5
    3 d 6
    4 c 7
    5 b 7
    5 d 8
    6 a 8
    7 d 9
    8 b 9
    9 -| (FINAL)

Active Alphabet
---------------

The active alphabet of an automation is the set tokens that can actually
occur in a word in the language.

`'abcd' / 'd' :alph;`\
or `{a,b,c};`

    (START) a 2
    (START) b 2
    (START) c 2
    2 -| (FINAL)

Reverse
-------

The reverse of a string `'abcd'` is the string `'dcba'`. The reverse of
a set of strings is the set of their reverses.

`{'abc','abd'} :rev;`\
or `{'dba','cba'};`

    (START) c 2
    (START) d 2
    2 b 3
    3 a 4
    4 -| (FINAL)

Prefixes
--------

The prefixes of `'abcd'` is the set `{ ^, 'a', 'ab', 'abc', 'abcd' }`.
The prefixes of a set of strings is the union of the prefix sets for
each word. Note that `R1 :pref` is an alternate (more efficient) way of
indicating the set `R1 / (R1 :alph)*`.

`{'abc','abd'} :pref;`\
or `{'abc','abd'} / {a,b,c,d}*;`\
or `{^,'a','ab','abc','abd'};`

    (START) -| (FINAL)
    (START) a 2
    2 -| (FINAL)
    2 b 3
    3 -| (FINAL)
    3 c 4
    3 d 4
    4 -| (FINAL)

Suffixes
--------

The suffixes of `'abcd'` is the set `{ ^, 'd', 'cd', 'bcd', 'abcd' }`.
The suffixes of a set of strings is the union of the suffix sets for
each word. Note that `R1 :suff` is an alternate way of indicating the
set `(R1 :alph)* \ R1`.

`{'abc','abd'} :suff;`\
or `{a,b,c,d}* \ {'abc','abd'};`\
or `{^,'c','d','bc','bd',`\
`  'abc','abd'};`

    (START) -| (FINAL)
    (START) a 2
    (START) b 3
    (START) c 4
    (START) d 4
    2 b 3
    3 c 4
    3 d 4
    4 -| (FINAL)

The factors (subwords) of a language can be obtained by using finding
the prefixes of the suffixes: `R1 :suff :pref`. For large sets this is
preferable to the equivalent but much more expensive `R1 :pref :suff`.

Complement
----------

Since the alphabet of interest is inferred from the context, the
complement must be defined with respect to a specific universe. Thus
there are two basic forms `R1 :acomp` and `R1 :comp`. The first of these
is equivalent to `(R1 :alph)* - R1` and the second is equivalent to
`(SIGMA :alph)* - R1` where `SIGMA` contains the reference alphabet.

`b a* :acomp;`\
or `SIGMA = {a,b}; b a* :comp;`\
or `{a,b}* - b a*;`\
or `(^ | a | b a* b) {a,b}*;`

    (START) -| (FINAL)
    (START) a 2
    (START) b 3
    2 -| (FINAL)
    2 a 2
    2 b 2
    3 a 3
    3 b 2

Stretching
----------

An operation that is sometimes useful involves replacing each letter in
the word by a sequence of $k$ of that letter. This can be done using the
`$` operator followed by a string of $k$ zeros.

`'abc' $ 0 0;`\
or `'aabbcc';`

    (START) a 2
    2 a 3
    3 b 4
    4 b 5
    5 c 6
    6 c 7
    7 -| (FINAL)

Priorities of Operators
-----------------------

Within a pair of matching parentheses or braces the expression is
computed in the following order:

1.  Kleene $+$, Kleene $*$ and Optional operators are evaluated from
    left to right on their single argument.

2.  Concatenations

3.  One pair of matching left and right quotients are performed if they
    exist.

4.  Set intersection and set difference operations are performed from
    left to right.

5.  Alternation (set union), symmetric difference, and shuffle
    operations are performed from left to right.

6.  One stretch operation

7.  The colon unary operations concatenation power, :alph, :rev, :pref,
    and :suff are performed left to right.

8.  The alternation implied by commas within braces is performed last.

Note that the colon unary operations will often require surrounding
parentheses in a larger expression.

Note that parentheses or braces can be used to perform grouping when the
priorities of operators would imply an incorrect interpretation. Use of
parentheses is usually recommended unless a set constructor is needed
for clarity. However, note that commas within parentheses have another
meaning to be discussed in a later section. Another form of grouping is
performed by string quotes. Thus `'ab'*` is equivalent to `(a b)*` and
not to `a b*`.
