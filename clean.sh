#!/bin/sh

local_position=`pwd` 

# clean cycle_analyzer
rm -f src/cycle_analyzer/*.java*
# clean Fail Parser
rm -f src/fail_analyzer/*.java*
# clean func_analyzer
rm -f src/func_analyzer/*.java*
# clean info_analyzer
rm -f src/info_analyzer/*.java*
# clean size_analyzer
rm -f src/size_analyzer/*.java*



