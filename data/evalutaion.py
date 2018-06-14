import os
import sys

name = sys.argv[1]
dataset = ""
if len(sys.argv) >= 3: dataset = sys.argv[2]

dirs = os.listdir("testdata")

for test_set_name in dirs:
	if dataset not in test_set_name: continue
	test_path = "testdata/" + test_set_name
	datas = os.listdir(test_path)
	datas = sorted(datas)
	result = []
	for data_name in datas:
		data_path = test_path + "/" + data_name
		print(data_path)
		os.system("../cmake-build-debug/" + name + " 2 4 1 1 " + data_path + "/1.gxl " + data_path + "/2.gxl > result.out")
		ans = int(open("result.out", "r").readline()[: -1])
		print(ans)
		result.append(ans)

	result_dir = "result/" + name
	if not os.path.exists(result_dir):
		os.makedirs(result_dir)
	output_file = open(result_dir + "/" + test_set_name + ".out", "w")
	for i in result:
		output_file.write(str(i) + "\n")	
	output_file.close()
		
