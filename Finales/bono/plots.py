import numpy as np;
import matplotlib.pyplot as plt
import sys

# Obtiene los datos
data = []
data.append(np.loadtxt("colapso1.data").T)

data.append(np.loadtxt("colapso2.data").T)

data.append(np.loadtxt("colapso3.data").T)

data.append(np.loadtxt("colapso4.data").T)

data.append(np.loadtxt("colapso5.data").T)

energy = np.loadtxt("energy.data");

data = np.array(data)

# Grafica

lim = 100;
for i in range(len(data)):
    temp = data[i]
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_xlabel("X_pc")
    ax.set_ylabel("Y_pc")
    ax.set_xlim((-lim, lim))
    ax.set_ylim((-lim, lim))
    ax.scatter(temp[0],temp[1], s = 5, alpha = 0.5, c = 'b')
    
    # Guarda la grafica
    filename = "colapso_Gravitacional" + str(i+1)
    plt.savefig(filename + '.pdf',format = 'pdf', transparent=True)
    plt.close()

# Grafica las diferencias de energia

energy = abs((energy[0:-2] - energy[1:-1])/energy[0:-2])
#energy = energy[np.where( energy )]
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_ylabel("dE/E")
ax.plot(np.arange(0,len(energy)),energy)
plt.savefig("Energy_evol" + '.pdf',format = 'pdf', transparent=True)
plt.close()
