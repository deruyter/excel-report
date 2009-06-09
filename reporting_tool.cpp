// reporting_tool.cpp : Defines the entry point for the console application.
//

#include "data.hpp"
#include "xml_dump.hpp"
#include "common_parse.hpp"

TestSession *current_session;
int size[LAST_SECTION];
int warn_level;
bool core_only, ext_only;
bool Debug_on;
bool Cruise_Control;
bool Monitoring;
SummaryClass summary;

void man() {
	printf("Syntax is:\n\treport [Options] <log directory>+\n ");
	printf("Options are:\n ");
	printf("\t-h | -help | --help : print this help message\n ");
	printf("\t-o <file> : redirect output to <file>\n ");
	printf("\t-i <option> : option to ignore\n");
	printf("\t-noccopt : ignore all compiler options\n");
	printf("\t-noobj : do not generate size sheet on object\n");
	printf("\t-nobin : do not generate size sheet on binaries\n");
	printf("\t-nocycles : do not generate size sheet on cycles\n");
	printf("\t-noext : do not generate sheet on extensions\n");
	printf("\t-nocore : do not generate sheet on core\n");
	printf("\t-s <section> : size on this section, valid section are: \n");
	printf("\t\ttext/data/rodata/bss/symtab/strtab/dbg/stxp70/total\n");
	printf("\t\ttext_rodata : to combine text+rodata\n");
	printf("\t-ref <fichier> : list of references directory to parse: \n");
	printf("\t-key <name> : directory name to consider under referenced directory from ref option \n");
	exit(0);
}

void decode_section(char *n) {
	if (!strcmp(n, "text")) {
		size[TEXT]=1;
		return;
	}
	if (!strcmp(n, "data")) {
		size[DATA]=1;
		return;
	}
	if (!strcmp(n, "rodata")) {
		size[RODATA]=1;
		return;
	}
	if (!strcmp(n, "bss")) {
		size[BSS]=1;
		return;
	}
	if (!strcmp(n, "symtab")) {
		size[SYMTAB]=1;
		return;
	}
	if (!strcmp(n, "strtab")) {
		size[STRTAB]=1;
		return;
	}
	if (!strcmp(n, "dbg")) {
		size[DBG_SECTION]=1;
		return;
	}
	if (!strcmp(n, "rela_text")) {
		size[RELA_TEXT]=1;
		return;
	}
	if (!strcmp(n, "rela_data")) {
		size[RELA_DATA]=1;
		return;
	}
	if (!strcmp(n, "rela_rodata")) {
		size[RELA_RODATA]=1;
		return;
	}
	if (!strcmp(n, "rela_dbg")) {
		size[RELA_DBG_FRAME]=1;
		return;
	}
	if (!strcmp(n, "stxp70")) {
		size[STXP70_EXTRECONF_INFO]=1;
		return;
	}
	if (!strcmp(n, "total")) {
		size[TOTAL]=1;
		return;
	}
	if (!strcmp(n, "text_rodata")) {
		size[RODATA_PLUS_TEXT]=1;
		return;
	}
	fprintf(stderr,"ERROR, This size section/combination is not allowed\n");
	exit(1);
}

int main(int argc, char* argv[]) {
	int i;
	Excel_Output *output=NULL;
	char filename[1024];
	char *ref_file=NULL, *key_name=NULL;
	RootDataClass rootdata=RootDataClass();
	summary=SummaryClass();
	bool obj_size = true;
	bool bin_size = true;
	bool func_size = true;
	bool cycle = true;
	bool compare_options = true;
	bool generate_output_info = true;
	int nb_tst=0;
	Section section_for_summary=TEXT;
	Cruise_Control=false;
	Monitoring=false;
	NameList *test_names = new NameList;
	warn_level=1;
	core_only=false;
	ext_only=false;
	for (i=0; i<LAST_SECTION; i++)
		size[i]=0;
	/*Useless flag to ignore*/
	rootdata.add_ignored_flags("-keep");
	rootdata.add_ignored_flags("-Mkeepasm");
	rootdata.add_ignored_flags("-fno-verbose-asm");
	Debug_on=false;
	if (argc==1) {
		man();
		exit(0);
	}

	while (--argc > 0) {
		++argv;
		if (!strcmp((*argv), "-o")) {
			if (--argc > 0) {
				++argv;
				output = new Excel_Output(*argv);
			}
		} else if (!strcmp((*argv), "-h") || !strcmp((*argv), "-help")
				|| !strcmp((*argv), "--help")) {
			man();
			exit(0);
		} else if (!strcmp((*argv), "-i")) {
			if (--argc > 0) {
				++argv;
				rootdata.add_ignored_flags(*argv);
			}
		} else if (!strcmp((*argv), "-s")) {
			if (--argc > 0) {
				++argv;
				decode_section(*argv);
			}
		} else if (!strcmp((*argv), "-ref")) {
			if (--argc > 0) {
				++argv;
				ref_file=strdup(*argv);
			}
		} else if (!strcmp((*argv), "-key")) {
			if (--argc > 0) {
				++argv;
				key_name=strdup(*argv);
			}
		} else if (!strcmp((*argv), "-Woff")) {
			warn_level=0;
		} else if (!strcmp((*argv), "-noobj")) {
			obj_size=false;
		} else if (!strcmp((*argv), "-nobin")) {
			bin_size=false;
		} else if (!strcmp((*argv), "-nofunc")) {
			func_size=false;
		} else if (!strcmp((*argv), "-nocycles")) {
			cycle=false;
		} else if (!strcmp((*argv), "-noccopt")) {
			compare_options=false;
		} else if (!strcmp((*argv), "-noext")) {
			core_only=true;
		} else if (!strcmp((*argv), "-nocore")) {
			ext_only=true;
		} else if (!strcmp((*argv), "-dbg")) {
			Debug_on=true;
		} else if (!strcmp((*argv), "-cruisec")) {
			Cruise_Control=true;
            warn_level=0;
            compare_options=false;
            size[TEXT]=1;
        } else if (!strcmp((*argv), "-monitor")) {
            Monitoring=true;
            generate_output_info=false;
            warn_level=0;
            compare_options=false;
            size[RODATA_PLUS_TEXT]=1;
            ref_file=strdup("References.txt");
            obj_size=false;
            bin_size=false;
            cycle=false;
        } else if (!strcmp((*argv), "-default")) {
            warn_level=0;
            compare_options=false;
            size[TEXT]=1;
            ref_file=strdup("References.txt");         
        } else if (!strcmp((*argv), "-nightly")) {
            warn_level=0;
            compare_options=false;
            size[RODATA_PLUS_TEXT]=1;
			section_for_summary=RODATA_PLUS_TEXT;
            ref_file=strdup("References.txt");         
        } else {
			test_names->push_back(strdup(*argv));
			nb_tst++;
		}
	}

	/*Ref treatment*/
	if (ref_file) {
		if (!key_name)
			if (nb_tst > 1) {
				fprintf(stderr,"ERROR, When ref file and several command line tests dirs, a key is needed\n");
				exit(1);
			}
		key_name = strdup(*(test_names->begin()));
		FILE *my_ref_file = fopen(ref_file, "r");
		if (my_ref_file==NULL) {
			fprintf(stderr,"ERROR, Specified ref file does not exist\n");
			exit(1);
		}
		char tmp_name[2048];
		char *session_name;
		char *pt;
		while (!feof(my_ref_file)) {
			fscanf(my_ref_file, "%s\n", tmp_name);
			if (tmp_name[strlen(tmp_name)-1]=='/')
				tmp_name[strlen(tmp_name)-1]='\0';

			pt=strrchr(tmp_name, '/');
			if (pt) {
				pt++;
				session_name=strdup(pt);
			} else {
				session_name=strdup(tmp_name);
			}
			strcat(tmp_name, "/");
			strcat(tmp_name, key_name);
			// TDR - Check info_....
			sprintf(filename, "%s/INFO", tmp_name);
			FILE *my_tmp_file = fopen(filename, "r");
			if (!my_tmp_file) {
				if (warn_level)
					fprintf(stderr,"WARNING: Directory %s does not contain any info\n",tmp_name);
			} else {
				rootdata.add_session(tmp_name, session_name);
				fclose(my_tmp_file);
			}
		}
		fclose(my_ref_file);
	}
	ForEachPt(test_names,iter_name) {
		rootdata.add_session(*iter_name, *iter_name);
	}
	/**/
	
	ForEachPt(rootdata.get_sessions(),iter_session) {
		current_session = *iter_session;
		char tmp_name[2048];
		sprintf(filename,"%s/BANNER",current_session->get_path());
		if (fopen(filename,"r")) {   
		    if (Debug_on) printf("Parse BANNER : %s, %d\n",filename,__LINE__);
		    parse_info(filename);
		} else {
		    sprintf(filename,"%s/INFO",current_session->get_path());
		    if (Debug_on) printf("Parse INFO : %s, %d\n",filename,__LINE__);
		    parse_info(filename);
		}
		sprintf(filename,"%s/FAILED",current_session->get_path());
		if (Debug_on) printf("Parse FAIL : %s, %d\n",filename,__LINE__);
		parse_fail(filename);
		if(obj_size || Monitoring) {
			sprintf(filename,"%s/codeSize.txt",current_session->get_path());
			if (Debug_on) printf("Parse Codesize : %s, %d\n",filename,__LINE__);
			parse_size(filename,SIZE_OBJ);
		}
		if(bin_size || Monitoring) {
			sprintf(filename,"%s/BinaryCodeSize.txt",current_session->get_path());
			if (Debug_on) printf("Parse Binary Codesize : %s, %d\n",filename,__LINE__);
			parse_size(filename,SIZE_BIN);
		}
		if(func_size || Monitoring) {
			sprintf(filename,"%s/BinaryFuncSize.txt",current_session->get_path());
			if (Debug_on) printf("Parse Function Codesize : %s, %d\n",filename,__LINE__);
			parse_funcsize(filename);
		}
		if(cycle || Monitoring) {
			sprintf(filename,"%s/cyclesCount.txt",current_session->get_path());
			if (Debug_on) printf("Parse Cycles : %s, %d\n",filename,__LINE__);
			parse_cycle(filename);
		}
	}

	if (!output) 	output = new Excel_Output("default_output.xls");

	output->generate_header();
	output->create_summary(rootdata);
	rootdata.remove_ignored_flags(compare_options);

	for (i=0; i<LAST_SECTION; i++) {
		if (size[i])
			break;
	}
	if (i==LAST_SECTION)
		size[TEXT]=1;

	ForEachRPt(rootdata.get_sessions(),iter_session) {
		summary.add_session((*iter_session)->get_path(), (*iter_session)->get_name());
	}
	
	/* For a better summary*/
    if (obj_size||Monitoring) {
    	rootdata.compute_data(SIZE_OBJ);
		ForEachPt(rootdata.get_disc(),iter) {
			ForEachPt(rootdata.get_sessions(),iter_session) {
				summary.add_summary_value((*iter_session)->get_path(),(*iter_session)->get_name(),(*iter)->get_test(),SIZE_OBJ,(*iter_session)->get_size(*iter,section_for_summary,SIZE_OBJ,false));
			}
		}
	}
    if (bin_size||Monitoring) {
    	rootdata.compute_data(SIZE_BIN);
		ForEachPt(rootdata.get_disc(),iter) {
			ForEachPt(rootdata.get_sessions(),iter_session) {
				summary.add_summary_value((*iter_session)->get_path(),(*iter_session)->get_name(),(*iter)->get_test(),SIZE_BIN,(*iter_session)->get_size(*iter,section_for_summary,SIZE_BIN,false));
			}
		}
    }
    if (func_size||Monitoring) {
    	rootdata.compute_data(SIZE_FUNC);
		ForEachPt(rootdata.get_disc(),iter) {
			ForEachPt(rootdata.get_sessions(),iter_session) {
				summary.add_summary_value((*iter_session)->get_path(),(*iter_session)->get_name(),(*iter)->get_test(),SIZE_FUNC,(*iter_session)->get_size(*iter,section_for_summary,SIZE_FUNC,false));
			}
		}
    }
    if (cycle||Monitoring) {
    	rootdata.compute_data(SPEED);
		ForEachPt(rootdata.get_disc(),iter) {
			ForEachPt(rootdata.get_sessions(),iter_session) {
				summary.add_summary_value((*iter_session)->get_path(),(*iter_session)->get_name(),(*iter)->get_test(),SPEED,(*iter_session)->get_cycle(*iter));
			}
		}
    }

	/* End Summary session*/

	/*Monitoring Session*/
	if (Monitoring) {
	    // Parse set_arm_ref_value();
        rootdata.compute_monitored_data(SIZE_OBJ);
        output->create_monitored_page(rootdata, SIZE_OBJ);
        rootdata.compute_monitored_data(SIZE_BIN);
        output->create_monitored_page(rootdata, SIZE_BIN);
        rootdata.compute_monitored_data(SIZE_FUNC);
        output->create_monitored_page(rootdata, SIZE_FUNC);
        rootdata.compute_monitored_data(SPEED);
        output->create_monitored_page(rootdata, SPEED);
        rootdata.compute_monitored_data(SPEED_Vs_ARM);
        output->create_monitored_page(rootdata, SPEED_Vs_ARM);
	}
	
	/*Size sheet generation*/
	if (obj_size) {
		rootdata.compute_data(SIZE_OBJ);
		for (i=0; i<LAST_SECTION; i++) {
			if (size[i]) output->create_page(rootdata, SIZE_OBJ, (Section)i);
		}
	}

	if (bin_size) {
		rootdata.compute_data(SIZE_BIN);
		for (i=0; i<LAST_SECTION; i++) {
			if (size[i])  output->create_page(rootdata, SIZE_BIN, (Section)i);
		}
	}

	if (func_size) {
		rootdata.compute_data(SIZE_FUNC);
		for (i=0; i<LAST_SECTION; i++) {
			if (size[i])  output->create_page(rootdata, SIZE_FUNC, (Section)i);
		}
		rootdata.compute_data(SIZE_APPLI);
		for (i=0; i<LAST_SECTION; i++) {
			if (size[i])  output->create_page(rootdata, SIZE_APPLI, (Section)i);
		}
	}

	/*Speed sheet generation*/
	if (cycle) {
		rootdata.compute_data(SPEED);
		output->create_page(rootdata, SPEED, LAST_SECTION);
	}


	output->excel_terminate();

	if (generate_output_info) {
    	if(Cruise_Control) {
    	    if(strstr(ref_file,"branch")!=NULL)    summary.dump_summary_cc("branch");
    	    else                                   summary.dump_summary_cc("ref");
    	}
		else summary.dump_summary();
	}

	return 0;
}

