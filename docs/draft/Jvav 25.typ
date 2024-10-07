#set text(font: "SimSun", lang: "zh")
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
  编程语言Jvav标准工作草案
])

#table(
  columns: 2,
  stroke: none
)[#text(12pt)[日期:]][#text(12pt)[2024-8-20]
][#text(12pt)[回复:]][#text(12pt)[heckerpowered (#text(12pt, user-color)[heckerpowered\@icloud.com])]]

#align(bottom)[
  这是一份草案，有很多不完整和错误的地方
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

= 基础 <baisc>
== 序言 <basic.preamble>

_#text(user-color)[实体]_ 是 _#text(user-color)[值]_, _#text(user-color)[对象]_, _#text(user-color)[函数]_, _#text(user-color)[类成员]_。

名字是#text(user-color)[标识符]。

名字由声明引入，可能是：
- #text(user-color)[函数声明]
- #text(user-color)[参数声明]
- #text(user-color)[变量声明]

== 编译单元

编译单元是最小可被编译的代码单元。

在编译单元中，最外层的作用域被称为#text(user-color)[全局作用域]。

#pagebreak()

= 作用域 <scope>
== 序言 <scope.preamble>

最外围的作用域被称为#text(user-color)[全局作用域]，由其他方式引入的作用域称为#text(user-color)[全局作用域]。

使用大括号将语句包裹起来，可创建一个局部作用域。

= 表达式 <expression>
== 序言 <expression.preamble>

#text(user-color)[表达式]是#text(user-color)[运算符]与#text(user-color)[操作数]的序列，它指定一项计算。

#text(user-color)[表达式]的求值可以产生一个结果，比如 2 + 2 的求值产生结果 4，也可能产生副作用。

表达式具有两种性质：#text(user-color)[类型]和#text(user-color)[值类别]。

表达式的类型是其结果的类型。

== 值类别 <expression.value-category>

表达式有3种值类别，是：左值、亡值、纯右值。

=== 左值 <expression.lvalue>

左值是求值可确定某个对象的标识的表达式。

下列表达式是左值表达式：
- 由任何变量的名字构成的表达式，不论类型。
- 返回类型是引用的函数调用表达式
- 到引用类型的转换
- 所有内建的赋值表达式
- 前置自增或自减表达式
- 内建的下标表达式a[n]，当a是左值时。
- 成员表达式a.m，其中a是左值，m是非静态成员变量的名字。
- 三元条件表达式 a ? b : c，其中b和c是同类型的左值表达式
- 字符串字面量

==== 性质 

- 可以取地址
- 可以用于初始化引用

=== 亡值 <expression.xvalue>

亡值是代表它的资源能够被重新使用的对象或位域的泛左值

#pagebreak()

= 声明 <declaration>
== 序言 <declaration.preamble>
== 说明符 <declaration.specifier>

=== 访问加宽说明符 <declaration.specifier.widen>

访问加宽说明符是widen，只能在类型内部出现，例如成员函数的访问限制说明符中。

访问加宽说明符修饰成员函数时，允许通过不可变的对象调用此成员函数，但当通过可变对象调用此成员函数时，此说明符也允许成员函数修改此对象。