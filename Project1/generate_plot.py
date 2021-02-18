import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec

# Read CSV file
column_names = ["time", "temp", "freq", "volt"]
df = pd.read_csv("cpu_log.csv", names=column_names)
time = df.time.tolist()
temp = df.temp.tolist()
freq = df.freq.tolist()
volt = df.volt.tolist()

# Create figure for plotting
fig = plt.figure()
gs = GridSpec(4, 2, figure=fig, hspace=0.5)
ax1 = fig.add_subplot(gs[0, :])
ax2 = fig.add_subplot(gs[1, :])
ax4 = fig.add_subplot(gs[2:, 0:1])
ax3 = fig.add_subplot(gs[2:, 1:2])
axis = (ax1,ax2,ax3,ax4,ax4)

# Limit axes
# ax1.set_ylim([0,100])
# ax2.set_ylim([0,1600])
# ax3.set_xlim([0,1600])
# ax3.set_ylim([0,100])
# ax4.set_xlim([0,1600])
# ax4.set_ylim([0,3.3])

# Format plot
fig.suptitle("CPU Monitor")
ax1.set(ylabel='Temperature (°C)')
ax1.set_xticklabels([])
ax2.set(ylabel='Frequency (MHz)', xlabel='Time (sec)')
ax3.set(ylabel='Temperature (°C)', xlabel='Frequency (MHz)')
ax4.set(ylabel='Voltage (V)', xlabel='Frequency (MHz)')

# Grid on
ax1.grid(True)
ax2.grid(True)
ax3.grid(True)
ax4.grid(True)

# Plot data
ax1.plot(time, temp, 'tab:red')
ax2.plot(time, freq)
ax3.scatter(freq, temp, c='orange')
ax4.scatter(freq, volt, c='green')
plt.show()