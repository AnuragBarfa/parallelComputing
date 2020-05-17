import os

files=[['blockwiseMultiplication.cpp','block.txt'],['colStripedMultiplication.cpp','col1.txt'],['rowStripedMultiplication.cpp','row1.txt']]

# for file in files:
# 	try:
# 		os.remove(file[1])
# 	except OSError:
# 		pass
# 	os.system('g++ -fopenmp '+file[0])
# 	n=4096
# 	while n<1000000000:
# 		n=n*8
# 		i=1
# 		print(n)
# 		while i<4096:
# 			i=i*2
# 			os.system('./a.out '+str(n)+' '+str(i))

for file in files:
	try:
		os.remove(file[1])
	except OSError:
	 	pass
	os.system('mpiCC '+file[0])
	n=16
	i=0
	while n*i<=1000:
		i=i+1
		os.system('mpirun -n '+str(16)+' ./a.out '+' '+str(n*i))
		# n=n*4
		
