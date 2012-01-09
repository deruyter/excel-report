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

#exit
mkdir bin
javac -classpath lib/java-cup-11a-runtime.jar:lib/poi-3.6/poi-3.6-20091214.jar -d bin -sourcepath src src/*/*.java 
if [ $? -ne 0 ]; then
	echo "###"
	echo "### ERROR when compiling sqa_report !! Leaving."
	echo "###"
	exit
fi
cd bin
echo "## Extracting java-cup-11a-runtime.jar ..."
jar xf ../lib/java-cup-11a-runtime.jar
echo "## Extracting poi-3.6/poi-3.6-20091214.jar ..."
jar xf ../lib/poi-3.6/poi-3.6-20091214.jar
cd ..
echo "## Building sqa_report.jar ..."
jar cfm sqa_report.jar MANIFEST -C bin/ . 




