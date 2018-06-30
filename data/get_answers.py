import os
import pandas
import sys
from subprocess import run

dataset = ""
if len(sys.argv) >= 2: dataset = sys.argv[1]

dirs = os.listdir("testdata")

for test_set_name in ["alkane", "acyclic", "pah", "mao", "MUTA-10", "MUTA-20", "MUTA-30", "MUTA-40", "MUTA-50",
                      "MUTA-60", "MUTA-70", "MUTA-mix"]:
    if dataset not in test_set_name: continue
    test_path = "testdata/" + test_set_name
    datas = os.listdir(test_path)
    datas = sorted(datas)
    result = []
    for data_name in datas:
        data_path = test_path + "/" + data_name
        num = int(data_name)
        if test_set_name.startswith("MUTA"):
            offset = 0
            if test_set_name.endswith("10"):
                offset = 0
            elif test_set_name.endswith("20"):
                offset = 15
            elif test_set_name.endswith("30"):
                offset = 30
            elif test_set_name.endswith("40"):
                offset = 45
            elif test_set_name.endswith("50"):
                offset = 60
            elif test_set_name.endswith("60"):
                offset = 75
            elif test_set_name.endswith("70"):
                offset = 90
            else:
                offset = 105
            dat = pandas.read_csv("muta_ans.csv", header=None, index_col=None).get_values()
            print(dat[offset + num - 1][0])
            print(dat[offset + num - 1][1])
            print(dat[offset + num - 1][2])
        else:
            dat = pandas.read_csv(test_set_name + "_ans.csv", header=None, index_col=None).get_values()
            print(dat[num - 1][0])
            print(dat[num - 1][1])
            print(dat[num - 1][2])
