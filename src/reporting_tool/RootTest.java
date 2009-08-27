package reporting_tool;

import java.util.ArrayList;

public class RootTest {
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

}
