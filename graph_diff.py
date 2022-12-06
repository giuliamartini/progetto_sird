import pandas as pd
import matplotlib.pyplot as plt
headers = ['Day', 'Susceptible', 'Infected', 'Recovered']
df = pd.read_csv('/home/giulia/progetto_sird-main/graph_diff.csv')
# print(df)
Day = df['Day']
S = df['Susceptible']

I = df['Infected']
R = df['Recovered']


fig, axs = plt.subplots(3, figsize=(10, 10))
axs[0].scatter(Day, I, s=5, c='r', marker='o'),  # infected
axs[0].set_title('Infected')
axs[0].set(xlabel=' ', ylabel='Infected')
axs[0].label_outer()
# susceptibles e recovered
axs[1].plot(Day, S, 'tab:blue', Day, R, 'tab:orange')
axs[1].set_title('Susceptibles (b) and Recovered (o)')
axs[1].set(
    xlabel=' ', ylabel='Susceptibles  and Recovered ')
#axs[0, 1].label_outer()
axs[2].plot(Day, S, 'tab:blue', Day, I, 'tab:red', Day, R, 'tab:orange')
axs[2].set_title('S - I - R')
axs[2].set(xlabel='Day', ylabel='S : blue, I: red, R : orange')
axs[2].label_outer()

plt.show()
