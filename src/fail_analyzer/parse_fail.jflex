package fail_analyzer;

import java_cup.runtime.*;

%%
%public 
%class Fail_Lexer
%unicode
%cup
%line
%column
%{
  private boolean line_sep=false;

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
l_sep   = [=]+
stbegin = [a-zA-Z/]
string  = [a-zA-Z/-@\-0-9_\.\+]*


%xstate IGNORE
%xstate IGNORE_LINE
%%

<IGNORE>[^\n=]*        			{ /* ignore */ }
<IGNORE>[^\n=]*{nl}    			{ /* ignore */ }
<IGNORE>[^\n=]*{l_sep}+[^\n=]   { /* ignore */ }
<IGNORE>[^\n=]*{l_sep}{l_sep} 	{line_sep=true; yybegin(YYINITIAL);}

<IGNORE_LINE>[^\n]* 			{ /* ignore */ }
<IGNORE_LINE>[^\n]*{nl}     	{yybegin(YYINITIAL);}

{nl} 							{ /* ignore */ }
{ws} 							{ /* ignore */ }
{l_sep} 						{if (!line_sep) yybegin(IGNORE); else  yybegin(IGNORE_LINE);}

{stbegin}{string} 				{return symbol(sym.FL_NAME,yytext());}

.    							{ /* ignore */ }

