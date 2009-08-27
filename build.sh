#!/bin/sh

local_position=`pwd` 

# build cycle_analyzer
cd src/cycle_analyzer
rm *.java*
java -jar ../JFlex.jar parse_cycle.jflex
java -jar ../java-cup-11a.jar -parser Cycle_Parser parse_cycle.cup
cd $local_position
# build Fail Parser
cd src/fail_analyzer
rm *.java*
java -jar ../JFlex.jar parse_fail.jflex
java -jar ../java-cup-11a.jar -parser Fail_Parser parse_fail.cup
cd $local_position
# build func_analyzer
cd src/func_analyzer
rm *.java*
java -jar ../JFlex.jar parse_func.jflex
java -jar ../java-cup-11a.jar -parser Func_Parser parse_func.cup
cd $local_position
# build info_analyzer
cd src/info_analyzer
rm *.java*
java -jar ../JFlex.jar parse_info.jflex
java -jar ../java-cup-11a.jar -parser Info_Parser parse_info.cup
cd $local_position
# build size_analyzer
cd src/size_analyzer
rm *.java*
java -jar ../JFlex.jar parse_size.jflex
java -jar ../java-cup-11a.jar -parser Size_Parser parse_size.cup
cd $local_position

exit
javac -classpath lib/java-cup-11a-runtime.jar:lib/poi-3.5-beta6-20090622.jar -d bin -sourcepath src src/*/*.java 
cd bin
jar xvf ../lib/java-cup-11a-runtime.jar
jar xvf ../lib/poi-3.5-beta6-20090622.jar
cd ..
jar cvfm sqa_report.jar MANIFEST -C bin/ . 




