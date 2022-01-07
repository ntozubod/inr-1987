find the minimized automaton over the alphabet $\{a,b\}$ that recognizes
words that contain at least one "$a$" and at least one "$b$". This can
intersection of words containing $a$ and words containing $b$:
$$\{a,b\}^*a\{a,b\}^* \wedge \{a,b\}^*b\{a,b\}^*.$$ This expression can
indicates a transition from state `3` to state `2` which reads an $a$.
alphabet $\{a,b\}^*$ that are not either all $a$'s or all $b$'s. This
$$\{a,b\}^* - ( \{a\}^* \cup \{b\}^* ).$$ This can be indicated to INR
>     ! " $ % & ( ) * + , - / : ; < = > ? @ [ \ ] ^ { | } ~
`a` $\equiv$ `{a}` $\equiv$ `{'a'}`.
vague similariy to the Greek letter $\Lambda$ that is often used for
representation of $\not c$. Alternative representations is `()` (empty
The concatenation closure operators (Kleene $+$ and Kleene $*$) are
To express the concatenation of a set with itself exactly $k$ times the
expression may be followed by a `:` and the value $k$. Thus `R1 :2` is
To express the concatenation of a set with itself $k$ or fewer times the
notation `R1? :`$k$ can be used. To express the concatenation of a set
with itself more than $k$ times the set difference can be used:
`R1* - (R1? :`$k$`)`.
`'abc'` $\cdot$ `'def'` $=$ `'abcdef'` we can define a right quotient
operator $/$ as `'abcdef'` / `'def'` $=$ `'abc'` and a left quotient
operator $\backslash$ as `'abc'` $\backslash$ `'abcdef'` $=$ `'def'`.
`!` operator. Thus $R_1 ! R_2 \equiv (R_1-R_2) \cup (R_2-R_1)$.
the symbol `!!` which mildly suggests the symbol $\amalg$ that is often
the word by a sequence of $k$ of that letter. This can be done using the
`$` operator followed by a string of $k$ zeros.
`'abc' $ 0 0;`\
1.  Kleene $+$, Kleene $*$ and Optional operators are evaluated from
0 or tape 1. Thus $a$ will occur in the forms $a_0$ and $a_1$. We can
The `$` operator can be used to reorganize the tape structure of a
projected by INR using the notation `R1 $ 0` and the range by the
notation `R1 $ 1`.
`(a,b)* $ 0;`\
`(a,b)* $ 1;`\
ternary transduction `R2 $ (0,2)` will select tapes 0 and 2 renumbering
`(a,b,c)* $ (0,2);`\
The `$` operator can also be used to cause a renumbering of the tapes.
the operator `$ (1,0)`.
`(a,b)* $ (1,0);`\
`(a,b,c)* $ (2,0);`\
A third use the `$` operator is stretching. A simple example of this was
`(a,b)* $ (0 0,1 1 1);`\
`a b* $ (0,0);`\
For example, if `$ (0,0)` is used often, a variable containing the value
`a b* $ [0] 0;`\
or `'bb' @ ((a,b)* $ (1,0));`\
The $k$-fold composition of a binary rational relation is denoted by
`:(`$k$`)`. That is, the number is written in parentheses.
domain. This operator is denoted by the symbol `||`. Thus if $f$ and $g$
are functions then $f$ `||` $g$ will agree with $f$ whenever the
argument is in $dom f$. Otherwise it will agree with $g$. Note that
`R || S` is simply a shorter form of `R | ( (S$0)-(R$0) @@ S )`


New line:
<img src="https://render.githubusercontent.com/render/math?math=x_{1,2} = \frac{-b \pm \sqrt{b^2-4ac}}{2b}">
to
<img src="https://render.githubusercontent.com/render/math?math=\{a,b\}^*a\{a,b\}^* \wedge \{a,b\}^*b\{a,b\}^*">
.
