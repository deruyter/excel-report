#include "xml_dump.hpp"
#include <math.h>

Excel_Output::Excel_Output(char* name) {
  _name = strdup(name);
  _file = fopen(_name,"w");
  if(!_file) {
    fprintf(stderr,"ERROR: Impossible to create file %s\n",_name);
    exit(1);
  }
}

char *Excel_Output::style(int id) {
  switch(id) {
  case PERCENT_STYL_ID: 
    return "s100";
  case BOLDLEFT_STYL_ID:   
    return "s101";
  case BOLDRIGHT_STYL_ID: 
    return "s102";
  case BOLDCENTER_STYL_ID: 
    return "s103";
  case FAIL_STYL_ID:
    return "s104";
  case BOLDCENTER_STYL_ID_16:
    return"s105";
  default: 
    fprintf(stderr,"ERROR: this style is not defined %d\n",id);
    exit(0);
  }
}

void Excel_Output::string_cell() {fprintf(_file, "<Cell><Data ss:Type =\"String\">");}
void Excel_Output::string_cell(char *string) {fprintf(_file, "<Cell><Data ss:Type =\"String\">%s</Data></Cell>\n", string);}
void Excel_Output::string_cell(char *string,char *string2) {fprintf(_file, "<Cell><Data ss:Type =\"String\">%s %s</Data></Cell>\n", string,string2);}
void Excel_Output::string_cell(int style_id,char *string){fprintf(_file, "<Cell ss:StyleID=\"%s\"><Data ss:Type =\"String\">%s</Data></Cell>\n", style(style_id),string);}  
void Excel_Output::string_cell(int position,int style_id,char *string){fprintf(_file, "<Cell ss:Index=\"%d\" ss:StyleID=\"%s\"><Data ss:Type =\"String\">%s</Data></Cell>", position,style(style_id),string);}
void Excel_Output::string_cell_id(int style_id){fprintf(_file, "<Cell ss:StyleID=\"%s\"><Data ss:Type =\"String\">", style(style_id));}  
void Excel_Output::string_out(char *string) {fprintf(_file, "%s",string);}
void Excel_Output::string_out(int val) {fprintf(_file, "%d",val);}
void Excel_Output::string_cell_end() {fprintf(_file, "</Data></Cell>\n");}

void Excel_Output::string_cell_comment(char *string) {fprintf(_file, "<Cell><Data ss:Type =\"String\">%s</Data>", string);}
void Excel_Output::begin_comment() {fprintf(_file, "<Comment><ss:Data xmlns=\"http://www.w3.org/TR/REC-html40\">");}
void Excel_Output::end_comment() {fprintf(_file, "</ss:Data></Comment></Cell>");}
void Excel_Output::string_comment(char *string) {fprintf(_file, "<Font html:Size=\"8\" html:Color=\"#000000\">%s</Font>",string);}
void Excel_Output::string_comment_bold(char *string) {fprintf(_file, "<B><Font html:Size=\"8\" html:Color=\"#000000\">%s</Font></B>",string);}

void Excel_Output::number_cell(int val) {fprintf(_file, "<Cell><Data ss:Type =\"Number\">%d</Data></Cell>\n",val);}
void Excel_Output::number_cell(int style_id,int val) {fprintf(_file, "<Cell ss:StyleID=\"%s\"><Data ss:Type =\"Number\">%d</Data></Cell>\n",style(style_id),val);}
void Excel_Output::number_cell(double val) {fprintf(_file, "<Cell><Data ss:Type =\"Number\">%f</Data></Cell>\n",val);}

void Excel_Output::start_row() { fprintf(_file,"<Row>\n");}
void Excel_Output::start_row(int position) { fprintf(_file, "<Row ss:Index=\"%d\">\n",position);}
void Excel_Output::end_row()   { fprintf(_file,"</Row>\n");}


void Excel_Output::generate_header() {
  fprintf(_file,"<?xml version=\"1.0\" ?>\n") ;
  fprintf(_file, "<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\t xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n\t xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n\t xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\t xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n");
  fprintf(_file, "<DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">");
  fprintf(_file, "<Author>ST</Author>\n<LastAuthor>ST</LastAuthor>\n");
  fprintf(_file, "<Created>%s</Created>\n<LastSaved>%s</LastSaved>\n",__DATE__,__DATE__);
  fprintf(_file, "<Company>STMicroelectronics</Company>\n<Version>1.1</Version>\n</DocumentProperties>\n");

  fprintf(_file, "<ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">\n<ActiveSheet>0</ActiveSheet>\n<FirstVisibleSheet>0</FirstVisibleSheet>\n</ExcelWorkbook>");

  // Styles --------------------------------------------------------------------------
  fprintf(_file, "<Styles>\n");
  fprintf(_file, "<Style ss:ID=\"Default\" ss:Name=\"Normal\"> <Alignment ss:Vertical=\"Bottom\"/> <Borders/> <Font/> <Interior/> <NumberFormat/> <Protection/> </Style>\n");
  // Bold center
  fprintf(_file, "<Style ss:ID=\"%s\" ss:Name=\"boldcenter\"> <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Bottom\"/> <Font ss:Bold=\"1\"/> </Style>\n",   style(BOLDCENTER_STYL_ID));
  // Bold center 16
  fprintf(_file, "<Style ss:ID=\"%s\" ss:Name=\"boldcenter16\"> <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Bottom\"/> <Font ss:Size=\"16\" ss:Bold=\"1\"/> </Style>\n",   style(BOLDCENTER_STYL_ID_16));
  // A percentage 
  fprintf(_file, "<Style ss:ID=\"%s\" ss:Name=\"percent\">  <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Bottom\"/> <NumberFormat ss:Format=\"0.000%%\"/> </Style>\n",   style(PERCENT_STYL_ID));
  // Bold left
  fprintf(_file, "<Style ss:ID=\"%s\" ss:Name=\"boldleft\"> <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Bottom\"/> <Font ss:Bold=\"1\"/> </Style>\n",   style(BOLDLEFT_STYL_ID));
  // Bold Right
  fprintf(_file, "<Style ss:ID=\"%s\" ss:Name=\"boldright\"> <Alignment ss:Horizontal=\"Right\" ss:Vertical=\"Bottom\"/> <Font ss:Bold=\"1\"/> </Style>\n",   style(BOLDRIGHT_STYL_ID));
  // FAIL
  fprintf(_file, "<Style ss:ID=\"%s\" ss:Name=\"fail\"> <Font ss:Color=\"white\"/> <Interior ss:Color=\"#993366\" ss:Pattern=\"Solid\"/> </Style>\n",   style(FAIL_STYL_ID));
  fprintf(_file, "</Styles>\n");
}

void Excel_Output::create_summary(RootDataClass& rootdata) {
  fprintf(_file, "<Worksheet ss:Name=\"Summary\">\n");
  fprintf(_file, "<Table>");
    
  // Columns formatting
   fprintf(_file, "<Column ss:AutoFitWidth=\"1\" ss:Width=\"30\" />\n");
   fprintf(_file, "<Column ss:AutoFitWidth=\"1\" ss:Width=\"70\" />\n");
   fprintf(_file, "<Column ss:AutoFitWidth=\"1\" ss:Width=\"90\" />\n");
   fprintf(_file, "<Column ss:AutoFitWidth=\"1\" ss:Width=\"220\" />\n");
   fprintf(_file, "<Column ss:AutoFitWidth=\"1\" ss:Width=\"70\" />\n");
   fprintf(_file, "<Column ss:AutoFitWidth=\"1\" ss:Width=\"220\" />\n");
   fprintf(_file, "<Column ss:AutoFitWidth=\"1\" ss:Width=\"150\" ss:Span=\"1\"/>\n");
   
  // First line
  start_row(); 
  string_cell(TABLE_VALUES_FIRST_COL,BOLDCENTER_STYL_ID,"Test Summary");
  end_row();

  // We go directly to the row 5
  fprintf(_file, "<Row ss:Index=\"5\"/>\n");

  start_row();
  string_cell(BOLDCENTER_STYL_ID,"ID");
  string_cell(BOLDCENTER_STYL_ID,"Session");
  string_cell(BOLDCENTER_STYL_ID,"Launch date");
  string_cell(BOLDCENTER_STYL_ID,"Compiler Version");
  string_cell(BOLDCENTER_STYL_ID,"Compiler Date");
  string_cell(BOLDCENTER_STYL_ID,"Compiler Flags");
  string_cell(BOLDCENTER_STYL_ID,"Simulator Version");
  string_cell(BOLDCENTER_STYL_ID,"Simulator Flags");
  end_row();

  int i=1;
  ForEachPt(rootdata.get_sessions(),iter_session) {
     start_row();
     number_cell(i++);   
     string_cell((*iter_session)->get_name());
     string_cell((*iter_session)->get_date().print_date(),(*iter_session)->get_date().print_time());
     string_cell((*iter_session)->get_cc_ver());
     string_cell((*iter_session)->get_cc_date());
     string_cell();
     ForEachPt((*iter_session)->get_cc_flags(),iter_flags) {
       string_out(*iter_flags);
       string_out(" ");
     }
     string_cell_end();
     string_cell((*iter_session)->get_sim_ver());
     string_cell();
     ForEachPt((*iter_session)->get_sim_flags(),iter_flags) {
       string_out(*iter_flags);
       string_out(" ");
     }
     string_cell_end();
     end_row();
  }
  fprintf(_file, "</Table><WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\"><Zoom>75</Zoom></WorksheetOptions>\n</Worksheet>\n");
}

void Excel_Output::excel_terminate() {
  fprintf(_file,"</Workbook>\n");
  fclose(_file);
}

void  Excel_Output::apply_format(int mode, int session_number, int nb_data) {
    
  fprintf(_file,"<AutoFilter x:Range=\"R%dC1:R%dC%d\" xmlns=\"urn:schemas-microsoft-com:office:excel\"> </AutoFilter>",HEADER_NB_ROWS,HEADER_NB_ROWS+nb_data ,TABLE_VALUES_FIRST_COL + session_number -1);
    
  //---------------------------------------------------------------------------------------------------------
  // Conditional formatting for MAIN TABLE
  fprintf(_file, "<ConditionalFormatting xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
  fprintf(_file, "<Range>R%dC%d:R%uC%u</Range>",HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+1, HEADER_NB_ROWS+nb_data, TABLE_VALUES_FIRST_COL + session_number-1);

  if (mode != 0) {
    fprintf(_file, "<Condition><Value1>or(RC=&quot;FAIL(Diff)&quot;,RC=&quot;FAIL(make error)&quot;)</Value1>");
    fprintf(_file, "<Format Style=\"color:white;background:#993366\"/></Condition>\n");

    // Bad
    fprintf(_file, "<Condition><Value1>AND(T(RC)=&quot;&quot;,RC&gt;1)</Value1>");
    fprintf(_file, "<Format Style=\"color:white;background:red\"/></Condition>\n");
	    
    // Good
    fprintf(_file, "<Condition><Value1>AND(T(RC)=&quot;&quot;,RC&lt;1)</Value1>");
    fprintf(_file, "<Format Style=\"color:white;background:green\"/></Condition>\n");

  } else {
    // "FAIL(make error)"
    fprintf(_file, "<Condition><Qualifier>Equal</Qualifier><Value1>FAIL(make error)</Value1>");
    fprintf(_file, "<Format Style=\"color:white;background:#993366\"/></Condition>\n");

    // "FAIL(Diff)"
    fprintf(_file, "<Condition><Qualifier>Equal</Qualifier><Value1>FAIL(Diff)</Value1>");
    fprintf(_file, "<Format Style=\"color:white;background:#993366\"/></Condition>\n");
  }
  fprintf(_file, "</ConditionalFormatting>\n");
  
  //---------------------------------------------------------------------------------------------------------
  // Conditional formatting for STATISTICS
  if (mode != 0) {
    fprintf(_file, "<ConditionalFormatting xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
    fprintf(_file, "<Range>R%dC%d:R%uC%u</Range>",STAT_FIRST_ROW, STAT_FIRST_COL+1, STAT_FIRST_ROW + STAT_NB_ROWS - 1, STAT_FIRST_COL + session_number -1) ;
    
    // Bad
    fprintf(_file, "<Condition><Qualifier>Greater</Qualifier><Value1>1</Value1>");
    fprintf(_file, "<Format Style=\"color:white;background:red\"/></Condition>\n");
    
    // Good
    fprintf(_file, "<Condition><Qualifier>Less</Qualifier><Value1>1</Value1>");
    fprintf(_file, "<Format Style=\"color:white;background:green\"/></Condition>\n");
    fprintf(_file, "</ConditionalFormatting>\n");
  }
  //---------------------------------------------------------------------------------------------------------
  // Conditional formatting for NUMBER OF FAILS
  fprintf(_file, "<ConditionalFormatting xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
  fprintf(_file, "<Range>R%dC%d:R%uC%u</Range>", NBFAILS_FIRST_ROW, NBFAILS_FIRST_COL+1, NBFAILS_FIRST_ROW, NBFAILS_FIRST_COL + session_number );

  // Bad
  fprintf(_file, "<Condition><Qualifier>Greater</Qualifier><Value1>0</Value1>");
  fprintf(_file, "<Format Style=\"color:white;background:red\"/></Condition>\n");
    
  // Good
  fprintf(_file, "<Condition><Qualifier>Equal</Qualifier><Value1>0</Value1>");
  fprintf(_file, "<Format Style=\"color:white;background:green\"/></Condition>\n");
  fprintf(_file, "</ConditionalFormatting>\n");
    
}


void Excel_Output::create_data_page(RootDataClass& rootdata,  char *name, char *title, int disc, Section sec) {

  fprintf(_file, "<Worksheet  ss:Name=\"%s\">\n<Table>\n", name);
  // Columns formatting
  fprintf(_file, "<Column ss:AutoFitWidth=\"0\" ss:Width=\"90\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"180\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"350\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"75\" ss:Span=\"%02d\"/>\n",rootdata.get_nb_sessions()-1);

  // Title
  start_row(); string_cell(TABLE_VALUES_FIRST_COL,BOLDCENTER_STYL_ID_16,title); end_row();
  
  // Number of FAILs
  start_row(NBFAILS_FIRST_ROW); string_cell(NBFAILS_FIRST_COL,BOLDCENTER_STYL_ID,"Num of FAILs");
  for (int i = 0 ; i < rootdata.get_nb_sessions() ; i++) {
    fprintf(_file,"<Cell ss:Formula=\"=COUNTIF(R14C:R%uC,&quot;FAIL*&quot;)\"></Cell>\n",HEADER_NB_ROWS + rootdata.get_nb_max_data()) ;
  }
  end_row();

  // Sessions
  start_row(VERS_NUM_FIRST_ROW); string_cell(VERS_NUM_FIRST_COL,BOLDCENTER_STYL_ID,"Session");
  ForEachPt(rootdata.get_sessions(),iter_session) {
    if(Cruise_Control) 	string_cell(BOLDCENTER_STYL_ID,(*iter_session)->get_cc_name());
    else				string_cell(BOLDCENTER_STYL_ID,(*iter_session)->get_name());
  }
  end_row();
    
  // Tags
  start_row();string_cell(VERS_NUM_FIRST_COL,BOLDCENTER_STYL_ID,"Tags");
  ForEachPt(rootdata.get_sessions(),iter_session) {
    if(Cruise_Control) string_cell(BOLDCENTER_STYL_ID,(*iter_session)->get_cc_name());
    else 			   string_cell(BOLDCENTER_STYL_ID,(*iter_session)->get_name());
  }
  end_row();

  // Empty line before values
  start_row(HEADER_NB_ROWS); end_row();

  // VALUES
  ForEachPt(rootdata.get_disc(),iter) {
    start_row();
    string_cell((*iter)->get_test());
    string_cell((*iter)->get_target());
    string_cell();
    ForEachPt((*iter)->get_options(),opt) {
      string_out(*opt);
      string_out(" ");
    }
    string_cell_end();
    ForEachPt(rootdata.get_sessions(),iter_session) {
      int val;
      if (disc!=SPEED) val = (*iter_session)->get_size(*iter,sec,disc);
      else             val = (*iter_session)->get_cycle(*iter);
	  switch(val) {
	  case NOT_EXECUTED: string_cell("(not executed)");break;
	  case HAS_FAILED:   string_cell("FAIL(make error)");break;
	  case NOT_RELEVANT: string_cell("Not Relevant.");break; 
	  case -1:           string_cell("No result.");break;
	  default:           number_cell(val); break;
	  }
    }
    end_row();
  }

  fprintf(_file, "</Table><WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\"><Visible>SheetHidden</Visible><Zoom>75</Zoom></WorksheetOptions>\n");
//  fprintf(_file, "</Table><WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\"><Zoom>75</Zoom></WorksheetOptions>\n");
  apply_format(0, rootdata.get_nb_sessions(),rootdata.get_nb_max_data());
  fprintf(_file, "</Worksheet>\n");
}



void Excel_Output::create_computed_page(RootDataClass& rootdata,  char *name, char *title, char *data_page, int mode, int disc, Section sec) {
  int nb_sessions_to_dump=rootdata.get_nb_sessions();
  int i;
  if (mode ==3) nb_sessions_to_dump ++;
  fprintf(_file, "<Worksheet  ss:Name=\"%s\">\n<Table>\n", name);
  // Columns formatting
  fprintf(_file, "<Column ss:AutoFitWidth=\"0\" ss:Width=\"90\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"180\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"150\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"75\" ss:Span=\"%02d\"/>\n",nb_sessions_to_dump-1);

  // Title
  start_row(); string_cell(TABLE_VALUES_FIRST_COL,BOLDCENTER_STYL_ID_16,title); end_row();
  
  // Number of FAILs
  start_row(NBFAILS_FIRST_ROW); string_cell(NBFAILS_FIRST_COL,BOLDCENTER_STYL_ID,"Num of FAILs");
  for (i = 0 ; i < nb_sessions_to_dump ; i++) {
    fprintf(_file,"<Cell ss:Formula=\"=COUNTIF(R14C:R%uC,&quot;FAIL*&quot;)\"></Cell>\n",HEADER_NB_ROWS + rootdata.get_nb_max_data()) ;
  }
  end_row();

  // Average
  start_row(STAT_FIRST_ROW); string_cell(TABLE_VALUES_FIRST_COL,BOLDCENTER_STYL_ID,"Average");
  for (i = 0 ; i < nb_sessions_to_dump-1 ; i++) {
    fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=AVERAGE(R%uC:R%uC)\"></Cell>\n",style(PERCENT_STYL_ID),HEADER_NB_ROWS+1, HEADER_NB_ROWS + rootdata.get_nb_max_data());	        
  }
  end_row();
  // end Average

  // Geomean
  start_row(); string_cell(TABLE_VALUES_FIRST_COL,BOLDCENTER_STYL_ID,"Geomean");
  for (i = 0 ; i < nb_sessions_to_dump-1 ; i++) {
    fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=GEOMEAN(R%uC:R%uC)\"></Cell>\n",style(PERCENT_STYL_ID),HEADER_NB_ROWS+1, HEADER_NB_ROWS + rootdata.get_nb_max_data());	        
  }
  end_row();
  // end Geomean

  // Min
  start_row(); string_cell(TABLE_VALUES_FIRST_COL,BOLDCENTER_STYL_ID,"Min");
  for (i =0  ; i < nb_sessions_to_dump-1 ; i++) {
    fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=MIN(R%uC:R%uC)\"></Cell>\n",style(PERCENT_STYL_ID),HEADER_NB_ROWS+1, HEADER_NB_ROWS + rootdata.get_nb_max_data());	        
  }
  end_row();
  // end Min

  // Max
  start_row();string_cell(TABLE_VALUES_FIRST_COL,BOLDCENTER_STYL_ID,"Max");
  for (i = 0 ; i < nb_sessions_to_dump-1 ; i++) {
    fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=MAX(R%uC:R%uC)\"></Cell>\n",style(PERCENT_STYL_ID),HEADER_NB_ROWS+1, HEADER_NB_ROWS + rootdata.get_nb_max_data());	        
  }
  end_row();
  // end Max


  // MEDIAN
  start_row();string_cell(TABLE_VALUES_FIRST_COL,BOLDCENTER_STYL_ID,"Median");
  for (i = 0 ; i < nb_sessions_to_dump-1 ; i++) {
    fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=MEDIAN(R%uC:R%uC)\"></Cell>\n",style(PERCENT_STYL_ID),HEADER_NB_ROWS+1, HEADER_NB_ROWS + rootdata.get_nb_max_data());	        
  }
  end_row();
  // end MEDIAN

  // Total
  start_row();string_cell(TABLE_VALUES_FIRST_COL,BOLDCENTER_STYL_ID,"Total");
  switch (mode) {
  case 1:
    for (i = 0 ; i < nb_sessions_to_dump-1 ; i++) {
      fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=sumif(R%dC%d:R%dC%d,&quot;&gt;0&quot;,%s!R%dC%d:%s!R%dC%d)/sumif(%s!R%dC%d:%s!R%dC%d,&quot;&gt;0&quot;,R%dC%d:R%dC%d)\"></Cell>\n",
	      style(PERCENT_STYL_ID), 
	      HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL, 
	      data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i, 
	      data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i, 
	      HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL); 
    }
    break;
  case 2:
    for (i = 1 ; i < nb_sessions_to_dump ; i++) {
      fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=sumif(%s!R%dC%d:%s!R%dC%d,&quot;&gt;0&quot;,%s!R%dC%d:%s!R%dC%d)/sumif(%s!R%dC%d:%s!R%dC%d,&quot;&gt;0&quot;,%s!R%dC%d:%s!R%dC%d)\"></Cell>\n",
	      style(PERCENT_STYL_ID), 
	      data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i-1, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i-1, 
	      data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i, 
	      data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i, 
	      data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i-1, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i-1); 
    }
    break;
  case 3:
    for (i = 0 ; i < nb_sessions_to_dump-1 ; i++) {
      fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=sumif(R%dC%d:R%dC%d,&quot;&gt;0&quot;,%s!R%dC%d:%s!R%dC%d)/sumif(%s!R%dC%d:%s!R%dC%d,&quot;&gt;0&quot;,R%dC%d:R%dC%d)\"></Cell>\n",
	      style(PERCENT_STYL_ID), 
	      HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL, 
	      data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i, 
	      data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i, 
	      HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL); 
    }
    break;
  default :
    break;
  }
  end_row();

  if (mode==3) {
    nb_sessions_to_dump--;
    start_row(VERS_NUM_FIRST_ROW-1);
    string_cell(VERS_NUM_FIRST_COL,BOLDCENTER_STYL_ID,"Ref ID");
    string_cell(" ");
    for (i = 0 ; i < nb_sessions_to_dump ; i++) number_cell(BOLDCENTER_STYL_ID,i+1);
    end_row();
    start_row();
    string_cell(BOLDRIGHT_STYL_ID,"Reference");
    number_cell(BOLDLEFT_STYL_ID,1);
  } else {
    start_row(VERS_NUM_FIRST_ROW);
  }

  string_cell(VERS_NUM_FIRST_COL,BOLDCENTER_STYL_ID,"Session");

  if(mode == 3 ) fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=INDIRECT(ADDRESS(ROW(),3+R%dC2,1,TRUE,&quot;%s&quot;))\"></Cell>\n",style(BOLDCENTER_STYL_ID),VERS_NUM_FIRST_ROW,data_page) ;
    
  for (i = 0 ; i < nb_sessions_to_dump ; i++) {
    if(mode != 3 && i == 0) {
      number_cell(BOLDCENTER_STYL_ID,i+1);
      continue;
    } 
    string_cell_id(BOLDCENTER_STYL_ID);
    switch(mode){
    case 1:
      string_out(1);
      break;
    case 2 :
      string_out(i);
      break;
    case 3 :
      string_out("REF");
      break;
    default :
      break;
    }
    string_out("=>"); string_out(i+1); string_cell_end();
  }
  end_row();

    
  // Tags
  start_row();string_cell(VERS_NUM_FIRST_COL,BOLDCENTER_STYL_ID,"Tags");
  if(mode == 3 )  fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=INDIRECT(ADDRESS(ROW(),3+R%dC2,1,TRUE,&quot;%s&quot;))\"></Cell>\n",style(BOLDCENTER_STYL_ID),VERS_NUM_FIRST_ROW,data_page) ;
  
  ForEachPt(rootdata.get_sessions(),iter_session) {
    if(Cruise_Control) 	string_cell(BOLDCENTER_STYL_ID,(*iter_session)->get_cc_name());
    else				string_cell(BOLDCENTER_STYL_ID,(*iter_session)->get_name());
  }
  end_row();
  
  // Empty line before values
  start_row(HEADER_NB_ROWS); end_row();

  // VALUES
  ForEachPt(rootdata.get_disc(),iter) {
    int ref=-1;
    bool first=true;
    start_row();
    fprintf(_file,"<Cell ss:Formula=\"=%s!RC1\"></Cell>\n",data_page) ;
    fprintf(_file,"<Cell ss:Formula=\"=%s!RC2\"></Cell>\n",data_page) ;
    fprintf(_file,"<Cell ss:Formula=\"=%s!RC3\"></Cell>\n",data_page) ;

    if(mode == 3) fprintf(_file,"<Cell ss:Formula=\"=INDIRECT(ADDRESS(ROW(),3+R%dC2,1,TRUE,&quot;%s&quot;))\"></Cell>\n",VERS_NUM_FIRST_ROW,data_page) ;

    i=0;
    ForEachPt(rootdata.get_sessions(),iter_session) {
      //TDR
      int value;
      if (disc!=SPEED) value = (*iter_session)->get_size(*iter,sec,disc);
      else             value = (*iter_session)->get_cycle(*iter);
      if (value != -1 && ref == -1) ref=i;
      switch(mode) {
      case 1:
      case 2:
	if (first) {
	  first=false;
	  switch(value) {
	  case NOT_EXECUTED: string_cell("(not executed)");break;
	  case HAS_FAILED:   string_cell("FAIL(make error)");break;
	  case NOT_RELEVANT: string_cell("Not Relevant.");break; 
	  case -1:           string_cell("No result.");break;
	  default:           number_cell(value); break;
	  }
	  break;
	}
	if(ref == -1) { string_cell("-");break;}
	if(ref == i)  {string_cell("First Value");break;}
	switch(value) {
	case NOT_EXECUTED: string_cell("(not executed)");break;
	case HAS_FAILED:   string_cell("FAIL(make error)");break;
	case NOT_RELEVANT: string_cell("Not Relevant.");break; 
	case -1:           string_cell("No result.");break;
	default:
	  if(mode == 1)	fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=if(%s!RC%d=0,&quot;div by 0&quot;,%s!RC/%s!RC%d)\"></Cell>\n",
				style(PERCENT_STYL_ID),
				data_page,ref+TABLE_VALUES_FIRST_COL,
				data_page,
				data_page,ref+TABLE_VALUES_FIRST_COL);
	  else {
	    fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=if(OR(%s!RC%d=0,T(%s!RC%d)&lt;&gt;&quot;&quot;,T(%s!RC)&lt;&gt;&quot;&quot;),&quot;- &quot;,%s!RC/%s!RC%d)\"></Cell>\n",
		    style(PERCENT_STYL_ID), data_page,i-1+TABLE_VALUES_FIRST_COL, 
		    data_page,i-1+TABLE_VALUES_FIRST_COL,data_page,
		    data_page,
		    data_page,i-1+TABLE_VALUES_FIRST_COL );
	  }
	  break;
	}
	break;
      case 3:
    	  fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=if(T(%s!RC%d)&lt;&gt;&quot;&quot;,%s!RC%d,if(OR(RC%d=0,T(RC%d)&lt;&gt;&quot;&quot;,T(%s!RC%d)&lt;&gt;&quot;&quot;),&quot;- &quot;,%s!RC%d/RC%d))\"></Cell>\n",
    			  style(PERCENT_STYL_ID),data_page,TABLE_VALUES_FIRST_COL+i,data_page,TABLE_VALUES_FIRST_COL+i,TABLE_VALUES_FIRST_COL,TABLE_VALUES_FIRST_COL,data_page,TABLE_VALUES_FIRST_COL+i, data_page,TABLE_VALUES_FIRST_COL+i,	TABLE_VALUES_FIRST_COL);
/*    	  fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=if(OR(RC%d=0,T(RC%d)&lt;&gt;&quot;&quot;,T(%s!RC%d)&lt;&gt;&quot;&quot;),&quot;- &quot;,%s!RC%d/RC%d)\"></Cell>\n",
    			  style(PERCENT_STYL_ID),TABLE_VALUES_FIRST_COL,TABLE_VALUES_FIRST_COL,data_page,TABLE_VALUES_FIRST_COL+i, data_page,TABLE_VALUES_FIRST_COL+i,	TABLE_VALUES_FIRST_COL);
*/	break;
      default: 
	printf("Should not happened\n");
	exit(2);
	
      } 
      i++;
    }
    end_row();
  }
//  fprintf(_file, "</Table><WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\"><Visible>SheetHidden</Visible><Zoom>75</Zoom></WorksheetOptions>\n");
  fprintf(_file, "</Table><WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\"><Zoom>75</Zoom></WorksheetOptions>\n");
  if (mode == 3) apply_format(1, nb_sessions_to_dump+1, rootdata.get_nb_max_data());
  else           apply_format(1, nb_sessions_to_dump, rootdata.get_nb_max_data());
  fprintf(_file, "</Worksheet>\n");
}




void Excel_Output::create_page(RootDataClass& rootdata, int disc, Section sec) {
  if(rootdata.get_nb_max_data()==0) return;
  if (disc == SIZE_OBJ || disc == SIZE_BIN) {
    char name[256],data_name[256],sheet_name[256],title[1024];
    switch (sec) {
    case TEXT :   sprintf(name,"Text"); break;
    case RODATA : sprintf(name,"Rodata"); break;
    case TOTAL : sprintf(name,"Total"); break;
    case RODATA_PLUS_TEXT : sprintf(name,"Text_Rodata"); break;
    default : fprintf(stderr,"ERROR : Section undefined\n");
      exit(1);
    }
    if(disc == SIZE_OBJ) strcat(name,"_Obj");
    if(disc == SIZE_BIN)strcat(name,"_Bin");
    sprintf(data_name,"Size_Data_%s",name);
    sprintf(title,"Size Datas on %s Section",name);
    create_data_page(rootdata, data_name, title, disc, sec);
    sprintf(title,"Size analysis on %s Section versus Previous",name);
    sprintf(sheet_name,"Size_%s_Vs_Prev",name);
    create_computed_page(rootdata, sheet_name, title, data_name, 2,disc,sec);
 
    sprintf(title,"Size analysis on %s Section versus Any",name);
    sprintf(sheet_name,"Size_%s_Vs_Any",name);
    create_computed_page(rootdata, sheet_name, title, data_name, 3,disc,sec);
  } else {
	create_data_page(rootdata, "Cycles_Data", "Cycles Datas", disc, sec);
    create_computed_page(rootdata, "Cycles_Vs_Prev", "Speed analysis Versus previous", "Cycles_Data", 2,disc,sec);
    create_computed_page(rootdata, "Cycles_Vs_Any", "Speed analysis Versus Any", "Cycles_Data", 3,disc,sec);
    
  }
  
}


void Excel_Output::dump_sumary_element(char *title, char *data_page, char *data_page_comp, int nb_data, int nb_sessions, int position) {
  int i;
  start_row();
  string_cell(BOLDLEFT_STYL_ID,title);
  fprintf(_file,"<Cell ss:Formula=\"=sum(%s!R%dC%d:%s!R%dC%d)\"></Cell>\n",data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL, data_page, HEADER_NB_ROWS + nb_data, TABLE_VALUES_FIRST_COL); 
  for (i = 1 ; i < nb_sessions ; i++) {
    fprintf(_file,"<Cell ss:StyleID=\"%s\" ss:Formula=\"=%s!R%dC%d\"></Cell>\n", style(PERCENT_STYL_ID),data_page_comp,position,4+i) ;
  }
  end_row();
}

void Excel_Output::dump_cycle_summary_value(SummaryClass& summary, Dump_Type type) {
	SummaryElem *base, *compare;
	base = *(summary.get_list_elem())->rbegin();
	ForEachRPt(summary.get_list_elem(),iter_elem) {
		compare = *iter_elem;
		if (compare->get_cycles(type)->empty()) number_cell(0);
		else {
			typeof(base->get_cycles(type)->begin()) vb,vc;
			double geomean=0.0,tmp_val;
			int nb_data=0;
			for (vb = base->get_cycles(type)->begin(), vc=compare->get_cycles(type)->begin(); vb != base->get_cycles(type)->end(); ++vb, ++vc) {
				if (*vb<=-1 || *vc<=-1) continue;
				nb_data++;
				tmp_val=(double)(*vc)/(double)(*vb);
				geomean += log(tmp_val);
			}
			if(nb_data) {
				geomean=exp(geomean/nb_data); 
				number_cell(geomean);
			} else {
				number_cell(0);
			}
		}
	}
}


void Excel_Output::dump_size_summary_value(SummaryClass& summary, Dump_Type type, bool is_obj) {
	SummaryElem *base, *compare;
	base = *(summary.get_list_elem())->rbegin();
	ForEachRPt(summary.get_list_elem(),iter_elem) {
		compare = *iter_elem;
		if (compare->get_size(type,is_obj)->empty()) number_cell(0);
		else {
			typeof(base->get_size(type,is_obj)->begin()) vb,vc;
			int nb_data=0;
			double sumb=0,sumc=0;
			for (vb = base->get_size(type,is_obj)->begin(), vc=compare->get_size(type,is_obj)->begin(); vb != base->get_size(type,is_obj)->end(); ++vb, ++vc) {
				if (*vb<=-1 || *vc<=-1) continue;
				nb_data++;
				sumb+=*vb; sumc+=*vc;
			}
			if(nb_data) {
				double moy = sumc/sumb;
				number_cell(moy);
			}else{
				number_cell(0);
			}
		}
	}
}

void Excel_Output::create_summary_ver2(RootDataClass& rootdata, int size[],	bool obj, int obj_data, bool bin, int bin_data, bool cycle,	int cycle_data, SummaryClass& summary) {
	//  char data_page[128]; 
	//  char data_page_comp[128]; 
	fprintf(_file, "<Worksheet ss:Name=\"Summary_v2\">\n");
	fprintf(_file, "<Table>");

	// Columns formatting
	fprintf(_file, "<Column ss:AutoFitWidth=\"1\" ss:Width=\"170\" />\n");
	fprintf(_file, "<Column ss:AutoFitWidth=\"1\" ss:Width=\"100\" ss:Span=\"%d\"/>\n",	rootdata.get_nb_sessions()-1);

	// First line
	start_row();
	string_cell(TABLE_VALUES_FIRST_COL,BOLDCENTER_STYL_ID,"Test Summary");
	end_row();

	// We go directly to the row 5
	fprintf(_file, "<Row ss:Index=\"5\"/>\n");

	start_row();
	string_cell(BOLDLEFT_STYL_ID, "Cycle Analysis");
	ForEachPt(rootdata.get_sessions(),iter_session) {
	    if(Cruise_Control) 	string_cell((*iter_session)->get_cc_name());
	    else 				string_cell((*iter_session)->get_name());
	}
	end_row();
	start_row();
	string_cell("Developer Validation suite");
	dump_cycle_summary_value(summary, Run_Valid);
	end_row();
	start_row();
	string_cell("EEMBC Networking");
	dump_cycle_summary_value(summary, EEMBC_Net);
	end_row();
	start_row();
	string_cell("EEMBC Consumer");
	dump_cycle_summary_value(summary, EEMBC_Cons);
	end_row();
//	start_row();
//	string_cell("Bluetooth");
//	dump_cycle_summary_value(summary, Bluetooth);
//	end_row();
	start_row();
	string_cell("CSD audio benchmarks");
	dump_cycle_summary_value(summary, Audio_CSD);
	end_row();
	start_row();
	string_cell("JPEG code");
	dump_cycle_summary_value(summary, Jpeg);
	end_row();
	
	start_row();
	end_row();

	start_row();
	string_cell(BOLDLEFT_STYL_ID, "Object size Analysis");
	ForEachPt(rootdata.get_sessions(),iter_session) {
	    if(Cruise_Control) 	string_cell((*iter_session)->get_cc_name());
	    else				string_cell((*iter_session)->get_name());
	}
	end_row();
	start_row();
	string_cell("Developer Validation suite");
	dump_size_summary_value(summary, Run_Valid, true);
	end_row();
	start_row();
	string_cell("EEMBC Networking");
	dump_size_summary_value(summary, EEMBC_Net, true);
	end_row();
	start_row();
	string_cell("EEMBC Consumer");
	dump_size_summary_value(summary, EEMBC_Cons, true);
	end_row();
	start_row();
	string_cell("Bluetooth");
	dump_size_summary_value(summary, Bluetooth, true);
	end_row();
	start_row();
	string_cell("CSD audio benchmarks");
	dump_size_summary_value(summary, Audio_CSD, true);
	end_row();
	start_row();
	string_cell("JPEG code");
	dump_size_summary_value(summary, Jpeg, true);
	end_row();

	start_row();
	end_row();

	start_row();
	string_cell(BOLDLEFT_STYL_ID, "Binary size Analysis");
	ForEachPt(rootdata.get_sessions(),iter_session) {
	    if(Cruise_Control) 	string_cell((*iter_session)->get_cc_name());
	    else				string_cell((*iter_session)->get_name());
	}
	end_row();
	start_row();
	string_cell("Developer Validation suite");
	dump_size_summary_value(summary, Run_Valid, false);
	end_row();
	start_row();
	string_cell("EEMBC Networking");
	dump_size_summary_value(summary, EEMBC_Net, false);
	end_row();
	start_row();
	string_cell("EEMBC Consumer");
	dump_size_summary_value(summary, EEMBC_Cons, false);
	end_row();
//	start_row();
//	string_cell("Bluetooth");
//	dump_size_summary_value(summary, Bluetooth, false);
//	end_row();
	start_row();
	string_cell("CSD audio benchmarks");
	dump_size_summary_value(summary, Audio_CSD, false);
	end_row();
	start_row();
	string_cell("JPEG code");
	dump_size_summary_value(summary, Jpeg, false);
	end_row();

	fprintf(_file,	"</Table><WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\"><Zoom>75</Zoom></WorksheetOptions>\n</Worksheet>\n");
}

