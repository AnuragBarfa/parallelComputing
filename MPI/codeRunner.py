import os


os.system('mpiCC arrayAddition.cpp')
os.system('mpirun -n 4 a.out 100')
stream=os.popen('echo Retured output')
output=stream.read()
print(output)