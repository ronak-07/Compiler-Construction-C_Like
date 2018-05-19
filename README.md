
Compiler-Construction-C_Like
===============================================
Implementing a compiler for a miniature programming language.

Phase-1 : Scanner Implementation
--------------------------------------------
A DFA-based Lexical Analyser that recognizes some of the basic lexemes. A driver program(parser) that calls your Scanner repeatedly, returning each token found by the Scanner in the input stream.

- Requirements Specification:

1.Input

2.Output: Return tokens either in the form of some number or as TK-identifier

3.Side Effects: White spaces removed

4.Exceptions: Invalid tokens

Our language reserves all the key words that can appear in the language.


Phase-2 : Parser Construction
----------------------------------------
- Requirements Specification:

1.Input: Lexer generated tokens

2.Output: Parse Tree in the form of any tree traversal or level wise output of the nonterminal from left to right. In case of errors parser report the errors and continue parsing.

3.Files
- Interface file : Parser.h (Currently named as Parser.c. Make sure you rename it to Parser.h)
- Implementation : Parser_1.c


Phase-3 : Three Address code Generation
-------------------------------------------------------
In essence we will write a C program and translate it into low-level three-address code using quadruples.
