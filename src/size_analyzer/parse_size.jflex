package size_analyzer;

import java_cup.runtime.*;
import reporting_tool.*;

%%
%public 
%class Size_Lexer
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
stbegin = [a-zA-Z/\.]
string  = [a-zA-Z/0-9_\.\-@\+]*


%%

{newline}                   { /* ignore */ }
{ws}                        { /* ignore */ }
";"                         {return symbol(sym.FL_SEPARATE);}
","                         {return symbol(sym.FL_COMA);}
".text"                     {return symbol(sym.FL_SECTION,Sections.TEXT);}
".gnu.linkonce.t"           {return symbol(sym.FL_SECTION,Sections.TEXT);}
".except_table"             {return symbol(sym.FL_SECTION,Sections.EXCEPT_TABLE);}
".except_table_supp"        {return symbol(sym.FL_SECTION,Sections.EXCEPT_TABLE);}
".bss1"                     {return symbol(sym.FL_SECTION,Sections.BSS);}
".bss2"                     {return symbol(sym.FL_SECTION,Sections.BSS);}
".bss4"                     {return symbol(sym.FL_SECTION,Sections.BSS);}
".bss8"                     {return symbol(sym.FL_SECTION,Sections.BSS);}
".bss"                      {return symbol(sym.FL_SECTION,Sections.BSS);}
".data"                     {return symbol(sym.FL_SECTION,Sections.DATA);}
".rodata"                   {return symbol(sym.FL_SECTION,Sections.RODATA);}
".robase"                   {return symbol(sym.FL_SECTION,Sections.ROBASE);}
".symtab"                   {return symbol(sym.FL_SECTION,Sections.SYMTAB);}
".strtab"                   {return symbol(sym.FL_SECTION,Sections.STRTAB);}
".rela.text"                {return symbol(sym.FL_SECTION,Sections.RELA_TEXT);}
".rela.data"                {return symbol(sym.FL_SECTION,Sections.RELA_DATA);}
".rela.rodata"              {return symbol(sym.FL_SECTION,Sections.RELA_RODATA);}
".rela.debug_frame"         {return symbol(sym.FL_SECTION,Sections.RELA_DBG_FRAME);}
".STXP70_EXTRECONF_INFO"    {return symbol(sym.FL_SECTION,Sections.STXP70_EXTRECONF_INFO);}
".STXP70_MEMINFO"           {return symbol(sym.FL_SECTION,Sections.STXP70_MEMINFO);}
".startup"                  {return symbol(sym.FL_SECTION,Sections.STARTUP);}
".syscall"                  {return symbol(sym.FL_SECTION,Sections.SYSCALL);}
".ivtable"                  {return symbol(sym.FL_SECTION,Sections.IVTABLE);}
".thandlers"                {return symbol(sym.FL_SECTION,Sections.THANDLERS);}
".ithandlers"               {return symbol(sym.FL_SECTION,Sections.ITHANDLERS);}
".stack1"                   {return symbol(sym.FL_SECTION,Sections.STACK1);}
".heap"                     {return symbol(sym.FL_SECTION,Sections.HEAP);}
".shstrtab"                 {return symbol(sym.FL_SECTION,Sections.SHSRTAB);}
".STXP70_MEMMAP_INFO"       {return symbol(sym.FL_SECTION,Sections.STXP70_MEMMAP_INFO);}
".secinit"                  {return symbol(sym.FL_SECTION,Sections.SECINIT);}
".tda"                      {return symbol(sym.FL_SECTION,Sections.TDA);}
".da1"                      {return symbol(sym.FL_SECTION,Sections.DA);}
".sda1"                     {return symbol(sym.FL_SECTION,Sections.SDA);}
".da2"                      {return symbol(sym.FL_SECTION,Sections.DA);}
".sda2"                     {return symbol(sym.FL_SECTION,Sections.SDA);}
".da4"                      {return symbol(sym.FL_SECTION,Sections.DA);}
".sda4"                     {return symbol(sym.FL_SECTION,Sections.SDA);}
".da8"                      {return symbol(sym.FL_SECTION,Sections.DA);}
".sda8"                     {return symbol(sym.FL_SECTION,Sections.SDA);}

".rela.sda_data1"           {return symbol(sym.FL_SECTION,Sections.RELA_SDA_DATA);}
".rela.sda_bss1"            {return symbol(sym.FL_SECTION,Sections.RELA_SDA_BSS);}
".rela.sda_ro1"             {return symbol(sym.FL_SECTION,Sections.RELA_SDA_RO);}
".rela.sda_data2"           {return symbol(sym.FL_SECTION,Sections.RELA_SDA_DATA);}
".rela.sda_bss2"            {return symbol(sym.FL_SECTION,Sections.RELA_SDA_BSS);}
".rela.sda_ro2"             {return symbol(sym.FL_SECTION,Sections.RELA_SDA_RO);}
".rela.sda_data4"           {return symbol(sym.FL_SECTION,Sections.RELA_SDA_DATA);}
".rela.sda_bss4"            {return symbol(sym.FL_SECTION,Sections.RELA_SDA_BSS);}
".rela.sda_ro4"             {return symbol(sym.FL_SECTION,Sections.RELA_SDA_RO);}
".rela.sda_data8"           {return symbol(sym.FL_SECTION,Sections.RELA_SDA_DATA);}
".rela.sda_bss8"            {return symbol(sym.FL_SECTION,Sections.RELA_SDA_BSS);}
".rela.sda_ro8"             {return symbol(sym.FL_SECTION,Sections.RELA_SDA_RO);}
".rela.da_data1"            {return symbol(sym.FL_SECTION,Sections.RELA_DA_DATA);}
".rela.da_bss1"             {return symbol(sym.FL_SECTION,Sections.RELA_DA_BSS);}
".rela.da_ro1"              {return symbol(sym.FL_SECTION,Sections.RELA_DA_RO);}
".rela.da_data2"            {return symbol(sym.FL_SECTION,Sections.RELA_DA_DATA);}
".rela.da_bss2"             {return symbol(sym.FL_SECTION,Sections.RELA_DA_BSS);}
".rela.da_ro2"              {return symbol(sym.FL_SECTION,Sections.RELA_DA_RO);}
".rela.da_data4"            {return symbol(sym.FL_SECTION,Sections.RELA_DA_DATA);}
".rela.da_bss4"             {return symbol(sym.FL_SECTION,Sections.RELA_DA_BSS);}
".rela.da_ro4"              {return symbol(sym.FL_SECTION,Sections.RELA_DA_RO);}
".rela.da_data8"            {return symbol(sym.FL_SECTION,Sections.RELA_DA_DATA);}
".rela.da_bss8"             {return symbol(sym.FL_SECTION,Sections.RELA_DA_BSS);}
".rela.da_ro8"              {return symbol(sym.FL_SECTION,Sections.RELA_DA_RO);}
".rela.tda_data"            {return symbol(sym.FL_SECTION,Sections.RELA_TDA_DATA);}
".rela.tda_bss"             {return symbol(sym.FL_SECTION,Sections.RELA_TDA_BSS);}
".rela.tda_ro"              {return symbol(sym.FL_SECTION,Sections.RELA_TDA_RO);}
".rela.tda"                 {return symbol(sym.FL_SECTION,Sections.RELA_TDA);}
".rela.sda1"                {return symbol(sym.FL_SECTION,Sections.RELA_SDA);}
".rela.sda2"                {return symbol(sym.FL_SECTION,Sections.RELA_SDA);}
".rela.sda4"                {return symbol(sym.FL_SECTION,Sections.RELA_SDA);}
".rela.sda8"                {return symbol(sym.FL_SECTION,Sections.RELA_SDA);}
".rela.da1"                 {return symbol(sym.FL_SECTION,Sections.RELA_DA);}
".rela.da2"                 {return symbol(sym.FL_SECTION,Sections.RELA_DA);}
".rela.da4"                 {return symbol(sym.FL_SECTION,Sections.RELA_DA);}
".rela.da8"                 {return symbol(sym.FL_SECTION,Sections.RELA_DA);}
".rela.rotda"               {return symbol(sym.FL_SECTION,Sections.RELA_TDA_RO);}
".rela.rosda1"              {return symbol(sym.FL_SECTION,Sections.RELA_SDA_RO);}
".rela.rosda2"              {return symbol(sym.FL_SECTION,Sections.RELA_SDA_RO);}
".rela.rosda4"              {return symbol(sym.FL_SECTION,Sections.RELA_SDA_RO);}
".rela.rosda8"              {return symbol(sym.FL_SECTION,Sections.RELA_SDA_RO);}
".rela.roda1"               {return symbol(sym.FL_SECTION,Sections.RELA_DA_RO);}
".rela.roda2"               {return symbol(sym.FL_SECTION,Sections.RELA_DA_RO);}
".rela.roda4"               {return symbol(sym.FL_SECTION,Sections.RELA_DA_RO);}
".rela.roda8"               {return symbol(sym.FL_SECTION,Sections.RELA_DA_RO);}

".sda_data1"                {return symbol(sym.FL_SECTION,Sections.SDA_DATA);}
".sda_bss1"                 {return symbol(sym.FL_SECTION,Sections.SDA_BSS);}
".sda_ro1"                  {return symbol(sym.FL_SECTION,Sections.SDA_RO);}
".sda_data2"                {return symbol(sym.FL_SECTION,Sections.SDA_DATA);}
".sda_bss2"                 {return symbol(sym.FL_SECTION,Sections.SDA_BSS);}
".sda_ro2"                  {return symbol(sym.FL_SECTION,Sections.SDA_RO);}
".sda_data4"                {return symbol(sym.FL_SECTION,Sections.SDA_DATA);}
".sda_bss4"                 {return symbol(sym.FL_SECTION,Sections.SDA_BSS);}
".sda_ro4"                  {return symbol(sym.FL_SECTION,Sections.SDA_RO);}
".sda_data8"                {return symbol(sym.FL_SECTION,Sections.SDA_DATA);}
".sda_bss8"                 {return symbol(sym.FL_SECTION,Sections.SDA_BSS);}
".sda_ro8"                  {return symbol(sym.FL_SECTION,Sections.SDA_RO);}
".rotda"                    {return symbol(sym.FL_SECTION,Sections.TDA_RO);}
".roda1"                    {return symbol(sym.FL_SECTION,Sections.DA_RO);}
".roda2"                    {return symbol(sym.FL_SECTION,Sections.DA_RO);}
".roda4"                    {return symbol(sym.FL_SECTION,Sections.DA_RO);}
".roda8"                    {return symbol(sym.FL_SECTION,Sections.DA_RO);}
".rosda1"                   {return symbol(sym.FL_SECTION,Sections.SDA_RO);}
".rosda2"                   {return symbol(sym.FL_SECTION,Sections.SDA_RO);}
".rosda4"                   {return symbol(sym.FL_SECTION,Sections.SDA_RO);}
".rosda8"                   {return symbol(sym.FL_SECTION,Sections.SDA_RO);}
".da_data1"                 {return symbol(sym.FL_SECTION,Sections.DA_DATA);}
".da_bss1"                  {return symbol(sym.FL_SECTION,Sections.DA_BSS);}
".da_ro1"                   {return symbol(sym.FL_SECTION,Sections.DA_RO);}
".da_data2"                 {return symbol(sym.FL_SECTION,Sections.DA_DATA);}
".da_bss2"                  {return symbol(sym.FL_SECTION,Sections.DA_BSS);}
".da_ro2"                   {return symbol(sym.FL_SECTION,Sections.DA_RO);}
".da_data4"                 {return symbol(sym.FL_SECTION,Sections.DA_DATA);}
".da_bss4"                  {return symbol(sym.FL_SECTION,Sections.DA_BSS);}
".da_ro4"                   {return symbol(sym.FL_SECTION,Sections.DA_RO);}
".da_data8"                 {return symbol(sym.FL_SECTION,Sections.DA_DATA);}
".da_bss8"                  {return symbol(sym.FL_SECTION,Sections.DA_BSS);}
".da_ro8"                   {return symbol(sym.FL_SECTION,Sections.DA_RO);}
".tda_data"                 {return symbol(sym.FL_SECTION,Sections.TDA_DATA);}
".tda_bss"                  {return symbol(sym.FL_SECTION,Sections.TDA_BSS);}
".tda_ro"                   {return symbol(sym.FL_SECTION,Sections.TDA_RO);}
".tbss"                     {return symbol(sym.FL_SECTION,Sections.TDA_BSS);}
".sbss1"                    {return symbol(sym.FL_SECTION,Sections.SDA_BSS);}
".sbss2"                    {return symbol(sym.FL_SECTION,Sections.SDA_BSS);}
".sbss4"                    {return symbol(sym.FL_SECTION,Sections.SDA_BSS);}
".sbss"                     {return symbol(sym.FL_SECTION,Sections.SDA_BSS);}

".debug_frame"              {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_srcinfo"            {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_sfnames"            {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_aranges"            {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_pubnames"           {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_info"               {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_abbrev"             {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_line"               {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_frame"              {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_str"                {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_loc"                {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_macinfo"            {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_weaknames"          {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_funcnames"          {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_typenames"          {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_varnames"           {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug_ranges"             {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
".debug"                    {return symbol(sym.FL_SECTION,Sections.DBG_SECTION);}
"Total"                     {return symbol(sym.FL_SECTION,Sections.TOTAL);}

{number}  					{return symbol(sym.FL_NUMBER,new Integer(yytext()));}
{stbegin}{string} 			{return symbol(sym.FL_NAME,yytext());}

"----- Results in"          {return symbol(sym.FL_RESULT_NAME_BEGIN);}
"-----"                     {return symbol(sym.FL_RESULT_NAME_END);}


.    {error("Illegal character: "+ yytext());}

