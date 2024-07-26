# Table of Contents
1. [Notation](#1-notation)
2. [Lexical Structure](#2-lexical-structure)
    1. [Keywords](#21-keywords)
    2. [Identifiers](#22-identifiers)
    3. [Comments](#23-comments)
    4. [Tokens](#24-tokens)
3. [Documents](#3-documents)
    1. [Imports](#31-imports)
4. [Modules](#4-modules)
    1. [Package](#41-package)
    2. [Aliases](#42-aliases)
    3. [Variables](#43-variables)
    4. [Functions](#44-functions)
    5. [Enumerations](#45-enumerations)
    6. [Objects](#46-objects)
    7. [Extends](#47-extensions)
5. [Statements](#5-statements)
6. [Expressions](#6-expressions)
    1. [Literals](#61-literal-expression)
    2. [Paths](#62-path-expressions)
    3. [Operators](#63-operator-expressions)
    4. [Grouped](#64-grouped-expressions)
    5. [Arrays](#65-array-expressions)
    6. [Indexes](#66-index-expressions)
    7. [Function calls](#67-function-call-expressions)
    8. [Method calls](#68-method-call-expressions)
    9. [Field Access](#69-field-access-expressions)
    10. [Returns](#610-return-expressions)
    11. [Blocks](#611-block-expressions)
    12. [Loops](#612-loop-expressions)
    13. [If-Then](#613-if-then-expressions)
7. [Names](#7-names)
    1. [Paths](#71-paths)
8. [Types](#8-types)
    1. [Pointers](#81-pointer-types)
    2. [Arrays](#82-array-types)

# Introduction
# **1.** Notation
# **2.** Lexical Structure
## **2.1** Keywords
## **2.2** Identifiers
## **2.3** Comments
## **2.4** Tokens
# **3.** Documents
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Document:</i>
    <i>Includes</i><sup>?</sup> <i>Modules</i><sup>?</sup>

<i>Includes:</i>
    <i><a href="#31-imports">Import</a></i>
  | <i><a href="#31-imports">Import</a></i> <i>Includes</i><sup>?</sup>

<i>Modules:</i>
    <i><a href="#4-modules">Module</a></i>
  | <i><a href="#4-modules">Module</a></i> <i>Modules</i><sup>?</sup>
</pre>

# **3.1** Imports
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Import:</i>
    <b><code>using</code></b> <i><a href="#simple-paths">SimplePath</a></i> <b><code>;</code></b>
</pre>

# **4.** Modules
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Module:</i>
    <i><a href="#41-package">Package</a></i>
  | <i><a href="#42-aliases">Alias</a></i>
  | <i><a href="#43-variables">Variable</a></i>
  | <i><a href="#44-functions">Function</a></i>
  | <i><a href="#45-enumerations">Enumeration</a></i>
  | <i><a href="#46-objects">Object</a></i>
  | <i><a href="#47-extensions">Extension</a></i>
</pre>

A module is simply a piece of a translation unit. [Documents](#3-documents) support multiple modules to exist within itself.

## **4.1** Package
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Package:</i>
    <b><code>module</code></b> <i><a href="#simple-paths">SimplePath</a></i> <b><code>;</code></b>
</pre>

## **4.2** Aliases
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Alias:</i>
    <b><code>alias</code></b> <a href="#22-identifiers">IDENTIFIER</a> <b><code>=</code></b> <i><a href="#8-types">Type</a></i> <b><code>;</code></b>
</pre>

## **4.3** Variables
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Variable:</i>
    <i>VarQualifier</i> <a href="#22-identifiers">IDENTIFIER</a> <b><code>:</code></b> <i><a href="#8-types">Type</a></i> <b><code>=</code></b> <i><a href="#6-expressions">Expression</a></i> <b><code>;</code></b>

<i>VarQualifier:</i>
    <b><code>var</code></b> | <b><code>const</code></b> | <b><code>static</code></b>
</pre>

## **4.4** Functions
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Function:</i>
    <b><code>def</code></b> <a href="#22-identifiers">IDENTIFIER</a> <i><a href="#48-generics">GenericParams</a></i><sup>?</sup> <i><a href="#where-clause">WhereClause</a></i><sup>?</sup><b><code>(</code></b><i>FunctionParameters</i><sup>?</sup><b><code>)</code></b> (<b><code>:</code></b><i><a href="#8-types">Type</a></i>)<sup>?</sup> (<i><a href="">BlockExpression</a></i> | <b><code>;</code></b>)

<i>FunctionParameters:</i>
    <i>FunctionParam</i> (<b><code>,</code></b><i>FunctionParam</i>)<sup>*</sup> <b><code>,</code></b><sup>?</sup>

<i>FunctionParam:</i>
    <a href="#22-identifiers">IDENTIFIER</a><b><code>:</code></b><i><a href="#8-types">Type</a></i>
</pre>

## **4.5** Enumerations
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Enumeration:</i>
    <b><code>enum</code></b> <a href="#22-identifiers">IDENTIFIER</a> <i><a href="#48-generics">GenericParams</a></i><sup>?</sup> <i><a href="#where-clause">WhereClause</a></i><sup>?</sup> <b><code>{</code></b><i>EnumEntries</i><sup>?</sup><b><code>}</code></b>

<i>EnumEntries:</i>
    <i>EnumValue</i> (<b><code>,</code></b><i>EnumValue</i>)<sup>*</sup> <b><code>,</code></b><sup>?</sup>

<i>EnumValue:</i>
    <a href="#22-identifiers">IDENTIFIER</a> (<b><code>=</code></b><i><a href="#6-expressions">Expression</a></i>)<sup>?</sup>
</pre>

## **4.6** Objects
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Object:</i>
    <b><code>object</code></b> <a href="#22-identifiers">IDENTIFIER</a> <i><a href="#48-generics">GenericParams</a></i><sup>?</sup> <i><a href="#where-clause">WhereClause</a></i><sup>?</sup> <b><code>{</code></b><i>ObjectFields</i><sup>?</sup><b><code>}</code></b>

<i>ObjectFields:</i>
    <i><a href="#42-aliases">Alias</a></i>
  | <i><a href="#43-variables">Variable</a></i>
  | <i><a href="#44-functions">Function</a></i>
  | <i><a href="#45-enumerations">Enumeration</a></i>
  | <i><a href="#46-objects">Object</a></i>
</pre>

Objects are effectively classes/structs in other langauges.

## **4.7** Extensions
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Extension:</i>
    <b><code>extend</code></b> <a href="#22-identifiers">IDENTIFIER</a> <i><a href="#48-generics">GenericParams</a></i><sup>?</sup> <i><a href="#where-clause">WhereClause</a></i><sup>?</sup> <b><code>{</code></b><i>ExtendFields</i><sup>?</sup><b><code>}</code></b>

<i>ExtendFields:</i>
    <i><a href="#42-aliases">Alias</a></i>
  | <i><a href="#44-functions">Function</a></i>
  | <i><a href="#45-enumerations">Enumeration</a></i>
  | <i><a href="#46-objects">Object</a></i>
</pre>

Extensions are a mechanism to reopen an object and add new [aliases](#42-aliases), [functions](#44-functions), [enumerations](#45-enumerations), and [objects](#46-objects).

## **4.8** Generics
<pre style="line-height:1.0">
<b>Syntax</b>
<i>GenericParams:</i>
    <b><code>&lt;</b></code><b><code>&gt;</b></code>
  | <b><code>&lt;</b></code>(<i>GenericParam</i><b><code>,</code></b>)<sup>*</sup> <i>GenericParam</i><b><code>,</code></b><sup>?</sup><b><code>&gt;</b></code>

<i>GenericParam:</i>
    // Please define me
</pre>

### Where clause
<pre style="line-height:1.0">
<b>Syntax</b>
<i>WhereClause:</i>
    // Please define me
</pre>

# **5.** Statements
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Statement:</i>
    <b><code>;</code></b>
  | <i><a href="#43-variables">Variable</a></i>
  | <i><a href="#6-expressions">Expression</a></i>
</pre>

# **6.** Expressions
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Expression:</i>
    <i>ExpressionWithoutBlock</i>
  | <i>ExpressionWithBlock</i>

<i>ExpressionWithoutBlock:</i>
    <i><a href="">LiteralExpression</a></i>
  | <i><a href="">PathExpression</a></i>
  | <i><a href="">OperatorExpression</a></i>
  | <i><a href="">GroupedExpression</a></i>
  | <i><a href="">ArrayExpression</a></i>
  | <i><a href="">IndexExpression</a></i>
  | <i><a href="">FunctionCallExpression</a></i>
  | <i><a href="">MethodCallExpression</a></i>
  | <i><a href="">FieldAccessExpression</a></i>
  | <i><a href="">NextExpression</a></i>
  | <i><a href="">BreakExpression</a></i>
  | <i><a href="">ReturnExpression</a></i>

<i>ExpressionWithBlock:</i>
    <i><a href="">BlockExpression</a></i>
  | <i><a href="">LoopExpression</a></i>
  | <i><a href="">IfThenExpression</a></i>
</pre>

## **6.1** Literal Expression
## **6.2** Path Expressions
## **6.3** Operator Expressions
## **6.4** Grouped Expressions
## **6.5** Array Expressions
## **6.6** Index Expressions
## **6.7** Function Call Expressions
## **6.8** Method Call Expressions
## **6.9** Field Access Expressions
## **6.10** Return Expressions
## **6.11** Block Expressions
## **6.12** Loop Expressions
## **6.13** If-Then Expressions
# **7.** Names
## **7.1** Paths
A *path* is a sequence of one or more segments separated by an access qualifier (`.`). If a path consists of only one segment, it refers to a [variable](#43-variables) in a local scope. If a path has multiple segments, it refers to either an item in a [module](#4-modules) or an item in an [object](#46-objects). This does share similarities with [method call expressions](#68-method-call-expressions) and [field access expressions](#69-field-access-expressions), but they are not the same.

### Simple paths
<pre style="line-height:1.0">
<b>Syntax</b>
<i>SimplePath:</i>
    <a href="#22-identifiers">IDENTIFIER</a> (<b><code>.</code></b><a href="#22-identifiers">IDENTIFIER</a>)<sup>*</sup>
</pre>

### Paths in expresssions
<pre style="line-height:1.0">
<b>Syntax</b>
<i>PathExpression:</i>
    <i>PathExprSegment</i> (<b><code>.</code></b><i>PathExprSegment</i>)<sup>*</sup>

<i>PathExprSegment:</i>
    <i>PathIdentSegment</i> <i>GenericArgs</i><sup>?</sup>

<i>PathIdentSegment:</i>
    <a href="#22-identifiers">IDENTIFIER</a> | <b><code>super</code></b> | <b><code>this</code></b>

<i>GenericArgs:</i>
    <b><code>&lt;</b></code><b><code>&gt;</b></code>
  | <b><code>&lt;</b></code>(<i><a href="#8-types">Type</a></i><b><code>,</code></b>)<sup>*</sup> <i><a href="#8-types">Type</a></i><b><code>,</code></b><sup>?</sup><b><code>&gt;</b></code>
</pre>

### Paths in types
<pre style="line-height:1.0">
<b>Syntax</b>
<i>TypePath:</i>
    <i>TypePathSegment</i> (<b><code>.</code></b><i>TypePathSegment</i>)<sup>*</sup>

<i>TypePathSegment:</i>
    <i>PathIdentSegment</i> (<b><code>.</code></b><sup>?</sup> <i>TypePathFn</i>)<sup>?</sup>

<i>TypePathFn:</i>
    <b><code>(</code></b><i>TypePathFnInputs</i><sup>?</sup><b><code>)</code></b> (<b><code>:</code></b><i><a href="#8-types">Type</a></i>)<sup>?</sup>

<i>TypePathFnInputs:</i>
    <i><a href="#8-types">Type</a></i> (<b><code>,</code></b><i><a href="#8-types">Type</a></i>)<sup>*</sup> <b><code>,</code></b><sup>?</sup>
</pre>

# **8.** Types
<pre style="line-height:1.0">
<b>Syntax</b>
<i>Type:</i>
    <i><a href="">TypePath</a></i>
  | <i><a href="">PointerType</a></i>
  | <i><a href="">ReferenceType</a></i>
  | <i><a href="">ArrayType</a></i>
</pre>

## **8.1** Pointer types
### Reference types
### Raw pointer types

## **8.2** Array types