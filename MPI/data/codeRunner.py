import os

files=[['mpiArrayAdditionNaive.cpp','naiveOutput.txt']]

for file in files:
	try:
		os.remove(file[1])
	except OSError:
		pass
	os.system('mpiCC '+file[0])
	i=4
	while i<1024:
		i=i*2
		os.system('mpirun -n 4 a.out '+str(i))