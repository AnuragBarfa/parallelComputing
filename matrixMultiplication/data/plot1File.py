#!/usr/bin/env python
import matplotlib.pyplot as plt
import csv

n = []
t=[]
t0 = []
t1 = []
t2 = []
t3 = []
data=[]
with open('default.csv','r') as csvfile:
	cursor = csv.reader(csvfile, delimiter=' ')
	for row in cursor:
		data.append(row)
i=0
while i<len(data):
	n.append(int(data[i][2]))
	t.append(4.216*(int(data[i][2])**3))
	t0.append(int(data[i+1][4]))
	t1.append(int(data[i+2][4]))
	t2.append(int(data[i+3][4]))
	t3.append(int(data[i+4][4]))
	i=i+5

plt.figure()
plt.xlabel('Matrix size (n)')
plt.ylabel('Computation time(ns)')
plt.title('default compiler options')
plt.plot(n,t,label='theoritical')
plt.plot(n,t0,label='naive')
plt.plot(n,t1,label='optimization1')
plt.plot(n,t2,label='optimization2')
plt.plot(n,t3,label='optimization3')
plt.legend(loc='upper left')
plt.show()