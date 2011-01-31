package reporting_tool;

import java.util.ArrayList;
import java.util.HashMap;

public class RootDataClass {

    private int nb_data_max;
    private ArrayList<TestSession> sessions;
    private ArrayList<String> ignored_flags;
    private HashMap<String, ArrayList<RootTest>> disc_test;

    private void add_test_disc(String test, String target, ArrayList<String> options) {
        ArrayList<RootTest> my_tests=disc_test.get(test);
        if (my_tests != null) {
            for (int i = 0; i < my_tests.size(); i++) {
                if (my_tests.get(i).is_same(test, target, options)) {
                    return;
                }
            }
            my_tests.add(new RootTest(test, target, options));
            nb_data_max++;
        } else {
            my_tests =  new ArrayList<RootTest>();
            my_tests.add(new RootTest(test, target, options));
            disc_test.put(test, my_tests);
            nb_data_max++;
        }
    }

    private void cleanup_test_disc() {
        nb_data_max = 0;
        if (!disc_test.isEmpty())
            disc_test.clear();
    }

    public RootDataClass() {
        sessions = new ArrayList<TestSession>();
        disc_test = new HashMap<String, ArrayList<RootTest>>();
        ignored_flags = new ArrayList<String>();
        nb_data_max = 0;
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
        for (int i = 0; i < sessions.size(); i++) {
            if (sessions.get(i).path.contentEquals(path) && sessions.get(i).name.contentEquals(name)) {
                if (sqa_report.warn_level > 0) {
                    System.err.println("WARNING: Session " + name + "%s already given, skipped\n");
                }
                return;
            }
        }
        sessions.add(new TestSession(path, name));
    }

    public void compute_data(Discriminent disc) {
        cleanup_test_disc();
        for (int i = 0; i < sessions.size(); i++) {
            TestSession mysession = sessions.get(i);
            for (int j = 0; j < mysession.get_tests(disc).size(); j++) {
                Test mytest = mysession.get_tests(disc).get(j);
                if (sqa_report.core_only && CommonData.is_extension_info(mytest.name)) {
                    continue;
                }
                if (sqa_report.ext_only && !CommonData.is_extension_info(mytest.name)) {
                    continue;
                }
                for (int k = 0; k < mytest.get_target().size(); k++) {
                    add_test_disc(mytest.name, mytest.get_target().get(k).name, mysession.compiler_flags);
                }
            }
        }
    }

    public void compute_monitored_data(Discriminent disc) {
        cleanup_test_disc();
        TestSession mysession = sessions.get(0);
        for (int j = 0; j < mysession.get_tests(disc).size(); j++) {
            Test mytest = mysession.get_tests(disc).get(j);
            for (int k = 0; k < mytest.get_target().size(); k++) {
                add_test_disc(mytest.name, mytest.get_target().get(k).name, mysession.compiler_flags);
            }
        }
    }

    public void remove_ignored_flags(boolean no_ignore) {
        if (no_ignore) {
            for (int i = 0; i < sessions.size(); i++) {
                TestSession mysession = sessions.get(i);
                for (int j = 0; i < ignored_flags.size(); i++) {
                    mysession.ignore_flag(ignored_flags.get(j));
                }
            }
        } else {
            for (int i = 0; i < sessions.size(); i++) {
                TestSession mysession = sessions.get(i);
                mysession.ignore_all_flags();
            }
        }

    }

    public void add_ignored_flags(String name) {
        ignored_flags.add(name);
    }

    public HashMap<String, ArrayList<RootTest>>  get_disc() {
//            ArrayList<RootTest> get_disc() {
        return disc_test;
    }
}
