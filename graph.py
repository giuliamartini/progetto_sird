import pandas as pd
import matplotlib.pyplot as plt
headers = ['Day', 'Susceptible', 'Infected', 'Recovered', 'Dead', 'Vaccinated']
df = pd.read_csv('/home/giulia/plague3/Graph.csv')
print(df)
Day = df['Day']
S = df['Susceptible']

I = df['Infected']
R = df['Recovered']
D = df['Dead']
V = df['Vaccinated']




fig, axs = plt.subplots(2, 2)
axs[0, 0].scatter(Day, I, s=5, c='r', marker='o'),  # infected
axs[0, 0].set_title('Infected')
axs[0, 0].set(xlabel='Day', ylabel='Infected')
axs[0, 0].label_outer()
# susceptibles e recovered
axs[0, 1].plot(Day, S, 'tab:blue', Day, R, 'tab:orange')
axs[0, 1].set_title('Susceptibles (b) and Recovered (o)')
axs[0, 1].set(
    xlabel='Day', ylabel='Susceptibles  and Recovered ')
#axs[0, 1].label_outer()
axs[1, 0].plot(Day, V, 'tab:green')  # Vaccinated
axs[1, 0].set_title('Vaccinated')
axs[1, 0].set(xlabel='Day', ylabel='Vaccinated')
axs[1, 0].label_outer()
axs[1, 1].scatter(Day, D, s=5, c='b', marker='o')
axs[1, 1].set_title('Deads')
axs[1, 1].set(xlabel='Day', ylabel='Deads')
axs[1, 1].label_outer()



for ax in fig.get_axes():
    ax.label_outer()
plt.show()
    
