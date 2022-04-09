# Unicode Support in INR

## Introduction

New functionality has been added to INR to allow for the full repertoire
of Unicode to be available for use in tokens.
Although INR itself assigns no particular meanings assigned to the actual
strings used for tokens, there is an obvious convention that assigns the
token `a` to refer to the lower case form of the first letter of the alphabet
as in 7-bit ASCII.

Note that several concepts are being mashed together into one with the
possibility of confusion:

(1) the abstract symbol *a* used to represent a specific letter in an
otherwise unspecified alphabet.
This is the usage that occurs in mathematics where *a* might mean a
unknown but specific constant that is different from other symbols such
as *b*, *c*, etc.
This is very common in texts on Formal Language Theory and Automata Theory.

(2) the programming equivalent where *a* is a label for something that can
be interpreted specific in a later context.
The downstream application of any automaton will give meaning to *a*.
Note that *a* may mean real-life actions such as 'apply the brakes', or
'save the file', or 'fire the rocket'.
INR will not know or care what this meaning is.

(3) the abstract idea of the letter *a* as used in the written for of
natural languages such as English, French, Swahili, Maori, etc.
This is still abstract as there is no specific bit pattern assigned to it
and it might appear very different in its various usages.
NB: This is what Unicode identifies using a *code point*, a non-negative
integer value that shows up in documentation as, for example, U+0061 but
really is only the 97th assigned character.

(4) the representation `a` (for code point U+0061) as it occurs in any
particular encoding.
In ASCII this is represented as the hex number 0x61, and it usually stored
in an 8-bit *byte* on modern computers.
NB: Unicode uses the term *octet* to specifically refer to an 8-bit quantity
but usually avoids this discussion except when talking about representations.
One such representation is called UTF-8, a variable length encoding that
assigns between 1 and 4 octets to each code point.
The letter *a* is represented as one octet containing the value 0x61.
INR will use only UTF-8 encoding for the present time.

(5) There is ONE more subtlety that occurs because of accents and other
kinds of annotations.
Here our example will shift to the *character* that we think of *a acute*,
that is, the letter *a* with a mark above it that looks like a short line
leaning to the right.
The reason for this discussion is that this is also an abstraction that
has conceptual and representation issues and leads to the need for some
supporting technologies.
Unicode calls this a *grapheme* that is made up of two *glyph*s.
One glyph represents the glyph *a* (U+0061) and the second represents the
acute accent (U+0301).
Thus *a acute* in Unicode requires two code points.
In UTF-8, this is encoded into a sequence of three octets ( 0x61 0xCC 0x81 ).

These five different views of what is meant by the letter *a* represent
why adding Unicode functionality to INR is subtle and difficult.
The goal has been to preserve the feel of INR.
Things that seem to be easy should still be easy.
Things that worked before should still work.

INR basically works at the level of (1) above with a little of (2).
However, (3) and (4) are used in very common application domains of text
processing and should be supported enough that they still appear natural
without breaking the (1) and (2) usages.
Level (5) usage involves support for more from the Unicode standard in order
to feel natural and will be post-poned

## Aside for masochists

Actually, I lied (a little).
The letter *a acute* in Unicode has a single code point ( U+00E1 ) and
can be represented in UTF-8 using a sequence of two octets ( 0xC3 0xA1 ).
This is correct and represents an engineering compromize made in the
early days of Unicode.
Because *a acute* occurred in a number of legacy encodings at the time
of the creation of Unicode, this meaning was preserved.
There are two distinct ways of representing *a acute* and in all real-world
senses of the term are the same character / grapheme.
This can create problems.
INR is not going to address this at the present but it is important to be
aware of:

`` `\x61\xCC\x81`; ``

    (START) á 2
    2 -| (FINAL)

`` `\xC3\xA1`; ``

    (START) á 2
    2 -| (FINAL)

`` `\x61\xCC\x81` ! `\xC3\xA1`; ``

    (START) á 2
    (START) á 2
    2 -| (FINAL)

A feature that will be added to INR at some future time under the name of
Normalization / Denormalization.
That is what it is referred to in the Unicode documentaton.

## Current

At the present time, the focus is on Unicode as expressed as sequences
of octets encoded as UTF-8 with no conversion to code point numbers or
to other transfer formats.
These may be provided at a later time.

UTF-8 is quite suitable for Linux and various similar operating system
environments.
Nonetheless, UTF-16 support should be possible in the future.
Most other encodings in use are reasonably close to one of these two
although GB 18030 might require more work.

Briefly, UTF-8 encoding represents Unicode code points as a series of one
to four octets.
It is a variable length coding that encodes all 7-bit ASCII as itself in
one octet (formerly called a byte) and larger code points appropriately
using multiple continuation octets as needed.
It satisfies a synchronization property that allow text matching for any
valid UTF-8 string query against a valid UTF-8 text by matching only octets.
There can be no misaligned matches.

UTF-16 also has this property as long as alignment on 16-bit short word
is preserved.

Thus, INR works with octet sequences.
For now, two strategies can be imagined:

1. There are only 256 separate octet values providing a feasible, though
large alphabet for INR processing.
Large alphabets may contribute to state explosion in multi-tape
transducers since information read can only be stored in the finite
control (i.e., the state) if different actions need to be distinguished.
Whether this is too large an alphabet will, of course, depend on the
application.

2. Yet another approach considers that for any application only a small number
of code points may actually be used.
Any input can be split into tokens, each of which is a valid UTF-8 octet
sequence corresponding to a single code point.
Although the alphabet size is potentially 1,114,112 (the number of valid
code points), the size is expected to be much smaller in many cases.

## Internal considerations

INR internally represents each token by an integer greater than or equal
to zero.
In previous versions of INR only values of 0 and 1 were reserved for the
empty word and endmarkers respectively although all of the printable
ASCII characters (plus tab and newline) were preloaded so that they would
have an integer ordering corresponding to their character ordering in
the ASCII character set.

Each of these characters has an external representation that is used for
printing.
These were internally the actual single character as C string although
on input and output `'\_'` was used for blank, `'\t'` was used for tab, 
`'\n'` was used for newline, and `'\\'` was used for backslash. 

In version 2.1.0c, this preloading of the token table is extended to all
octets.
Thus indexes 2 to 257 represent the 256 possible octet values ordered by
their character values.
All of the printable characters have string value as before and
unprintable characters are represented by a two digit hexadecimal number
as a C string of length 2.

For approach number 2, no preloading is done.
The internal index is assigned as each character is encountered.
The external form is the UTF-8 sequence in its string form.
Note that combining forms are treated as separate tokens.

## Changes to Lex.c:

Assign distinct print strings in symbol table to each code point.

Modify `''` strings to admit all octets in the input (from only printable).
Modify `` ` ` `` tokens to admit all octets in the input (from only printable).

Some examples should clarify all of the above discussion:

`'abc';`

    (START) a 2
    2 b 3
    3 c 4
    4 -| (FINAL)

as before

`` `abc`;``

    (START) abc 2
    2 -| (FINAL)

as before

``( `a` `b` `c` );``

    (START) a 2
    2 b 3
    3 c 4
    4 -| (FINAL)

as before

`'ἀπὸ';`

    (START) \xE1 2
    2 \xBC 3
    3 \x80 4
    4 \xCF 5
    5 \x80 6
    6 \xE1 7
    7 \xBD 8
    8 \xB8 9
    9 -| (FINAL)

octet form

`` `ἀπὸ`;``

    (START) ἀπὸ 2
    2 -| (FINAL)

unsurprising


``( `ἀ` `π` `ὸ` );``

    (START) ἀ 2
    2 π 3
    3 ὸ 4
    4 -| (FINAL)

broken apart

## Changes to Aload.c:

`:readwords` will admit all octets as does Lex.c.
Only approach 1 is currently supported.

## New code in Aunicode.c:

Some new routines in Aunicode.c have been added to INR to make it easier
to process text in UTF-8 format.

### `:slurp_octets`, `:spit_octets`

`INP = :slurp_octets c.md;`

This is a new function that allows the full content of a file to be read
in in one chunk and converted to a DFA that recognizes exactly one string,
that is, the string of octets.
In this example `c.md` is the name of the file and INP is an INR variable
that will contain the resulting DFA.

`OUT :spit_octets trans.md;`

This is new function that takes a DFA stored in the INR variable OUT and
writes the sequence of octets traced out by the path from the start state
to the final state.
It is the inverse of :slurp_octets.
In this example the output goes to the file `trans.md`.

Note that the DFA should match exactly one string.
At present, this function has undefined behaviour if more that one string
occurs.

### `:slurp_utf8`, `:spit_utf8`

Again these two routines repeat the pattern, except that the input is
parsed into UTF-8 sequences and these are looked up or added to the
token table as they are seen.
The alphabet used is detected in the process of reading the file.

The function `:alph` can be used to extract this alphabet from this input
if it is needed.

### `:gen_min`

All of the above spit functions have undefined behaviour if their input
automaton represents more than one string.
An easy way of selecting a unique string to be written is using the new
`:gen_min` fuction.

`OUT = WRK :gen_min;` will conceptually do the following:

1. Calculate the length of the shortest string recognized by WRK.

2. Produce an automaton that recognizes all of the strings of this shortest
length.

3. Select the string that comes first in lexicographic order (alphabetic
order).
This is the output and is represented as a DFA recognizing exactly one
string.

For a lexicographic order to work, the alphabet must be ordered.
For this purpose, the integer value stored in the token symbol table is
used.
This will have the effect that octet automata will have a natural binary
ordering.
Because of a nice property of UTF-8 encoding, this will also correspond to
code point order (not true of UTF-16 by the way).

For the third approach the order will depend on when the code points were
first encountered, and the result might seem less predictable.

## Sample code

Samples contains a programming task using the octet based approach.
See [md_eqn](../../samples/md_eqn)
