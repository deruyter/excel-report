# XML reports reader Makefile

#### Tools ###############################################################

ifdef PURIFY
DBG = 1
CC = purify -best_effort gcc
else
CC = g++
endif

EXTENSION =

YACC = /usr/bin/bison
LEX = flex

#### Flags  ##############################################################
ifdef DBG
ifdef PURIFY
YFLAGS  = -d --debug
LFLAGS  = -+
CCFLAGS = -g -Wall
else
YFLAGS  = -d --debug --verbose --token-table
CCFLAGS = -DDBG -g -Wall
endif
else
YFLAGS  = -d
LFLAGS  = 
CCFLAGS = -Wall -O3
endif

#### List of source files to be used ########################

SRC_DIR = .
ifdef SYS
BIN_DIR = bin/$(host-type)
else
BIN_DIR = .
endif

YACCSRC1  = $(SRC_DIR)/parse_info.ypp
CYACC1    = $(SRC_DIR)/parse_info_yacc.cpp
LEXSRC1   = $(SRC_DIR)/parse_info.lex
CLEX1     = $(SRC_DIR)/parse_info_lex.cpp

YACCSRC2  = $(SRC_DIR)/parse_size.ypp
CYACC2    = $(SRC_DIR)/parse_size_yacc.cpp
LEXSRC2   = $(SRC_DIR)/parse_size.lex
CLEX2     = $(SRC_DIR)/parse_size_lex.cpp

YACCSRC3  = $(SRC_DIR)/parse_cycle.ypp
CYACC3    = $(SRC_DIR)/parse_cycle_yacc.cpp
LEXSRC3   = $(SRC_DIR)/parse_cycle.lex
CLEX3     = $(SRC_DIR)/parse_cycle_lex.cpp

YACCSRC4  = $(SRC_DIR)/parse_fail.ypp
CYACC4    = $(SRC_DIR)/parse_fail_yacc.cpp
LEXSRC4   = $(SRC_DIR)/parse_fail.lex
CLEX4     = $(SRC_DIR)/parse_fail_lex.cpp

CFILES =  $(CYACC1) $(CLEX1) $(CYACC2) $(CLEX2) $(CYACC3) $(CLEX3) $(CYACC4) $(CLEX4)\
	$(SRC_DIR)/data.cpp \
	$(SRC_DIR)/xml_dump.cpp \
	$(SRC_DIR)/reporting_tool.cpp


ALL_OBJS = $(patsubst %.cpp,%.o,$(notdir $(CFILES)))

PGM := $(BIN_DIR)/sqa_report$(EXTENSION)

all:    $(PGM)


$(PGM): lex $(ALL_OBJS)
	${CC} $(CCFLAGS) $(CFLAGS) -I $(SRC_DIR) -o $@ $(ALL_OBJS)
	chmod ug+rwx $(PGM)

clean :
	/bin/rm -f *.o $(PGM) $(SRC_DIR)/*_yacc.* $(SRC_DIR)/*_lex.*

lex:	$(CYACC1) $(CLEX1) $(CYACC2) $(CLEX2) $(CYACC3) $(CLEX3) $(CYACC4) $(CLEX4) 


$(CYACC1) : $(YACCSRC1)
	    $(YACC) $(YFLAGS) -p yy1 $(YACCSRC1) -o $(CYACC1)

$(CYACC2) : $(YACCSRC2)
	    $(YACC) $(YFLAGS) -p yy2 $(YACCSRC2) -o $(CYACC2)

$(CYACC3) : $(YACCSRC3)
	    $(YACC) $(YFLAGS) -p yy3 $(YACCSRC3) -o $(CYACC3)

$(CYACC4) : $(YACCSRC4)
	    $(YACC) $(YFLAGS) -p yy4 $(YACCSRC4) -o $(CYACC4)

$(CLEX1) : $(LEXSRC1)
	   $(LEX) $(LFLAGS) -o$(CLEX1) -Pyy1 $(LEXSRC1)

$(CLEX2) : $(LEXSRC2)
	   $(LEX) $(LFLAGS) -o$(CLEX2) -Pyy2 $(LEXSRC2)

$(CLEX3) : $(LEXSRC3)
	   $(LEX) $(LFLAGS) -o$(CLEX3) -Pyy3 $(LEXSRC3)

$(CLEX4) : $(LEXSRC4)
	   $(LEX) $(LFLAGS) -o$(CLEX4) -Pyy4 $(LEXSRC4)

%.o : $(SRC_DIR)/%.cpp 
	${CC} ${CCFLAGS} $(CFLAGS) -I . -I $(SRC_DIR) -o $@ -c $<


