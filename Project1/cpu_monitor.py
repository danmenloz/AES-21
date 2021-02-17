from gpiozero import CPUTemperature
from time import sleep, strftime, time
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import matplotlib.animation as animation
import datetime as dt
import re
import subprocess

# Function to log data
def write_log(t_stamp, t_diff, temp, freq, volt):
    with open("cpu_log.csv", "a") as log:
        log.write("{},{:.4f},{:.2f},{},{}\n".format(t_stamp,t_diff,temp,freq,volt))        

# Object to read temperature
cpu = CPUTemperature()

# Create figure for plotting
# fig, axs = plt.subplots(2, 2)
fig = plt.figure() #constrained_layout=True
gs = GridSpec(4, 2, figure=fig, hspace=0.5)
ax1 = fig.add_subplot(gs[0, :])
ax2 = fig.add_subplot(gs[1, :])
ax4 = fig.add_subplot(gs[2:, 0:1])
ax3 = fig.add_subplot(gs[2:, 1:2])
axis = (ax1,ax2,ax3,ax4,ax4)


# Emtpy lists
xtime = []
ytemp = []
yfreq = []
yvolt = []

# Start time stamp
t_start = time()

# This function is called periodically from FuncAnimation
def animate(i, xtime, ytemp, yfreq, yvolt):

    # Read temperature (Celsius)
    temp_c = cpu.temperature

    # Subprocess call
    output = subprocess.check_output("vcgencmd measure_clock arm; vcgencmd measure_volts core", shell=True)

    # Read CPU frequency
    freq = re.search("48\)=(.+)\n", output.decode('utf-8'))
    freq = int(freq.group(1))/1000000.0

    # Read CPU voltage
    volt = re.search("=(.+)V\n", output.decode('utf-8'))
    volt = float(volt.group(1))
    
    # Add x and y to lists
    t_stamp = dt.datetime.now().strftime('%H:%M:%S.%f')
    t_diff = time() - t_start
    xtime.append("{:.3f}".format(t_diff))
    ytemp.append(temp_c)
    yfreq.append(freq)
    yvolt.append(volt)

    # Log data
    write_log(t_stamp, t_diff, temp_c, freq, volt)
    
    # Draw x and y lists
    for ax in axis:
        ax.clear() 
    ax1.plot(xtime, ytemp, 'tab:red')
    ax2.plot(xtime, yfreq)
    ax3.scatter(yfreq, ytemp, c="orange")
    ax4.scatter(yfreq, yvolt, c="green")

    # Format plot
    fig.suptitle("CPU Monitor")
    ax1.set(ylabel='Temperature (°C)')
    ax1.set_xticklabels([])
    ax2.set(ylabel='Frequency (MHz)', xlabel='Time (sec)')
    n = len(xtime)
    m = 5 # number of tick labels to leave
    if n>m:
        new_labels = ['']*n # emtpy list
        new_labels[-1] = str(xtime[-1]) # last tick label
        for i in range(m):
            idx = int((n-1)/m*i)
            new_labels[idx] = str(xtime[idx])
        ax2.set_xticklabels(new_labels)
    ax3.set(ylabel='Temperature (°C)', xlabel='Frequency (MHz)')
    ax4.set(ylabel='Voltage (V)', xlabel='Frequency (MHz)')

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xtime, ytemp, yfreq, yvolt), interval=50)
plt.show()
