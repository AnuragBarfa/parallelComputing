#!/usr/bin/env python
import matplotlib.pyplot as plt
import csv

fig, axs = plt.subplots(2, 2,sharex=True, sharey=True,)
fig.suptitle('Plot for different compiler option varying matrix size')

allFiles=['O1.csv','O2.csv','O3.csv','Ofast.csv']
for j in range(0,len(allFiles)):
	n = []
	t0 = []
	t1 = []
	t2 = []
	t3 = []
	data=[]
	with open(allFiles[j],'r') as csvfile:
		cursor = csv.reader(csvfile, delimiter=' ')
		for row in cursor:
			data.append(row)
	i=0
	while i<len(data):
		n.append(int(data[i][2]))
		t0.append(int(data[i+1][4]))
		t1.append(int(data[i+2][4]))
		t2.append(int(data[i+3][4]))
		t3.append(int(data[i+4][4]))
		i=i+5
	axs[j//2,j%2].set_title("-"+allFiles[j].split(".")[0]+" option")	
	axs[j//2,j%2].plot(n,t0)
	axs[j//2,j%2].plot(n,t1)
	axs[j//2,j%2].plot(n,t2)
	axs[j//2,j%2].plot(n,t3)	
# give x and y label to all subplots
for ax in axs.flat:
    ax.set(xlabel='matrix size', ylabel='computation time(ns)')

# reomve all inner x and y label only boundary label remains
for ax in axs.flat:
    ax.label_outer()
plt.show()
