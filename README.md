# Compiler

The program was written in C++ using Visual Studio and is best executed in the Visual Studio environment.  
The program requires the manual input of two file-paths – one for the input file that is to be read,  
and one for the output file in which token/lexeme and production rules are printed.  
Please enter the file paths in lines 33 and 34 in Main.cpp.

Program consists of two parts, the lexer and the parser. The main method reads the input file,  
removes blank spaces and comments, and feeds the “cleaned” file to the lexer.  
The lexer then identifies lexemes and tokens (and invalid tokens) and feeds each lexeme/token pair to the parser.  
The parser then checks the production rules to see if the current structure of the program is grammatically correct.  
If no error is detected, the program parses until the end of the file. If an error was detected,  
the program breaks off lexing and/or parsing and prints the line number in  
which the error occurred as well as an expected output or production rule. 
