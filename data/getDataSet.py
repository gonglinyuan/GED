import random
import os
import sys

lst = []
for root, dirs, files in os.walk(sys.argv[1], topdown=False):
	for name in files:
		if "(" in name: continue
		lst.append(os.path.join(root, name))
output_dir = "testdata/" + sys.argv[1]
if not os.path.exists(output_dir):
	os.makedirs(output_dir)

Tnum = 20
for i in range(Tnum):
	u = random.randint(0, len(lst) - 1)
	v = random.randint(0, len(lst) - 1)
	print(u, v, len(lst))
	current_dir = output_dir + "/" + str(i + 1)
	if not os.path.exists(current_dir):
		os.makedirs(current_dir)
	os.system("cp " + lst[u] + " " + current_dir + "/1.gxl")
	os.system("cp " + lst[v] + " " + current_dir + "/2.gxl")

