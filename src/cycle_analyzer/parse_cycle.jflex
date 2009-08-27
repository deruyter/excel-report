package cycle_analyzer;

import java_cup.runtime.*;

%%
%public 
%class Cycle_Lexer
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


nl      = \r|\n|\r\n;
ws      = [ \t\f]+
number	= [0-9]+

%%

{nl} 							{ /* ignore */ }
{ws} 							{ /* ignore */ }
","[^0-9]*         				{return symbol(sym.FL_COMA);}
{number}    					{return symbol(sym.FL_NUMBER,new Long(yytext()));}
[^","\n]+     					{return symbol(sym.FL_NAME,yytext());}
[^","\n]+","[^0-9 ][^","\n]+    {return symbol(sym.FL_NAME,yytext());}

.    {error("Illegal character: "+ yytext());}
