#include "data.hpp"
#include <ctype.h>


static char *compute_test_target_name(char *name, char **target);
static char *compute_size_test_target_name(char *name, char *tname ,char **target);


bool is_extension_info(char *name) {
  if (!strncmp(name,"TSTx_test",9)               || !strncmp(name,"vx2_fgtdec",10)   ||
      !strncmp(name,"vx2_test_memspace",17)      || !strncmp(name,"TS3x_test",9)    ||
      !strncmp(name,"admX_AppliUsingadmX",19)    || !strncmp(name,"TS2x_test",9)    ||
      !strncmp(name,"QMx_farrow_interpolator",23)|| !strncmp(name,"QMx_mixer",9)   ||
      !strncmp(name,"Tx_CSD_extension_test",21)  || !strncmp(name,"MP1v_viterbi",12) ||
      !strncmp(name,"TS4x_test",9) 				 || !strncmp(name,"Fx_SampleRate",13))  return true;
  return false;
}


DateType::DateType(short d,short m,short y,short h,short min,short sec) {
  _year=y;
  _month=m;
  _day=d;
  _hour=h;
  _min=min;
  _sec=sec;
}


/*****************************************/
/*               DateType                */
/*****************************************/

char *DateType::print_date() {
  char *name =(char *) malloc(20);
  sprintf(name,"%2d/%2d/%4d",_month,_day,_year);
  return name;
}

char *DateType::print_time() {
  char *name =(char *) malloc(20);
  sprintf(name,"%2d:%2d:%2d",_hour,_min,_sec);
  return name;
}


/*****************************************/
/*                Target                 */
/*****************************************/

Target::Target(char *name){
  _name=strdup(name);
  for (int i=0;i<LAST_SECTION;i++) _size[i]=0;
  _cycles=-1;
}

int Target::get_size(Section section) { 
  if(section < LAST_SECTION) { 
    if (section == RODATA_PLUS_TEXT )  return _size[TEXT]+_size[RODATA];
    return _size[section];
  } 
  fprintf(stderr,"ERROR ! should not happen\n");
  return -1;
}




/*****************************************/
/*                 Test                  */
/*****************************************/

Test::Test(char *name) {
  _name=strdup(name);
  _targets=new TargetList;
  _current_target=NULL;
  _nb_target = 0;
}

Target *Test::find_target(char *name) {
  if(!_current_target || strcmp(_current_target->get_name(),name)) {
    _current_target=NULL;
    ForEachPt(_targets,iter) {
      if (!strcmp((*iter)->get_name(),name)) {
	_current_target=*iter;
	return _current_target;
      }
    }
  }
  return _current_target;
}

Target *Test::add_target(char *name) {
  _current_target=new Target(name);
  _targets->push_back(_current_target);
  return _current_target;
}



/*****************************************/
/*              TestSession              */
/*****************************************/

TestSession::TestSession(char *path, char *name) {
  _obj_size_tests=new TestList;
  _bin_size_tests=new TestList;
  _func_size_tests=new TestList;
  _speed_tests=new TestList;
  _compiler_flags=new NameList;
  _simulator_flags=new NameList;
  _failures=new NameList;
  _path=strdup(path);
  _name=strdup(name);
  _empty=strdup("(none)");
  _extension_built=false;
  _simulator_version=NULL;
  _compiler_version=NULL;
}


char *TestSession::get_cc_name() {
	char *tmp;
	char *name=strdup(_path);
	if (!strcmp(_path,_name))  return _name;
	tmp=strtok(name,"/");
	tmp=strtok(NULL, "/");
	tmp=strtok(NULL, "/");
	tmp=strtok(NULL, "/");
	tmp=strtok(NULL, "/");
	tmp=strtok(NULL, "/");
	return strdup(tmp+(sizeof("open64-linux")));
}

void TestSession::dump_info() {
  printf("Session named:%s, Launched %s at %s\n",_name, get_date().print_date(),get_date().print_time());
  printf("\tCompiler: %s\n",get_name());
  printf("\tCompiler Version: %s\n",get_cc_ver());
  printf("\tCompiler Date: %s\n",get_cc_date());
  printf("\tCompiler Flags: ");
  ForEachPt(get_cc_flags(),iter) {
    printf("%s ",*iter);
  }
  printf("\n\tSimulator Version: %s\n",get_sim_ver());
  printf("\tSimulator Flags: ");
  ForEachPt(get_sim_flags(),iter) {
    printf("%s ",*iter);
  }
  printf("\n\tTest suits: ");
  ForEachPt(_obj_size_tests,iter) {
    printf("%s ",(*iter)->get_name());
  }
  printf("\n\n");
}

void TestSession::set_date(short d,short m,short y,short h,short min,short sec) {
  _date=DateType(d,m,y,h,min,sec);
}

void TestSession::set_compiler_name(char *name) {
  _compiler_name=strdup(name);
}

void TestSession::set_compiler_version(char *name,char *date){
  _compiler_version=strdup(name);
  _compiler_date=strdup(date);
}

void TestSession::set_simulator_version(char *name){
  _simulator_version=strdup(name);
}

void TestSession::set_support_mode(char *name) {
  _mode=strdup(name);
}

void TestSession::add_compiler_flags(char *name) {
  _compiler_flags->push_back(strdup(name));
}

void TestSession::add_simulator_flags(char *name){
  _simulator_flags->push_back(strdup(name));
}

void TestSession::add_failure(char *name){
  _failures->push_back(strdup(name));
}

void TestSession::add_test_name(char *name){
  ForEachPt(_obj_size_tests,iter) {
    if (!strcmp((*iter)->get_name(),name)) {
      if(warn_level) fprintf(stderr,"WARNING: Test suit %s already given, skipped\n",name);
      return;
    }
  }

  if(!strncmp(name,"valid_extension",15) || !strcmp(name,"valid-extensions")) {
    if(_extension_built) return;
    _extension_built=true;
    _obj_size_tests->push_back(new Test("TSTx_test"));
    _obj_size_tests->push_back(new Test("vx2_fgtdec"));
    _obj_size_tests->push_back(new Test("vx2_test_memspace"));
    _obj_size_tests->push_back(new Test("TS3x_test"));
    _obj_size_tests->push_back(new Test("admX_AppliUsingadmX"));
    _obj_size_tests->push_back(new Test("TS2x_test"));
    _obj_size_tests->push_back(new Test("QMx_farrow_interpolator"));
    _obj_size_tests->push_back(new Test("QMx_mixer"));
    _obj_size_tests->push_back(new Test("Tx_CSD_extension_test"));
    _obj_size_tests->push_back(new Test("MP1v_viterbi"));
    _obj_size_tests->push_back(new Test("Fx_SampleRate"));
    _obj_size_tests->push_back(new Test("TS4x_test"));
    
    _bin_size_tests->push_back(new Test("TSTx_test"));
    _bin_size_tests->push_back(new Test("vx2_fgtdec"));
    _bin_size_tests->push_back(new Test("vx2_test_memspace"));
    _bin_size_tests->push_back(new Test("TS3x_test"));
    _bin_size_tests->push_back(new Test("admX_AppliUsingadmX"));
    _bin_size_tests->push_back(new Test("TS2x_test"));
    _bin_size_tests->push_back(new Test("QMx_farrow_interpolator"));
    _bin_size_tests->push_back(new Test("QMx_mixer"));
    _bin_size_tests->push_back(new Test("Tx_CSD_extension_test"));
    _bin_size_tests->push_back(new Test("MP1v_viterbi"));
    _bin_size_tests->push_back(new Test("Fx_SampleRate"));
    _bin_size_tests->push_back(new Test("TS4x_test"));
    
    _func_size_tests->push_back(new Test("TSTx_test"));
    _func_size_tests->push_back(new Test("vx2_fgtdec"));
    _func_size_tests->push_back(new Test("vx2_test_memspace"));
    _func_size_tests->push_back(new Test("TS3x_test"));
    _func_size_tests->push_back(new Test("admX_AppliUsingadmX"));
    _func_size_tests->push_back(new Test("TS2x_test"));
    _func_size_tests->push_back(new Test("QMx_farrow_interpolator"));
    _func_size_tests->push_back(new Test("QMx_mixer"));
    _func_size_tests->push_back(new Test("Tx_CSD_extension_test"));
    _func_size_tests->push_back(new Test("MP1v_viterbi"));
    _func_size_tests->push_back(new Test("Fx_SampleRate"));
    _func_size_tests->push_back(new Test("TS4x_test"));
    
    return;
  }

  if(!strcmp(name,"whetstone")){
    _obj_size_tests->push_back(new Test("whetstone-float"));
    _obj_size_tests->push_back(new Test("whetstone-double"));
    _bin_size_tests->push_back(new Test("whetstone-float"));
    _bin_size_tests->push_back(new Test("whetstone-double"));
    _func_size_tests->push_back(new Test("whetstone-float"));
    _func_size_tests->push_back(new Test("whetstone-double"));
    return;
  } 

  _obj_size_tests->push_back(new Test(name));
  _bin_size_tests->push_back(new Test(name));
  _func_size_tests->push_back(new Test(name));

}

void TestSession::add_test_size(char *test_name, char *target, Section sec, int size, int disc) {
	Target *mytarget;
	char *target_name;
	if(strstr(target, "Perfs_BILBO.o")) return;
    if(strstr(target, "Verbose.o")) return;
    if(strstr(target, "Check.o")) return;
    if(strncmp("./",target,2) == 0) {
    	target = &target[2];
    }

    char *local_test_name = compute_size_test_target_name(test_name,target,&target_name);
  
    Test *current_test=NULL;
	ForEachPt(get_tests(disc),iter) {
		if (!strcmp((*iter)->get_name(),local_test_name)) {
			current_test=*iter;
			break;
		}
	}
	if (!current_test) {
		add_test_name(local_test_name);
		add_test_size(local_test_name, target_name, sec, size, disc);
		return;
	}

	mytarget = current_test->find_target(target_name);
	if (!mytarget) mytarget = current_test->add_target(target_name);

	if (mytarget->get_size(sec)!=0) {
		// if(warn_level) fprintf(stderr,"WARNING: Size value for test %s target %s section %s already given, skipped\n",test_name,target,section_name(sec));
		size = size + mytarget->get_size(sec);
	}
	mytarget->set_size(sec, size);
}


void TestSession::add_test_func_size(char *test_name,char *target,char *object, char *function, int size, bool aggregated) {
	Target *mytarget;
	char *target_name;
	int disc=SIZE_FUNC;

	if(strstr(function, "TH_InitTimer")) return;
	if(strstr(function, "TH_GetOverhead")) return;
	if(strstr(function, "TH_end")) return;
	if(strstr(function, "BENCH_START")) return;
	if(strstr(function, "BENCH_STOP")) return;
	if(strstr(function, "BENCH_STATUS")) return;
	if(strstr(function, "BENCH_INT_MESSAGE")) return;
	if(strstr(function, "BENCH_FLOAT_MESSAGE")) return;
	if(strstr(function, "BENCH_STR_MESSAGE")) return;
	if(strstr(function, "BENCH_CHECK")) return;

	
    /* PStone;auto/ auto/src/auto.o autom 4550 4194532*/
    /* PStone;auto/ auto/src/auto.o 4188 autom */

    
    /* Test Name : <test_name>_<target>
     * Target :  <function> (<object>)
     */
    
    char *tmp_test_name = compute_size_test_target_name(test_name,object,&target_name);
    Test *current_test=NULL;
    if(target[strlen(target)-1] == '/')  target[strlen(target)-1]='\0';
    char *local_test_name; 
    if (strncmp(target,"src",3)==0) {
    	local_test_name=tmp_test_name;
    } else {
    	local_test_name=(char *) malloc(strlen(tmp_test_name) + strlen(target) + 5);
    	sprintf(local_test_name,"%s - %s",tmp_test_name,target);
    }
    
    if (aggregated) {
	    target_name=(char *) malloc(strlen(function) + 22 );
	    sprintf(target_name,"%s (Aggregated Object)",function);
//	    printf("%s, %s\n",local_test_name,target_name);
    } else {    
	    target_name=(char *) malloc(strlen(function) + strlen(object) + 5);
	    sprintf(target_name,"%s (%s)",function,object);
    }       
    
    ForEachPt(get_tests(disc),iter) {
		if (!strcmp((*iter)->get_name(),local_test_name)) {
			current_test=*iter;
			break;
		}
	}
	if (!current_test) {
		add_test_name(local_test_name);
		add_test_func_size(test_name,target,object,function,size,aggregated);
		return;
	}


    mytarget = current_test->find_target(target_name);
	if (!mytarget) mytarget = current_test->add_target(target_name);

	if (mytarget->get_size(TEXT)!=0 && aggregated) 	return;
	mytarget->set_size(TEXT, size);
}


char *TestSession::section_name(Section sec) {
  switch(sec){
  case TEXT: 			return ".text";
  case DATA: 			return ".data";
  case RODATA: 			return ".rodata";
  case BSS: 			return ".bss";
  case SYMTAB: 			return ".symtab";
  case STRTAB: 			return ".strtab";
  case DBG_SECTION: 		return ".debug_section";
  case RELA_TEXT: 		return ".rela.text";
  case RELA_DATA: 		return ".rela.data";
  case RELA_RODATA: 		return ".rela.rodata";
  case RELA_DBG_FRAME: 		return ".rela.debug_frame";
  case STXP70_EXTRECONF_INFO: 	return ".STXP70_EXTRECONF_INFO";
  case STXP70_MEMINFO: 		return ".STXP70_MEMINFO";
  case STXP70_MEMMAP_INFO: 	return ".STXP70_MEMMAP_INFO";
  case STARTUP: 		return ".startup";
  case THANDLERS: 		return ".thandler";
  case STACK1: 			return ".stack1";
  case HEAP: 			return ".heap";
  case DA: 			return ".da";
  case TDA: 			return ".tda";
  case SDA: 			return ".sda";
  case SECINIT: 		return ".secinit";
  case SHSRTAB: 		return ".shsrtab";
  case TDA_DATA: 		return ".tda_data";
  case TDA_BSS: 		return ".tda_bss";
  case TDA_RO:   		return ".tda_ro";
  case SDA_DATA: 		return ".sda_data";
  case SDA_BSS:  		return ".sda_bss";
  case SDA_RO:   		return ".sda_ro";
  case DA_DATA:  		return ".da_data";
  case DA_BSS:   		return ".da_bss";
  case DA_RO:    		return ".da_bss";
  case SBSS:   			return ".sbss";
  case SYSCALL:  		return ".syscall";
  case ITHANDLERS: 		return ".ithandler";
  case IVTABLE: 		return ".ivtable";
  case ROBASE: 			return ".robase";
  case TOTAL: 			return ".TOTAL";
  case RELA_TDA_DATA:   	return ".rela.tda_data";
  case RELA_TDA_BSS:    	return ".rela.tda_bss";
  case RELA_TDA_RO:     	return ".rela.tda_ro";
  case RELA_SDA_DATA:   	return ".rela.sda_data";
  case RELA_SDA_BSS:    	return ".rela.sda_bss";
  case RELA_SDA_RO:     	return ".rela.sda_ro";
  case RELA_DA_DATA:    	return ".rela.da_data";
  case RELA_DA_BSS:     	return ".rela.da_bss";
  case RELA_DA_RO:      	return ".rela.da_bss";
  default: 			return "unknown";
  }
}
  
Test *TestSession::find_test(char *name, int disc) {
  ForEachPt(get_tests(disc),iter) {
    if (!strcmp((*iter)->get_name(),name)) return *iter;
  }
  return NULL;
}

static bool has_failed(char *test_name, char *target_name, NameList *failed) {
    ForEachPt(failed,iter) {
    	if (!strncmp(test_name,*iter,strlen(*iter))) {
    		return true;
    	}
    }
	return false;
}

//TDR
int TestSession::get_size(RootTest *tests, Section sec, int disc, bool monitor) {
  if (!is_same_list(get_cc_flags(),tests->get_options())) return -1;
  Test *mytst= find_test(tests->get_test(), disc);
  if (!mytst) {
	  if (has_failed(tests->get_test(),tests->get_target(),_failures))  return   HAS_FAILED;
	  return -1;  
  }
  if (monitor) {
      int value=0;
      if (!strcmp(tests->get_target(),tests->get_test()) || !strcmp(tests->get_test(),"jpeg_csd")) {
          //Test has only one target
          ForEachPt(mytst->get_target(),iter) {
              value += (*iter)->get_size(sec); 
          }          
          return value;
      }
      if (!strcmp(tests->get_test(),"audio_csd")) {
          //Specific treatment for audio_csd
          if(!strcmp(tests->get_target(),"Huffman decoding")) {
              ForEachPt(mytst->get_target(),iter) {
                  if (strstr((*iter)->get_name(),"huffmandecoding.o")) value += (*iter)->get_size(sec);
                  if (strstr((*iter)->get_name(),"bit_parse.o")) value += (*iter)->get_size(sec);
              }
              return value;
          }
          if(!strcmp(tests->get_target(),"IIR")) {
              ForEachPt(mytst->get_target(),iter) {
                  if (strstr((*iter)->get_name(),"iir.o")) return (*iter)->get_size(sec);
              }           
          }
          if(!strcmp(tests->get_target(),"IFFT")) {
              ForEachPt(mytst->get_target(),iter) {
                  if (strstr((*iter)->get_name(),"ifft.o")) return (*iter)->get_size(sec);
              }           
          }
          if(!strcmp(tests->get_target(),"IDCT")) {
                  ForEachPt(mytst->get_target(),iter) {
                      if (strstr((*iter)->get_name(),"idct.o")) return (*iter)->get_size(sec);
                  }           
          }
          if(!strcmp(tests->get_target(),"SRC")) {
                  ForEachPt(mytst->get_target(),iter) {
                      if (strstr((*iter)->get_name(),"oversampleinterpolate.o")) return (*iter)->get_size(sec);
                  }           
          }
          return HAS_FAILED;
      }      
      if (!strcmp(tests->get_target(),"Total")) {
          ForEachPt(mytst->get_target(),iter) {
              if (strstr((*iter)->get_name(),tests->get_target())) {
                  value += (*iter)->get_size(sec);
              }
          }
          return value;
      }
      ForEachPt(mytst->get_target(),iter) {
          if (strstr((*iter)->get_name(),tests->get_target())) {
              value += (*iter)->get_size(sec); 
          }
      }
      return value;
  } else {
      Target *mytg = mytst->find_target(tests->get_target());
      if (!mytg) {
    	  if (has_failed(tests->get_test(),tests->get_target(),_failures))  return   HAS_FAILED;
    	  return -1;  
      }
      int value = mytg->get_size(sec); 
      if (value <0 && has_failed(mytst->get_name(),mytg->get_name(),_failures)) return   HAS_FAILED;
      return value;
  }
  return   HAS_FAILED;
}

int TestSession::get_cycle(RootTest *tests) {
  if (!is_same_list(get_cc_flags(),tests->get_options()))  return -1;
  Test *mytst= find_test(tests->get_test(),SPEED);
  if (!mytst) {
	  if (has_failed(tests->get_test(),tests->get_target(),_failures))  return   HAS_FAILED;
	  return -1;  
  }
  Target *mytg = mytst->find_target(tests->get_target());
  if (!mytg) {
	  if (has_failed(tests->get_test(),tests->get_target(),_failures))  return   HAS_FAILED;
	  return -1;  
  }
  int value = mytg->get_cycle(); 
  if (value <=0 && has_failed(mytst->get_name(),mytg->get_name(),_failures)) return   HAS_FAILED;
  return value;
}

void TestSession::ignore_flag(char *flag) {
  ForEachPt(get_cc_flags(),iter) {
    if(!strcmp(*iter,flag)) {
      //printf("Should remove %s\n",flag);fflush(stdout);
      get_cc_flags()->erase(iter);
      if (warn_level) fprintf(stderr,"WARNING: Option %s ignored\n",flag);
      return;
    }
  }  
}

void TestSession::ignore_all_flags() {
  if (warn_level) fprintf(stderr,"WARNING: all options are ignored\n");
  get_cc_flags()->clear(); 
}  


void TestSession::add_cycle_count(char *test_name, int number) {
  ForEachPt(get_tests(SPEED),iter) {
    if (!strcmp((*iter)->get_name(),test_name)) {
      if(warn_level) fprintf(stderr,"WARNING: Test suit %s already given, skipped\n",test_name);
      return;
    }
  }
  char *target_name;
  char *local_test_name = compute_test_target_name(test_name,&target_name);
  Test *mytest = find_test(local_test_name,SPEED);
  if (!mytest) mytest =  new Test(local_test_name);
  get_tests(SPEED)->push_back(mytest);
  Target *mytarget=mytest->find_target(target_name);
  if (!mytarget) mytarget=mytest->add_target(target_name);
  
  if(mytarget->get_cycle()!=-1)  number +=mytarget->get_cycle();
  mytarget->set_cycle(number);
  
}

TestList *TestSession::get_tests(int disc) {
  if (disc == SIZE_OBJ) return _obj_size_tests; 
  if (disc == SIZE_BIN) return _bin_size_tests; 
  if (disc == SIZE_FUNC) return _func_size_tests;
  if (disc == SPEED) return _speed_tests; 
  return NULL;
}


/*****************************************/
/*               RootTest                */
/*****************************************/

RootTest::RootTest(char *test, char *target, NameList *options) {
  _test=test;
  _target=target;
  _options=options;

}

bool RootTest::is_same(char *test, char *target, NameList *options) {
  if(!strcmp(_test,test) && !strcmp(_target,target) && is_same_list(_options,options)) 
    return true;
  return false;
}




/*****************************************/
/*           RootDataClass               */
/*****************************************/


RootDataClass::RootDataClass() {
  _sessions = new SessionList;
  _disc_test = NULL;
  _ignored_flags = new NameList;
  _nb_sessions=0;
  _nb_data_max=0;
  
}

void RootDataClass::add_session(char *path, char *name) {
  ForEachPt(_sessions,iter_session) {
    if (!strcmp((*iter_session)->get_path(),path) && !strcmp((*iter_session)->get_name(),name)) {
      if(warn_level) fprintf(stderr,"WARNING: Session %s already given, skipped\n",name);
      continue;
    }
  }
  _sessions->push_back(new TestSession(path,name));
  _nb_sessions++;
}  

void RootDataClass::add_test_disc(char *test, char *target, NameList *options) {
  ForEachPt(_disc_test,iter) {
    if ((*iter)->is_same(test,target,options)) return;
  }
//  printf("DISC: %s, %s\n",test,target);
  _disc_test->push_back(new RootTest(test,target,options));
  _nb_data_max++;
}

void RootDataClass::cleanup_test_disc() {
  _nb_data_max=0;
  if(_disc_test) _disc_test->~RootTestList();
  _disc_test = new RootTestList;
}


void RootDataClass::remove_ignored_flags(bool no_ignore) {
  if (no_ignore) {
    ForEachPt(_sessions,session) {
      ForEachPt(_ignored_flags,flag) {
	(*session)->ignore_flag(*flag);
      }
    }
  } else {
    ForEachPt(_sessions,session) {
      (*session)->ignore_all_flags();
    }
  }
}


void RootDataClass::compute_data(int disc) {
  cleanup_test_disc();
  ForEachPt(_sessions,session) {
    ForEachPt((*session)->get_tests(disc),test) {
      ForEachPt((*test)->get_target(),target) {
        if (core_only && is_extension_info((*test)->get_name()))  continue;
        if (ext_only && !is_extension_info((*test)->get_name()))  continue;
	add_test_disc((*test)->get_name(),(*target)->get_name(), (*session)->get_cc_flags());
//	printf("DISC: %s, %s\n",(*test)->get_name(),(*target)->get_name());
      }
    }
  }
}


/* Here we only add tests from ARM ref file :
 * - Audio_peech
 * - EEMBC
 * - JPEG CSD
 * - bluetooth
 */
void RootDataClass::compute_monitored_data(int disc) {
    cleanup_test_disc();
    TestSession *session = *(_sessions->begin());
    ForEachPt(session->get_tests(disc),test) {
        ForEachPt((*test)->get_target(),target) {
            add_test_disc((*test)->get_name(),(*target)->get_name(), session->get_cc_flags());
            //  printf("DISC: %s, %s\n",(*test)->get_name(),(*target)->get_name());
        }
    }
}




/*****************************************/
/*               Summary                 */
/*****************************************/

SummaryElem::SummaryElem(char *path,char *name) {
	_path = path;
	_name = name;
	_cycles = new IntList;
	_NW_cycles = new IntList;
	_CO_cycles = new IntList;
	_BT_cycles = new IntList;
	_CSD_cycles = new IntList;
	_JPEG_cycles = new IntList;
	_sizes_obj = new IntList;
	_sizes_bin = new IntList;
	_sizes_func = new IntList;
	_NW_sizes_obj=new IntList;
	_CO_sizes_obj=new IntList;
	_BT_sizes_obj=new IntList;
	_CSD_sizes_obj=new IntList;
	_JPEG_sizes_obj =new IntList;
	_NW_sizes_bin =new IntList;
	_CO_sizes_bin =new IntList;
	_BT_sizes_bin =new IntList;
	_CSD_sizes_bin =new IntList;
	_JPEG_sizes_bin =new IntList;
	_NW_sizes_func =new IntList;
	_CO_sizes_func =new IntList;
	_BT_sizes_func =new IntList;
	_CSD_sizes_func =new IntList;
	_JPEG_sizes_func =new IntList;
}


void SummaryElem::add_cycle(char * tname, int val)  {
  bool is_ext=is_extension_info(tname);
  if (core_only && is_ext)  return;
  if (ext_only && !is_ext)  return;
  _cycles->push_back(val);
  if (!strcmp(tname,"eembc_networking")) _NW_cycles->push_back(val);
  if (!strcmp(tname,"eembc_consumer")) _CO_cycles->push_back(val);
  if (!strcmp(tname,"bluetooth")) _BT_cycles->push_back(val);
  
  if (!strcmp(tname,"SRC") || !strcmp(tname,"IDCT") || !strcmp(tname,"Second order IIR") 
		  || !strcmp(tname,"IFFT") || !strcmp(tname,"Huffman decoding")) _CSD_cycles->push_back(val);
  if (!strcmp(tname,"JPEG Decoder CSD")) _JPEG_cycles->push_back(val);
}

void SummaryElem::add_bin_size(char * tname, int val) {
  bool is_ext=is_extension_info(tname);
  if (core_only && is_ext)  return;
  if (ext_only && !is_ext)  return;
  _sizes_bin->push_back(val);
  if (!strcmp(tname,"eembc_networking")) _NW_sizes_bin->push_back(val);
  if (!strcmp(tname,"eembc_consumer")) _CO_sizes_bin->push_back(val);
  if (!strcmp(tname,"bluetooth")) _BT_sizes_bin->push_back(val);
  if (!strcmp(tname,"audio_csd")) _CSD_sizes_bin->push_back(val);
  if (!strcmp(tname,"jpeg_csd")) _JPEG_sizes_bin->push_back(val);
}

void SummaryElem::add_obj_size(char * tname, int val) {
  bool is_ext=is_extension_info(tname);
  if (core_only && is_ext)  return;
  if (ext_only && !is_ext)  return;
  _sizes_obj->push_back(val);
  if (!strcmp(tname,"eembc_networking")) _NW_sizes_obj->push_back(val);
  if (!strcmp(tname,"eembc_consumer")) _CO_sizes_obj->push_back(val);
  if (!strcmp(tname,"bluetooth")) _BT_sizes_obj->push_back(val);
  if (!strcmp(tname,"audio_csd")) _CSD_sizes_obj->push_back(val);
  if (!strcmp(tname,"jpeg_csd")) _JPEG_sizes_obj->push_back(val);
}


void SummaryElem::add_func_size(char * tname, int val) {
  bool is_ext=is_extension_info(tname);
  if (core_only && is_ext)  return;
  if (ext_only && !is_ext)  return;
  _sizes_func->push_back(val);
  if (!strcmp(tname,"eembc_networking")) _NW_sizes_func->push_back(val);
  if (!strcmp(tname,"eembc_consumer")) _CO_sizes_func->push_back(val);
  if (!strcmp(tname,"bluetooth")) _BT_sizes_func->push_back(val);
  if (!strcmp(tname,"audio_csd")) _CSD_sizes_func->push_back(val);
  if (!strcmp(tname,"jpeg_csd")) _JPEG_sizes_func->push_back(val);
}


IntList *SummaryElem::get_cycles(Dump_Type type) {
	switch (type) {
	case Run_Valid : 
		return _cycles;
	case EEMBC_Net :
		return _NW_cycles;
	case EEMBC_Cons :
		return _CO_cycles;
	case Bluetooth :
		return _BT_cycles;
	case Jpeg :
		return _JPEG_cycles;
	case Audio_CSD :
		return _CSD_cycles;
	}
	return NULL;
}
IntList *SummaryElem::get_size(Dump_Type type, int disc) {
	if(disc == SIZE_OBJ) {
		switch (type) {
		case Run_Valid : 
			return _sizes_obj;
		case EEMBC_Net :
			return _NW_sizes_obj;
		case EEMBC_Cons :
			return _CO_sizes_obj;
		case Bluetooth :
			return _BT_sizes_obj;
		case Jpeg :
			return _JPEG_sizes_obj;
		case Audio_CSD :
			return _CSD_sizes_obj;
		}	
	} 
	if(disc == SIZE_BIN) {
		switch (type) {
		case Run_Valid : 
			return _sizes_bin;
		case EEMBC_Net :
			return _NW_sizes_bin;
		case EEMBC_Cons :
			return _CO_sizes_bin;
		case Bluetooth :
			return _BT_sizes_bin;
		case Jpeg :
			return _JPEG_sizes_bin;
		case Audio_CSD :
			return _CSD_sizes_bin;
		}
	}
	return NULL;
}

char *SummaryElem::get_cc_name() {
	char *tmp;
	char *name=strdup(_path);
	tmp=strtok(name,"/");
	tmp=strtok(NULL, "/");
	tmp=strtok(NULL, "/");
	tmp=strtok(NULL, "/");
	tmp=strtok(NULL, "/");
	tmp=strtok(NULL, "/");
	return strdup(tmp+(sizeof("open64-linux")));
}

SummaryClass::SummaryClass() {
  _elem = new SummaryElemList;
  _nb_elem = 0;
}

void SummaryClass::add_session(char *path,char *name) {
  SummaryElem *my_elem = new SummaryElem(path,name);
  _elem->push_back(my_elem);
}

void SummaryClass::add_summary_value(char *path, char * name, char *tname, int type, int value) {
  ForEachPt(_elem,iter_elem) {
    if (!strcmp((*iter_elem)->get_name(),name) && !strcmp((*iter_elem)->get_path(),path)) {
      if (type==SPEED) (*iter_elem)->add_cycle(tname,value);
      if (type==SIZE_OBJ) (*iter_elem)->add_obj_size(tname,value);
      if (type==SIZE_BIN) (*iter_elem)->add_bin_size(tname,value);
      if (type==SIZE_FUNC) (*iter_elem)->add_func_size(tname,value);
      break;
    }
  }
}

static char *get_version_num(char *name) {
	int i = strlen(name)-1;
	while (isdigit(name[i])) {
		i--;
	}
	
	return strdup(&name[i+1]);
}

static bool is_trunk(char *name) {
	return (strstr(name,"trunk") != NULL);
}

void SummaryClass::dump_summary_cc(char *name) {
	SummaryElem *base, *compare;
	std::_List_iterator<SummaryElem*, SummaryElem*&, SummaryElem**> iter=_elem->begin();
	base = *iter;
	iter++;
	compare = *iter;
	iter++;
	fprintf(stdout,"<property name=\"%s_%s_compiler\" value=\"%s\"/>\n",base->get_name(),name,get_version_num(compare->get_cc_name()));
	fprintf(stdout,"<property name=\"%s_%s_cycles\" value=\"",base->get_name(),name);
	if (compare->get_cycles(Run_Valid)->empty()) fprintf(stdout," Max Gain: N/A, Max loss:  N/A, Geomean: N/A");
	else {
		typeof(base->get_cycles(Run_Valid)->begin()) vb,vc;
		int nb_data=0;;
		double sumb=0,sumc=0,min=0,max=0,tmp_val;
		for (vb = base->get_cycles(Run_Valid)->begin(), vc=compare->get_cycles(Run_Valid)->begin(); vb != base->get_cycles(Run_Valid)->end(); ++vb, ++vc) {
			if (*vb<=-1 || *vc<=-1) continue;
			nb_data++;
			sumb+=*vb; sumc+=*vc;
			tmp_val=double (double (*vb)- double (*vc))/ double (*vc);
			if (tmp_val> max) max = tmp_val;
			if (tmp_val < min) min = tmp_val;
		}
		double moy = (((sumb/nb_data)-(sumc/nb_data))/(sumc/nb_data));
		char minst[15],maxst[15],moyst[15];
		sprintf(minst,"%4.4f",min*100.00);
		sprintf(maxst,"%4.4f",max*100.00);
		sprintf(moyst,"%4.4f",moy*100.00);
		fprintf(stdout," Max Gain: %5.5s, Max Loss: %5.5s, Geomean: %5.5s",minst,maxst,moyst);
	}
	fprintf(stdout,"\" />\n<property name=\"%s_%s_obj_size\" value=\"",base->get_name(),name);
	if (compare->get_size(Run_Valid,SIZE_OBJ)->empty()) fprintf(stdout," Max Gain: N/A, Max loss:  N/A, Average: N/A");
	else {
		typeof(base->get_size(Run_Valid,SIZE_OBJ)->begin()) vb,vc;
		int nb_data=0;;
		double sumb=0,sumc=0,min=0,max=0,tmp_val;
		for (vb = base->get_size(Run_Valid,SIZE_OBJ)->begin(), vc=compare->get_size(Run_Valid,SIZE_OBJ)->begin(); vb != base->get_size(Run_Valid,SIZE_OBJ)->end(); ++vb, ++vc) {
			if (*vb<=-1 || *vc<=-1) continue;
			nb_data++;
			sumb+=*vb; sumc+=*vc;
			tmp_val=double (double (*vb)- double (*vc))/ double (*vc);
			if (tmp_val> max) max = tmp_val;
			if (tmp_val < min) min = tmp_val;
		}
		double moy = (((sumb/nb_data)-(sumc/nb_data))/(sumc/nb_data));
		char minst[15],maxst[15],moyst[15];
		sprintf(minst,"%4.4f",min*100.00);
		sprintf(maxst,"%4.4f",max*100.00);
		sprintf(moyst,"%4.4f",moy*100.00);
		fprintf(stdout," Max Gain: %5.5s, Max Loss: %5.5s, Average: %5.5s",minst,maxst,moyst);
	}
	fprintf(stdout,"\" />\n<property name=\"%s_%s_bin_size\" value=\"",base->get_name(),name);
	if (compare->get_size(Run_Valid,SIZE_BIN)->empty()) fprintf(stdout," Max Gain: N/A, Max loss:  N/A, Average: N/A");
	else {
		typeof(base->get_size(Run_Valid,SIZE_BIN)->begin()) vb,vc;
		int nb_data=0;;
		double sumb=0,sumc=0,min=0,max=0,tmp_val;
		for (vb = base->get_size(Run_Valid,SIZE_BIN)->begin(), vc=compare->get_size(Run_Valid,SIZE_BIN)->begin(); vb != base->get_size(Run_Valid,SIZE_BIN)->end(); ++vb, ++vc) {
			if (*vb<=-1 || *vc<=-1) continue;
			nb_data++;
			sumb+=*vb; sumc+=*vc;
			tmp_val=double (double (*vb)- double (*vc))/ double (*vc);
			if (tmp_val> max) max = tmp_val;
			if (tmp_val < min) min = tmp_val;
		}
		double moy = (((sumb/nb_data)-(sumc/nb_data))/(sumc/nb_data));
		char minst[15],maxst[15],moyst[15];
		sprintf(minst,"%4.4f",min*100.00);
		sprintf(maxst,"%4.4f",max*100.00);
		sprintf(moyst,"%4.4f",moy*100.00);
		fprintf(stdout," Max Gain: %5.5s, Max Loss: %5.5s, Average: %5.5s",minst,maxst,moyst);
	}
		
	fprintf(stdout,"\" />\n");
	if (iter != _elem->end()) { 
		iter=_elem->end();
		iter--;
		compare=*iter;
	} else {
		return;
	}
}


void SummaryClass::dump_summary() {
	SummaryElem *base, *compare;
	int i=0;
	base = *_elem->begin();
	fprintf(stdout, "%22.22s  |", "");
	ForEachPt(_elem,iter_elem) {
		if(base == *iter_elem ) continue;
		i++;
		if (i==4) break;
		compare = *iter_elem;
		fprintf(stdout,"%22.22s  |",compare->get_name());
	}
	i=0;
	fprintf(stdout,	"\n------------------------|------------------------|------------------------|------------------------|\n");
	fprintf(stdout, "%16.16s Cycles |", base->get_name());
	ForEachPt(_elem,iter_elem) {
		if(base == *iter_elem ) continue;
		compare = *iter_elem;
		i++;
		if (i==4) break;
		if (compare->get_cycles(Run_Valid)->empty()) fprintf(stdout," %.5s / %.5s / %.5s  |","  -  ","  -  ","  -  ");
		else {
			typeof(base->get_cycles(Run_Valid)->begin()) vb,vc;
			int nb_data=0;;
			double sumb=0,sumc=0,min=0,max=0,tmp_val;
			for (vb = base->get_cycles(Run_Valid)->begin(), vc=compare->get_cycles(Run_Valid)->begin(); vb != base->get_cycles(Run_Valid)->end(); ++vb, ++vc) {
				if (*vb<=-1 || *vc<=-1) continue;
				nb_data++;
				sumb+=*vb; sumc+=*vc;
				tmp_val=double (double (*vb)- double (*vc))/ double (*vc);
				if (tmp_val> max) max = tmp_val;
				if (tmp_val < min) min = tmp_val;
			}
			double moy = (((sumb/nb_data)-(sumc/nb_data))/(sumc/nb_data));
			char minst[15],maxst[15],moyst[15];
			sprintf(minst,"%4.4f",min*100.00);
			sprintf(maxst,"%4.4f",max*100.00);
			sprintf(moyst,"%4.4f",moy*100.00);
			fprintf(stdout," %5.5s / %5.5s / %5.5s  |",minst,maxst,moyst);
		}
	}
	fprintf(stdout, "\n%14.14s Size Obj |", base->get_name());
	i=0;
	ForEachPt(_elem,iter_elem) {
		if(base == *iter_elem ) continue;
		compare = *iter_elem;
		i++;
		if (i==4) break;
		if (compare->get_size(Run_Valid,SIZE_OBJ)->empty()) fprintf(stdout," %5.5s / %5.5s / %5.5s  |","  -  ","  -  ","  -  ");
		else {
			typeof(base->get_size(Run_Valid,SIZE_OBJ)->begin()) vb,vc;
			int nb_data=0;;
			double sumb=0,sumc=0,min=0,max=0,tmp_val;
			for (vb = base->get_size(Run_Valid,SIZE_OBJ)->begin(), vc=compare->get_size(Run_Valid,SIZE_OBJ)->begin(); vb != base->get_size(Run_Valid,SIZE_OBJ)->end(); ++vb, ++vc) {
				if (*vb<=-1 || *vc<=-1) continue;
				nb_data++;
				sumb+=*vb; sumc+=*vc;
				tmp_val=double (double (*vb)- double (*vc))/ double (*vc);
				if (tmp_val> max) max = tmp_val;
				if (tmp_val < min) min = tmp_val;
			}
			double moy = (((sumb/nb_data)-(sumc/nb_data))/(sumc/nb_data));
			char minst[15],maxst[15],moyst[15];
			sprintf(minst,"%4.4f",min*100.00);
			sprintf(maxst,"%4.4f",max*100.00);
			sprintf(moyst,"%4.4f",moy*100.00);
			fprintf(stdout," %5.5s / %5.5s / %5.5s  |",minst,maxst,moyst);
		}
	}
	i=0;
	fprintf(stdout, "\n%14.14s Size Bin |", base->get_name());
	ForEachPt(_elem,iter_elem) {
		if(base == *iter_elem ) continue;
		compare = *iter_elem;
		i++;
		if (i==4) break;
		if (compare->get_size(Run_Valid,SIZE_BIN)->empty()) fprintf(stdout," %5.5s / %5.5s / %5.5s  |","  -  ","  -  ","  -  ");
		else {
			typeof(base->get_size(Run_Valid,SIZE_BIN)->begin()) vb,vc;
			int nb_data=0;;
			double sumb=0,sumc=0,min=0,max=0,tmp_val;
			for (vb = base->get_size(Run_Valid,SIZE_BIN)->begin(), vc=compare->get_size(Run_Valid,SIZE_BIN)->begin(); vb != base->get_size(Run_Valid,SIZE_BIN)->end(); ++vb, ++vc) {
				if (*vb<=-1 || *vc<=-1) continue;
				nb_data++;
				sumb+=*vb; sumc+=*vc;
				tmp_val=double (double (*vb)- double (*vc))/ double (*vc);
				if (tmp_val> max) max = tmp_val;
				if (tmp_val < min) min = tmp_val;
			}
			double moy = (((sumb/nb_data)-(sumc/nb_data))/(sumc/nb_data));
			char minst[15],maxst[15],moyst[15];
			sprintf(minst,"%4.4f",min*100.00);
			sprintf(maxst,"%4.4f",max*100.00);
			sprintf(moyst,"%4.4f",moy*100.00);
			fprintf(stdout," %5.5s / %5.5s / %5.5s  |",minst,maxst,moyst);
		}
	}
	fprintf(stdout, "\n");
}

/*****************************************/
/*                Misc                   */
/*****************************************/

bool is_same_list(NameList *l1,NameList *l2) {
  bool found;
  ForEachPt(l1,iter) {
    found=false;
    ForEachPt(l2,iter2) {
      if (!strcmp(*iter,*iter2)) { 
	found=true;
	break;
      }
    }
    if (!found) return false;
  }
  ForEachPt(l2,iter) {
    found=false;
    ForEachPt(l1,iter2) {
      if (!strcmp(*iter,*iter2)) { 
	found=true;
	break;
      }
    }
    if (!found) return false;
  }
  return true;
}

static char *compute_size_test_target_name(char *name, char *tname ,char **target) {
    char *my_name = NULL;
    if(!strcmp(name,"eembc_v11.automotive"))         my_name = strdup("eembc_automotive");
    else if(!strcmp(name,"eembc_v11.consumer"))      my_name = strdup("eembc_consumer");
    else if (!strcmp(name, "eembc_v20.networking"))  my_name = strdup("eembc_networking");
    else if (!strcmp(name, "eembc_v11.telecom"))     my_name = strdup("eembc_telecom");
    else my_name = strdup(name);
    
    if (strstr(tname,".u")) *target = strdup(strrchr(tname,'/')+1);
    else *target = strdup(tname);
    return my_name;
}


static char *compute_test_target_name(char *name, char **target) {
    char *tmp;
    char *my_name=strdup(name);

    if(!strncmp(my_name,"eembc_v",7)) {
         tmp=strstr(my_name,"--");
         my_name=&tmp[2];
    }

    if ((tmp=strstr(my_name, "--"))) {
        *target=strdup(&tmp[2]);
        *tmp='\0';
        if (!strcmp(my_name, "LAO-Kernels") && (tmp=strstr(*target, "-"))) {
            *target=&tmp[1];
            tmp = strtok(*target, " ");
            *target=tmp;
        } else if (!strcmp(my_name, "LAO-Kernels")) {
            tmp = strtok(*target, " ");
            *target=tmp;
        } else if (!strcmp(my_name, "Consumer"))     return "eembc_consumer";
        else if (!strcmp(my_name, "Networking"))     return "eembc_networking";
        else if (!strcmp(my_name, "Telecom"))        return "eembc_telecom";
        else if (!strcmp(my_name, "Automotive"))     return "eembc_automotive";
        else if (!strcmp(my_name, "QMx_mixer")) {
            *target=strndup(*target,strlen(*target)-6);
        }
        else if ((tmp=strstr(*target, "--"))) {
            *target=strdup(&tmp[2]);
        }
        else if (!strcmp(my_name,"PStone") || !strcmp(my_name,"diagnostics") || !strcmp(my_name,"Audio_speech")) {
            tmp = strstr(*target,"/");
            *tmp='\0';
        }
        else if (!strncmp(*target,"./src/",6)) *target=&(*target)[6]; 
        else if (!strncmp(*target,"src/",4)) *target=&(*target)[4]; 
        return my_name;
    }
  *target=strdup(name);
  return name;
  
}
