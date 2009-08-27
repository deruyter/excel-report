package reporting_tool;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.util.Date;
import java.util.ListIterator;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

import reporting_tool.Summary.SummaryElement;

public class Xml_Output {

	private enum StyleID{
		  PERCENT_STYL_ID,
		  BOLDLEFT_STYL_ID,
		  BOLDRIGHT_STYL_ID,
		  BOLDCENTER_STYL_ID,
		  FAIL_STYL_ID,
		  BOLDCENTER_STYL_ID_16,
	}
	
	private static final int HEADER_NB_ROWS	 		=	17 ;
	private static final int TABLE_VALUES_FIRST_COL = 	4;

	private static final int VERS_NUM_FIRST_ROW     =	15;
	private static final int VERS_NUM_FIRST_COL		=	3;

	private static final int STAT_FIRST_ROW			=	7;
	private static final int STAT_NB_ROWS 			=	6;
	private static final int STAT_FIRST_COL			=	4;

	private static final int NBFAILS_FIRST_ROW		=	5;
	private static final int NBFAILS_FIRST_COL   	=	3;

	
	private PrintStream output;
	private String getDate() {
	    DateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd");
	    Date date = new Date();
	    return dateFormat.format(date);
	} 

	  public Xml_Output(String name) {
		  try {
			output = new PrintStream(new FileOutputStream(name), true);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			System.exit(1);
		}
	  }
	  public void generate_header() {
		  output.printf("<?xml version=\"1.0\" ?>\n") ;
		  output.printf( "<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\t xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n\t xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n\t xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n\t xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n");
		  output.printf( "<DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">");
		  output.printf( "<Author>ST</Author>\n<LastAuthor>ST</LastAuthor>\n");
		  output.printf( "<Created>%s</Created>\n<LastSaved>%s</LastSaved>\n",getDate(),getDate());
		  output.printf( "<Company>STMicroelectronics</Company>\n<Version>1.1</Version>\n</DocumentProperties>\n");

		  output.printf( "<ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">\n<ActiveSheet>0</ActiveSheet>\n<FirstVisibleSheet>0</FirstVisibleSheet>\n</ExcelWorkbook>");

		  // Styles --------------------------------------------------------------------------
		  output.printf( "<Styles>\n");
		  output.printf( "<Style ss:ID=\"Default\" ss:Name=\"Normal\"> <Alignment ss:Vertical=\"Bottom\"/> <Borders/> <Font/> <Interior/> <NumberFormat/> <Protection/> </Style>\n");
		  // Bold center
		  output.printf( "<Style ss:ID=\"%s\" ss:Name=\"boldcenter\"> <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Bottom\"/> <Font ss:Bold=\"1\"/> </Style>\n",   style(StyleID.BOLDCENTER_STYL_ID));
		  // Bold center 16
		  output.printf( "<Style ss:ID=\"%s\" ss:Name=\"boldcenter16\"> <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Bottom\"/> <Font ss:Size=\"16\" ss:Bold=\"1\"/> </Style>\n", style(StyleID.BOLDCENTER_STYL_ID_16));
		  // A percentage 
		  output.printf( "<Style ss:ID=\"%s\" ss:Name=\"percent\">  <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Bottom\"/> <NumberFormat ss:Format=\"0.000%%\"/> </Style>\n",   style(StyleID.PERCENT_STYL_ID));
		  // Bold left
		  output.printf( "<Style ss:ID=\"%s\" ss:Name=\"boldleft\"> <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Bottom\"/> <Font ss:Bold=\"1\"/> </Style>\n",   style(StyleID.BOLDLEFT_STYL_ID));
		  // Bold Right
		  output.printf( "<Style ss:ID=\"%s\" ss:Name=\"boldright\"> <Alignment ss:Horizontal=\"Right\" ss:Vertical=\"Bottom\"/> <Font ss:Bold=\"1\"/> </Style>\n",   style(StyleID.BOLDRIGHT_STYL_ID));
		  // FAIL
		  output.printf( "<Style ss:ID=\"%s\" ss:Name=\"fail\"> <Font ss:Color=\"white\"/> <Interior ss:Color=\"#993366\" ss:Pattern=\"Solid\"/> </Style>\n",   style(StyleID.FAIL_STYL_ID));
		  output.printf( "</Styles>\n");
  
	  }
	  public void excel_terminate() {
		  output.printf("</Workbook>\n");
		  output.flush();
		  output.close();
	  }

	  public void create_summary(RootDataClass rootdata) {
		  output.printf( "<Worksheet ss:Name=\"Summary\">\n");
		  output.printf( "<Table>");

		  // Columns formatting
		  output.printf( "<Column ss:AutoFitWidth=\"1\" ss:Width=\"30\" />\n");
		  output.printf( "<Column ss:AutoFitWidth=\"1\" ss:Width=\"70\" />\n");
		  output.printf( "<Column ss:AutoFitWidth=\"1\" ss:Width=\"90\" />\n");
		  output.printf( "<Column ss:AutoFitWidth=\"1\" ss:Width=\"220\" />\n");
		  output.printf( "<Column ss:AutoFitWidth=\"1\" ss:Width=\"70\" />\n");
		  output.printf( "<Column ss:AutoFitWidth=\"1\" ss:Width=\"220\" />\n");
		  output.printf( "<Column ss:AutoFitWidth=\"1\" ss:Width=\"150\" ss:Span=\"1\"/>\n");

		  // First line
		  start_row(); 
		  string_cell(TABLE_VALUES_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Test Summary");
		  end_row();

		  // We go directly to the row 5
		  output.printf( "<Row ss:Index=\"5\"/>\n");

		  start_row();
		  string_cell(StyleID.BOLDCENTER_STYL_ID,"ID");
		  string_cell(StyleID.BOLDCENTER_STYL_ID,"Session");
		  string_cell(StyleID.BOLDCENTER_STYL_ID,"Launch date");
		  string_cell(StyleID.BOLDCENTER_STYL_ID,"Compiler Version");
		  string_cell(StyleID.BOLDCENTER_STYL_ID,"Compiler Date");
		  string_cell(StyleID.BOLDCENTER_STYL_ID,"Compiler Flags");
		  string_cell(StyleID.BOLDCENTER_STYL_ID,"Simulator Version");
		  string_cell(StyleID.BOLDCENTER_STYL_ID,"Simulator Flags");
		  end_row();

		  int i=1;
		  for (i=0;i<rootdata.get_nb_sessions();i++) {
			  TestSession my_session=rootdata.get_session(i);
			  start_row();
			  number_cell(i+1);   
			  string_cell(my_session.name);
			  string_cell(my_session.date.print_date(),my_session.date.print_time());
			  string_cell(my_session.get_cc_ver());
			  string_cell(my_session.get_cc_date());
			  string_cell();
			  ListIterator<String> iter_elem = my_session.compiler_flags.listIterator();
			  while(iter_elem.hasNext()) {
				  string_out(iter_elem.next());
				  string_out(" ");
			  }
			  string_cell_end();
			  string_cell(my_session.get_sim_ver());
			  string_cell();
			  iter_elem = my_session.simulator_flags.listIterator();
			  while(iter_elem.hasNext()) {
				  string_out(iter_elem.next());
				  string_out(" ");
			  }
			  string_cell_end();
			  end_row();
		  }
		  output.printf( "</Table><WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\"><Zoom>75</Zoom></WorksheetOptions>\n</Worksheet>\n");
	  }
	  public void create_page(RootDataClass rootdata, Discriminent disc, Sections sec){
		  if(rootdata.get_nb_max_data()==0) return;
		  if (disc == Discriminent.SIZE_OBJ || disc == Discriminent.SIZE_BIN || disc == Discriminent.SIZE_FUNC || disc == Discriminent.SIZE_APPLI) {
		    String name=null,data_name,sheet_name,title;
		    switch (sec) {
		    case TEXT :   name="Text"; break;
		    case DATA :   name="Data"; break;
		    case BSS :   name="Bss"; break;
		    case RODATA : name="Rodata"; break;
		    case TOTAL : name="Total"; break;
		    case RODATA_PLUS_TEXT : name="Text_Rodata"; break;
		    default : System.err.println("ERROR : Section undefined");
		      System.exit(1);
		    }
		    if(disc == Discriminent.SIZE_OBJ) 	name=name.concat("_Obj");
		    if(disc == Discriminent.SIZE_BIN)	name=name.concat("_Bin");
		    if(disc == Discriminent.SIZE_FUNC)	name=name.concat("_Func");
		    if(disc == Discriminent.SIZE_APPLI)	name=name.concat("_Appli");
		    data_name="Size_Data_" + name;
		    title = "Size Datas on " + name +" Section";
		    create_data_page(rootdata, data_name, title, disc, sec, false);
		    
		    title="Size analysis on "+ name + " Section versus Previous";
		    sheet_name="Size_"+ name +"_Vs_Prev";
		    create_computed_page(rootdata, sheet_name, title, data_name, 2,disc,sec, false);
		 
		    if(rootdata.get_nb_sessions() <= 2) return;
		    title="Size analysis on "+ name +" Section versus Any";
		    sheet_name="Size_"+ name +"_Vs_Any";
		    create_computed_page(rootdata, sheet_name, title, data_name, 3,disc,sec, false);
		  } else {
			create_data_page(rootdata, "Cycles_Data", "Cycles Datas", disc, sec, false);
		    create_computed_page(rootdata, "Cycles_Vs_Prev", "Speed analysis Versus previous", "Cycles_Data", 2,disc,sec, false);
		    if(rootdata.get_nb_sessions() <= 2) return;
		    create_computed_page(rootdata, "Cycles_Vs_Any", "Speed analysis Versus Any", "Cycles_Data", 3,disc,sec, false);
		  }
	  }
	  public void create_monitored_page(RootDataClass rootdata, Discriminent disc){
		  	String  data_name,sheet_name,title;
		    if(rootdata.get_nb_max_data()==0) return;
		    if (disc == Discriminent.SIZE_OBJ) {
		      title = "Size Datas on Objects";
		      data_name="Size_Text_Obj";
		      create_data_page(rootdata, data_name, title, disc, Sections.TEXT, true);
		      title="Obj Size analysis on TEXT Section versus First";
		      sheet_name="Text_Size_Obj_Vs_Arm";
		      create_computed_page(rootdata, sheet_name, title, data_name, 1,disc,Sections.TEXT, true);
		      return;
		    }
		    if (disc == Discriminent.SIZE_BIN) {
		      title = "Size Datas on Binaries";
		      data_name="Size_Text_Rodata_Obj";
		      create_data_page(rootdata, data_name, title, Discriminent.SIZE_OBJ, Sections.RODATA_PLUS_TEXT, true);
		      title="Obj Size analysis on RODATA + TEXT Section versus First";
		      sheet_name="Text_Rodata_Size_Evolution";
		      create_computed_page(rootdata, sheet_name, title, data_name, 1,Discriminent.SIZE_OBJ,Sections.RODATA_PLUS_TEXT, true);
		    }
		/*    if (disc == Discriminent.SIZE_FUNC) {
		      sprintf(title,"Function Size Datas");
		      sprintf(data_name,"Func_Size_Data");
		      create_data_page(rootdata, data_name, title, disc, TEXT, true);
		      sprintf(title,"Function Size analysis on TEXT Section versus First");
		      sprintf(sheet_name,"Func_Size_Vs_Arm");
		      create_computed_page(rootdata, sheet_name, title, data_name, 1,disc,TEXT, true);
		      return;
		    }
		    */if (disc == Discriminent.SPEED) {
		      create_data_page(rootdata, "Speed_Evol_Data", "Speed Data", disc, Sections.LAST_SECTION, true);
		      create_computed_page(rootdata, "Speed_Evolution", "Speed Analysis", "Speed_Evol_Data", 1,disc,Sections.LAST_SECTION, true);
		    }
		    
		    if (disc == Discriminent.SPEED_Vs_ARM) {
		      create_data_page(rootdata, "Speed_Evol_Data_Arm", "Speed Data with ARM", disc, Sections.LAST_SECTION, true);
		      create_computed_page(rootdata, "Speed_Vs_Arm", "Speed Analysis Versus Arm", "Speed_Evol_Data_Arm", 1,disc,Sections.LAST_SECTION, true);
		      
		    }
		  
	  }
	
	  private String style(StyleID id) {
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
		    System.err.printf("ERROR: this style is not defined %d\n",id);
		    System.exit(0);
		  }
		return null;
		}
	  
	  private boolean is_normalized_test(String name) {
		    if (name.contentEquals("eembc_networking")) return true;
		    if (name.contentEquals("eembc_consumer")) return true;
		    return false;
		}

	  private void string_cell() {output.printf( "<Cell><Data ss:Type =\"String\">");}
	  private void string_cell(String string) {output.printf( "<Cell><Data ss:Type =\"String\">%s</Data></Cell>\n", string);}
	  private void string_cell(String string,String string2) {output.printf( "<Cell><Data ss:Type =\"String\">%s %s</Data></Cell>\n", string,string2);}
	  private void string_cell(StyleID style_id,String string){output.printf( "<Cell ss:StyleID=\"%s\"><Data ss:Type =\"String\">%s</Data></Cell>\n", style(style_id),string);}  
	  private void string_cell(int position,StyleID style_id,String string){output.printf( "<Cell ss:Index=\"%d\" ss:StyleID=\"%s\"><Data ss:Type =\"String\">%s</Data></Cell>", position,style(style_id),string);}
	  private void string_cell_id(StyleID style_id){output.printf( "<Cell ss:StyleID=\"%s\"><Data ss:Type =\"String\">", style(style_id));}  
	  private void string_out(String string) {output.printf( "%s",string);}
	  private void string_out(int val) {output.printf( "%d",val);}
	  private void string_cell_end() {output.printf( "</Data></Cell>\n");}

	  private void number_cell(int val) {output.printf( "<Cell><Data ss:Type =\"Number\">%d</Data></Cell>\n",val);}
	  private void number_cell(StyleID style_id,int val) {output.printf( "<Cell ss:StyleID=\"%s\"><Data ss:Type =\"Number\">%d</Data></Cell>\n",style(style_id),val);}
	  private void number_cell(double val) {output.printf( "<Cell><Data ss:Type =\"Number\">%f</Data></Cell>\n",val);}
	  private void number_cell(StyleID style_id,double val) {output.printf( "<Cell ss:StyleID=\"%s\"><Data ss:Type =\"Number\">%f</Data></Cell>\n",style(style_id),val);}

	  private void start_row() { output.printf("<Row>\n");}
	  private void start_row(int position) { output.printf( "<Row ss:Index=\"%d\">\n",position);}
	  private void end_row()   { output.printf("</Row>\n");}

	  private void apply_format(int mode, int session_number, int nb_data) {
		    
		  output.printf("<AutoFilter x:Range=\"R%dC1:R%dC%d\" xmlns=\"urn:schemas-microsoft-com:office:excel\"> </AutoFilter>",HEADER_NB_ROWS,HEADER_NB_ROWS+nb_data ,TABLE_VALUES_FIRST_COL + session_number -1);
		    
		  //---------------------------------------------------------------------------------------------------------
		  // Conditional formatting for MAIN TABLE
		  output.printf( "<ConditionalFormatting xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
		  output.printf( "<Range>R%dC%d:R%dC%d</Range>",HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+1, HEADER_NB_ROWS+nb_data, TABLE_VALUES_FIRST_COL + session_number-1);

		  if (mode != 0) {
		    output.printf( "<Condition><Value1>or(RC=&quot;FAIL(Diff)&quot;,RC=&quot;FAIL(make error)&quot;)</Value1>");
		    output.printf( "<Format Style=\"color:white;background:#993366\"/></Condition>\n");

		    // Bad
		    output.printf( "<Condition><Value1>AND(T(RC)=&quot;&quot;,RC&gt;1)</Value1>");
		    output.printf( "<Format Style=\"color:white;background:red\"/></Condition>\n");
			    
		    // Good
		    output.printf( "<Condition><Value1>AND(T(RC)=&quot;&quot;,RC&lt;1)</Value1>");
		    output.printf( "<Format Style=\"color:white;background:green\"/></Condition>\n");

		  } else {
		    // "FAIL(make error)"
		    output.printf( "<Condition><Qualifier>Equal</Qualifier><Value1>FAIL(make error)</Value1>");
		    output.printf( "<Format Style=\"color:white;background:#993366\"/></Condition>\n");

		    // "FAIL(Diff)"
		    output.printf( "<Condition><Qualifier>Equal</Qualifier><Value1>FAIL(Diff)</Value1>");
		    output.printf( "<Format Style=\"color:white;background:#993366\"/></Condition>\n");
		  }
		  output.printf( "</ConditionalFormatting>\n");
		  
		  //---------------------------------------------------------------------------------------------------------
		  // Conditional formatting for STATISTICS
		  if (mode != 0) {
		    output.printf( "<ConditionalFormatting xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
		    output.printf( "<Range>R%dC%d:R%dC%d</Range>",STAT_FIRST_ROW, STAT_FIRST_COL+1, STAT_FIRST_ROW + STAT_NB_ROWS - 1, STAT_FIRST_COL + session_number -1) ;
		    
		    // Bad
		    output.printf( "<Condition><Qualifier>Greater</Qualifier><Value1>1</Value1>");
		    output.printf( "<Format Style=\"color:white;background:red\"/></Condition>\n");
		    
		    // Good
		    output.printf( "<Condition><Qualifier>Less</Qualifier><Value1>1</Value1>");
		    output.printf( "<Format Style=\"color:white;background:green\"/></Condition>\n");
		    output.printf( "</ConditionalFormatting>\n");
		  }
		  //---------------------------------------------------------------------------------------------------------
		  // Conditional formatting for NUMBER OF FAILS
		  output.printf( "<ConditionalFormatting xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
		  output.printf( "<Range>R%dC%d:R%dC%d</Range>", NBFAILS_FIRST_ROW, NBFAILS_FIRST_COL+1, NBFAILS_FIRST_ROW, NBFAILS_FIRST_COL + session_number );

		  // Bad
		  output.printf( "<Condition><Qualifier>Greater</Qualifier><Value1>0</Value1>");
		  output.printf( "<Format Style=\"color:white;background:red\"/></Condition>\n");
		    
		  // Good
		  output.printf( "<Condition><Qualifier>Equal</Qualifier><Value1>0</Value1>");
		  output.printf( "<Format Style=\"color:white;background:green\"/></Condition>\n");
		  output.printf( "</ConditionalFormatting>\n");
		    

	  }
	  private void dump_size_summary_value(CommonData.Dump_Type type, Discriminent disc){
			SummaryElement base, compare;
			base = sqa_report.summary.get_list_elem().get(sqa_report.summary.get_list_elem().size());
			ListIterator<SummaryElement> iter_elem = sqa_report.summary.get_list_elem().listIterator(1);
			while(iter_elem.hasNext()) {
				compare = iter_elem.next();
				if(base==compare) {
				    number_cell(1);
				    continue;
				}
				if (compare.get_size(type,disc).isEmpty()) number_cell(0);
				else {
					Integer nb_data=0;
					double sumb=0,sumc=0;
					ListIterator<Long> vb = base.get_size(type,disc).listIterator();
					ListIterator<Long> vc = compare.get_size(type,disc).listIterator();
					while (vb.hasNext() && vc.hasNext()) {
						Long valb = vb.next();
						Long valc = vc.next();
						if (valb <= -1 || valc <= -1)  continue;
						nb_data++;
						sumb += valb.doubleValue();
						sumc += valc.doubleValue();
					}
					if(nb_data > 0) {
						double moy = sumc/sumb; 
						if (sqa_report.Monitoring) number_cell(StyleID.PERCENT_STYL_ID,moy);
						else number_cell(moy);
					} else {
						number_cell(0);
					}
				}
			}

	  }
	  private void dump_cycle_summary_value(CommonData.Dump_Type type) {
			SummaryElement base, compare;
			base = sqa_report.summary.get_list_elem().get(sqa_report.summary.get_list_elem().size());
			ListIterator<SummaryElement> iter_elem = sqa_report.summary.get_list_elem().listIterator(1);
			while(iter_elem.hasNext()) {
				compare = iter_elem.next();
				if(base==compare) {
				    number_cell(1);
				    continue;
				}
				if (compare.get_cycles(type).isEmpty()) number_cell(0);
				else {
					Integer nb_data=0;
					double geomean=0.0,tmp_val;
					ListIterator<Long> vb = base.get_cycles(type).listIterator();
					ListIterator<Long> vc = compare.get_cycles(type).listIterator();
					while (vb.hasNext() && vc.hasNext()) {
						Long valb = vb.next();
						Long valc = vc.next();
						if (valb <= -1 || valc <= -1)  continue;
						nb_data++;
						tmp_val= (valb.doubleValue() - valc.doubleValue())/ valc.doubleValue();
						geomean += java.lang.Math.log(tmp_val);
					}
					if(nb_data > 0) {
						geomean=java.lang.Math.exp(geomean/nb_data); 
						if (sqa_report.Monitoring) number_cell(StyleID.PERCENT_STYL_ID,geomean);
						else number_cell(geomean);
					} else {
						number_cell(0);
					}
				}
			}
	  }
	  private void create_data_page(RootDataClass rootdata,  String name, String title, Discriminent disc, Sections sec, boolean monitor){
		  int my_session_number=rootdata.get_nb_sessions();
		  if (monitor && (sec==Sections.TEXT || disc==Discriminent.SPEED_Vs_ARM))   my_session_number++;
		  output.printf( "<Worksheet  ss:Name=\"%s\">\n<Table>\n", name);
		  // Columns formatting
		  output.printf( "<Column ss:AutoFitWidth=\"0\" ss:Width=\"90\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"180\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"80\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"75\" ss:Span=\"%02d\"/>\n",my_session_number-1);

		  // Title
		  start_row(); string_cell(TABLE_VALUES_FIRST_COL,StyleID.BOLDCENTER_STYL_ID_16,title); end_row();

		  // Number of FAILs
		  start_row(NBFAILS_FIRST_ROW); string_cell(NBFAILS_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Num of FAILs");
		  for (int i = 0 ; i < my_session_number ; i++) {
			  output.printf("<Cell ss:Formula=\"=COUNTIF(R14C:R%dC,&quot;FAIL*&quot;)\"></Cell>\n",HEADER_NB_ROWS + rootdata.get_nb_max_data()) ;
		  }
		  end_row();

		  // Sessions
		  start_row(VERS_NUM_FIRST_ROW); string_cell(VERS_NUM_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Session");
		  if (monitor && sec==Sections.TEXT) string_cell(StyleID.BOLDCENTER_STYL_ID,"ARM Ref");
		  for (int j=0;j<rootdata.get_nb_sessions();j++) {
			  TestSession my_session=rootdata.get_session(j);
			  if(sqa_report.Cruise_Control) 	string_cell(StyleID.BOLDCENTER_STYL_ID,my_session.compiler_name);
			  else				string_cell(StyleID.BOLDCENTER_STYL_ID,my_session.name);
		  }
		  end_row();

		  // Tags
		  start_row();string_cell(VERS_NUM_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Tags");
		  if (monitor && (sec==Sections.TEXT || disc==Discriminent.SPEED_Vs_ARM)) string_cell(StyleID.BOLDCENTER_STYL_ID,"ARM Ref");
		  for (int j=0;j<rootdata.get_nb_sessions();j++) {
			  TestSession my_session=rootdata.get_session(j);
			  if(sqa_report.Cruise_Control) string_cell(StyleID.BOLDCENTER_STYL_ID,my_session.compiler_name);
			  else 			   string_cell(StyleID.BOLDCENTER_STYL_ID,my_session.name);
		  }
		  end_row();

		  // Empty line before values
		  start_row(HEADER_NB_ROWS); end_row();

		  // VALUES
		  for (int i=0;i<rootdata.get_disc().size();i++) {
			  RootTest my_test =  rootdata.get_disc().get(i);
			  start_row();
			  string_cell(my_test.get_test());
			  string_cell(my_test.get_target());
			  string_cell();
			  ListIterator<String> iter_elem = my_test.get_options().listIterator();
			  while(iter_elem.hasNext()) {
				  string_out(iter_elem.next());
				  string_out(" ");
			  }
			  string_cell_end();
			  for (int j=0;j<rootdata.get_nb_sessions();j++) {
				  TestSession my_session=rootdata.get_session(j);
				  long val;
				  if (disc!=Discriminent.SPEED) val = my_session.get_size(my_test,sec,disc,monitor);
				  else             val = my_session.get_cycle(my_test);
				  switch((int)val) {
				  case CommonData.NOT_EXECUTED: string_cell("(not executed)");break;
				  case CommonData.HAS_FAILED:   string_cell("FAIL(make error)");break;
				  case CommonData.NOT_RELEVANT: string_cell("Not Relevant.");break; 
				  case -1:           string_cell("No result.");break;
				  default:           number_cell(val); break;
				  }
			  }
			  end_row();
		  }
		  output.printf( "</Table><WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\"><Visible>SheetHidden</Visible><Zoom>75</Zoom></WorksheetOptions>\n");
		  apply_format(0,my_session_number,my_session_number);
		  output.printf( "</Worksheet>\n");
	  }
	  private void create_computed_page(RootDataClass rootdata,  String name, String title, String data_page, int mode, Discriminent disc, Sections sec, boolean monitor){
		  int nb_sessions_to_dump=rootdata.get_nb_sessions();
		  int i,j;
		  if (mode == 3 || (monitor && (sec==Sections.TEXT || disc==Discriminent.SPEED_Vs_ARM))) nb_sessions_to_dump ++;
		  output.printf( "<Worksheet  ss:Name=\"%s\">\n<Table>\n", name);
		  // Columns formatting
		  output.printf( "<Column ss:AutoFitWidth=\"0\" ss:Width=\"90\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"180\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"150\"/>\n<Column ss:AutoFitWidth=\"0\" ss:Width=\"75\" ss:Span=\"%02d\"/>\n",nb_sessions_to_dump-1);

		  // Title
		  start_row(); string_cell(TABLE_VALUES_FIRST_COL,StyleID.BOLDCENTER_STYL_ID_16,title); end_row();

		  // Number of FAILs
		  start_row(NBFAILS_FIRST_ROW); string_cell(NBFAILS_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Num of FAILs");
		  for (i = 0 ; i < nb_sessions_to_dump ; i++) {
			  output.printf("<Cell ss:Formula=\"=COUNTIF(R14C:R%dC,&quot;FAIL*&quot;)\"></Cell>\n",HEADER_NB_ROWS + rootdata.get_nb_max_data()) ;
		  }
		  end_row();

		  if (disc!=Discriminent.SPEED_Vs_ARM) {
			  // Average
			  start_row(STAT_FIRST_ROW); string_cell(TABLE_VALUES_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Average");
			  for (i = 0 ; i < nb_sessions_to_dump-1 ; i++) {
				  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=AVERAGE(R%dC:R%dC)\"></Cell>\n",style(StyleID.PERCENT_STYL_ID),HEADER_NB_ROWS+1, HEADER_NB_ROWS + rootdata.get_nb_max_data());	        
			  }
			  end_row();
			  // end Average

			  // Geomean
			  start_row(); string_cell(TABLE_VALUES_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Geomean");
			  for (i = 0 ; i < nb_sessions_to_dump-1 ; i++) {
				  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=GEOMEAN(R%dC:R%dC)\"></Cell>\n",style(StyleID.PERCENT_STYL_ID),HEADER_NB_ROWS+1, HEADER_NB_ROWS + rootdata.get_nb_max_data());	        
			  }
			  end_row();
			  // end Geomean

			  // Min
			  start_row(); string_cell(TABLE_VALUES_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Min");
			  for (i =0  ; i < nb_sessions_to_dump-1 ; i++) {
				  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=MIN(R%dC:R%dC)\"></Cell>\n",style(StyleID.PERCENT_STYL_ID),HEADER_NB_ROWS+1, HEADER_NB_ROWS + rootdata.get_nb_max_data());	        
			  }
			  end_row();
			  // end Min

			  // Max
			  start_row();string_cell(TABLE_VALUES_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Max");
			  for (i = 0 ; i < nb_sessions_to_dump-1 ; i++) {
				  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=MAX(R%dC:R%dC)\"></Cell>\n",style(StyleID.PERCENT_STYL_ID),HEADER_NB_ROWS+1, HEADER_NB_ROWS + rootdata.get_nb_max_data());	        
			  }
			  end_row();
			  // end Max


			  // MEDIAN
			  start_row();string_cell(TABLE_VALUES_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Median");
			  for (i = 0 ; i < nb_sessions_to_dump-1 ; i++) {
				  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=MEDIAN(R%dC:R%dC)\"></Cell>\n",style(StyleID.PERCENT_STYL_ID),HEADER_NB_ROWS+1, HEADER_NB_ROWS + rootdata.get_nb_max_data());	        
			  }
			  end_row();
			  // end MEDIAN

			  // Total
			  start_row();string_cell(TABLE_VALUES_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Total");
			  switch (mode) {
			  case 1:
				  for (j = 0 ; j < nb_sessions_to_dump-1 ; j++) {
					  int k=j;
					  if (monitor && (sec==Sections.TEXT||disc==Discriminent.SPEED_Vs_ARM)) k++;
					  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=sumif(R%dC%d:R%dC%d,&quot;&gt;0&quot;,%s!R%dC%d:%s!R%dC%d)/sumif(%s!R%dC%d:%s!R%dC%d,&quot;&gt;0&quot;,R%dC%d:R%dC%d)\"></Cell>\n",
							  style(StyleID.PERCENT_STYL_ID), 
							  HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL, 
							  data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+k, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+k, 
							  data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+k, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+k, 
							  HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL); 
				  }
				  break;
			  case 2:
				  for (i = 1 ; i < nb_sessions_to_dump ; i++) {
					  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=sumif(%s!R%dC%d:%s!R%dC%d,&quot;&gt;0&quot;,%s!R%dC%d:%s!R%dC%d)/sumif(%s!R%dC%d:%s!R%dC%d,&quot;&gt;0&quot;,%s!R%dC%d:%s!R%dC%d)\"></Cell>\n",
							  style(StyleID.PERCENT_STYL_ID), 
							  data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i-1, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i-1, 
							  data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i, 
							  data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i, 
							  data_page,HEADER_NB_ROWS+1, TABLE_VALUES_FIRST_COL+i-1, data_page, HEADER_NB_ROWS + rootdata.get_nb_max_data(), TABLE_VALUES_FIRST_COL+i-1); 
				  }
				  break;
			  case 3:
				  for (i = 0 ; i < nb_sessions_to_dump-1 ; i++) {
					  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=sumif(R%dC%d:R%dC%d,&quot;&gt;0&quot;,%s!R%dC%d:%s!R%dC%d)/sumif(%s!R%dC%d:%s!R%dC%d,&quot;&gt;0&quot;,R%dC%d:R%dC%d)\"></Cell>\n",
							  style(StyleID.PERCENT_STYL_ID), 
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
		  }
		  if (mode==3) {
			  nb_sessions_to_dump--;
			  start_row(VERS_NUM_FIRST_ROW-1);
			  string_cell(VERS_NUM_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Ref ID");
			  string_cell(" ");
			  for (i = 0 ; i < nb_sessions_to_dump ; i++) number_cell(StyleID.BOLDCENTER_STYL_ID,i+1);
			  end_row();
			  start_row();
			  string_cell(StyleID.BOLDRIGHT_STYL_ID,"Reference");
			  number_cell(StyleID.BOLDLEFT_STYL_ID,1);
		  } else {
			  start_row(VERS_NUM_FIRST_ROW);
		  }

		  string_cell(VERS_NUM_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Session");

		  if(mode == 3 ) output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=INDIRECT(ADDRESS(ROW(),3+R%dC2,1,TRUE,&quot;%s&quot;))\"></Cell>\n",style(StyleID.BOLDCENTER_STYL_ID),VERS_NUM_FIRST_ROW,data_page) ;

		  for (i = 0 ; i < nb_sessions_to_dump ; i++) {
			  if(mode != 3 && i == 0) {
				  number_cell(StyleID.BOLDCENTER_STYL_ID,i+1);
				  continue;
			  } 
			  string_cell_id(StyleID.BOLDCENTER_STYL_ID);
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
		  start_row();string_cell(VERS_NUM_FIRST_COL,StyleID.BOLDCENTER_STYL_ID,"Tags");
		  if (mode == 3 ) {
			  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=INDIRECT(ADDRESS(ROW(),3+R%dC2,1,TRUE,&quot;%s&quot;))\"></Cell>\n",style(StyleID.BOLDCENTER_STYL_ID),VERS_NUM_FIRST_ROW,data_page) ;
			  for (i=0;i<rootdata.get_nb_sessions();i++) {
				  TestSession my_session=rootdata.get_session(i);
				  if(sqa_report.Cruise_Control)  string_cell(StyleID.BOLDCENTER_STYL_ID,my_session.compiler_name);
				  else                string_cell(StyleID.BOLDCENTER_STYL_ID,my_session.name);
			  }      
		  } else {
			  for (i = 0 ; i < nb_sessions_to_dump ; i++)
				  output.printf("<Cell ss:Formula=\"=%s!RC%d\"></Cell>\n",data_page,TABLE_VALUES_FIRST_COL+i) ;
		  }
		  end_row();

		  // Empty line before values
		  start_row(HEADER_NB_ROWS); end_row();

		  // VALUES
		  ListIterator<RootTest> iter_disc =  rootdata.get_disc().listIterator();
		  while(iter_disc.hasNext()) {
			  RootTest my_test =  iter_disc.next();
			  int ref=-1;
			  boolean first=true;
			  start_row();
			  output.printf("<Cell ss:Formula=\"=%s!RC1\"></Cell>\n",data_page);
			  output.printf("<Cell ss:Formula=\"=%s!RC2\"></Cell>\n",data_page);
			  output.printf("<Cell ss:Formula=\"=%s!RC3\"></Cell>\n",data_page);

			  if(mode == 3) output.printf("<Cell ss:Formula=\"=INDIRECT(ADDRESS(ROW(),3+R%dC2,1,TRUE,&quot;%s&quot;))\"></Cell>\n",VERS_NUM_FIRST_ROW,data_page);
			  i=0;
			  if (monitor && (sec==Sections.TEXT||disc==Discriminent.SPEED_Vs_ARM)) {
				  first=false;
				  ref=0;
				  output.printf("<Cell ss:Formula=\"=%s!RC%d\"></Cell>\n",data_page,TABLE_VALUES_FIRST_COL) ;
				  i++;
			  }
			  for (int sessiter=0;sessiter<rootdata.get_nb_sessions();sessiter++) {
				  TestSession my_session=rootdata.get_session(sessiter);
				  //TDR
				  long value;
				  if (disc!=Discriminent.SPEED && disc!=Discriminent.SPEED_Vs_ARM) value = my_session.get_size(my_test,sec,disc,monitor);
				  else value = my_session.get_cycle(my_test);
				  if(disc==Discriminent.SPEED_Vs_ARM && is_normalized_test(my_test.get_test())) {
					  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=if(%s!RC%d=0,&quot;div by 0&quot;,((1000000*RC3)/%s!RC)/%s!RC%d)\"></Cell>\n",
							  style(StyleID.PERCENT_STYL_ID),
							  data_page,ref+TABLE_VALUES_FIRST_COL,
							  data_page,
							  data_page,ref+TABLE_VALUES_FIRST_COL);
					  continue;
				  } 
				  if (value != -1 && ref == -1) ref=i;
				  switch(mode) {
				  case 1:
				  case 2:
					  if (first) {
						  first=false;
						  switch((int)value) {
						  case CommonData.NOT_EXECUTED: string_cell("(not executed)");break;
						  case CommonData.HAS_FAILED: string_cell("FAIL(make error)");break;
						  case CommonData.NOT_RELEVANT: string_cell("Not Relevant.");break;
						  case -1: string_cell("No result.");break;
						  default: number_cell(value); break;
						  }
						  break;
					  }
					  if(ref == -1) {string_cell("-");break;}
					  if(ref == i) {string_cell("First Value");break;}
					  switch((int)value) {
					  case CommonData.NOT_EXECUTED: string_cell("(not executed)");break;
					  case CommonData.HAS_FAILED: string_cell("FAIL(make error)");break;
					  case CommonData.NOT_RELEVANT: string_cell("Not Relevant.");break;
					  case -1: string_cell("No result.");break;
					  default:
						  if(mode == 1) output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=if(%s!RC%d=0,&quot;div by 0&quot;,%s!RC/%s!RC%d)\"></Cell>\n",
								  style(StyleID.PERCENT_STYL_ID),
								  data_page,ref+TABLE_VALUES_FIRST_COL,
								  data_page,
								  data_page,ref+TABLE_VALUES_FIRST_COL);
						  else {
							  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=if(OR(%s!RC%d=0,T(%s!RC%d)&lt;&gt;&quot;&quot;,T(%s!RC)&lt;&gt;&quot;&quot;),&quot;- &quot;,%s!RC/%s!RC%d)\"></Cell>\n",
									  style(StyleID.PERCENT_STYL_ID), data_page,i-1+TABLE_VALUES_FIRST_COL,
									  data_page,i-1+TABLE_VALUES_FIRST_COL,data_page,
									  data_page,
									  data_page,i-1+TABLE_VALUES_FIRST_COL );
						  }
					  break;
					  }
					  break;
				  case 3:
					  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=if(T(%s!RC%d)&lt;&gt;&quot;&quot;,%s!RC%d,if(OR(RC%d=0,T(RC%d)&lt;&gt;&quot;&quot;,T(%s!RC%d)&lt;&gt;&quot;&quot;),&quot;- &quot;,%s!RC%d/RC%d))\"></Cell>\n",
							  style(StyleID.PERCENT_STYL_ID),data_page,TABLE_VALUES_FIRST_COL+i,data_page,TABLE_VALUES_FIRST_COL+i,TABLE_VALUES_FIRST_COL,TABLE_VALUES_FIRST_COL,data_page,TABLE_VALUES_FIRST_COL+i, data_page,TABLE_VALUES_FIRST_COL+i, TABLE_VALUES_FIRST_COL);
					  /*    	  output.printf("<Cell ss:StyleID=\"%s\" ss:Formula=\"=if(OR(RC%d=0,T(RC%d)&lt;&gt;&quot;&quot;,T(%s!RC%d)&lt;&gt;&quot;&quot;),&quot;- &quot;,%s!RC%d/RC%d)\"></Cell>\n",
		                 style(StyleID.PERCENT_STYL_ID),TABLE_VALUES_FIRST_COL,TABLE_VALUES_FIRST_COL,data_page,TABLE_VALUES_FIRST_COL+i, data_page,TABLE_VALUES_FIRST_COL+i,	TABLE_VALUES_FIRST_COL);
					   */break;
				  default:
					  System.err.println("Should not happened\n");
				  System.exit(2);

				  }
				  i++;
			  }
			  end_row();
		  }
		  int nb_max_data=rootdata.get_nb_max_data();
		  if(monitor && disc==Discriminent.SPEED) {
			  start_row();
			  string_cell("Developer Validation suite");
			  string_cell("Developer Validation suite");
			  string_cell("");
			  dump_cycle_summary_value(CommonData.Dump_Type.Run_Valid);
			  end_row();
			  nb_max_data++;
		  }
		  if(sqa_report.Monitoring && !monitor && disc==Discriminent.SIZE_OBJ) {
			  start_row();
			  string_cell("Developer Validation suite");
			  string_cell("Developer Validation suite");
			  string_cell("");
			  dump_size_summary_value(CommonData.Dump_Type.Run_Valid,disc);
			  end_row();
			  nb_max_data++;
		  }
		  //  output.printf( "</Table><WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\"><Visible>SheetHidden</Visible><Zoom>75</Zoom></WorksheetOptions>\n");
		  output.printf( "</Table><WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\"><Zoom>75</Zoom></WorksheetOptions>\n");
		  if (disc != Discriminent.SPEED_Vs_ARM) {
			  if (mode == 3) apply_format(1, nb_sessions_to_dump+1, nb_max_data);
			  else           apply_format(1, nb_sessions_to_dump, nb_max_data);
		  }
		  output.printf( "</Worksheet>\n");
	  }
	  
}
