#ifndef __XML_DUMP__
#define __XML_DUMP__


//#include <stdio.h>
#include <iostream>
#include <list>
#include "data.hpp"
#define 	HEADER_NB_ROWS	 		17
#define 	TABLE_VALUES_FIRST_COL  	4

#define 	VERS_NUM_FIRST_ROW      	15
#define 	VERS_NUM_FIRST_COL		3

#define 	STAT_FIRST_ROW			7
#define 	STAT_NB_ROWS 			6
#define 	STAT_FIRST_COL			4

#define 	NBFAILS_FIRST_ROW		5
#define 	NBFAILS_FIRST_COL   		3

// Cell value when no result from the report
#define NO_RESULT_STRING "No result."
#define FIRST_VALUE      "First value."
#define FAIL_STRING      "FAIL"
#define FAIL_STRLEN	 4
#define MISSING_VALUES   ""
#define DIV_BY_ZERO      "DIV_BY_ZERO"


class Excel_Output {
public:
  Excel_Output(char *name);
  ~Excel_Output(void) {};
  void generate_header();
  void excel_terminate();
  void create_summary(RootDataClass& rootdata);
  void create_summary_ver2(RootDataClass& rootdata, int size[], bool obj, int obj_data, bool bin, int bin_data, bool cycle, int cycle_data);
  void create_page(RootDataClass& rootdata, int disc, Section sec);
  void create_monitored_page(RootDataClass& rootdata, int disc);
private:
  FILE *_file;
  char *_name;

  void start_row() ;
  void start_row(int position) ;
  void end_row();
  void string_cell(); 
  void string_cell(char *string); 
  void string_cell(char *string,char *string2); 
  void string_cell(int style_id,char *string);
  void string_cell(int position, int style_id,char *string);
  void string_cell_id(int style_id);
  void string_out(char *string); 
  void string_out(int val); 
  void string_cell_end(); 
  void string_cell_comment(char *string);
  void begin_comment();
  void end_comment();
  void string_comment(char *string);
  void string_comment_bold(char *string);
  void number_cell(int val);
  void number_cell(double val);
  void number_cell(int style_id,double val);
  void number_cell(int style_id,int val);
  void apply_format(int mode, int session_number, int nb_data);
  char *style(int id);  
  void dump_size_summary_value(Dump_Type type, bool is_obj);
  void dump_cycle_summary_value(Dump_Type type);
  void create_data_page(RootDataClass& rootdata,  char *name, char *title, int disc, Section sec, bool monitor);
  void create_computed_page(RootDataClass& rootdata,  char *name, char *title, char *data_page, int mode, int disc, Section sec, bool monitor);
  void dump_sumary_element(char *title, char *data_page, char *data_page_comp, int nb_data, int nb_sessions, int position);
};
#endif
