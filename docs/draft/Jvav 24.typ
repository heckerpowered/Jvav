#set heading(numbering: "1.")

#let keyword-color = eastern
#let since-color = green
#let user-color = blue
#let bracket-color = orange

#align(center, text(17pt)[
  Jvav 24 Release
])

#align(center, [
  _Released July 2024 _
])

Jvav 24 is the first major Jvav Release, supports a few syntaxes and features. The versions of Jvav are named by year, e.g. a Jvav version released in 2024 should be called Jvav 24.

This is an early release of Jvav, many if the details are have yet to be finalized, so this is just an introduction to the syntax

= Compilation unit <compilation-unit>

In general, a Jvav source file corresponds to a _compilation unit_, and a _compilation unit_ contains several members.

Members are:
- Function declaration #emph(text(since-color)[(since Jvav 24)])
- Global statement #emph(text(since-color)[(since Jvav 24)])

_Global statement_ are any valid statement #emph(text(since-color)[(since Jvav 24)])

= Scope <scope>

== General
In a Jvav source file, the outermost scope are called _global scope_. When we declare a function, the scope within the function is called _local scope_.

A _local scope_ can be created by surrounding statements with curly bracket syntax.

[Example 
` 
Complation unit #1:

...

// global scope
{ // local scope
  ...
}
`
-- _end example_]

== Block scope <scope.block>

= Expressions <expression>

== Preamble <expression.preamble>

== Primary expressions <expression.primary>

== Compound expressions <expression.compound>

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
  #emph(text(keyword-color)[if]) #emph(text(bracket-color)[(])_#sub[optional]_ #emph(text(user-color)[condition])#emph(text(bracket-color)[)])_#sub[optional]_ { #emph(text(user-color)[statement-true]) }
][(1)
][
    #emph(text(keyword-color)[if]) #emph(text(bracket-color)[(])_#sub[optional]_ #emph(text(user-color)[condition])#emph(text(bracket-color)[)])_#sub[optional]_ { #emph(text(user-color)[statement-true]) } #emph(text(keyword-color)[else]) { #emph(text(user-color)[statement-false]) }
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

The parentheses of the wrapping condition can be empty, e.g.

#emph(text(keyword-color)[if]) #emph(text(user-color)[condition]) {
  
#h(0.5cm) #emph(text(user-color)[statement-true])

}

== Iteration statements <statement.iteration>

=== General <statement.iteration.general>

=== While statement <statement.iteration.while>

=== Do-while statement <statement.iteration.do-while>

=== For statement <statement.iteration.for>

== Jump statements <statement.jump>

=== General <statement.jump.general>

=== Break statement <statement.jump.break>

=== Continue statement <statement.jump.continue>

=== Return statement <statement.jump.return>

= Declarations <declaration>

== Preamble <declaration.preamble>

A declaration is a statement (@statement)

== Variable declaration <declaration.variable>

A variable declaration is a statement that introduces and optionally initialize one identifiers.

#table(
  columns: 2,
  stroke: none,
  align: (left, right)
)[#emph(text(keyword-color)[let]) #emph(text(user-color)[variable-name])_:_ #emph(text(user-color)[type])_#sub[optional]_ = #emph(text(user-color)[initializer])][(1)
][#emph(text(keyword-color)[var]) #emph(text(user-color)[variable-name])_:_ #emph(text(user-color)[type])_#sub[optional]_ = #emph(text(user-color)[initializer])][(1)]

#table(
  columns: 3,
  stroke: none,
  align: (right, center, left)
)[_variable-name_][\-][the name of the variable, any valid identifier
][_type_][\-][possibly empty, the type of the variable
][_initializer_][\-][the initial value of the variable, any valid expression]

1. Declare a variable with the type and the initializer, the value of the variable is mutable.
2. Declare a constant with the type and the initializer, which the value cannot be changed after declaration.

If the _type clause_ is empty, then the type of the variable is deduced from the initializer.