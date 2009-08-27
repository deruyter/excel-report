package reporting_tool;

import java.util.ArrayList;

public class RootDataClass {

	private int nb_data_max;
	private ArrayList<TestSession> sessions;
	private ArrayList<RootTest> disc_test;
	private ArrayList<String> ignored_flags;

	
	private void add_test_disc(String test, String target, ArrayList<String> options) {
		for(int i=0;i<disc_test.size();i++) {
			if (disc_test.get(i).is_same(test,target,options)) return;
		}
		disc_test.add(new RootTest(test,target,options));
		nb_data_max++;
	}

	private void cleanup_test_disc() {
		nb_data_max=0;
		if(!disc_test.isEmpty()) {
			disc_test.clear();
		}
	}

	public RootDataClass() {
		sessions = new ArrayList<TestSession>();
		disc_test = new ArrayList<RootTest>();
		ignored_flags = new ArrayList<String>();
		nb_data_max=0;
	}

	public int get_nb_sessions() {
		return sessions.size();
	}
	public int get_nb_max_data() {
		return nb_data_max;
	}

	public TestSession get_session(int position) {	
		return sessions.get(position);
	}


	public void add_session(String path, String name) {
		for(int i=0;i<sessions.size();i++) {
			if (sessions.get(i).path.contentEquals(path) && sessions.get(i).name.contentEquals(name)) {
				if(sqa_report.warn_level > 0) System.err.println("WARNING: Session "+ name + "%s already given, skipped\n");
				continue;
			}
		}
		sessions.add(new TestSession(path,name));
	}
	
	public void compute_data(Discriminent disc) {
		cleanup_test_disc();
		for(int i=0;i<sessions.size();i++) { 
			TestSession mysession = sessions.get(i);
			for(int j=0;j<mysession.get_tests(disc).size();j++) { 
				Test mytest = mysession.get_tests(disc).get(j);
				if (sqa_report.core_only && CommonData.is_extension_info(mytest.name))  continue;
				if (sqa_report.ext_only && !CommonData.is_extension_info(mytest.name))  continue;
				for(int k=0;k<mytest.get_target().size();k++) { 
					add_test_disc(mytest.name,mytest.get_target().get(k).name, mysession.compiler_flags);
					//printf("DISC: %s, %s\n",(*test)->get_name(),(*target)->get_name());
				}
			}
		}
	}
	public void compute_monitored_data(Discriminent disc){
		cleanup_test_disc();
		TestSession mysession = sessions.get(0);
		for(int j=0;j<mysession.get_tests(disc).size();j++) { 
			Test mytest = mysession.get_tests(disc).get(j);
			for(int k=0;k<mytest.get_target().size();k++) { 
				add_test_disc(mytest.name,mytest.get_target().get(k).name, mysession.compiler_flags);
				//  printf("DISC: %s, %s\n",(*test)->get_name(),(*target)->get_name());
			}
		}
	}
	public void remove_ignored_flags(boolean no_ignore) {
		if (no_ignore) {
			for(int i=0;i<sessions.size();i++) { 
				TestSession mysession = sessions.get(i);
				for(int j=0;i<ignored_flags.size();i++) { 
					mysession.ignore_flag(ignored_flags.get(j));
				}
			}
		} else {
			for(int i=0;i<sessions.size();i++) { 
				TestSession mysession = sessions.get(i);
				mysession.ignore_all_flags();
			}
		}

	}
	public void add_ignored_flags(String name) {
		ignored_flags.add(name);
	}

	public ArrayList<RootTest> get_disc() {
		return disc_test;
	}
}
