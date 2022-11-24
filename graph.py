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


#plt.figure(figsize=(9, 3))

# plt.subplot(131)
#plt.scatter( S)
# plt.subplot(132)
#plt.scatter( I)
# plt.subplot(133)
#plt.plot( R)
#plt.plot( D)
# plt.suptitle('Sird')
# plt.show()

# plot
#plt.plot(S, c='g')
plt.scatter(Day, I, s=5, c='b', marker='o')

#plt.plot(R, c='y')
# plt.plot(D)
#plt.plot(V, c='r')
plt.xlabel('Days')
plt.ylabel('Infected')
plt.show()
