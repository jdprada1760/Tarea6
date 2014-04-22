import numpy as np
import matplotlib.pyplot as plt
import sys

# Obtiene los datos
datass = np.loadtxt("datass.dat").T
# Organiza los grupos de graficas
plot = []
for i in range(30):
    plot.append([datass[2*i ], datass[2*i + 1]])
plot = np.array(plot)
# Grafica
for i in range(30):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(plot[i][0],plot[i][1])
    ax.set_title( "Grafica con x = " + str(30-i))
    plt.savefig("x=" + str(30-i) + '.pdf',format = 'pdf', transparent=True)
    plt.close()
