%option 8bit nodefault yylineno

/**************
include section
**************/
%{
#include "common_parse.hpp"
#include "parse_cycle_yacc.hpp"
 static char *current_file_name;
 static int line_num;
 static bool line_sep;
%}

nl              (\r\n|\r|\n)
ws              [ \t\r]+
number		[0-9]+

%%

{nl} {line_num++;}
{ws} ;
","[^0-9]*         {return FL_COMA;}
{number}    {yy3lval.val=atoi(yy3text);return FL_NUMBER;}
[^","\n]+     {yy3lval.name=strdup(yy3text);return FL_NAME;}
[^","\n]+","[^0-9 ][^","\n]+     {yy3lval.name=strdup(yy3text);return FL_NAME;}


.    {fprintf(stderr,"ERROR: parse issue (%c) line %d\n", *yy3text, yy3lineno); exit(1);}
%%

/************************************************************
Functions body : Only for parsing purpose
*************************************************************/

int parse_cycle(char *file_name) {
  int status;
  FILE *file;
  line_sep=false;
  current_file_name = (char *) malloc(strlen(file_name)+20);
  sprintf(current_file_name,"%s",file_name);
  file=fopen(current_file_name,"r");
  line_num=1;
  yyin = file;
  if (file==NULL) {
    if(warn_level) fprintf(stderr,"WARNING: unable to process file %s\n",current_file_name);
    return(0);
  }
  yy3restart(yyin);
  status=yy3parse();
  fclose(file);
  return status;
}

/* in case of mistakes in the parser*/
int yy3error(char *s) {
  fprintf(stderr,"ERROR: %s reading '%s' in file %s at line %d\n", s,yy3text,current_file_name,line_num);
  exit(1);
}

int yy3wrap (){
  return 1;
}
