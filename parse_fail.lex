%option 8bit nodefault yylineno

/**************
include section
**************/
%{
#include "common_parse.hpp"
#include "parse_fail_yacc.hpp"
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


%x IGNORE
%x IGNORE_LINE
%%

<IGNORE>[^\n=]*         ;
<IGNORE>[^\n=]*{nl}     {++line_num;}
<IGNORE>[^\n=]*{line_separator}+[^\n=]    ;
<IGNORE>[^\n=]*{line_separator}{line_separator}   {line_sep=true; BEGIN(INITIAL);}

<IGNORE_LINE>[^\n]* ;
<IGNORE_LINE>[^\n]*{nl}     {++line_num;BEGIN(INITIAL);}

{nl} {line_num++;}
{ws} ;
{line_separator} 	{if (!line_sep)BEGIN(IGNORE); else  BEGIN(IGNORE_LINE);};

{stbegin}{string} {yy4lval.name=strdup(yy4text);return FL_NAME;}

.    ;



%%

/************************************************************
Functions body : Only for parsing purpose
*************************************************************/

int parse_fail(char *file_name) {
  int status;
  FILE *file;
  line_sep=false;
  current_file_name = (char *) malloc(strlen(file_name)+20);
  sprintf(current_file_name,"%s",file_name);
  file=fopen(current_file_name,"r");
  line_num=1;
  yyin = file;
  if (file==NULL) {
    //fprintf(stderr,"WARNING: unable to process file %s\n",current_file_name);
    return 0;
  }
  //printf("Begin Parsing %s .....\n",current_file_name);
  yy4restart(yyin);
  status=yy4parse();
  fclose(file);
  //printf("Finish Parsing %s ..... lines \n",current_file_name);
  return status;
}

/* in case of mistakes in the parser*/
int yy4error(char *s) {
  fprintf(stderr,"ERROR: %s reading '%s' in file %s at line %d\n", s,yy4text,current_file_name,line_num);
  exit(1);
}

int yy4wrap (){
  return 1;
}
