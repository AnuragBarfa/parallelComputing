import os

files=[['trapezoidalRule.cpp','output2.txt']]

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
	# try:
	# 	os.remove(file[1])
	# except OSError:
	# 	pass
	os.system('g++ -fopenmp '+file[0])
	n=128
	while n<1000000000:
		n=n*2
		os.system('./a.out '+str(n)+' '+str(128))