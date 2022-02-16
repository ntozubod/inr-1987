# Unicode Support in INR

This is new functionality added to version 2.1.0c in Feb 2022 to support
the use of Unicode processing in UTF-8 format.
At the present time the focus is on Unicode as expressed as a sequence
of octets encoded as UTF-8 and no conversion to code point numbers or
to other transfer formats is provided.
This may be provided at a later time but there are lots of solutions to
these types of problems.

As said above, the focus is on UTF-8 for now.
This is quite suitable for Linux and various similar operating system
environments.
Another encoding that might be useful to consider is UTF-16 and planning
is currently keeping this in mind without directly implementing anything.

Most other encodings currently in use are reasonably close in spirit to one
of these two.
The only exception that stands out is GB 18030 that might require more
work to deal with properly.

Briefly, UTF-8 encoding represents Unicode code points as a series of one
to four octets.
It is a variable length coding that encodes all 7-bit ASCII as itself in
one octet (formerly called a byte) and larger code points appropriately
using multiple continuation octets as needed.
It satisfies a synchronization property that allow text matching for any
valid UTF-8 string query against a valid UTF-8 text by matching only octets.
There can be no misaligned matches.

UTF-16 also has this property which is very convenient for applications
such as INR might provide with the caveat that alignment much be done at
a 16-bit short word level.
Misalignments can occur if octet level matching is done.

Thus INR works with the octet sequence provided as input or describe in
the INR program text.
For now, three strategies have been partially implemented:

1. There are only 256 separate octet values and this is a feasible, though
large alphabet for INR processing.
Some issues are easy to imagine if a transducer (2 or more tapes) has a lot
of states that need to remember an input in order to immediately write the
same value.
Since, many applications are like this, there will be other solutions (in
particular, the second one).

2. An obvious solution to the state explosion just mentioned is to split
each 8-bit octet into two 4-bit hex digits.
These have traditionally been called nibbles.
Such an alphabet has 16 values but has a problem that it fails the
above synchronization property at the nibble level.
One way to fix this is to distinguish the nibble that represents the
high order part of the octet from that representing the low order part.
The alphabet now has 32 values.

3. Another approach considers that for any application only a small number
of code points may actually be used.
Any input can be split into tokens, each of which is a valid UTF-8 octet
sequence corresponding to a single code point.
The alphabet size is potentially 1,114,112 (the number of valid code points)
although in practice the size is expected to be  much smaller.

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

More preloading is done to handle high and low order nibble values:
indexes 258 to 273 represent the 16 possible high order nibble values and
indexes 274 to 289 represent the 16 possible low order nibble values.
Their string representation of high nibbles are a hex digit preceded by
`[`.
Low nibbles are represented by a hex digit followed by `]`.

For approach number 3, no preloading is done.
The internal index is assigned as each character is encountered.
The external form is the UTF-8 sequence in its string form.
Note that combining forms are treated as separate tokens.

## Changes to Lex.c:

Assign distinct print strings in symbol table to each code point.

Modify `''` strings to admit all octets in the input (from only printable).
Modify `` ` ` `` tokens to admit all octets in the input (from only printable).
Create new `""` strings to map octets in the input to a pair of nibbles: high
and low.

## Changes to Aload.c:

`:lwds` will admit all octets as does Lex.c

## New code in Aunicode.c:

Some new routines in Aunicode.c have been added to INR to make it easier
to process text in UTF-8 format.

`'abc';`

    (START) a 2
    2 b 3
    3 c 4
    4 -| (FINAL)

as before

`"abc";`

    (START) [6 2
    2 1] 3
    3 [6 4
    4 2] 5
    5 [6 6
    6 3] 7
    7 -| (FINAL)

with new high / low nibbles

`` `abc`;``

    (START) abc 2
    2 -| (FINAL)

as before

``( `a` `b` `c` );``

    (START) abc 2
    2 -| (FINAL)

as before

`"ἀπὸ";`

    (START) [E 2
    2 1] 3
    3 [B 4
    4 C] 5
    5 [8 6
    6 0] 7
    7 [C 8
    8 F] 9
    9 [8 10
    10 0] 11
    11 [E 12
    12 1] 13
    13 [B 14
    14 D] 15
    15 [B 16
    16 8] 17
    17 -| (FINAL)

a little longer

`'ἀπὸ';`

    (START) E1 2
    2 BC 3
    3 80 4
    4 CF 5
    5 80 6
    6 E1 7
    7 BD 8
    8 B8 9
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

`:slurp_octets`

`:spit_octets`

`:gen_min`

`:slurp_nibbles`

`:spit_nibbles`

`:slurp_utf8`

`:spit_utf8`

## Sample code

Samples contains a programming task using the nibble based approach.
See [md_eqn_nibbles](../../samples/md_eqn_nibbles)
