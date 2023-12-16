import subprocess
import sys

# count = 3
# len = 10
# insert_rate = 0.01
# mutate_rate = 0.01

print("algo,count,len,insert_rate,mutate_rate,score,time_us,mem_mb")

algos = ["star","tree"]
# algos = ["exact","bound","star","tree"]
# 2.csv 2 [0,5000] 0.01 0.01
# 3.csv 3 [0,500] 0.01 0.01
# 3_longer.csv 3 [0,1450] 0.01 0.01

# runs = [[3, int(50*x), 0.1, 0.1] for x in range(1,11)]
runs = [[x, 10000, 0.05, 0.05] for x in range(2,11)]
# runs = [[3, int(50*x), 0.01, 0.01] for x in range(30,60)]


for r in runs:
    for algo in algos:
        for i in range(3):
            [count, len, insert_rate, mutate_rate] = r
            result = subprocess.check_output([".\\build\\algo_project.exe", algo, f'{count}', f'{len}', f'{insert_rate}', f'{mutate_rate}'])
            [score, time_us, mem_mb, _] = result.decode().split("\r\n")
            print(f'{algo},{count},{len},{insert_rate},{mutate_rate},{score},{time_us},{mem_mb}')
            print(f'{algo},{count},{len},{insert_rate},{mutate_rate},{score},{time_us},{mem_mb}', file=sys.stderr)

