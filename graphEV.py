import matplotlib.pyplot as plt
import numpy as np

sort = []
time = []

file = open('E:/VS CODE C/results/data.txt', 'r')
data = file.read().splitlines()

for i in data:
    broken = i.split(',')
    sort.append(broken[1])
    time.append(float(broken[2]))

plt.bar(sort, time)

plt.title("graphEV [beta]")
plt.xlabel('Sorts used')
plt.ylabel('Time in seconds')

plt.show()