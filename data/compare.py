import sys

with open(sys.argv[1], 'r') as out, open(sys.argv[2], 'r') as ans:
    lst = []
    for s in out:
        if s.startswith('Academic'):
            continue
        lst.append(float(s.split(';')[-1]))
    lst2 = []
    for s in ans:
        if s.startswith('Academic'):
            continue
        lst2.append(float(s.split(';')[-1]))

if len(lst) != len(lst2):
    print('output number not same!!')
    exit(-1)

err_cnt = 0
for i in range(len(lst)):
    if (int(round(lst[i])) != int(round(lst2[i]))):
        print(str(i + 1) + "th data case is wrong")
        err_cnt = err_cnt + 1

print('compare finished, err_cnt = ' + str(err_cnt))
