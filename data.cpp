#include "data.hpp"
#include <ctype.h>


static char *compute_test_target_name(char *name, char **target);
ValTab arm_ref_value;



void RootDataClass::compute_monitored_data(int disc) {
  cleanup_test_disc();
  ForEachPt(_sessions,session) {
      
      if (disc == SIZE_BIN || disc == SPEED) add_test_disc("jpeg_csd","JPEG Decoder CSD", (*session)->get_cc_flags());

      add_test_disc("audio_csd","Huffman decoding", (*session)->get_cc_flags());
      add_test_disc("audio_csd","IDCT", (*session)->get_cc_flags());
      add_test_disc("audio_csd","IFFT", (*session)->get_cc_flags());
      add_test_disc("audio_csd","IIR", (*session)->get_cc_flags());
      add_test_disc("audio_csd","SRC", (*session)->get_cc_flags());

      if (disc==SIZE_BIN)  
          add_test_disc("eembc_consumer","empty", (*session)->get_cc_flags());
      if (disc==SPEED || disc==SPEED_Vs_ARM || disc==SIZE_BIN) {
          add_test_disc("eembc_consumer","rgbyiq01", (*session)->get_cc_flags());
          add_test_disc("eembc_consumer","rgbhpg01", (*session)->get_cc_flags());
          add_test_disc("eembc_consumer","rgbcmy01", (*session)->get_cc_flags());
          add_test_disc("eembc_consumer","djpeg", (*session)->get_cc_flags());
          add_test_disc("eembc_consumer","cjpeg", (*session)->get_cc_flags());
//          add_test_disc("eembc_consumer","Total", (*session)->get_cc_flags());
      }
      
      if(disc!=SPEED && disc!=SPEED_Vs_ARM) 
          add_test_disc("eembc_networking","empty", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","ip_pktcheckb1m", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","ip_pktcheckb2m", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","ip_pktcheckb4m", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","ip_pktcheckb512k", (*session)->get_cc_flags());
      if(disc==SPEED ||  disc==SPEED_Vs_ARM)
          add_test_disc("eembc_networking","ip_reassemblyIT", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","ip_reassembly", (*session)->get_cc_flags());
      if(disc==SPEED ||  disc==SPEED_Vs_ARM)      
          add_test_disc("eembc_networking","natIT", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","nat", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","ospfv2", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","qos", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","routelookup", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","tcpbulk", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","tcpjumbo", (*session)->get_cc_flags());
      add_test_disc("eembc_networking","tcpmixed", (*session)->get_cc_flags());
//      add_test_disc("eembc_networking","Total", (*session)->get_cc_flags());

      if (disc==SIZE_BIN) {
          add_test_disc("eembc_automotive","empty", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","ttsprk01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","tblook01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","rspeed01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","puwmod01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","pntrch01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","matrix01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","iirflt01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","idctrn01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","canrdr01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","cacheb01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","bitmnp01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","basefp01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","aiifft01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","aifirf01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","aifftr01", (*session)->get_cc_flags());
          add_test_disc("eembc_automotive","a2time01", (*session)->get_cc_flags());
//          add_test_disc("eembc_automotive","Total", (*session)->get_cc_flags());
    
          add_test_disc("eembc_telecom","empty", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","autcor00data_1", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","autcor00data_2", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","autcor00data_3", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","conven00data_1", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","conven00data_2", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","conven00data_3", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","fbital00data_2", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","fbital00data_3", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","fbital00data_6", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","fft00data_1", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","fft00data_2", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","fft00data_3", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","viterb00data_1", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","viterb00data_2", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","viterb00data_3", (*session)->get_cc_flags());
          add_test_disc("eembc_telecom","viterb00data_4", (*session)->get_cc_flags());
 //         add_test_disc("eembc_telecom","Total", (*session)->get_cc_flags());
      }
      
      if(disc!=SPEED && disc!=SPEED_Vs_ARM) {
          add_test_disc("bluetooth","bluetooth", (*session)->get_cc_flags());
      }
  }
}

static void set_arm_ref_size() {
    arm_ref_value["jpeg_csd"]["JPEG Decoder CSD"].size          = 0;

    arm_ref_value["bluetooth"]["bluetooth"].size        = 383376;
    
    arm_ref_value["audio_csd"]["Huffman decoding"].size = 652;
    arm_ref_value["audio_csd"]["IDCT"].size             = 1536;
    arm_ref_value["audio_csd"]["IFFT"].size             = 772;
    arm_ref_value["audio_csd"]["IIR"].size              = 252;
    arm_ref_value["audio_csd"]["SRC"].size              = 772;
    
    arm_ref_value["eembc_automotive"]["empty"].size     = 0;
    arm_ref_value["eembc_automotive"]["ttsprk01"].size  = 0;
    arm_ref_value["eembc_automotive"]["tblook01"].size  = 0;
    arm_ref_value["eembc_automotive"]["rspeed01"].size  = 0;
    arm_ref_value["eembc_automotive"]["puwmod01"].size  = 0;
    arm_ref_value["eembc_automotive"]["pntrch01"].size  = 0;
    arm_ref_value["eembc_automotive"]["matrix01"].size  = 0;
    arm_ref_value["eembc_automotive"]["iirflt01"].size  = 0;
    arm_ref_value["eembc_automotive"]["idctrn01"].size  = 0;
    arm_ref_value["eembc_automotive"]["canrdr01"].size  = 0;
    arm_ref_value["eembc_automotive"]["cacheb01"].size  = 0;
    arm_ref_value["eembc_automotive"]["bitmnp01"].size  = 0;
    arm_ref_value["eembc_automotive"]["basefp01"].size  = 0;
    arm_ref_value["eembc_automotive"]["aiifft01"].size  = 0;
    arm_ref_value["eembc_automotive"]["aifirf01"].size  = 0;
    arm_ref_value["eembc_automotive"]["a2time01"].size  = 0;
    arm_ref_value["eembc_automotive"]["Total"].size  = 0;
    
    arm_ref_value["eembc_consumer"]["empty"].size   = 0;
    arm_ref_value["eembc_consumer"]["rgbyiq01"].size= 0;
    arm_ref_value["eembc_consumer"]["rgbhpg01"].size= 0;
    arm_ref_value["eembc_consumer"]["rgbcmy01"].size= 0;
    arm_ref_value["eembc_consumer"]["djpeg"].size   = 0;
    arm_ref_value["eembc_consumer"]["cjpeg"].size   = 0;
    arm_ref_value["eembc_consumer"]["Total"].size   = 
        arm_ref_value["eembc_consumer"]["empty"].size +
        arm_ref_value["eembc_consumer"]["rgbyiq01"].size +
        arm_ref_value["eembc_consumer"]["rgbhpg01"].size +
        arm_ref_value["eembc_consumer"]["rgbcmy01"].size +
        arm_ref_value["eembc_consumer"]["djpeg"].size   +
        arm_ref_value["eembc_consumer"]["cjpeg"].size   ;

    arm_ref_value["eembc_networking"]["empty"].size             = 160;
    arm_ref_value["eembc_networking"]["ip_pktcheckb512k"].size  = 2476;
    arm_ref_value["eembc_networking"]["ip_pktcheckb1m"].size    = 2476;
    arm_ref_value["eembc_networking"]["ip_pktcheckb2m"].size    = 2472;
    arm_ref_value["eembc_networking"]["ip_pktcheckb4m"].size    = 2488;
    arm_ref_value["eembc_networking"]["ip_reassembly"].size     = 4856;
    arm_ref_value["eembc_networking"]["nat"].size               = 20700;
    arm_ref_value["eembc_networking"]["ospfv2"].size            = 1600;
    arm_ref_value["eembc_networking"]["qos"].size               = 18464;
    arm_ref_value["eembc_networking"]["routelookup"].size       = 1752;
    arm_ref_value["eembc_networking"]["tcpbulk"].size           = 8620;
    arm_ref_value["eembc_networking"]["tcpjumbo"].size          = 8620;
    arm_ref_value["eembc_networking"]["tcpmixed"].size          = 8620;
    arm_ref_value["eembc_networking"]["Total"].size          = 
        arm_ref_value["eembc_networking"]["empty"].size             +
        arm_ref_value["eembc_networking"]["ip_pktcheckb512k"].size  +
        arm_ref_value["eembc_networking"]["ip_pktcheckb1m"].size    +
        arm_ref_value["eembc_networking"]["ip_pktcheckb2m"].size    +
        arm_ref_value["eembc_networking"]["ip_pktcheckb4m"].size    +
        arm_ref_value["eembc_networking"]["ip_reassembly"].size     +
        arm_ref_value["eembc_networking"]["nat"].size               +
        arm_ref_value["eembc_networking"]["ospfv2"].size            +
        arm_ref_value["eembc_networking"]["qos"].size               +
        arm_ref_value["eembc_networking"]["routelookup"].size       +
        arm_ref_value["eembc_networking"]["tcpbulk"].size           +
        arm_ref_value["eembc_networking"]["tcpjumbo"].size          +
        arm_ref_value["eembc_networking"]["tcpmixed"].size ;

    arm_ref_value["eembc_telecom"]["empty"].size            = 0;
    arm_ref_value["eembc_telecom"]["autcor00data_1"].size   = 0;
    arm_ref_value["eembc_telecom"]["autcor00data_2"].size   = 0;
    arm_ref_value["eembc_telecom"]["autcor00data_3"].size   = 0;
    arm_ref_value["eembc_telecom"]["conven00data_1"].size   = 0;
    arm_ref_value["eembc_telecom"]["conven00data_2"].size   = 0;
    arm_ref_value["eembc_telecom"]["conven00data_3"].size   = 0;
    arm_ref_value["eembc_telecom"]["fbital00data_2"].size   = 0;
    arm_ref_value["eembc_telecom"]["fbital00data_3"].size   = 0;
    arm_ref_value["eembc_telecom"]["fbital00data_6"].size   = 0;
    arm_ref_value["eembc_telecom"]["fft00data_1"].size      = 0;
    arm_ref_value["eembc_telecom"]["fft00data_2"].size      = 0;
    arm_ref_value["eembc_telecom"]["fft00data_3"].size      = 0;
    arm_ref_value["eembc_telecom"]["viterb00data_1"].size   = 0;
    arm_ref_value["eembc_telecom"]["viterb00data_2"].size   = 0;
    arm_ref_value["eembc_telecom"]["viterb00data_3"].size   = 0;
    arm_ref_value["eembc_telecom"]["viterb00data_4"].size   = 0;
}



static void set_arm_ref_cycles() {
    arm_ref_value["jpeg_csd"]["JPEG Decoder CSD"].cycles        = 0;

    arm_ref_value["bluetooth"]["bluetooth"].cycles       = 0;
    
    arm_ref_value["audio_csd"]["Huffman decoding"].cycles   = 26015;
    arm_ref_value["audio_csd"]["IDCT"].cycles               = 2627;
    arm_ref_value["audio_csd"]["IFFT"].cycles               = 77372;
    arm_ref_value["audio_csd"]["IIR"].cycles                = 16200;
    arm_ref_value["audio_csd"]["SRC"].cycles                = 716145;

    arm_ref_value["eembc_automotive"]["empty"].cycles   = 0;
    arm_ref_value["eembc_automotive"]["ttsprk01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["tblook01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["rspeed01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["puwmod01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["pntrch01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["matrix01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["iirflt01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["idctrn01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["canrdr01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["cacheb01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["bitmnp01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["basefp01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["aiifft01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["aifirf01"].cycles= 0;
    arm_ref_value["eembc_automotive"]["a2time01"].cycles= 0;
    
    arm_ref_value["eembc_consumer"]["djpeg"].cycles     = 0.024953885220113;
    arm_ref_value["eembc_consumer"]["cjpeg"].cycles     = 0.020500810192018;
    arm_ref_value["eembc_consumer"]["rgbyiq01"].cycles  = 0.146300641967217;
    arm_ref_value["eembc_consumer"]["rgbhpg01"].cycles  = 0.143243901390898;
    arm_ref_value["eembc_consumer"]["rgbcmy01"].cycles  = 0.194373282226118;

    arm_ref_value["eembc_networking"]["ip_pktcheckb512k"].cycles= 11.410;
    arm_ref_value["eembc_networking"]["ip_pktcheckb1m"].cycles  = 5.93;
    arm_ref_value["eembc_networking"]["ip_pktcheckb2m"].cycles  = 3.02;
    arm_ref_value["eembc_networking"]["ip_pktcheckb4m"].cycles  = 1.51;
    arm_ref_value["eembc_networking"]["ip_reassemblyIT"].cycles = 4.62;
    arm_ref_value["eembc_networking"]["ip_reassembly"].cycles   = 1.09;
    arm_ref_value["eembc_networking"]["natIT"].cycles           = 6.25;
    arm_ref_value["eembc_networking"]["nat"].cycles             = 1.36;
    arm_ref_value["eembc_networking"]["ospfv2"].cycles          = 1.580;
    arm_ref_value["eembc_networking"]["qos"].cycles             = 0.07;
    arm_ref_value["eembc_networking"]["routelookup"].cycles     = 3.580;
    arm_ref_value["eembc_networking"]["tcpbulk"].cycles         = 18.36;
    arm_ref_value["eembc_networking"]["tcpjumbo"].cycles        = 24.69;
    arm_ref_value["eembc_networking"]["tcpmixed"].cycles        = 10.81;

    arm_ref_value["eembc_telecom"]["empty"].cycles= 0;
    arm_ref_value["eembc_telecom"]["autcor00data_1"].cycles= 0;
    arm_ref_value["eembc_telecom"]["autcor00data_2"].cycles= 0;
    arm_ref_value["eembc_telecom"]["autcor00data_3"].cycles= 0;
    arm_ref_value["eembc_telecom"]["conven00data_1"].cycles= 0;
    arm_ref_value["eembc_telecom"]["conven00data_2"].cycles= 0;
    arm_ref_value["eembc_telecom"]["conven00data_3"].cycles= 0;
    arm_ref_value["eembc_telecom"]["fbital00data_2"].cycles= 0;
    arm_ref_value["eembc_telecom"]["fbital00data_3"].cycles= 0;
    arm_ref_value["eembc_telecom"]["fbital00data_6"].cycles= 0;
    arm_ref_value["eembc_telecom"]["fft00data_1"].cycles= 0;
    arm_ref_value["eembc_telecom"]["fft00data_2"].cycles= 0;
    arm_ref_value["eembc_telecom"]["fft00data_3"].cycles= 0;
    arm_ref_value["eembc_telecom"]["viterb00data_1"].cycles= 0;
    arm_ref_value["eembc_telecom"]["viterb00data_2"].cycles= 0;
    arm_ref_value["eembc_telecom"]["viterb00data_3"].cycles= 0;
    arm_ref_value["eembc_telecom"]["viterb00data_4"].cycles= 0;
}

static void set_arm_iter() {
    arm_ref_value["eembc_consumer"]["rgbyiq01"].iter  = 40;
    arm_ref_value["eembc_consumer"]["rgbhpg01"].iter  = 40;
    arm_ref_value["eembc_consumer"]["rgbcmy01"].iter  = 40;
    arm_ref_value["eembc_consumer"]["djpeg"].iter     = 40;
    arm_ref_value["eembc_consumer"]["cjpeg"].iter     = 40;

    arm_ref_value["eembc_networking"]["ip_pktcheckb512k"].iter= 100;
    arm_ref_value["eembc_networking"]["ip_pktcheckb1m"].iter  = 100;
    arm_ref_value["eembc_networking"]["ip_pktcheckb2m"].iter  = 100;
    arm_ref_value["eembc_networking"]["ip_pktcheckb4m"].iter  = 100;
    arm_ref_value["eembc_networking"]["ip_reassemblyIT"].iter = 500;
    arm_ref_value["eembc_networking"]["ip_reassembly"].iter   = 500;
    arm_ref_value["eembc_networking"]["natIT"].iter           = 1000;
    arm_ref_value["eembc_networking"]["nat"].iter             = 1000;
    arm_ref_value["eembc_networking"]["ospfv2"].iter          = 100;
    arm_ref_value["eembc_networking"]["qos"].iter             = 100;
    arm_ref_value["eembc_networking"]["routelookup"].iter     = 100;
    arm_ref_value["eembc_networking"]["tcpbulk"].iter         = 20000;
    arm_ref_value["eembc_networking"]["tcpjumbo"].iter        = 30000;
    arm_ref_value["eembc_networking"]["tcpmixed"].iter        = 10000;
}

void set_arm_ref_value() {
    set_arm_ref_size();
    set_arm_ref_cycles();
    set_arm_iter();
}


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
    return;
  }

  if(!strcmp(name,"whetstone")){
    _obj_size_tests->push_back(new Test("whetstone-float"));
    _obj_size_tests->push_back(new Test("whetstone-double"));
    _bin_size_tests->push_back(new Test("whetstone-float"));
    _bin_size_tests->push_back(new Test("whetstone-double"));
    return;
  } 

  _obj_size_tests->push_back(new Test(name));
  _bin_size_tests->push_back(new Test(name));
}

void TestSession::add_test_size(char *test_name, char *target, Section sec, 	int size, int disc) {
	Target *mytarget;
	if (!strcmp(test_name, "mp3-32"))      	sprintf(test_name, "%s", "mp3_32");
	if (!strcmp(test_name, "mp3-64"))		sprintf(test_name, "%s", "mp3_64");
	if (!strcmp(test_name, "WB-AMR"))		sprintf(test_name, "%s", "wb_amr");
    if (!strcmp(test_name, "pstone"))       sprintf(test_name, "%s", "PStone");
    if (!strcmp(test_name, "fpu_zoom"))     sprintf(test_name, "%s", "FPU_Zoom");
    if (!strcmp(test_name, "exposurecode")) sprintf(test_name, "%s", "FP_Exposure");
	//printf("%s, %s\n",test_name,target);
	Test *current_test=NULL;
	ForEachPt(get_tests(disc),iter) {
		if (!strcmp((*iter)->get_name(),test_name)) {
			current_test=*iter;
			break;
		}
	}
	if (!current_test) {
		add_test_name(test_name);
		add_test_size(test_name, target, sec, size, disc);
		return;
		//fprintf(stderr, "ERROR: Test suit %s does not exist !!!\n", test_name);
		//exit(1);
	}
	if (!strncmp(test_name, "eembc_", 6) && !strncmp(target, "bin/", 4)) {
		char *tmp_targ=strndup(&target[4], strlen(target)-8);
		free(target);
		target=tmp_targ;
	}

	mytarget=current_test->find_target(target);
	if (!mytarget)
		mytarget=current_test->add_target(target);

	if (mytarget->get_size(sec)!=0) {
		// if(warn_level) fprintf(stderr,"WARNING: Size value for test %s target %s section %s already given, skipped\n",test_name,target,section_name(sec));
		size = size + mytarget->get_size(sec);
	}
	mytarget->set_size(sec, size);
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
IntList *SummaryElem::get_size(Dump_Type type, bool is_obj) {
	if(is_obj) {
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
	} else {
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

void SummaryClass::dump_summary_cc() {
	SummaryElem *base, *compare;
	std::_List_iterator<SummaryElem*, SummaryElem*&, SummaryElem**> iter=_elem->begin();
	//On Cruise_control, we compare base Vs previous and base Vs last to obtain :
	//		Test Vs Previous in the same branch
	//		Test Vs Trunk last rebase
	base = *iter;
	iter++;
	compare = *iter;
	iter++;
	fprintf(stdout,"<property name=\"%s_prev_compiler\" value=\"%s\"/>\n",base->get_name(),get_version_num(compare->get_cc_name()));
	fprintf(stdout,"<property name=\"%s_prev_cycles\" value=\"",base->get_name());
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
	fprintf(stdout,"\" />\n<property name=\"%s_prev_obj_size\" value=\"",base->get_name());
	if (compare->get_size(Run_Valid,true)->empty()) fprintf(stdout," Max Gain: N/A, Max loss:  N/A, Average: N/A");
	else {
		typeof(base->get_size(Run_Valid,true)->begin()) vb,vc;
		int nb_data=0;;
		double sumb=0,sumc=0,min=0,max=0,tmp_val;
		for (vb = base->get_size(Run_Valid,true)->begin(), vc=compare->get_size(Run_Valid,true)->begin(); vb != base->get_size(Run_Valid,true)->end(); ++vb, ++vc) {
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
	fprintf(stdout,"\" />\n<property name=\"%s_prev_bin_size\" value=\"",base->get_name());
	if (compare->get_size(Run_Valid,false)->empty()) fprintf(stdout," Max Gain: N/A, Max loss:  N/A, Average: N/A");
	else {
		typeof(base->get_size(Run_Valid,false)->begin()) vb,vc;
		int nb_data=0;;
		double sumb=0,sumc=0,min=0,max=0,tmp_val;
		for (vb = base->get_size(Run_Valid,false)->begin(), vc=compare->get_size(Run_Valid,false)->begin(); vb != base->get_size(Run_Valid,false)->end(); ++vb, ++vc) {
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
		if (!is_trunk(compare->get_cc_name())) return;
	} else {
		return;
	}
	fprintf(stdout,"<property name=\"%s_trunk_compiler\" value=\"%s\"/>\n",base->get_name(),get_version_num(compare->get_cc_name()));
	fprintf(stdout,"<property name=\"%s_trunk_cycles\" value=\"",base->get_name());
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
	fprintf(stdout,"\" />\n<property name=\"%s_trunk_obj_size\" value=\"",base->get_name());
	if (compare->get_size(Run_Valid,true)->empty()) fprintf(stdout," Max Gain: N/A, Max loss:  N/A, Average: N/A");
	else {
		typeof(base->get_size(Run_Valid,true)->begin()) vb,vc;
		int nb_data=0;;
		double sumb=0,sumc=0,min=0,max=0,tmp_val;
		for (vb = base->get_size(Run_Valid,true)->begin(), vc=compare->get_size(Run_Valid,true)->begin(); vb != base->get_size(Run_Valid,true)->end(); ++vb, ++vc) {
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
	fprintf(stdout,"\" />\n<property name=\"%s_trunk_bin_size\" value=\"",base->get_name());
	if (compare->get_size(Run_Valid,false)->empty()) fprintf(stdout," Max Gain: N/A, Max loss:  N/A, Average: N/A");
	else {
		typeof(base->get_size(Run_Valid,false)->begin()) vb,vc;
		int nb_data=0;;
		double sumb=0,sumc=0,min=0,max=0,tmp_val;
		for (vb = base->get_size(Run_Valid,false)->begin(), vc=compare->get_size(Run_Valid,false)->begin(); vb != base->get_size(Run_Valid,false)->end(); ++vb, ++vc) {
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
		fprintf(stdout,"Max Gain: %5.5s, Max Loss: %5.5s, Average: %5.5s",minst,maxst,moyst);
	}
	fprintf(stdout,"\" />\n");
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
		if (compare->get_size(Run_Valid,true)->empty()) fprintf(stdout," %5.5s / %5.5s / %5.5s  |","  -  ","  -  ","  -  ");
		else {
			typeof(base->get_size(Run_Valid,true)->begin()) vb,vc;
			int nb_data=0;;
			double sumb=0,sumc=0,min=0,max=0,tmp_val;
			for (vb = base->get_size(Run_Valid,true)->begin(), vc=compare->get_size(Run_Valid,true)->begin(); vb != base->get_size(Run_Valid,true)->end(); ++vb, ++vc) {
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
		if (compare->get_size(Run_Valid,false)->empty()) fprintf(stdout," %5.5s / %5.5s / %5.5s  |","  -  ","  -  ","  -  ");
		else {
			typeof(base->get_size(Run_Valid,false)->begin()) vb,vc;
			int nb_data=0;;
			double sumb=0,sumc=0,min=0,max=0,tmp_val;
			for (vb = base->get_size(Run_Valid,false)->begin(), vc=compare->get_size(Run_Valid,false)->begin(); vb != base->get_size(Run_Valid,false)->end(); ++vb, ++vc) {
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

static char *compute_test_target_name(char *name, char **target) {
  if(!strcmp(name,"des.out") || !strcmp(name,"compress.out") || !strcmp(name,"compress.out") || !strcmp(name,"g3fax.out") || !strcmp(name,"jpeg.out")
     ||!strcmp(name,"eval2.out") || !strcmp(name,"pocsag.out") || !strcmp(name,"engine.out") || !strcmp(name,"dhry21.out") || !strcmp(name,"summin.out") 
     ||!strcmp(name,"fir_int.out") || !strcmp(name,"v42bis.out") || !strcmp(name,"auto.out") || !strcmp(name,"ucbqsort.out") || !strcmp(name,"blit.out") 
     ) {
    *target=strndup(name,strlen(name)-4);
    return "pstone";
  }

  
  if(!strcmp(name,"JPEG Decoder CSD")) {
      *target=strdup("JPEG Decoder CSD");
      return "jpeg_csd";
  }
  

  if(!strcmp(name,"Second order IIR")) {
      *target=strdup("IIR");
      return "audio_csd";
  }
  
  if(!strcmp(name,"SRC") || !strcmp(name,"IDCT") || !strcmp(name,"Second order IIR") || !strcmp(name,"IFFT") || !strcmp(name,"Huffman decoding")) {
      *target=strdup(name);
      return "audio_csd";
  }

  
  if(!strncmp(name,"BDTI--",6)) {
    *target=strdup(&name[6]);
    return "benchs64 (BDTI)";
  }

  if(!strncmp(name,"LAO--",5)) {
    char *tmp = strtok(name," ");
    *target=strdup(&tmp[5]);
    return "benchs64 (LAO)";
  }

  if(!strncmp(name,"Automotive--",12)) {
    *target=strdup(&name[12]);
    return "eembc_automotive";
  }

  if(!strncmp(name,"Telecom--",9)) {
    *target=strdup(&name[9]);
    return "eembc_telecom";
  }

  if(!strncmp(name,"Consumer--",10)) {
    *target=strdup(&name[10]);
    return "eembc_consumer";
  }

  if(!strncmp(name,"Networking--",12)) {
    *target=strdup(&name[12]);
    return "eembc_networking";
  }

  if(!strncmp(name,"fp_aec--",8)) {
    *target=strdup(&name[8]);
    return "fp_aec";
  }

  if(!strncmp(name,"Stanford--",10)) {
    *target=strdup(&name[10]);
    return "stanford";
  }

  if(!strcmp(name,"OUT> Dhrystone")) {
    *target=strdup("dhrystone");
    return "dhrystone";
  }

  if(!strcmp(name,"BaseStation")) {
    *target=strdup("basestation");
    return "basestation";
  }

  if(!strcmp(name,"efr-test22cod")) {
    *target=strdup("Coder - test22");
    return "EFR";
  }

  if(!strcmp(name,"efr-test22dec")) {
    *target=strdup("Decoder - test22");
    return "EFR";
  }

  if(!strcmp(name,"efr-dtx03cod")) {
    *target=strdup("Coder - dtx03");
    return "EFR";
  }

  if(!strcmp(name,"efr-dtx03dec")) {
    *target=strdup("Decoder - dtx03");
    return "EFR";
  }

  if(!strcmp(name,"wb_amrcod")) {
    *target=strdup("Coder");
    return "wb_amr";
  }

  if(!strcmp(name,"wb_amrdec")) {
    *target=strdup("Decoder");
    return "wb_amr";
  }

  if(!strncmp(name,"Konica--",8)) {
    *target=strdup(&name[8]);
    return "konica";
  }

  *target=strdup(name);
  return name;
}

/*
exposurecode 
whetstone-float 
whetstone-double 
mp3_32  
mp3_64 
stvid 

*/
