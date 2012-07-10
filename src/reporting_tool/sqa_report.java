package reporting_tool;

import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import func_analyzer.*;
import size_analyzer.*;
import cycle_analyzer.*;
import info_analyzer.*;
import fail_analyzer.*;

public class sqa_report {

	public static int warn_level = 0;
	public static boolean core_only, ext_only;
	public static boolean Debug_on;
	public static boolean Hudson;
	public static boolean Aci;
    public static String  Aci_output_file_name = null;
	public static TestSession current_test_session;
	public static Discriminent current_parse_discriminent;
	static boolean obj_size = true;
	static boolean bin_size = true;
	static boolean func_size = true;
	static boolean cycle = true;
	static boolean sum_cycle = false ;
	static boolean compare_options = true;
	static boolean generate_output_info = true;
	private static int nb_tst = 0;
	private static String output_file_name = null;
	static String ref_file = null, key_name = null;
	static Sections section_for_summary = Sections.TEXT;
	static ArrayList<String> test_names = new ArrayList<String>();
	static RootDataClass rootdata = new RootDataClass();
	static ArrayList<Sections> sizes_for_computation = new ArrayList<Sections>();
	public static Summary summary = new Summary();
	private static Xls_Output output_xls;

	/**
	 * Help function
	 * @param
	 * @author thomas deruyter
	 */
	static void man() {
		System.out.println("Syntax is:\n\treport [Options] <log directory>+");
		System.out.println("Options are: ");
		System.out.println("\t-h | -help | --help : print this help message");
		System.out.println("\t-o <file> : redirect output to <file>");
		System.out.println("\t-i <option> : option to ignore");
		System.out.println("\t-noccopt : ignore all compiler options");
		System.out.println("\t-noobj : do not generate size sheet on object");
		System.out.println("\t-nobin : do not generate size sheet on binaries");
		System.out.println("\t-nocycles : do not generate size sheet on cycles");
		System.out.println("\t-noext : do not generate sheet on extensions");
		System.out.println("\t-nocore : do not generate sheet on core");
		System.out.println("\t-s <section> : size on this section, valid section are:");
		System.out.println("\t\ttext/data/rodata/bss/symtab/strtab/dbg/stxp70/total");
		System.out.println("\t\ttext_rodata : to combine text+rodata");
		System.out.println("\t-ref <fichier> : list of references directory to parse:");
		System.out.println("\t-key <name> : directory name to consider under referenced directory from ref option");
		System.out.println("\t-aci : generate a summary of performance in yaml format. By default, stdout is used");
		System.out.println("\t-aci-output <name> : specify a filename to output the aci summary");
		System.exit(0);
	}

	static void decode_section(String string) {
		if (string.contentEquals("text")) {
			sizes_for_computation.add(Sections.TEXT);
			sizes_for_computation.add(Sections.EXCEPT_TABLE);
			return;
		}
		if (string.contentEquals("data")) {
			sizes_for_computation.add(Sections.DATA);
			return;
		}
		if (string.contentEquals("rodata")) {
			sizes_for_computation.add(Sections.RODATA);
			return;
		}
		if (string.contentEquals("bss")) {
			sizes_for_computation.add(Sections.BSS);
			return;
		}
		if (string.contentEquals("symtab")) {
			sizes_for_computation.add(Sections.SYMTAB);
			return;
		}
		if (string.contentEquals("strtab")) {
			sizes_for_computation.add(Sections.STRTAB);
			return;
		}
		if (string.contentEquals("dbg")) {
			sizes_for_computation.add(Sections.DBG_SECTION);
			return;
		}
		if (string.contentEquals("rela_text")) {
			sizes_for_computation.add(Sections.RELA_TEXT);
			return;
		}
		if (string.contentEquals("rela_data")) {
			sizes_for_computation.add(Sections.RELA_DATA);
			return;
		}
		if (string.contentEquals("rela_rodata")) {
			sizes_for_computation.add(Sections.RELA_RODATA);
			return;
		}
		if (string.contentEquals("rela_dbg")) {
			sizes_for_computation.add(Sections.RELA_DBG_FRAME);
			return;
		}
		if (string.contentEquals("stxp70")) {
			sizes_for_computation.add(Sections.STXP70_EXTRECONF_INFO);
			return;
		}
		if (string.contentEquals("total")) {
			sizes_for_computation.add(Sections.TOTAL);
			;
			return;
		}
		if (string.contentEquals("text_rodata")) {
			sizes_for_computation.add(Sections.RODATA_PLUS_TEXT);
			;
			return;
		}
		System.err.println("ERROR, This size section/combination is not allowed\n");
		System.exit(1);

	}

	static void parse_arguments(String[] args) {
		int i = 0;
		while (i < args.length) {
			if (args[i].contentEquals("-o")) {
				if (i + 1 < args.length) {
					i++;
					output_file_name = args[i];
				}
			} else if (args[i].contentEquals("-h") || args[i].contentEquals("-help")
					|| args[i].contentEquals("--help")) {
				man();
				System.exit(0);
			} else if (args[i].contentEquals("-i")) {
				if (i + 1 < args.length) {
					i++;
					rootdata.add_ignored_flags(args[i]);
				}
			} else if (args[i].contentEquals("-s")) {
				if (i + 1 < args.length) {
					i++;
					decode_section(args[i]);
				}
			} else if (args[i].contentEquals("-ref")) {
				if (i + 1 < args.length) {
					i++;
					ref_file = args[i];
				}
			} else if (args[i].contentEquals("-key")) {
				if (i + 1 < args.length) {
					i++;
					key_name = args[i];
				}
			} else if (args[i].contentEquals("-Won")) {
				warn_level = 1;
			} else if (args[i].contentEquals("-noobj")) {
				obj_size = false;
			} else if (args[i].contentEquals("-nobin")) {
				bin_size = false;
			} else if (args[i].contentEquals("-nofunc")) {
				func_size = false;
			} else if (args[i].contentEquals("-nocycles")) {
				cycle = false;
			} else if (args[i].contentEquals("-noccopt")) {
				compare_options = false;
			} else if (args[i].contentEquals("-noext")) {
				core_only = true;
			} else if (args[i].contentEquals("-nocore")) {
				ext_only = true;
			} else if (args[i].contentEquals("-dbg")) {
				Debug_on = true;
			} else if (args[i].contentEquals("-hudson")) {
				Hudson = true;
				compare_options = false;
				sizes_for_computation.add(Sections.TEXT);
			} else if (args[i].contentEquals("-aci")) {
				Aci = true;
				compare_options = false;
				sizes_for_computation.add(Sections.TEXT);
                        } else if (args[i].contentEquals("-aci-output")) {
                                if (i + 1 < args.length) {
                                        i++;
                                        Aci_output_file_name = args[i];
                                }
			} else if (args[i].contentEquals("-monitor")) {
				//Monitoring=true;
				generate_output_info = false;
				compare_options = false;
				sizes_for_computation.add(Sections.RODATA_PLUS_TEXT);
				ref_file = "References.txt";
				obj_size = false;
				bin_size = false;
				cycle = false;
				System.out.println("Warning option -monitor not yet implemented");
				System.exit(0);
			} else if (args[i].contentEquals("-default")) {
				compare_options = false;
				sizes_for_computation.add(Sections.TEXT);
				ref_file = "References.txt";
			} else if (args[i].contentEquals("-nightly")) {
				compare_options = false;
				sizes_for_computation.add(Sections.RODATA_PLUS_TEXT);
				section_for_summary = Sections.RODATA_PLUS_TEXT;
				ref_file = "References.txt";
			} else {
				test_names.add(args[i]);
				nb_tst++;
			}
			i++;
		}
	}

	static void treat_ref_file() {
		if (key_name == null && nb_tst > 1) {
			System.err.println("ERROR, When ref file and several command line tests dirs, a key is needed\n");
			System.exit(1);
		}
		if (key_name == null) {
			key_name = test_names.get(0);
		}
		try {
			BufferedReader input = new BufferedReader(new FileReader(ref_file));
			try {
				String line = null; //not declared within while loop
				while ((line = input.readLine()) != null) {
					String session_name = line;
					//Remove trailing spaces
					while ((line.endsWith(" ") || line.endsWith("\t")) && line.length() > 0) {
						line = line.substring(0, line.length() - 1);
					}
					if (line.length() == 0) continue;
					if (line.endsWith("/")) {
						System.out.println("Line contains / at end :" + line);
						line = line.substring(0, line.length() - 1);
					}
					if (line.lastIndexOf("/") > 0) {
						session_name = line.substring(line.lastIndexOf("/") + 1, line.length());
					}
					line = line + "/" + key_name;
					if (Hudson || Aci) {
						try {
							FileReader myfile = new FileReader(line + "/BANNER");
							rootdata.add_session(line, session_name);
							myfile.close();
						} catch (FileNotFoundException e) {
						}
					} else {
						try {
							FileReader myfile = new FileReader(line + "/INFO");
							rootdata.add_session(line, session_name);
							myfile.close();
						} catch (FileNotFoundException e) {
							System.err.println("WARNING: Directory " + line + " does not contain any INFO");
						}
					}
				}
			} finally {
				input.close();
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	static void parse_session(TestSession current_session) {
		String file_to_parse = current_session.path + "/BANNER";
		current_test_session = current_session;
		Info_Lexer iscanner = null;
		try {
			if (Debug_on) {
				System.out.println("Parse File : " + file_to_parse);
			}
			iscanner = new Info_Lexer(new FileReader(file_to_parse));
			try {
				Info_Parser p = new Info_Parser(iscanner);
				p.parse();
			} catch (IOException er) {
				System.out.println("An I/O error occured while parsing : \n" + er);
				System.exit(1);
			}
		} catch (FileNotFoundException e) {
			file_to_parse = current_session.path + "/INFO";
			try {
				if (Debug_on) {
					System.out.println("Parse File : " + file_to_parse);
				}
				iscanner = new Info_Lexer(new FileReader(file_to_parse));
				try {
					Info_Parser p = new Info_Parser(iscanner);
					p.parse();
				} catch (IOException er) {
					System.out.println("An I/O error occured while parsing : \n" + er);
					System.exit(1);
				}
			} catch (FileNotFoundException er) {
				if (!Hudson && !Aci) {
					System.out.println("Unable to find " + file_to_parse);
				}
			} catch (IOException er) {
				e.printStackTrace();
			} catch (Exception er) {
				e.printStackTrace();
			}
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
		file_to_parse = current_session.path + "/FAILED";
		try {
			if (Debug_on) {
				System.out.println("Parse File : " + file_to_parse);
			}
			Fail_Lexer scanner = new Fail_Lexer(new FileReader(file_to_parse));
			try {
				Fail_Parser p = new Fail_Parser(scanner);
				p.parse();
			} catch (IOException er) {
				System.out.println("An I/O error occured while parsing : \n" + er);
				System.exit(1);
			}
		} catch (FileNotFoundException e) {
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
		if (obj_size) {
			current_parse_discriminent = Discriminent.SIZE_OBJ;
			file_to_parse = current_session.path + "/codeSize.txt";
			try {
				if (Debug_on) {
					System.out.println("Parse File : " + file_to_parse);
				}
				Size_Lexer scanner = new Size_Lexer(new FileReader(file_to_parse));
				try {
					Size_Parser p = new Size_Parser(scanner);
					p.parse();
				} catch (IOException er) {
					System.out.println("An I/O error occured while parsing : \n" + er);
					System.exit(1);
				}
			} catch (FileNotFoundException e) {
				if (!Hudson && !Aci) {
					System.out.println("Unable to find" + file_to_parse);
				}
			} catch (IOException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		if (bin_size) {
			current_parse_discriminent = Discriminent.SIZE_BIN;
			file_to_parse = current_session.path + "/BinaryCodeSize.txt";
			try {
				if (Debug_on) {
					System.out.println("Parse File : " + file_to_parse);
				}
				Size_Lexer scanner = new Size_Lexer(new FileReader(file_to_parse));
				try {
					Size_Parser p = new Size_Parser(scanner);
					p.parse();
				} catch (IOException er) {
					System.out.println("An I/O error occured while parsing : \n" + er);
					System.exit(1);
				}
			} catch (FileNotFoundException e) {
				if (!Hudson && !Aci) {
					System.out.println("Unable to find" + file_to_parse);
				}
			} catch (IOException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		if (func_size) {
			file_to_parse = current_session.path + "/BinaryFuncSize.txt";
			try {
				if (Debug_on) {
					System.out.println("Parse File : " + file_to_parse);
				}
				Func_Lexer scanner = new Func_Lexer(new FileReader(file_to_parse));
				try {
					Func_Parser p = new Func_Parser(scanner);
					p.parse();
				} catch (IOException er) {
					System.out.println("An I/O error occured while parsing : \n" + er);
					System.exit(1);
				}
			} catch (FileNotFoundException e) {
				if (!Hudson && !Aci) {
					System.out.println("Unable to find" + file_to_parse);
				}
			} catch (IOException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		if (cycle) {
			file_to_parse = current_session.path + "/cyclesCount.txt";
			try {
				if (Debug_on) {
					System.out.println("Parse File : " + file_to_parse);
				}
				Cycle_Lexer scanner = new Cycle_Lexer(new FileReader(file_to_parse));
				try {
					Cycle_Parser p = new Cycle_Parser(scanner);
					p.parse();
				} catch (IOException er) {
					System.out.println("An I/O error occured while parsing : \n" + er);
					System.exit(1);
				}
			} catch (FileNotFoundException e) {
				if (!Hudson && !Aci) {
					System.out.println("Unable to find" + file_to_parse);
				}
			} catch (IOException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	static void generate_text_summary() {
		if (Debug_on) {
			System.out.println("generate_text_summary #1");
		}
		if (obj_size) {
			rootdata.compute_data(Discriminent.SIZE_OBJ);
			ArrayList<RootTest> my_tests = rootdata.get_disc();
			for (int k = 0; k < my_tests.size(); k++) {
				for (int j = 0; j < rootdata.get_nb_sessions(); j++) {
					summary.add_summary_value(rootdata.get_session(j).path, rootdata.get_session(j).name,
							my_tests.get(k).get_test(), Discriminent.SIZE_OBJ,
							rootdata.get_session(j).get_size(my_tests.get(k),
									section_for_summary, Discriminent.SIZE_OBJ, false));
				}
			}
		}
		if (Debug_on) {
			System.out.println("generate_text_summary #2");
		}
		if (bin_size) {
			rootdata.compute_data(Discriminent.SIZE_BIN);
			ArrayList<RootTest> my_tests = rootdata.get_disc();
			for (int k = 0; k < my_tests.size(); k++) {
				for (int j = 0; j < rootdata.get_nb_sessions(); j++) {
					summary.add_summary_value(rootdata.get_session(j).path,
							rootdata.get_session(j).name,
							my_tests.get(k).get_test(),
							Discriminent.SIZE_BIN,
							rootdata.get_session(j).get_size(my_tests.get(k),
									section_for_summary, Discriminent.SIZE_BIN, false));
				}
			}
		}
		if (Debug_on) {
			System.out.println("generate_text_summary #3");
		}
		if (func_size) {
			rootdata.compute_data(Discriminent.SIZE_FUNC);
			ArrayList<RootTest> my_tests = rootdata.get_disc();
			for (int k = 0; k < my_tests.size(); k++) {
				for (int j = 0; j < rootdata.get_nb_sessions(); j++) {
					summary.add_summary_value(rootdata.get_session(j).path,
							rootdata.get_session(j).name,
							my_tests.get(k).get_test(),
							Discriminent.SIZE_FUNC,
							rootdata.get_session(j).get_size(my_tests.get(k),
									section_for_summary, Discriminent.SIZE_FUNC, false));
				}
			}
		}
		if (Debug_on) {
			System.out.println("generate_text_summary #4");
		}
		if (cycle) {
			if (Debug_on) {
				System.out.println("generate_text_summary #4.1");
			}
			rootdata.compute_data(Discriminent.SPEED);
			if (Debug_on) {
				System.out.println("generate_text_summary #4.2");
			}
			rootdata.compute_data(Discriminent.SPEED);
			ArrayList<RootTest> my_tests = rootdata.get_disc();
			for (int k = 0; k < my_tests.size(); k++) {
				for (int j = 0; j < rootdata.get_nb_sessions(); j++) {
					summary.add_summary_value(rootdata.get_session(j).path,
							rootdata.get_session(j).name,
							my_tests.get(k).get_test(),
							Discriminent.SPEED,
							rootdata.get_session(j).get_cycle(my_tests.get(k),Discriminent.SPEED));
				}
			}
			if (Debug_on) {
				System.out.println("generate_text_summary #4.3");
			}
		}
		if (Debug_on) {
			System.out.println("generate_text_summary #5");
		}
	}

	/**
	 * Main entry point
	 * @param args : Command line Argument
	 * @author thomas deruyter
	 */
	public static void main(String[] args) {
		int i;
		/*Useless flag to ignore*/
		rootdata.add_ignored_flags("-keep");
		rootdata.add_ignored_flags("-Mkeepasm");
		rootdata.add_ignored_flags("-fno-verbose-asm");
		Debug_on = false;
		if (args.length == 0) {
			man();
			System.exit(0);
		}
		parse_arguments(args);
		/*Ref treatment*/
		if (ref_file != null) {
			treat_ref_file();
		}
		for (i = 0; i < test_names.size(); i++) {
			rootdata.add_session(test_names.get(i), test_names.get(i));
		}
		/*Parsing phase*/
		for (i = 0; i < rootdata.get_nb_sessions(); i++) {
			parse_session(rootdata.get_session(i));
		}
		/*End Parsing Phase*/

		if (Debug_on) {
			System.out.println("Parsing phase done");
		}
		/*Start Processing phase*/
		if (output_file_name == null) {
                    output_file_name = "default_output.xls";
                }
                output_xls = new Xls_Output(output_file_name);
		output_xls.generate_header();
		output_xls.create_summary(rootdata);
		if (Debug_on) {
			System.out.println("Summary phase done");
		}

		rootdata.remove_ignored_flags(compare_options);
		if (sizes_for_computation.isEmpty()) {
			sizes_for_computation.add(Sections.TEXT);
		}

		for (i = rootdata.get_nb_sessions() - 1; i >= 0; i--) {
			rootdata.get_session(i);
			summary.add_session(rootdata.get_session(i).path, rootdata.get_session(i).name);
		}

		if (Debug_on) {
			System.out.println("Compute1 phase done");
		}
		/* Generate summary */
		generate_text_summary();
		if (Debug_on) {
			System.out.println("Summary text phase done");
		}

		/*Size sheet generation*/
		if (obj_size) {
			rootdata.compute_data(Discriminent.SIZE_OBJ);
			for (i = 0; i < sizes_for_computation.size(); i++) {
				output_xls.create_new_page(rootdata, Discriminent.SIZE_OBJ, sizes_for_computation.get(i));
			}
		}
		if (Debug_on) {
			System.out.println("Obj phase done");
		}

		if (bin_size) {
			rootdata.compute_data(Discriminent.SIZE_BIN);
			for (i = 0; i < sizes_for_computation.size(); i++) {
				output_xls.create_new_page(rootdata, Discriminent.SIZE_BIN, sizes_for_computation.get(i));
				}
		}

		if (Debug_on) {
			System.out.println("Bin phase done");
		}
		if (func_size) {
			rootdata.compute_data(Discriminent.SIZE_FUNC);
			for (i = 0; i < sizes_for_computation.size(); i++) {
				output_xls.create_new_page(rootdata, Discriminent.SIZE_FUNC, sizes_for_computation.get(i));
			}
			rootdata.compute_data(Discriminent.SIZE_APPLI);
			for (i = 0; i < sizes_for_computation.size(); i++) {
				output_xls.create_new_page(rootdata, Discriminent.SIZE_APPLI, sizes_for_computation.get(i));
			}
		}
		if (Debug_on) {
			System.out.println("Func phase done");
		}

		/*Speed sheet generation*/
		if (cycle) {
			if (Debug_on) {
				System.out.println("generate cycle #1");
			}
			rootdata.compute_data(Discriminent.SPEED);
			if (Debug_on) {
				System.out.println("generate cycle #2");
			}
			output_xls.create_new_page(rootdata, Discriminent.SPEED, Sections.LAST_SECTION);
			if (Debug_on) {
				System.out.println("generate cycle #3");
			}
		}
		if (sum_cycle) {
			rootdata.compute_data(Discriminent.SPEED_APPLI);
			if (Debug_on) {
				System.out.println("generate cycle #4");
			}
			output_xls.create_new_page(rootdata, Discriminent.SPEED_APPLI, Sections.LAST_SECTION);
		}
		if (Debug_on) {
			System.out.println("Cycles phase done");
		}
		output_xls.excel_terminate();
		if (generate_output_info) {
			if (Hudson) {
				String tmp_str = output_file_name;
				if (output_file_name.lastIndexOf("/") > 0) {
					tmp_str = output_file_name.substring(output_file_name.lastIndexOf("/") + 1, output_file_name.length());
				}
				if (ref_file != null && ref_file.contains("branch")) {
					summary.dump_hudson_summary(null, tmp_str);
				} else if (ref_file != null) {
					summary.dump_hudson_summary(null, tmp_str);
				} else {
					summary.dump_hudson_summary(output_file_name.substring(output_file_name.lastIndexOf("/") + 1,output_file_name.lastIndexOf(".")), tmp_str);
				}
			} else if (Aci) {
				String tmp_str = output_file_name;
				if (output_file_name.lastIndexOf("/") > 0) {
                                    tmp_str = output_file_name.substring(output_file_name.lastIndexOf("/") + 1, output_file_name.length());
				}
				if (ref_file != null && ref_file.contains("branch")) {
					summary.dump_aci_summary(null, tmp_str, Aci_output_file_name);
				} else if (ref_file != null) {
					summary.dump_aci_summary(null, tmp_str, Aci_output_file_name);
				} else {
					summary.dump_aci_summary(output_file_name.substring(output_file_name.lastIndexOf("/") + 1,output_file_name.lastIndexOf(".")), tmp_str, Aci_output_file_name);
				}
            } else {
				summary.dump_summary(false, "");
			}
		}
	}
}
