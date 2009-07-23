%option 8bit nodefault yylineno

/**************
include section
**************/
%{
#include "common_parse.hpp"
#include "parse_func_yacc.hpp"
 static char *current_file_name;
 static int line_num;
 static bool line_sep;
%}


nl              (\r\n|\r|\n)
ws              [ \t]+
number			[0-9]+
stbegin         [a-zA-Z/\._]
string          [a-zA-Z/0-9_\.\-@]*


%%

{nl} 						{line_num++; return FL_NEW_LINE;}
{ws} ;
";"                         return FL_SEPARATE;

{number}  {yy5lval.val=atoi(yy5text);return FL_NUMBER;}
-{number} {yy5lval.val=atoi(yy5text);return FL_NUMBER;}

{stbegin}{string} {yy5lval.name=strdup(yy5text);return FL_NAME;}


.    {fprintf(stderr,"ERROR: parse issue (%c) line %d\n", *yy5text, yy5lineno); exit(1);}



%%

/************************************************************
Functions body : Only for parsing purpose
*************************************************************/

int parse_funcsize(char *file_name) {
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
  //printf("Begin Parsing %s .....\n",current_file_name);
  yy5restart(yyin);
  status=yy5parse();
  fclose(file);
  //printf("Finish Parsing %s ..... lines \n",current_file_name);
  return status;
}

/* in case of mistakes in the parser*/
int yy5error(char *s) {
  fprintf(stderr,"ERROR: %s reading '%s' in file %s at line %d\n", s,yy5text,current_file_name,line_num);
  exit(1);
}

int yy5wrap (){
  return 1;
}
