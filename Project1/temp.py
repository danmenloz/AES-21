# Starter code based on https://projects.raspberrypi.org/en/projects/temperature-log 
# and https://github.com/raspberrypilearning/temperature-log
from gpiozero import CPUTemperature
from time import sleep, strftime, time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import datetime as dt

# Function to write temperature log
def write_temp(t_stamp, t_diff, temp):
    with open("cpu_temp.csv", "a") as log:
        log.write("{},{:.4f},{:.2f}\n".format(t_stamp,t_diff,temp))        

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

    # Add x and y to lists
    t_stamp = dt.datetime.now().strftime('%H:%M:%S.%f')
    t_diff = time() - t_start
    xs.append("{:.3f}".format(t_diff))
    ys.append(temp_c)

    # Log temperature
    write_temp(t_stamp, t_diff, temp_c)

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
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=500)
plt.show()
