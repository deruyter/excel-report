%option 8bit nodefault yylineno

/**************
include section
**************/
%{
#include "common_parse.hpp"
#include "parse_size_yacc.hpp"
 static char *current_file_name;
 static int line_num;
 static bool line_sep;
 int disc_set;
%}


nl              (\r\n|\r|\n)
ws              [ \t]+
number		[0-9]+
stbegin         [a-zA-Z/\.]
string          [a-zA-Z/0-9_\.\-@]*


%%

{nl} {line_num++;}
{ws} ;
";"                         return FL_SEPARATE;
","                         return FL_COMA;
".text"                     {yy2lval.sec=TEXT;return FL_SECTION;}
".bss1"                     {yy2lval.sec=BSS;return FL_SECTION;}
".bss2"                     {yy2lval.sec=BSS;return FL_SECTION;}
".bss4"                     {yy2lval.sec=BSS;return FL_SECTION;}
".bss8"                     {yy2lval.sec=BSS;return FL_SECTION;}
".bss"                      {yy2lval.sec=BSS;return FL_SECTION;}
".data"                     {yy2lval.sec=DATA;return FL_SECTION;}
".rodata"                   {yy2lval.sec=RODATA;return FL_SECTION;}
".robase"                   {yy2lval.sec=ROBASE;return FL_SECTION;}
".symtab"                   {yy2lval.sec=SYMTAB;return FL_SECTION;}
".strtab"                   {yy2lval.sec=STRTAB;return FL_SECTION;}
".rela.text"                {yy2lval.sec=RELA_TEXT;return FL_SECTION;}
".rela.data"                {yy2lval.sec=RELA_DATA;return FL_SECTION;}
".rela.rodata"              {yy2lval.sec=RELA_RODATA;return FL_SECTION;}
".rela.debug_frame"         {yy2lval.sec=RELA_DBG_FRAME;return FL_SECTION;}
".STXP70_EXTRECONF_INFO"    {yy2lval.sec=STXP70_EXTRECONF_INFO;return FL_SECTION;}
".STXP70_MEMINFO"           {yy2lval.sec=STXP70_MEMINFO;return FL_SECTION;}
".startup"                  {yy2lval.sec=STARTUP;return FL_SECTION;}
".syscall"                  {yy2lval.sec=SYSCALL;return FL_SECTION;}
".ivtable"                  {yy2lval.sec=IVTABLE;return FL_SECTION;}
".thandlers"                {yy2lval.sec=THANDLERS;return FL_SECTION;}
".ithandlers"               {yy2lval.sec=ITHANDLERS;return FL_SECTION;}
".stack1"                   {yy2lval.sec=STACK1;return FL_SECTION;}
".heap"                     {yy2lval.sec=HEAP;return FL_SECTION;}
".shstrtab"                 {yy2lval.sec=SHSRTAB;return FL_SECTION;}
".STXP70_MEMMAP_INFO"       {yy2lval.sec=STXP70_MEMMAP_INFO;return FL_SECTION;}
".secinit"                  {yy2lval.sec=SECINIT;return FL_SECTION;}
".tda"                      {yy2lval.sec=TDA;return FL_SECTION;}
".da1"                      {yy2lval.sec=DA;return FL_SECTION;}
".sda1"                     {yy2lval.sec=SDA;return FL_SECTION;}
".da2"                      {yy2lval.sec=DA;return FL_SECTION;}
".sda2"                     {yy2lval.sec=SDA;return FL_SECTION;}
".da4"                      {yy2lval.sec=DA;return FL_SECTION;}
".sda4"                     {yy2lval.sec=SDA;return FL_SECTION;}
".da8"                      {yy2lval.sec=DA;return FL_SECTION;}
".sda8"                     {yy2lval.sec=SDA;return FL_SECTION;}

".rela.sda_data1"           {yy2lval.sec=RELA_SDA_DATA;return FL_SECTION;}
".rela.sda_bss1"            {yy2lval.sec=RELA_SDA_BSS;return FL_SECTION;}
".rela.sda_ro1"             {yy2lval.sec=RELA_SDA_RO;return FL_SECTION;}
".rela.sda_data2"           {yy2lval.sec=RELA_SDA_DATA;return FL_SECTION;}
".rela.sda_bss2"            {yy2lval.sec=RELA_SDA_BSS;return FL_SECTION;}
".rela.sda_ro2"             {yy2lval.sec=RELA_SDA_RO;return FL_SECTION;}
".rela.sda_data4"           {yy2lval.sec=RELA_SDA_DATA;return FL_SECTION;}
".rela.sda_bss4"            {yy2lval.sec=RELA_SDA_BSS;return FL_SECTION;}
".rela.sda_ro4"             {yy2lval.sec=RELA_SDA_RO;return FL_SECTION;}
".rela.sda_data8"           {yy2lval.sec=RELA_SDA_DATA;return FL_SECTION;}
".rela.sda_bss8"            {yy2lval.sec=RELA_SDA_BSS;return FL_SECTION;}
".rela.sda_ro8"             {yy2lval.sec=RELA_SDA_RO;return FL_SECTION;}
".rela.da_data1"            {yy2lval.sec=RELA_DA_DATA;return FL_SECTION;}
".rela.da_bss1"             {yy2lval.sec=RELA_DA_BSS;return FL_SECTION;}
".rela.da_ro1"              {yy2lval.sec=RELA_DA_RO;return FL_SECTION;}
".rela.da_data2"            {yy2lval.sec=RELA_DA_DATA;return FL_SECTION;}
".rela.da_bss2"             {yy2lval.sec=RELA_DA_BSS;return FL_SECTION;}
".rela.da_ro2"              {yy2lval.sec=RELA_DA_RO;return FL_SECTION;}
".rela.da_data4"            {yy2lval.sec=RELA_DA_DATA;return FL_SECTION;}
".rela.da_bss4"             {yy2lval.sec=RELA_DA_BSS;return FL_SECTION;}
".rela.da_ro4"              {yy2lval.sec=RELA_DA_RO;return FL_SECTION;}
".rela.da_data8"            {yy2lval.sec=RELA_DA_DATA;return FL_SECTION;}
".rela.da_bss8"             {yy2lval.sec=RELA_DA_BSS;return FL_SECTION;}
".rela.da_ro8"              {yy2lval.sec=RELA_DA_RO;return FL_SECTION;}
".rela.tda_data"            {yy2lval.sec=RELA_TDA_DATA;return FL_SECTION;}
".rela.tda_bss"             {yy2lval.sec=RELA_TDA_BSS;return FL_SECTION;}
".rela.tda_ro"              {yy2lval.sec=RELA_TDA_RO;return FL_SECTION;}
".rela.tda"                 {yy2lval.sec=RELA_TDA;return FL_SECTION;}
".rela.sda1"                {yy2lval.sec=RELA_SDA;return FL_SECTION;}
".rela.sda2"                {yy2lval.sec=RELA_SDA;return FL_SECTION;}
".rela.sda4"                {yy2lval.sec=RELA_SDA;return FL_SECTION;}
".rela.sda8"                {yy2lval.sec=RELA_SDA;return FL_SECTION;}
".rela.da1"                 {yy2lval.sec=RELA_DA;return FL_SECTION;}
".rela.da2"                 {yy2lval.sec=RELA_DA;return FL_SECTION;}
".rela.da4"                 {yy2lval.sec=RELA_DA;return FL_SECTION;}
".rela.da8"                 {yy2lval.sec=RELA_DA;return FL_SECTION;}
".rela.rotda"               {yy2lval.sec=RELA_TDA_RO;return FL_SECTION;}
".rela.rosda1"              {yy2lval.sec=RELA_SDA_RO;return FL_SECTION;}
".rela.rosda2"              {yy2lval.sec=RELA_SDA_RO;return FL_SECTION;}
".rela.rosda4"              {yy2lval.sec=RELA_SDA_RO;return FL_SECTION;}
".rela.rosda8"              {yy2lval.sec=RELA_SDA_RO;return FL_SECTION;}
".rela.roda1"               {yy2lval.sec=RELA_DA_RO;return FL_SECTION;}
".rela.roda2"               {yy2lval.sec=RELA_DA_RO;return FL_SECTION;}
".rela.roda4"               {yy2lval.sec=RELA_DA_RO;return FL_SECTION;}
".rela.roda8"               {yy2lval.sec=RELA_DA_RO;return FL_SECTION;}



".sda_data1"                {yy2lval.sec=SDA_DATA;return FL_SECTION;}
".sda_bss1"                 {yy2lval.sec=SDA_BSS;return FL_SECTION;}
".sda_ro1"                  {yy2lval.sec=SDA_RO;return FL_SECTION;}
".sda_data2"                {yy2lval.sec=SDA_DATA;return FL_SECTION;}
".sda_bss2"                 {yy2lval.sec=SDA_BSS;return FL_SECTION;}
".sda_ro2"                  {yy2lval.sec=SDA_RO;return FL_SECTION;}
".sda_data4"                {yy2lval.sec=SDA_DATA;return FL_SECTION;}
".sda_bss4"                 {yy2lval.sec=SDA_BSS;return FL_SECTION;}
".sda_ro4"                  {yy2lval.sec=SDA_RO;return FL_SECTION;}
".sda_data8"                {yy2lval.sec=SDA_DATA;return FL_SECTION;}
".sda_bss8"                 {yy2lval.sec=SDA_BSS;return FL_SECTION;}
".sda_ro8"                  {yy2lval.sec=SDA_RO;return FL_SECTION;}
".rotda"                    {yy2lval.sec=TDA_RO;return FL_SECTION;}
".roda1"                    {yy2lval.sec=DA_RO;return FL_SECTION;}
".roda2"                    {yy2lval.sec=DA_RO;return FL_SECTION;}
".roda4"                    {yy2lval.sec=DA_RO;return FL_SECTION;}
".roda8"                    {yy2lval.sec=DA_RO;return FL_SECTION;}
".rosda1"                   {yy2lval.sec=SDA_RO;return FL_SECTION;}
".rosda2"                   {yy2lval.sec=SDA_RO;return FL_SECTION;}
".rosda4"                   {yy2lval.sec=SDA_RO;return FL_SECTION;}
".rosda8"                   {yy2lval.sec=SDA_RO;return FL_SECTION;}
".da_data1"                 {yy2lval.sec=DA_DATA;return FL_SECTION;}
".da_bss1"                  {yy2lval.sec=DA_BSS;return FL_SECTION;}
".da_ro1"                   {yy2lval.sec=DA_RO;return FL_SECTION;}
".da_data2"                 {yy2lval.sec=DA_DATA;return FL_SECTION;}
".da_bss2"                  {yy2lval.sec=DA_BSS;return FL_SECTION;}
".da_ro2"                   {yy2lval.sec=DA_RO;return FL_SECTION;}
".da_data4"                 {yy2lval.sec=DA_DATA;return FL_SECTION;}
".da_bss4"                  {yy2lval.sec=DA_BSS;return FL_SECTION;}
".da_ro4"                   {yy2lval.sec=DA_RO;return FL_SECTION;}
".da_data8"                 {yy2lval.sec=DA_DATA;return FL_SECTION;}
".da_bss8"                  {yy2lval.sec=DA_BSS;return FL_SECTION;}
".da_ro8"                   {yy2lval.sec=DA_RO;return FL_SECTION;}
".tda_data"                 {yy2lval.sec=TDA_DATA;return FL_SECTION;}
".tda_bss"                  {yy2lval.sec=TDA_BSS;return FL_SECTION;}
".tda_ro"                   {yy2lval.sec=TDA_RO;return FL_SECTION;}

".tbss"                     {yy2lval.sec=TDA_BSS;return FL_SECTION;}

".sbss1"                    {yy2lval.sec=SDA_BSS;return FL_SECTION;}
".sbss2"                    {yy2lval.sec=SDA_BSS;return FL_SECTION;}
".sbss4"                    {yy2lval.sec=SDA_BSS;return FL_SECTION;}
".sbss"                     {yy2lval.sec=SDA_BSS;return FL_SECTION;}

".debug_frame"              {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_srcinfo"            {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_sfnames"            {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_aranges"            {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_pubnames"           {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_info"               {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_abbrev"             {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_line"               {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_frame"              {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_str"                {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_loc"                {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_macinfo"            {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_weaknames"          {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_funcnames"          {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_typenames"          {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_varnames"           {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug_ranges"             {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
".debug"                    {yy2lval.sec=DBG_SECTION;return FL_SECTION;}
"Total"                     {yy2lval.sec=TOTAL;return FL_SECTION;}


{number}  {yy2lval.val=atoi(yy2text);return FL_NUMBER;}

{stbegin}{string} {yy2lval.name=strdup(yy2text);return FL_NAME;}


.    {fprintf(stderr,"ERROR: parse issue (%c) line %d\n", *yy2text, yy2lineno); exit(1);}



%%

/************************************************************
Functions body : Only for parsing purpose
*************************************************************/

int parse_size(char *file_name, int disc) {
  int status;
  FILE *file;
  disc_set=disc;
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
  yy2restart(yyin);
  status=yy2parse();
  fclose(file);
  //printf("Finish Parsing %s ..... lines \n",current_file_name);
  return status;
}

/* in case of mistakes in the parser*/
int yy2error(char *s) {
  fprintf(stderr,"ERROR: %s reading '%s' in file %s at line %d\n", s,yy2text,current_file_name,line_num);
  exit(1);
}

int yy2wrap (){
  return 1;
}
