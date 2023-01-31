# we have a set of data which contains (average waiting time, average turnaround time, average response time) for each data set of processes. We want to plot a bargraph for each of these values.

import matplotlib.pyplot as plt
import numpy as np

# data to plot
n_groups = 3
waiting_time = (551,163,739)
turnaround_time = (755,219,913)
response_time = (323,113,427)

# create plot
fig, ax = plt.subplots()
index = np.arange(n_groups)
bar_width = 0.2
opacity = 0.8

rects1 = plt.bar(index, waiting_time, bar_width,
alpha=opacity,
color='b',
label='Waiting Time')

rects2 = plt.bar(index + bar_width, turnaround_time, bar_width,
alpha=opacity,
color='g',
label='Turnaround Time')

rects3 = plt.bar(index + 2*bar_width, response_time, bar_width,
alpha=opacity,
color='r',
label='Response Time')


plt.xlabel('Data Set')
plt.ylabel('Time')
plt.title('Average Time for each Data Set')
plt.xticks(index + bar_width, ('Data Set 1', 'Data Set 2', 'Data Set 3'))
plt.legend()

plt.tight_layout()
# save as image
plt.savefig('bargraphs.png')
plt.show()