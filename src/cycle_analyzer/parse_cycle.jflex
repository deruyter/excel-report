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
  
  private void debug(String Token) {
  	if (false) {
     	System.out.println("Token detected is "+ Token +" at line "+(yyline+1)+", column "+(yycolumn+1)+" : "+ yytext());
    }
  }
  
  private void error(String message) {
     System.out.println("Error at line "+(yyline+1)+", column "+(yycolumn+1)+" : "+message);
     System.out.println("Text is: " + yytext());
     System.exit(1);
  }
%}


%xstate RESULTNAME

nl      = \r|\n|\r\n;
ws      = [ \t\f]+
number	= [0-9]+
stbegin = [a-zA-Z/\.]
string  = [a-zA-Z/0-9_\.\-@\+]*


%%

<RESULTNAME>{ws}				{ /* ignore */ }
<RESULTNAME>{stbegin}{string}   {debug("FL_NAME 3"); return symbol(sym.FL_NAME,yytext());}
<RESULTNAME>"-----"   			{yybegin(YYINITIAL); return symbol(sym.FL_RESULT_NAME_END);}



{nl} 										{ /* ignore */ }
{ws} 										{ /* ignore */ }
","[^0-9]*         							{debug("FL_COMA"); return symbol(sym.FL_COMA);}
{number}    								{debug("FL_NUMBER"); return symbol(sym.FL_NUMBER,new Long(yytext()));}
"----- Results in"              			{debug("FL_RESULT_NAME_BEGIN"); yybegin(RESULTNAME); return symbol(sym.FL_RESULT_NAME_BEGIN);}
{stbegin}[^","\n]+     						{debug("FL_NAME 1"); return symbol(sym.FL_NAME,yytext());}
{stbegin}[^","\n]+","[^0-9 ][^","\n]+    	{debug("FL_NAME 2"); return symbol(sym.FL_NAME,yytext());}




.    {error("Illegal character: "+ yytext());}
