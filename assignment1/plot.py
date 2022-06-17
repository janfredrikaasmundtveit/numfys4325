import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import scipy.interpolate
from matplotlib import cm
import matplotlib.tri as mtri
import pandas as pd
from scipy.interpolate import griddata
from mpl_toolkits.mplot3d import Axes3D

data = []
x = []
y = []
z = []
with open('l2.dat') as fobj:
    for line in fobj:
        row = line.split()
        data.append(row[:-1])
        x.append(row[1])
        y.append(row[2])
        z.append(row[5])
xf = []
yf = []
zf = []
datf=[]
with open('fractal.dat') as fract:
	for line in fract:
		row = line.split()
		datf.append(row[:-1])
		xf.append(row[0])
		yf.append(row[1])
		zf.append(row[2])

for i in range (len(x)):     
	x[i]=float(x[i])
	y[i]=float(y[i])
	z[i]=float(z[i])
for i in range (len(xf)):   	
	xf[i]=float(xf[i])
	yf[i]=float(yf[i])
	zf[i]=float(zf[i])


fig = plt.figure()
ax = fig.add_subplot(1,1,1, projection='3d')
ax.plot_trisurf(x, y, z,  cmap='gray', edgecolor='none', alpha=0.5)
ax.plot(xf, yf)

plt.savefig('plot.png')