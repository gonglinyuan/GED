import os
import pandas
import random
import xml.etree.ElementTree as ET

subsets = ["10", "20", "30", "40", "50", "60", "70", "mix"]
lst2 = []
for subset_size in subsets:
    cost_mat = {}
    for a, b, c, d, e in pandas.read_csv("best_cost/muta" + subset_size + ".csv").get_values():
        cost_mat[(a + ".gxl", b + ".gxl")] = (c, d, e)
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
    Tnum = 15
    for i in range(Tnum):
        u = random.randint(0, len(data_name) - 1)
        v = random.randint(0, len(data_name) - 1)
        name1 = data_name[u].split('/')[-1]
        name2 = data_name[v].split('/')[-1]
        # print(name1, name2, cost_mat[(name1, name2)])
        if name1 == name2:
            lst2.append((0, 0, 0))
        else:
            lst2.append(cost_mat[(name1, name2)])
        current_dir = output_dir + "/" + str(i + 1)
        if not os.path.exists(current_dir):
            os.makedirs(current_dir)
        os.system("cp " + data_name[u] + " " + current_dir + "/1.gxl")
        os.system("cp " + data_name[v] + " " + current_dir + "/2.gxl")

print(pandas.DataFrame(lst2).to_csv(header=None, index=None))
