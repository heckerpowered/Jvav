#set text(font: "PT Sans")
#set heading(numbering: "1.")
#set page(footer: context [
  #h(1fr)
  #counter(page).display(
    "1"
  )
])

#let keyword-color = eastern
#let since-color = green
#let user-color = blue
#let bracket-color = orange

#align(center, text(24pt)[
  Working Draft, Standard for Programming Language Jvav
])

#table(
  columns: 2,
  stroke: none
)[#text(12pt)[Date:]][#text(12pt)[2024-8-20]
][#text(12pt)[Reply-to:]][#text(12pt)[heckerpowered (#text(12pt, user-color)[heckerpowered\@icloud.com])]]

#align(bottom)[
  This is an early release of Jvav, many if the details are have yet to be finalized, so this is just an introduction to the syntax
]

#pagebreak()

#show outline.entry.where(
  level: 1
): it => {
  v(12pt, weak: true)
  strong(it)
}
#outline(
  indent: auto
)

#pagebreak()

= Basics <basics>
== Preamble <basics.preamble>

An _#text(user-color)[entity]_ is a _#text(user-color)[value]_, _#text(user-color)[object]_, _#text(user-color)[function]_, _#text(user-color)[class member]_.

A name is a _#text(user-color)[identifier]_.

Every name is introduced by a *declaration*, which is a
- _#text(user-color)[function-declaration]_ (#text(user-color)[@declaration.function])
- _#text(user-color)[parameter-declaration]_ (#text(user-color)[@declaration.function.parameter])
- _#text(user-color)[variable-declaration]_ (#text(user-color)[@declaration.variable])

== Compilation unit <basics.compilation-unit>

A compilation unit is the smallest unit of code that can be compiled individually.

Each compilation unit has a _#text(user-color)[global scope]_, which contains the entire compilation unit.

== Phases of compilation

Jvav source files are processed by the compiler to produce Jvav programs.

=== Lexical analysis

Parsing a Jvav source file into a collection of tokens using a lexical analyzer (a.k.a. lexer).

==== Tokens

Token is the smallest meaningful element in a compilation unit

Tokens are:
- _#text(user-color)[identifiers]_
- _#text(user-color)[keywords]_
- _#text(user-color)[literals]_
- _#text(user-color)[operators and punctuators]_

If any part of the source code cannot be parsed into any of the above tokens, then the part is considered to be an invalid character and the program is ill-formed.

=== Syntactic Analysis

Syntactic analysis is also known as parser. The collection of tokens is not yet semantic.Parser parses the collection of tokens into meaningful combinations. They are usually expressions or statements.

#table(
  columns: 4,
  stroke: none,
  align: (right, center, left, right)
)[Assignment expression syntax][-][_#text(user-color)[identifier]_ _#text(user-color)[equal]_ _#text(user-color)[expression]_
][(1)][Binary expression syntax][-][_#text(user-color)[expression]_ _#text(user-color)[operator-token]_][(2)
][Block statement syntax][-][_#text(user-color)[open-brace-token]_ _#text(user-color)[statements]_ _#text(user-color)[close-brace-token]_][(3)
][Break statement syntax][-][_#text(user-color)[break-keyword]_][(4)
][Continue statement syntax][-][_#text(user-color)[continue-keyword]_][(5)
][Call expression syntax][-][_#text(user-color)[identifier]_ _#text(user-color)[open-parenthesis-token]_ _#text(user-color)[argument-list]_ _#text(user-color)[close-parenthesis-token]_][(6)
][Do-while statement syntax][-][_#text(user-color)[do-keyword]_ _#text(user-color)[statements]_ _#text(user-color)[while-keyword]_ _#text(user-color)[expression]_][(7)
][Else clause syntax][-][_#text(user-color)[else-keyword]_ _#text(user-color)[else-statements]_][(8)
][Expression statement syntax][-][_#text(user-color)[expression]_][(9)
][For statement syntax][-][_#text(user-color)[for-keyword]_ _#text(user-color)[open-parenthesis-token]_ _#text(user-color)[init-statement]_ _#text(user-color)[colon-token]_ _#text(user-color)[condition-expression]_ _#text(user-color)[colon-token]_ _#text(user-color)[expression]_ _#text(user-color)[close-parenthesis-token]_][(10)
][Function declaration syntax][-][_#text(user-color)[function-keyword]_ _#text(user-color)[identifier]_ _#text(user-color)[open-parenthesis-token]_ _#text(user-color)[parameter-list]_ _#text(user-color)[close-parenthesis-token]_ _#text(user-color)[type-clause]_ _#text(user-color)[statements]_][(11)
][Name expression][-][_#text(user-color)[identifier]_][(12)
][Parameter syntax][-][_#text(user-color)[identifier]_ _#text(user-color)[type-clause]_][(13)
][Parenthesized expression syntax][-][_#text(user-color)[open-parenthesis-token]_ _#text(user-color)[expression]_ _#text(user-color)[close-parenthesis-token]_][(14)
][Return statement syntax][-][_#text(user-color)[return-keyword]_][(15)
][Type clause syntax][-][_#text(user-color)[colon-token]_ _#text(user-color)[identifier]_][(16)
][Unary expression syntax][-][_#text(user-color)[operator-token]_ _#text(user-color)[expression]_][(17)
][Variable declaration syntax][-][_#text(user-color)[keyword]_ _#text(user-color)[identifier]_ _#text(user-color)[type-clause]#sub[optional]_ _#text(user-color)[condition-expression]_ _#text(user-color)[equals-token]_ _#text(user-color)[initializer]_][(18)
][While statement syntax][-][_#text(user-color)[while-keyword]_ _#text(user-color)[expression]_ _#text(user-color)[statements]_][(19)]

#pagebreak()

= Scope <scope>

== General
In a Jvav source file, the outermost scope are called _global scope_. When we declare a function, the scope within the function is called _local scope_.

A _local scope_ can be created by surrounding statements with curly bracket syntax.

== Block scope <scope.block>

The following syntaxes introduces a _block scope_ that includes statements:
- selection or iteration statements (@statement.selection, @statement.iteration)
- compound statement

= Expressions <expression>
== Preamble <expression.preamble>

An expression is a sequence of operators and operands that specifies a computation. An expression can result in a value and can cause side effects.

== Primary expressions <expression.primary>

Primary expression are:
- literal expression
- name expression
- (expression)

=== Literals <expression.primary.literal>

There are several kinds of literals:
- number literal
- string literal
- boolean literal

==== Number literal <expression.primary.literal.number>

===== Kinds of number literal <expression.primary.literal.number.kinds>
- #emph(text(user-color)[binary-literal])
- #emph(text(user-color)[octal-literal])
- #emph(text(user-color)[decimal-literal])
- #emph(text(user-color)[hexadecimal-literal])

For number literals other than decimal, theyall have specific prefixes.

===== Binary literal <expression.primary.literal.number.binary>

#emph(text(bracket-color)[0b]) #emph(text(user-color)[binary-digit])

#emph(text(bracket-color)[0B]) #emph(text(user-color)[binary-digit])

binary digit is one of:

#emph(text(bracket-color)[0]) #emph(text(bracket-color)[1])

===== Octal literal <expression.primary.literal.number.octal>

#emph(text(bracket-color)[0]) #emph(text(user-color)[octal-digit])

octal digit is on of:

#emph(text(bracket-color)[0]) #emph(text(bracket-color)[1]) #emph(text(bracket-color)[2]) #emph(text(bracket-color)[3]) #emph(text(bracket-color)[4]) #emph(text(bracket-color)[5]) #emph(text(bracket-color)[6]) #emph(text(bracket-color)[7])

===== Decimal literal <expression.primary.literal.number.decimal>

Numbers start with non-zero digit

decimal digit is on of:

#emph(text(bracket-color)[0]) #emph(text(bracket-color)[1]) #emph(text(bracket-color)[2]) #emph(text(bracket-color)[3]) #emph(text(bracket-color)[4]) #emph(text(bracket-color)[5]) #emph(text(bracket-color)[6]) #emph(text(bracket-color)[7]) #emph(text(bracket-color)[8]) #emph(text(bracket-color)[9])  

===== Hexadecimal literal <expression.primary.literal.number.hexadecimal>

#emph(text(bracket-color)[0x]) #emph(text(user-color)[hexadecimal-digit])

#emph(text(bracket-color)[0X]) #emph(text(user-color)[hexadecimal-digit])

hexadecimal digit is one of:

#emph(text(bracket-color)[0]) #emph(text(bracket-color)[1]) #emph(text(bracket-color)[2]) #emph(text(bracket-color)[3]) #emph(text(bracket-color)[4]) #emph(text(bracket-color)[5]) #emph(text(bracket-color)[6]) #emph(text(bracket-color)[7]) #emph(text(bracket-color)[8]) #emph(text(bracket-color)[9])  

#emph(text(bracket-color)[a]) #emph(text(bracket-color)[b]) #emph(text(bracket-color)[c]) #emph(text(bracket-color)[d]) #emph(text(bracket-color)[e]) #emph(text(bracket-color)[f])

#emph(text(bracket-color)[A]) #emph(text(bracket-color)[B]) #emph(text(bracket-color)[C]) #emph(text(bracket-color)[D]) #emph(text(bracket-color)[E]) #emph(text(bracket-color)[F])

==== String literal <expression.primary.literal.string>

#emph(text(bracket-color)["]) #emph(text(user-color)[character-sequence])_#sub[optional]_  #emph(text(bracket-color)["])

If any line break character appears in _#text(user-color)[character-sequence]_, the string is unterminated, the compiler reports diagnostics.

The backslash(\\) is _escape character_, when a backslash is detected in source file, the subsequent character is included into the _#text(user-color)[character-sequence]_, the backslash character and the meaning of the subsequent character is ignored. For example, the quote(") symbol represents the end of _#text(user-color)[character-sequence]_ which would not appear in the _#text(user-color)[character-sequence]_, but a backslash before it adds it to the _#text(user-color)[character-sequence]_.

==== Boolean literal <expression.primary.literal.boolean>

The Boolean literals are the keywords false and true. Such literals have type bool.

=== Parenthesized expression <expression.primary.parenthesized>

A parenthesized expression (_E_) is a primary expression whose type and result are identical to those of _E_. The parenthesized expression can be used in exactly the same contexts as those where E can be used, and with the same meaning, except as otherwise indicated.

=== Name expression <expression.primary.name>

A name expression holds an identifier that refers to function, variable or a constant.

== Compound expressions <expression.compound>

=== Postfix expressions <expression.compound.postfix>

==== General 

Postfix expressions group left-to-right.

==== Call expression

A call expression is a postfix expression followed by parentheses containing a possibly empty, comma-separated list of expressions which constitute the arguments to the function.

Recursive calls are permitted.

= Statements <statement>

Statements are executed in sequence except where noted elsewhere. 

Statements are starts with a keyword, otherwise it is a expression statement. #emph(text(since-color)[(since Jvav 24)])

== Block statement

A _block statement_ (also known as a compound) groups a sequence of statements into a single statement.

#h(0.5cm) _compound-statement_:

#h(1cm) #emph(text(bracket-color)[{]) #emph(text(user-color)[statement-sequence])#sub[optional] #emph(text(bracket-color)[}])

#h(0.5cm) _statement-sequence_:

#h(1cm) #emph(text(user-color)[statement])

#h(1cm) #emph(text(user-color)[statement-sequence]) #emph(text(user-color)[statement])

A block statement defines a block scope (@scope.block).

== Selection statements <statement.selection>
=== If statement <statement.selection.if>

If statement executes statements conditionally, if the condition yields #emph(text(bracket-color)[true]) the first sub-statement is executed. If the #emph(text(blue)[else]) part is present and the condition yields #emph(text(bracket-color)[false]), the second sub-statement is executed.


#table(
  columns: 2,
  stroke: none,
  align: (left, right)
)[
    _#text(keyword-color)[if] #text(user-color)[condition] { #text(user-color)[statement-true] }_
][(1)
][
    _#text(keyword-color)[if] #text(user-color)[condition] { #text(user-color)[statement-true] } #text(keyword-color)[else] { #text(user-color)[statement-false] }_
][(2)]

1. #emph(text(keyword-color)[If]) statement without an #emph(text(keyword-color)[else]) branch
2. #emph(text(keyword-color)[If]) statement with an #emph(text(keyword-color)[else]) branch

#table(
  columns: 3,
  stroke: none,
  align: (right, center, left)
)[_condition_][-][a #emph(text(user-color)[expression]) which will yield a value of type #emph(text(keyword-color)[bool])
][_statement-true_][-][the #emph(text(user-color)[statement]) to be executed if the #emph(text(user-color)[condition]) yields #emph(text(bracket-color)[true])
][_statement-false_][-][the #emph(text(user-color)[statement]) to be executed if the #emph(text(user-color)[condition]) yields #emph(text(bracket-color)[false])
]

== Iteration statements <statement.iteration>
=== General <statement.iteration.general>

Iteration statements specify looping, and have following syntax:

_#text(keyword-color)[while] #text(user-color)[condition]  { #text(user-color)[statement] }_

_#text(keyword-color)[do] { #text(user-color)[statement] } #text(keyword-color)[while] #text(user-color)[expression]_

_#text(keyword-color)[for] #text(user-color)[init-statement]#sub[optional] _;_ #text(user-color)[condition]_;_ #text(user-color)[expression] { #text(user-color)[statement] }_ 

The sub-statement in an _#text(user-color)[iteration-statement]_ defines a block scope which is entered and exited each time through the loop.

=== While statement <statement.iteration.while>

While statement executes the sub-statement repeatedly until the value of _#text(user-color)[condition]_ becomes _#text(bracket-color)[false]_. The expression of the _#text(user-color)[condition]_ is evaluated before each execution of the sub-statement.

=== Do-while statement <statement.iteration.do-while>

Do-while statement executes the sub-statement unconditionally first, then executes the sub-statement repeatedly until the value of _#text(user-color)[condition]_ becomes _#text(bracket-color)[false]_. The expression of the _#text(user-color)[condition]_ is evaluated before each execution of the sub-statement except the first time.

=== For statement <statement.iteration.for>

For statement executes the sub-statement repeatedly until the value of _#text(user-color)[condition]_ becomes _#text(bracket-color)[false]_, while the statement does not need to manage the loop condition.

The _#text(user-color)[init-statement]_ is executed before the first execution of sub-statement, the _#text(user-color)[expression]_ is evaluated after each execution of the sub-statement.

== Jump statements <statement.jump>
=== General <statement.jump.general>

Jump statements unconditionally transfer control, has following syntax:

_#text(keyword-color)[break]_

_#text(keyword-color)[continue]_

_#text(keyword-color)[return] #text(user-color)[expression]_

=== Break statement <statement.jump.break>

A _#text(keyword-color)[break]_ statement shall be enclosed by _#text(user-color)[iteration-statement]_. The _#text(keyword-color)[break]_ statement causes the termination of the _#text(user-color)[iteration-statement]_; control passes to the statement following the terminated statement, if any.

=== Continue statement <statement.jump.continue>

A _#text(keyword-color)[continue]_ statement shall be enclosed by _#text(user-color)[iteration-statement]_. The _#text(keyword-color)[continue]_ statement causes the termination of current loop and immediately starts the next loop if any.

=== Return statement <statement.jump.return>

A function returns control to its caller by the return statement.

The _#text(user-color)[expression]_ of _#text(keyword-color)[return]_ statement is called its operand. A _#text(keyword-color)[return]_ statement with no operand shall be used only in a function whose has no return type. The type of the operand must match the type of the function's return type.

= Declarations <declaration>

== Preamble <declaration.preamble>

A declaration is a statement (@statement)

== Type clause

_: #text(user-color)[identifier]_

Type clause specifies the type of the declaration, such as the variable's type and the function's return type, and can usually be empty.

== Variable declaration <declaration.variable>

A variable declaration is a statement that introduces and optionally initialize one identifiers.

#table(
  columns: 2,
  stroke: none,
  align: (left, right)
)[#emph(text(keyword-color)[let]) #emph(text(user-color)[variable-name]) #emph(text(user-color)[type-clause])_#sub[optional]_ = #emph(text(user-color)[initializer])][(1)
][#emph(text(keyword-color)[var]) #emph(text(user-color)[variable-name]) #emph(text(user-color)[type-clause])_#sub[optional]_ = #emph(text(user-color)[initializer])][(1)]

#table(
  columns: 3,
  stroke: none,
  align: (right, center, left)
)[_#text(user-color)[variable-name]_][\-][the name of the variable, any valid identifier
][_#text(user-color)[type-clause]_][\-][possibly empty, the type of the variable
][_#text(user-color)[initializer]_][\-][the initial value of the variable, any valid expression]

1. Declare a variable with the type and the initializer, the value of the variable is mutable.
2. Declare a constant with the type and the initializer, which the value cannot be changed after declaration.

If the _#text(user-color)[type-clause]_ is empty, then the type of the variable is deduced from the initializer.

== Function declaration <declaration.function>

A function declaration declares a function in current scope and associates the function's name, parameters and return type.

#table(
  columns: 2,
  stroke: none,
  align: (left, right)
)[_#text(keyword-color)[fun] #text(user-color)[function-name] #text(bracket-color)[(] #text(user-color)[parameter-list] #text(bracket-color)[)] { #text(user-color)[statement] }_][(1)
][_#text(keyword-color)[fun] #text(user-color)[function-name] #text(bracket-color)[(] #text(user-color)[parameter-list] #text(bracket-color)[)] #text(user-color)[type-clause]#sub[optional] { #text(user-color)[statement] }_][(2)]

#table(
  columns: 3,
  stroke: none,
  align: (right, center, left)
)[_#text(user-color)[function-name]_][-][the name of the function, any valid identifier
][_#text(user-color)[parameter-list]_][-][a list of parameter syntax
][_#text(user-color)[type-clause]_][-][possibly empty, the return type of the function]

1. Declare a function with no _#text(user-color)[type-clause]_, which means no return value.
2. Declare a function with a _#text(user-color)[type-clause]_.

The parameters of the function are in the same scope of the _#text(user-color)[statement]_  

=== Parameter syntax <declaration.function.parameter>

Parameter syntax declares a parameter of a function.

_#text(user-color)[parameter-name] _:_ #text(user-color)[type-clause]_
