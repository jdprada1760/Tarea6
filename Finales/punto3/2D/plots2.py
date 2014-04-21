import numpy as np
import matplotlib.pyplot as plt
import sys

# Contiene todas las graficas convertidas a python structures
totalGraphs = []

# Obtiene cada archivo pasado por parametro
i = 0
j = 0
for i in range(1,len(sys.argv)):
    temp = np.loadtxt(sys.argv[i]).T[1:-1]
    totalGraphs.append(temp)

totalGraphs = np.array(totalGraphs)
# Grafica
i = 0
for i in range(len(totalGraphs)):
    lim = 200
    x_t = totalGraphs[i]
    N_trajectories = len(x_t)
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_xlabel("X_kpc")
    ax.set_ylabel("Y_kpc")
    ax.set_xlim((-lim, lim))
    ax.set_ylim((-lim, lim))		
    k = 0
    n_points = 1000
    xs = x_t[0][2:2+n_points]
    ys = x_t[1][2:2+n_points]
    ax.scatter(xs,ys, c = "b", alpha = 0.7)
    x2s = x_t[0][-n_points:-1]
    y2s = x_t[1][-n_points:-1]
    ax.scatter(x2s,y2s, c = "r", alpha = 0.7)
    x3s = x_t[0][0:2]
    y3s = x_t[1][0:2]
    ax.scatter(x3s,y3s, c = "g", s = 200)
	   
    # Guarda la grafica
    filename = sys.argv[i+1]
    plt.savefig(filename + '.pdf',format = 'pdf', transparent=True)
    plt.close()
