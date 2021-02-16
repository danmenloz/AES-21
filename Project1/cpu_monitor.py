from gpiozero import CPUTemperature
from time import sleep, strftime, time
import matplotlib.pyplot as plt
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
# fig = plt.figure()
# ax = fig.add_subplot(1, 1, 1)

fig, axs = plt.subplots(2, 2)
# axs[0, 0].hist(data[0])
# axs[1, 0].scatter(data[0], data[1])
# axs[0, 1].plot(data[0], data[1])
# axs[1, 1].hist2d(data[0], data[1])

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
    freq = int(freq.group(1))

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

    # Limit x and y lists to 20 items
    # xs = xs[-20:]
    # ys = ys[-20:]
    
    # Draw x and y lists
    axs[0,0].clear()
    axs[0,1].clear()
    axs[1,0].clear()
    axs[1,1].clear()
    axs[0,0].plot(xtime, ytemp)
    axs[0,1].plot(xtime, yfreq)
    axs[1,0].scatter(ytemp, yfreq)
    axs[1,1].scatter(yvolt, yfreq)

    # Format plot
    axs[0, 0].set_title('CPU Temperature')
    axs[0, 1].set_title('CPU Frequency')
    axs[1, 0].set_title('Temp vs Freq')
    axs[1, 1].set_title('Voltage vs Freq')
    # plt.xticks(rotation=45, ha='right')
    # plt.subplots_adjust(bottom=0.30)
    # plt.title('Temperature over Time')
    # plt.ylabel('Temperature (deg C)')


# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xtime, ytemp, yfreq, yvolt), interval=500)
plt.show()
