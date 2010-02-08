package func_analyzer;

import java_cup.runtime.*;

%%
%public 
%class Func_Lexer
%unicode
%cup
%line
%column
%{
  private Symbol symbol(int sym) {
    return new Symbol(sym, yyline+1, yycolumn+1);
  }
  
  private Symbol symbol(int sym, Object val) {
    return new Symbol(sym, yyline+1, yycolumn+1, val);
  }
  
  private void error(String message) {
     System.out.println("Error at line "+(yyline+1)+", column "+(yycolumn+1)+" : "+message);
     System.exit(1);
  }
%}

newline = \r|\n|\r\n;
ws      = [ \t\f]+
number	= [0-9]+
stbegin = [a-zA-Z_/\.]
string  = [a-zA-Z/0-9_\.\-@\+]*

%%

{newline} 					{return symbol(sym.FL_NEW_LINE);}
{ws}                        { /* ignore */ }
";"                         {return symbol(sym.FL_SEPARATE);}

{number}  					{return symbol(sym.FL_NUMBER,new Integer(yytext()));}
-{number} 					{return symbol(sym.FL_NUMBER,new Integer(yytext()));}

{stbegin}{string} 			{return symbol(sym.FL_NAME,yytext());}

.    {error("Illegal character: "+ yytext());}

