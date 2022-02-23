#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.ticker import FormatStrFormatter
from matplotlib.ticker import MaxNLocator

print ("Génération du graphique\n")

timestamps_datas = pd.read_csv('./datas_timestamps_plot.csv')
timestamps_datas_np = timestamps_datas.to_numpy()

x = timestamps_datas_np[:,1]
y = timestamps_datas_np[:,0]
xlab = 'Temps (ns)'
ylab = 'Thread ID'

figure = None

if(timestamps_datas_np.size/2 > 120):

    figure, axs = plt.subplots(2, 1, figsize=[11, 9])
    axs[0].set_title("Ensemble de l'exécution")
    axs[0].plot(y,x)
    axs[0].yaxis.set_major_locator(MaxNLocator(integer=True))
    axs[1].set_title("Zoom sur 50 valeurs")
    axs[1].plot(y[4:54], x[4:54])
    axs[1].yaxis.set_major_locator(MaxNLocator(integer=True))

    figure.set_figheight(8)
    figure.set_figwidth(13)
    
else:
    plt.title('THREADS CONTEXT SWITCHES', fontsize=15, fontweight='bold')
    plt.plot(y, x)

plt.xlabel(xlab, fontsize=12, fontweight='bold')
plt.ylabel(ylab, fontsize=12, fontweight='bold')



plt.show()
