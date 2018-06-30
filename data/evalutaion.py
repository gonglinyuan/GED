import os
import sys
from subprocess import run

dataset = ""
if len(sys.argv) >= 2: dataset = sys.argv[1]

dirs = os.listdir("testdata")

for test_set_name in dirs:
    if dataset not in test_set_name: continue
    test_path = "testdata/" + test_set_name
    datas = os.listdir(test_path)
    datas = sorted(datas)
    result = []
    for data_name in datas:
        data_path = test_path + "/" + data_name
        # print(data_path)
        # print(" ".join(["./GED", "2", "4", "1", "1", data_path + "/1.gxl ", data_path + "/2.gxl"]))
        run(["./GED", "2", "4", "1", "1", data_path + "/1.gxl", data_path + "/2.gxl"])

for test_set_name in dirs:
    if dataset not in test_set_name: continue
    test_path = "testdata/" + test_set_name
    datas = os.listdir(test_path)
    datas = sorted(datas)
    result = []
    for data_name in datas:
        data_path = test_path + "/" + data_name
        # print(data_path)
        # print(" ".join(["./GED", "2", "4", "1", "1", data_path + "/1.gxl ", data_path + "/2.gxl"]))
        run(["./GED", "2", "4", "1", "2", data_path + "/1.gxl", data_path + "/2.gxl"])

for test_set_name in dirs:
    if dataset not in test_set_name: continue
    test_path = "testdata/" + test_set_name
    datas = os.listdir(test_path)
    datas = sorted(datas)
    result = []
    for data_name in datas:
        data_path = test_path + "/" + data_name
        # print(data_path)
        # print(" ".join(["./GED", "2", "4", "1", "1", data_path + "/1.gxl ", data_path + "/2.gxl"]))
        run(["./GED", "6", "2", "3", "1", data_path + "/1.gxl", data_path + "/2.gxl"])
