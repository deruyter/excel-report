%option 8bit nodefault yylineno

/**************
include section
**************/
%{
#include "common_parse.hpp"
#include "parse_info_yacc.hpp"
 static char *current_file_name;
 static int line_num;
 static bool line_sep;
%}


nl              (\r\n|\r|\n)
ws              [ \t]+
number		[0-9]+
line_separator  [=]+
stbegin         [a-zA-Z/]
string          [a-zA-Z/-@\-0-9_\.]*


%x CPVERSION
%x LNAME
%x IGNORE
%x IGNORE_LINE
%x VERSIONVAL
%x COMPFLAGS
%%

<IGNORE>[^\n]*         ;
<IGNORE>[^\n]*{nl}     {BEGIN(INITIAL);yyterminate();}

<IGNORE_LINE>[^\n]* ;
<IGNORE_LINE>[^\n]*{nl}     {++line_num;BEGIN(INITIAL);}

<LNAME>[^\n]*     {yy1lval.name=strdup(yy1text);return FL_NAME;}
<LNAME>{nl}     {++line_num;BEGIN(INITIAL);}

<VERSIONVAL>[^ ]*" "       {yy1lval.name=strdup(yy1text);BEGIN(CPVERSION);return FL_NAME;}

<CPVERSION>{ws} ;
<CPVERSION>"HPC" ;
<CPVERSION>"STMicroelectronics" ;
<CPVERSION>"Compilers: Version"{ws}           	BEGIN(VERSIONVAL);
<CPVERSION>"SX C compiler release"{ws}        	BEGIN(VERSIONVAL);
<CPVERSION>{number}"/"{number}"/"{number}    	{yy1lval.name=strdup(yy1text);return FL_DATE;}
<CPVERSION>{number}                          	{yy1lval.name=strdup(yy1text);return FL_DATE;}
<CPVERSION>"Unknown"						  	{yy1lval.name=strdup("00000000");return FL_DATE;}
<CPVERSION>"["{number}"/"{number}"/"{number}"]" {yy1lval.name=strdup(yy1text);BEGIN(IGNORE_LINE);return FL_DATE;}
<CPVERSION>"("                 BEGIN(IGNORE_LINE);

<COMPFLAGS>{nl}   {++line_num;BEGIN(INITIAL);}
<COMPFLAGS>[^ \n]*  {yy1lval.name=strdup(yy1text);  return FL_OPTION;}
<COMPFLAGS>{ws}   ;


{nl} {line_num++;}
{ws} ;
{line_separator} 	{if (line_sep) BEGIN(IGNORE); else line_sep=true;};
"Validation"{ws}":"    	return FL_VALID;
"SX Toolset"{ws}":"    	{BEGIN(LNAME);return FL_TOOLSET;}
"stxp70cc"{ws}":"      	{BEGIN(CPVERSION);return FL_VERSION;}
"sxcc"{ws}":"      	{BEGIN(CPVERSION);return FL_VERSION;}
"simu version"{ws}":"  	{BEGIN(LNAME);return FL_SIMU;}
"runner flags"{ws}":"  	return FL_SIM_FLAGS;
"extra flags"{ws}":"  	BEGIN(IGNORE_LINE);
"flags"{ws}":"         	{BEGIN(COMPFLAGS);return FL_COMP_FLAGS;}
"double mode"{ws}":"   	return FL_MODE;
"logdir"{ws}":"        	return FL_LOGDIR;
"tests"{ws}":"         	return FL_TESTS;
"short double"         	{yy1lval.name=strdup(yy1text);return FL_NAME;}


"|"|":"|"| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" ;


"Mon"|"Tue"|"Wed"|"Thu"|"Fri"|"Sat"|"Sun" return FL_DAY;

"Jan"  {yy1lval.val=1; return FL_MONTH;}
"Feb"  {yy1lval.val=2; return FL_MONTH;}
"Mar"  {yy1lval.val=3; return FL_MONTH;}
"Apr"  {yy1lval.val=4; return FL_MONTH;}
"May"  {yy1lval.val=5; return FL_MONTH;}
"Jun"  {yy1lval.val=6; return FL_MONTH;}
"Jul"  {yy1lval.val=7; return FL_MONTH;}
"Aug"  {yy1lval.val=8; return FL_MONTH;}
"Sep"  {yy1lval.val=9; return FL_MONTH;}
"Oct"  {yy1lval.val=10;return FL_MONTH;}
"Nov"  {yy1lval.val=11;return FL_MONTH;}
"Dec"  {yy1lval.val=12;return FL_MONTH;}

{number}  {yy1lval.val=atoi(yy1text);return FL_NUMBER;}

"("{string}")" {yy1lval.name=strdup(yy1text);return FL_MACHINE_NAME;}

{stbegin}{string} {yy1lval.name=strdup(yy1text);return FL_NAME;}

"-"{string}  {yy1lval.name=strdup(yy1text); return FL_OPTION;}

.    {fprintf(stderr,"ERROR: parse issue (%c) line %d in file %s\n", *yy1text, yy1lineno,current_file_name); exit(1);}



%%

/************************************************************
Functions body : Only for parsing purpose
*************************************************************/

int parse_info(char *file_name) {
  int status;
  FILE *file;
  line_sep=false;
  current_file_name = (char *) malloc(strlen(file_name)+20);
  sprintf(current_file_name,"%s",file_name);
  file=fopen(current_file_name,"r");
  line_num=1;
  yyin = file;
  if (file==NULL) {
    fprintf(stderr,"ERROR: unable to process file %s\n",current_file_name);
    exit(1);
  }
  //printf("Begin Parsing %s .....\n",current_file_name);
  yy1restart(yyin);
  status=yy1parse();
  fclose(file);
  //printf("Finish Parsing %s ..... lines \n",current_file_name);
  return status;
}

/* in case of mistakes in the parser*/
int yy1error(char *s) {
  fprintf(stderr,"ERROR: %s reading '%s' in file %s at line %d\n", s,yy1text,current_file_name,line_num);
  exit(1);
}

int yy1wrap (){
  return 1;
}
