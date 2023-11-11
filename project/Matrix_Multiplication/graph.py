# imprting necessory libraries

import subprocess
import re
import matplotlib.pyplot as plot
import time
import matplotlib
matplotlib.use('Agg') # setting backend for matplotlib in non-interactive mode

#function to plot CPU vs SEQ execution times
def plot_CPU_SEQ_times(CPU_Times, SEQ_Times):
    # Plotting the graph
    n = len(CPU_Times)
    x = range(1, n+1)

    #plotting CPU and SEQ times array ion x andd y axes
    plot.plot(x, CPU_Times, label="CPU Times")
    plot.plot(x, SEQ_Times, label="SEQ Times")

    # naming the x axis
    plot.xlabel('Run Number')
    # naming the y axis
    plot.ylabel('execution Time (ms)')
    # giving a title to my graph
    plot.title('CPU vs SEQ Times')
    plot.legend()# show a legend on the plot
    time.sleep(1)
    plot.savefig('CPU_vs_SEQ.png') # saving the plot as a image
    plot.show()

# function to run C code and return the execution time
def Execute_Kernel_Code(c_code_path, mode):
    total_time_taken = None
    # Compile the C code using GCC
    compile_command = "gcc {} -o executable -lOpenCL".format(c_code_path)
    subprocess.run(compile_command, shell=True, check=True) # run the compilation command

    # Passsing command klines arguments to teh executable
    if mode == "cpu":
        run_command = "./executable cpu" # CPU based Kernel Execution
    elif mode == "gpu":
        run_command = "./executable gpu" # GPU based Kernel Execution
    elif mode == "seq":
        run_command = "./executable seq" # Normal Sequential Execution
    
    # Storing the output of the command in result
    result = subprocess.run(run_command, shell=True, stdout=subprocess.PIPE, check=True)

    # Extract the last line of the output
    last_line = result.stdout.decode('utf-8').strip().split('\n')[-1]

    # Use regular expression to extract the value
    match = re.search(r'Total Time Taken: (\d+\.\d+)', last_line)
    if match: #getting the value of total timetaken for execution
        total_time_taken = match.group(1)
        return total_time_taken # returning total Execution time
    return None

if __name__ == "__main__":
    CPU_Times, SEQ_Times = [], []

    print("\n\t\t :::::::::::::::::::: Running OpenCL Kernel Code using CPU :::::::::::::::::::: \n")
    for i in range(10): # runnign the CPU kernel code for 10 times
        print ("Getting CPU Time for Iteration: ", i)
        total_time = Execute_Kernel_Code("Matrix_Mul_CPU.c", mode="cpu") # returning the total execution time
        CPU_Times.append(float(total_time)) # appending result in CPU times array
    print("\n\t\t :::::::::::::::::::  ::::::::::::::::::::::::::::::::::::: :::::::::::::::::::: \n")

    print("\n\t\t :::::::::::::::::::: Running Code using SeQ Matrix Mutli.. :::::::::::::::::::: \n")
    for i in range(10): # running Sequential code for 10 times
        print ("Getting SEQ Time for Iteration: ", i)
        total_time = Execute_Kernel_Code("Matrix_Mul_CPU.c", mode="seq") # returning the total execution time
        SEQ_Times.append(float(total_time)) # appending result in SEQ times array
    print("\n\t\t :::::::::::::::::::  ::::::::::::::::::::::::::::::::::::: :::::::::::::::::::: \n")

    print ("[+] - Calculated CPU Times Array: ", CPU_Times)
    print ("\n[+] - Calculated SEQ Times Array: ", SEQ_Times)
    print ("Plotting Graph for CPU vs SEQ execution times\n")
    plot_CPU_SEQ_times(CPU_Times, SEQ_Times) # plotting the graph for CPU vs SEQ execution times
    print ("[+] - Graph Saved as CPU_vs_SEQ.png\n")
