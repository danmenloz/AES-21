from gpiozero import CPUTemperature
from time import sleep, strftime, time
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import matplotlib.animation as animation
import datetime as dt
import re
import subprocess

# Function to log data
def write_log(t_diff, temp, freq, volt):
    with open("cpu_log.csv", "a") as log:
        log.write("{:.4f},{:.2f},{},{}\n".format(t_diff,temp,freq,volt))        

# Object to read temperature
cpu = CPUTemperature()

# Create figure for plotting
fig = plt.figure() #constrained_layout=True
gs = GridSpec(4, 2, figure=fig, hspace=0.5)
ax1 = fig.add_subplot(gs[0, :])
ax2 = fig.add_subplot(gs[1, :])
ax4 = fig.add_subplot(gs[2:, 0:1])
ax3 = fig.add_subplot(gs[2:, 1:2])
axis = (ax1,ax2,ax3,ax4,ax4)

# Emtpy lists
samples = 500
xtime = list(range(0, samples))
ytemp = [0] * samples
yfreq = [0] * samples

# Start time stamp
t_start = time()

ax1.set_ylim([0,100])
ax2.set_ylim([0,1600])
ax3.set_xlim([0,1600])
ax3.set_ylim([0,100])
ax4.set_xlim([0,1600])
ax4.set_ylim([0,3.3])


line1, = ax1.plot(xtime, ytemp, 'tab:red')
line2, = ax2.plot(xtime, yfreq)


fig.suptitle("CPU Monitor")
ax1.set(ylabel='Temperature (°C)')
ax1.set_xticklabels([])
ax2.set(ylabel='Frequency (MHz)', xlabel='Time (sec)')
ax3.set(ylabel='Temperature (°C)', xlabel='Frequency (MHz)')
ax4.set(ylabel='Voltage (V)', xlabel='Frequency (MHz)')

ax1.grid(True)
ax2.grid(True)
ax3.grid(True)
ax4.grid(True)

# This function is called periodically from FuncAnimation
def animate(i, xtime, ytemp, yfreq):

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
    t_diff = time() - t_start
    xtime.append("{:.3f}".format(t_diff))
    ytemp.append(temp_c)
    yfreq.append(freq)

    # Log data
    write_log(t_diff, temp_c, freq, volt)

    ytemp = ytemp[-samples:]
    yfreq = yfreq[-samples:]

    line1.set_ydata(ytemp)
    line2.set_ydata(yfreq)

    ax3.plot(freq, temp_c, color='orange', marker='o')
    ax4.plot(freq, volt, color='green', marker='o')

    return line1,line2,

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xtime, ytemp, yfreq), interval=10, blit=True)
plt.show()
