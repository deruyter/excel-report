#ifndef __DATA_STRUCT__
#define __DATA_STRUCT__

#include <stdio.h>
#include <string.h>
#include <list>
#include <map>

extern int warn_level;
extern bool core_only, ext_only;
extern bool Cruise_Control;
extern bool Monitoring;

typedef struct {
    int size;
    double cycles;
    int thumb_size;
    int thumb_cycles;
    int iter;
} TypeVal;

typedef std::map<char*, TypeVal>  CharMap;
typedef std::map<char*, CharMap> ValTab;

extern ValTab arm_ref_value;
extern void set_arm_ref_value();

#define ForEachPt(container,iter)						\
  for (typeof(container->begin()) iter = container->begin(); iter != container->end(); ++iter)

#define ForEachRPt(container,iter)						\
  for (typeof(container->rbegin()) iter = container->rbegin(); iter != container->rend(); ++iter)

#define NOT_EXECUTED    -2
#define HAS_FAILED      -3
#define NOT_RELEVANT    -4

enum {
  SIZE_OBJ,
  SIZE_BIN,
  SPEED,
  SPEED_Vs_ARM
};

enum {
  PERCENT_STYL_ID,
  BOLDLEFT_STYL_ID,
  BOLDRIGHT_STYL_ID,
  BOLDCENTER_STYL_ID,
  FAIL_STYL_ID,
  BOLDCENTER_STYL_ID_16,
};

typedef enum {
	Run_Valid,
	EEMBC_Net,
	EEMBC_Cons,
	Bluetooth,
	Jpeg,
	Audio_CSD,
} Dump_Type;

typedef enum {
  TEXT,
  DATA,
  RODATA,
  BSS,
  SYMTAB,
  STRTAB,
  DBG_SECTION,
  RELA_TEXT,
  RELA_DATA,
  RELA_RODATA,
  RELA_DBG_FRAME,
  STXP70_EXTRECONF_INFO,
  STXP70_MEMINFO,
  STXP70_MEMMAP_INFO,
  STARTUP,
  THANDLERS,
  STACK1,
  HEAP,
  SECINIT,
  SHSRTAB,
  DA,
  TDA,
  SDA,
  RELA_TDA_DATA,
  RELA_TDA_BSS,
  RELA_TDA_RO,
  RELA_SDA_DATA,
  RELA_SDA_BSS,
  RELA_SDA_RO,
  RELA_DA_DATA,
  RELA_DA_BSS,
  RELA_DA_RO,
  RELA_TDA,
  RELA_SDA,
  RELA_DA,
  TDA_DATA,
  TDA_BSS,
  TDA_RO,
  SDA_DATA,
  SDA_BSS,
  SDA_RO,
  DA_DATA,
  DA_BSS,
  DA_RO,
  SBSS,
  SYSCALL,
  ITHANDLERS,
  IVTABLE,
  ROBASE,
  TOTAL,
  RODATA_PLUS_TEXT,
  LAST_SECTION,
} Section;

class DateType {
 private :
  short _year;
  short _month;
  short _day;
  short _hour;
  short _min;
  short _sec;

 public:
  DateType() {};
  DateType(short d,short m,short y,short h,short min,short sec);
  ~DateType(void) {};
  char *print_date();
  char *print_time();
};

class Target {
 public:
  Target(char *name);
  ~Target(void) {};
  char *get_name()	{return _name;}
  void set_size(Section section, int size) {_size[section]=size;}
  void set_cycle(int cycles) {_cycles=cycles;}
  int get_size(Section section);
  int get_cycle() {return _cycles;}
 private:
  char *_name;
  int _size[LAST_SECTION];
  int _cycles;
};

typedef std::list<Target*> TargetList;

class Test {
 public:
  Test(char *name);
  ~Test(void) {};
  char *get_name()	{return _name;}
  TargetList *get_target() {return _targets;}
  Target *find_target(char *name);
  Target *add_target(char *name);
 private:
  char *_name;
  int _nb_target;
  Target *_current_target;
  TargetList *_targets;
};

typedef std::list<Test*> TestList;
typedef std::list<char*> NameList;

bool is_same_list(NameList *l1,NameList *l2);

class RootTest {
public:
  RootTest(char *test, char *target, NameList *options);
  ~RootTest() {};
  char *get_test() {return _test;}
  char *get_target() {return _target;}
  NameList *get_options() {return _options;}
  bool is_same(char *test, char *target, NameList *options);
private:
  char *_test;
  char *_target;
  NameList *_options;
};


class TestSession {
public:
  TestSession(char *path,char *name);
  ~TestSession(void)       {};
  char *get_cc_name();
  char *get_name()	    	{return _name;}
  char *get_path()          {return _path;}
  char *get_cc_ver()        {return _compiler_version?_compiler_version:_empty;}
  char *get_cc_date()       {return _compiler_date?_compiler_date:_empty;}
  NameList *get_cc_flags()  {return _compiler_flags;}
  char *get_sim_ver()       {return _simulator_version?_simulator_version:_empty;}
  NameList *get_sim_flags() {return _simulator_flags;}
  DateType get_date()       {return _date;}
  void dump_info();
  void set_date(short d,short m,short y,short h,short min,short sec);
  void set_compiler_name(char *name);
  void set_simulator_version(char *name);
  void set_compiler_version(char *name, char *date);
  void set_support_mode(char *name);
  void add_compiler_flags(char *name);
  void add_simulator_flags(char *name);
  void add_test_name(char *name);
  void add_test_size(char *test_name,char *target,Section sec,int size,int disc);
  void add_cycle_count(char *test_name, int number);
  void add_failure(char *name);
  TestList *get_tests(int disc);
  int get_size(RootTest *tests, Section sec, int disc, bool monitor);
  int get_cycle(RootTest *tests);
  void ignore_flag(char *flag);
  void ignore_all_flags();
private:
  char *_empty;
  char *_name;
  DateType _date; 
  char *_path;
  char *_compiler_name;
  char *_compiler_version;
  char *_compiler_date;
  char *_simulator_version;
  char *_mode;
  NameList *_compiler_flags;
  NameList *_simulator_flags;
  NameList *_failures;
  TestList *_obj_size_tests; 
  TestList *_bin_size_tests; 
  TestList *_speed_tests; 
  char *section_name(Section sec);
  Test *find_test(char *name, int disc);
  bool _extension_built;
};

typedef std::list<int> IntList;

class SummaryElem {
 public:
  SummaryElem(char *path,char *name);
  ~SummaryElem(void) {};
  void add_cycle(char * tname, int val);
  void add_bin_size(char * tname, int val);
  void add_obj_size(char * tname, int val);
  char *get_name() {return _name;}
  char *get_cc_name();
  char *get_path() {return _path;}
  IntList *get_cycles(Dump_Type type);
  IntList *get_size(Dump_Type type, bool is_obj);
 private:
  char *_name;
  char *_path;
  IntList *_cycles;
  IntList *_sizes_obj;
  IntList *_sizes_bin;
  IntList *_NW_cycles;
  IntList *_CO_cycles;
  IntList *_BT_cycles;
  IntList *_CSD_cycles;
  IntList *_JPEG_cycles;
  IntList *_NW_sizes_obj;
  IntList *_CO_sizes_obj;
  IntList *_BT_sizes_obj ;
  IntList *_CSD_sizes_obj;
  IntList *_JPEG_sizes_obj;
  IntList *_NW_sizes_bin;
  IntList *_CO_sizes_bin;
  IntList *_BT_sizes_bin;
  IntList *_CSD_sizes_bin;
  IntList *_JPEG_sizes_bin;
  };

typedef std::list<TestSession*> SessionList;
typedef std::list<RootTest*> RootTestList;
typedef std::list<SummaryElem*> SummaryElemList;


class  SummaryClass {
 public:
  SummaryClass();
  ~SummaryClass(void) {};
  void dump_summary();
  void dump_summary_cc();
  void add_session(char *path,char *name);
  void add_summary_value(char *path, char * name, char *tname, int type, int value);
  SummaryElemList *get_list_elem() {return _elem;};
 private:
  SummaryElemList *_elem;
  int _nb_elem;
} ;

extern SummaryClass summary;



class RootDataClass {
 public:
  RootDataClass();
  ~RootDataClass(void) {};
  int get_nb_sessions() {return _nb_sessions;}
  int get_nb_max_data() {return _nb_data_max;}
  void add_session(char *path,char *name);
  SessionList *get_sessions() {return _sessions;}
  void compute_data(int disc);
  void compute_monitored_data(int disc);
  void remove_ignored_flags(bool no_ignore);
  void add_ignored_flags(char *name) {_ignored_flags->push_back(strdup(name));}
  RootTestList *get_disc() {return _disc_test;}
  void dump_summary();
 private:
  int _nb_sessions;
  int _nb_data_max;
  SessionList *_sessions;
  RootTestList *_disc_test;
  void add_test_disc(char *test, char *target, NameList *options);
  void cleanup_test_disc();
  NameList *_ignored_flags;
};
#endif
