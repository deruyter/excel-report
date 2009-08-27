package reporting_tool;

import java.util.ArrayList;

public class CommonData {

	public static final int NOT_EXECUTED = -2;
	public static final int HAS_FAILED   = -3;
	public static final int NOT_RELEVANT = -4;

	
	public static boolean  is_extension_info(String name) {
		if (name.contains("TSTx_test") || name.contains("vx2_fgtdec") || name.contains("vx2_test_memspace") ||
				name.contains("vx2_test_memspace") || name.contains("TS3x_test") || name.contains("TS2x_test") ||
				name.contains("admX_AppliUsingadmX") || name.contains("QMx_farrow_interpolator") || name.contains("Tx_CSD_extension_test") ||
				name.contains("QMx_mixer") || name.contains("MP1v_viterbi") || name.contains("Fx_SampleRate") || name.contains("TS4x_test")) {
			return true;
		}
		return false;
	}	
	
	public static boolean is_same_list(ArrayList<String> options1, ArrayList<String> options2) {
		if(options1.size() == options2.size() && options1.containsAll(options2)) 
			return true;
		return false;
	}

	public enum  Dump_Type {
		Run_Valid,
		EEMBC_Net,
		EEMBC_Cons,
		Bluetooth,
		Jpeg,
		Audio_CSD,
	} ;

}
