package reporting_tool;

import java.util.ArrayList;

public class RootTest implements java.lang.Comparable {
	String test;
	String target;
	ArrayList<String> options;

	public String get_test() {return test;}
	public String get_target() {return target;}
	public ArrayList<String> get_options() {return options;}

	public RootTest(String test, String target, ArrayList<String> options) {
		this.test = test;
		this.target = target;
		this.options = options;
	}

	public boolean is_same(String test, String target, ArrayList<String> options) {
		if(this.test.equals(test) && this.target.equals(target) && CommonData.is_same_list(this.options,options)) 
			return true;
		return false;
	}

	public int compareTo(Object other) { 
	      String v1 = ((RootTest) other).get_test(); 
	      return this.test.compareTo(v1);
	   } 
}
