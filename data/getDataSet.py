import os
import pandas
import random
import sys

lst = []

cost_mat = {}
for a, b, c, d, e in pandas.read_csv("best_cost/" + sys.argv[1] + ".csv").get_values():
    cost_mat[(a + ".gxl", b + ".gxl")] = (c, d, e)

for root, dirs, files in os.walk(sys.argv[1], topdown=False):
    for name in files:
        if "(" in name: continue
        lst.append(os.path.join(root, name))
output_dir = "testdata/" + sys.argv[1]
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

Tnum = 30
lst2 = []
for i in range(Tnum):
    name1, name2 = "dataset.cxl", "dataset.cxl"
    while name1 == "dataset.cxl" or name2 == "dataset.cxl":
        u = random.randint(0, len(lst) - 1)
        v = random.randint(0, len(lst) - 1)
        # print(lst[u], lst[v], len(lst))
        name1 = lst[u].split('/')[-1]
        name2 = lst[v].split('/')[-1]
    if name1 == name2:
        lst2.append((0, 0, 0))
    else:
        lst2.append(cost_mat[(name1, name2)])
    current_dir = output_dir + "/" + str(i + 1)
    if not os.path.exists(current_dir):
        os.makedirs(current_dir)
    os.system("cp " + lst[u] + " " + current_dir + "/1.gxl")
    os.system("cp " + lst[v] + " " + current_dir + "/2.gxl")

print(pandas.DataFrame(lst2).to_csv(header=None, index=None))