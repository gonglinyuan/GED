import os
import sys
from subprocess import run
lst = []
for root, dirs, files in os.walk(sys.argv[1], topdown=False):
    for name in files:
        lst.append(os.path.join(root, name))
for i in range(len(lst)):
    if lst[i].endswith(".gxl"):
        for j in range(i, len(lst)):
            if lst[j].endswith(".gxl"):
                # run(["F2_symbolic_distance", "2", "4", "1", "1", lst[i], lst[j]])
                run(["./../cmake-build-debug/GED", "2", "4", "1", "1", lst[i], lst[j]])
