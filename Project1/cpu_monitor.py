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
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

# Start time stamp
t_start = time()

# This function is called periodically from FuncAnimation
def animate(i, xs, ys):

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
    xs.append("{:.3f}".format(t_diff))
    ys.append(temp_c)
    # ys.append(freq)

    # Log data
    write_log(t_stamp, t_diff, temp_c, freq, volt)

    # Limit x and y lists to 20 items
    xs = xs[-20:]
    ys = ys[-20:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys)

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('Temperature over Time')
    plt.ylabel('Temperature (deg C)')


# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=100)
plt.show()
