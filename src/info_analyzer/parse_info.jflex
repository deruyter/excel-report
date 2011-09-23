package info_analyzer;

import java_cup.runtime.*;

%%
%public 
%class Info_Lexer
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
line_separator = [=]+
stbegin        = [a-zA-Z/]
string         = [a-zA-Z/-@\-0-9_\.]*


%xstate CPVERSION
%xstate LNAME
%xstate IGNORE_LINE
%xstate VERSIONVAL
%xstate COMPFLAGS

%%

<IGNORE_LINE>[^\n]* 		{ /* ignore */ }
<IGNORE_LINE>[^\n]*{nl}     {yybegin(YYINITIAL);}

<LNAME>[^\n]*     			{return symbol(sym.FL_NAME,yytext());}
<LNAME>{nl}     			{yybegin(YYINITIAL);}

<VERSIONVAL>[^ ]*" "       	{yybegin(CPVERSION);return symbol(sym.FL_NAME,yytext());}

<CPVERSION>{ws} 								{ /* ignore */ }
<CPVERSION>"HPC" 								{ /* ignore */ }
<CPVERSION>"STMicroelectronics" 				{ /* ignore */ }
<CPVERSION>"Compilers: Version"{ws}           	{yybegin(VERSIONVAL);}
<CPVERSION>"SX C compiler release"{ws}        	{yybegin(VERSIONVAL);}
<CPVERSION>{number}"/"{number}"/"{number}    	{return symbol(sym.FL_DATE,yytext());}
<CPVERSION>{number}                          	{return symbol(sym.FL_DATE,yytext());}
<CPVERSION>"Unknown"						  	{return symbol(sym.FL_DATE,"00000000");}
<CPVERSION>"["{number}"/"{number}"/"{number}"]" {yybegin(IGNORE_LINE);return symbol(sym.FL_DATE,yytext());}
<CPVERSION>"("                 					{yybegin(IGNORE_LINE);}
<CPVERSION>.                  					{yybegin(IGNORE_LINE);}

<COMPFLAGS>{nl}   			{yybegin(YYINITIAL );}
<COMPFLAGS>[^ \n]*  		{return symbol(sym.FL_OPTION,yytext());}
<COMPFLAGS>{ws}   			{ /* ignore */ }


{nl} 					{ /* ignore */ }
{ws} 					{ /* ignore */ }
{line_separator} 		{if (line_sep) {yyclose();return symbol(sym.EOF);} else line_sep=true;}
"Validation"{ws}":"    	{return symbol(sym.FL_VALID);}
"SX Toolset"{ws}":"    	{yybegin(LNAME);return symbol(sym.FL_TOOLSET);}
"stxp70cc"{ws}":"      	{yybegin(CPVERSION);return symbol(sym.FL_VERSION);}
"st200cc"{ws}":"      	{yybegin(CPVERSION);return symbol(sym.FL_VERSION);}
"sxcc"{ws}":"      		{yybegin(CPVERSION);return symbol(sym.FL_VERSION);}
"simu version"{ws}":"  	{yybegin(LNAME);return symbol(sym.FL_SIMU);}
"runner flags"{ws}":"  	{return symbol(sym.FL_SIM_FLAGS);}
"extra flags"{ws}":"  	{yybegin(IGNORE_LINE);}
"flags"{ws}":"         	{yybegin(COMPFLAGS);return symbol(sym.FL_COMP_FLAGS);}
"double mode"{ws}":"   	{return symbol(sym.FL_MODE);}
"logdir"{ws}":"        	{return symbol(sym.FL_LOGDIR);}
"tests"{ws}":"         	{return symbol(sym.FL_TESTS);}
"short double"         	{return symbol(sym.FL_NAME,yytext());}
"profiling  activated"{ws}":" { return symbol(sym.FL_PROF);}
"Pass 1 flags"{ws}":" { yybegin(COMPFLAGS);return symbol(sym.FL_COMP_FLAGS);}
"Pass 2 flags"{ws}":" { yybegin(COMPFLAGS);return symbol(sym.FL_COMP_FLAGS);}

"|"|":"|"| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" { /* ignore */ }


"Mon"|"Tue"|"Wed"|"Thu"|"Fri"|"Sat"|"Sun" {return symbol(sym.FL_DAY);}

"Jan"  {return symbol(sym.FL_MONTH,1);}
"Feb"  {return symbol(sym.FL_MONTH,2);}
"Mar"  {return symbol(sym.FL_MONTH,3);}
"Apr"  {return symbol(sym.FL_MONTH,4);}
"May"  {return symbol(sym.FL_MONTH,5);}
"Jun"  {return symbol(sym.FL_MONTH,6);}
"Jul"  {return symbol(sym.FL_MONTH,7);}
"Aug"  {return symbol(sym.FL_MONTH,8);}
"Sep"  {return symbol(sym.FL_MONTH,9);}
"Oct"  {return symbol(sym.FL_MONTH,10);}
"Nov"  {return symbol(sym.FL_MONTH,11);}
"Dec"  {return symbol(sym.FL_MONTH,12);}

{number}  			{return symbol(sym.FL_NUMBER,new Integer(yytext()));}
"("{string}")" 		{return symbol(sym.FL_MACHINE_NAME,yytext());}
{stbegin}{string} 	{return symbol(sym.FL_NAME,yytext());}
"-"{string}  		{return symbol(sym.FL_OPTION,yytext());}
.    				{error("Illegal character: "+ yytext());}

