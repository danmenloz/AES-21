import subprocess
import time
import os
import signal
from gpiozero import CPUTemperature
import matplotlib.pyplot as plt

test = "onDemand" # baseline, onDemand, performance, powersave

# Object to read temperature
cpu = CPUTemperature()

if test=="baseline":
    print('Test baseline')
    subprocess.Popen("sudo cpufreq-set -g ondemand",shell=True)
    # Run monitor script
    p = subprocess.Popen("python3 cpu_monitor_fast.py",shell=True)
    time.sleep(1) # wait to launch monitor
    time.sleep(30)
    # p.terminate()
    os.kill(p.pid, signal.SIGINT) # ctrl+C
    # generate  plot
    subprocess.run("python3 generate_plot.py",shell=True)
elif test == "onDemand":
    print('Test onDemand')
    subprocess.Popen("sudo cpufreq-set -g ondemand",shell=True)
    # Run monitor script
    p = subprocess.Popen("python3 cpu_monitor_fast.py",shell=True)
    time.sleep(2) # wait to launch monitor
    idle_temp = cpu.temperature
    time.sleep(5)
    # Start time stamp
    t_start = time.time()
    # Run C/C++ benchmark
    subprocess.Popen("~/Documents/exploringrpi/chp05/performance/n-body 5000000",shell=True).wait()
    # Compute benchmark runtime
    t_diff = time.time() - t_start
    print('C/C++ runtime: ' + str(t_diff) + ' sec')
    # wait for temperature to fall
    while cpu.temperature > (idle_temp+1):
        time.sleep(0.5)
    # wait another 5 sec
    # time.sleep(5)
    # os.kill(p.pid, signal.SIGINT) # ctrl+C
    # p.terminate()
    # plt.close("all")
    p.kill()
    # generate  plot
    subprocess.run("python3 generate_plot.py",shell=True)
elif test == "performance":
    print('Test performance interval=1000')
    subprocess.Popen("sudo cpufreq-set -g performance",shell=True)
    # Run monitor script
    p = subprocess.Popen("python3 cpu_monitor_fast.py",shell=True)
    time.sleep(1) # wait to launch monitor
    idle_temp = cpu.temperature
    time.sleep(5)
    # Start time stamp
    t_start = time.time()
    # Run C/C++ benchmark
    subprocess.Popen("~/Documents/exploringrpi/chp05/performance/n-body 5000000",shell=True).wait()
    # Compute benchmark runtime
    t_diff = time.time() - t_start
    print('C/C++ runtime: ' + str(t_diff) + ' sec')
    # wait for temperature to fall
    while cpu.temperature > (idle_temp+1):
        time.sleep(0.5)
    p.kill()
    # generate plot
    subprocess.run("python3 generate_plot.py",shell=True)
elif test == "powersave":
    print('Test powersave')
    subprocess.Popen("sudo cpufreq-set -g powersave",shell=True)
    # Run monitor script
    p = subprocess.Popen("python3 cpu_monitor_fast.py",shell=True)
    time.sleep(3) # wait to launch monitor
    idle_temp = cpu.temperature
    time.sleep(5)
    # Start time stamp
    t_start = time.time()
    # Run C/C++ benchmark
    subprocess.Popen("~/Documents/exploringrpi/chp05/performance/n-body 5000000",shell=True).wait()
    # Compute benchmark runtime
    t_diff = time.time() - t_start
    print('C/C++ runtime: ' + str(t_diff) + ' sec')
    # wait for temperature to fall
    while cpu.temperature > (idle_temp+1):
        time.sleep(0.5)
    p.kill()
    # generate plot
    subprocess.run("python3 generate_plot.py",shell=True)
    
    