import os
import xml.etree.ElementTree as ET
import random

subsets = ["10", "20", "30", "40", "50", "60", "70", "mix"]
for subset_size in subsets:
	if subset_size == "mix":
		xml_file = "MUTA-GED/Mutagenicity-subsets/mixed-graphs.cxl"
	else:
		xml_file = "MUTA-GED/Mutagenicity-subsets/train" + subset_size + ".cxl"
	tree = ET.parse(xml_file)
	root = tree.getroot()[0]
	data_name = []
	for child in root:
		data_name.append("MUTA-GED/Mutagenicity/" + child.get('file'))
	output_dir = "testdata/MUTA-" + subset_size
	if not os.path.exists(output_dir):
		os.makedirs(output_dir)
	Tnum = 10
	for i in range(Tnum):
		u = random.randint(0, len(data_name) - 1)
		v = random.randint(0, len(data_name) - 1)
		print(u, v, len(data_name))
		current_dir = output_dir + "/" + str(i + 1)
		if not os.path.exists(current_dir):
			os.makedirs(current_dir)
		os.system("cp " + data_name[u] + " " + current_dir + "/1.gxl")
		os.system("cp " + data_name[v] + " " + current_dir + "/2.gxl")
