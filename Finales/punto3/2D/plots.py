import numpy as np
import matplotlib.pyplot as plt
import sys

# Contiene todas las graficas convertidas a python structures
totalData = []

# Obtiene cada archivo pasado por parametro
i = 0
j = 0
for i in range(1,len(sys.argv)):
    temp = np.array( np.loadtxt(sys.argv[i]) )
    temp = temp.T
    # Prepara los datos en un mejor formato para graficarlos
    dataTemp = []
    for j in range(0,(len(temp)-1)/2):
        dataTemp.append(np.array(temp[[2*j,2*j+1]]).T)
       
    dataTemp = np.array(dataTemp)
    totalData.append(dataTemp)

totalData = np.array(totalData)
# Grafica
i = 0
for i in range(len(totalData)):
    for j in range(3):
	lim = 21
        x_t = totalData[i]
        N_trajectories = len(x_t)
        fig = plt.figure()
        ax = fig.add_subplot(111)
        ax.set_xlabel("X_kpc")
        ax.set_ylabel("Y_kpc")
	ax.set_xlim((-lim, lim))
	ax.set_ylim((-lim, lim))		
        k = 0
        n = int(j*(len(x_t[0])-2)/4) +1
        for k in range(100):
	    mid = 2*(len(x_t)/4)
            x = x_t[mid+k][:n].T
            xs = x_t[mid+k][n]
            plt.plot(x[0],x[1])
            plt.scatter(xs[0],xs[1], c = xs[0])

	for k in range(100):
	    x = x_t[k][:n].T
            xs = x_t[k][n]
            plt.plot(x[0],x[1])
            plt.scatter(xs[0],xs[1], c = xs[0])
   
        # Guarda la grafica
        filename = sys.argv[i+1] + "___" + str(j+1)
        plt.savefig(filename + '.pdf',format = 'pdf', transparent=True)
        plt.close()
