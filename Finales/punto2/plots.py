import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import sys

# Contiene todas las graficas convertidas a python structures
totalData = []

# Obtiene cada archivo pasado por parametro
i = 0
j = 0
data = np.array( np.loadtxt(sys.argv[1]) )
data = data.T
data1 = data[[1,2,3]]
data2 = data[[4,5,6]]
data3 = data[[7,8,9]]
i = 0
n_dim = 3
n_masses = 3
print len(data)
x_t = np.array([data1.T,data2.T,data3.T])

# Grafica
i = 0
for j in range(5):
    lim = 150
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlabel("X_AU")
    ax.set_ylabel("Y_AU")
    ax.set_zlabel("Z_AU")
    ax.set_xlim((-lim, lim))
    ax.set_ylim((-lim, lim))
    k = 0
    n = int(j*(len(x_t[0])-2)/4) +1
    x1 = x_t[0][:n].T
    xs1 = x_t[0][n]
    ax.plot(x1[0],x1[1],x1[2])
    x2 = x_t[1][:n].T
    xs2 = x_t[1][n]
    ax.plot(x2[0],x2[1],x2[2])
    x3 = x_t[2][:n].T
    xs3 = x_t[2][n]
    ax.plot(x3[0],x3[1],x3[2])
    ax.scatter(xs1[0],xs1[1],xs1[2], c = "b")
    ax.scatter(xs2[0],xs2[1],xs2[2], c = "g")
    ax.scatter(xs3[0],xs3[1],xs3[2], c = "r")
    

    # Guarda la grafica
    filename = "evolucion3D" + "___" + str(j+1)
    plt.savefig(filename + '.pdf',format = 'pdf', transparent=True)
    plt.close()


energy = data[-1]
fig = plt.figure()
ax = fig.add_subplot(111)
plt.plot(np.arange(len(energy)),energy, label = "Evolucion de la energia")
plt.savefig("energy_evolution" + '.pdf',format = 'pdf', transparent=True)
