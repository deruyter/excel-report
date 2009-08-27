package reporting_tool;

import java.util.HashMap;

public class Target {
	public String name;
	public HashMap<Sections,Integer> sizes;
	public long cycles;

	public Target(String name) {
		this.name=name;
		this.sizes = new HashMap<Sections,Integer>();
		this.cycles = -1;
	}

	public String get_name()	{
		return name;
	}

	public void set_size(Sections section, Integer size) {
		sizes.put(section, size);
	}
	
	public void set_cycle(long cycles) {
		this.cycles=cycles;
	}
	
	public Integer get_tgt_size(Sections section) {
		if (sizes.isEmpty()) return 0;
		Integer value=0;
		if (section == Sections.RODATA_PLUS_TEXT ) {
			if (sizes.containsKey(Sections.RODATA)) value += sizes.get(Sections.RODATA);
			if (sizes.containsKey(Sections.TEXT)) value += sizes.get(Sections.TEXT);
		} else 	if (sizes.containsKey(section)) value = sizes.get(section);
		return value;
	}
	
	public long get_cycle() {
		return cycles;
	}

}
