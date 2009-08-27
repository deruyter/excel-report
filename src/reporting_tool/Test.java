package reporting_tool;

import java.util.ArrayList;

public class Test {

	public String name;
	int nb_target;
	private Target current_target;
	ArrayList<Target> targets;

	public Test(String name) {
		this.name = name;
		this.targets = new ArrayList<Target>();
	}

	public ArrayList<Target> get_target() {
		return targets;
	}

	public Target find_target(String name) {
		if(current_target == null || !current_target.name.contentEquals(name)) {
			current_target=null;
			for (int i=0; i<targets.size();i++) {
				if (targets.get(i).name.contentEquals(name)) {
					current_target=targets.get(i);
					return current_target;
				}
			}
		}
		return current_target;
	}
	
	public Target add_target(String name){
		current_target=new Target(name);
		targets.add(current_target);
		return current_target;
	}

}
