# CPU_Scheduling_simulator

## Contents
* About Project
* Motivation
* About CPU Scheduler
* Structure
* Results
* Tech/framework used

&nbsp;

## About Project

It is CPU scheuduling simulator created by C language. It siumulates FCFS, NSJF, and RR algorithm.

&nbsp;


## Motivation
School operating system project.

&nbsp;

## About CPU Scheduler
CPU Scheduler is part of operating system that allocates CPU resources to processes. Process or program requres CPU and other resources to run. When process is running, it changes its state like below.

1. New : new process is created
2. Running : instructions in process are executed
3. Waiting : process is waiting for events(ex I/O completion) to happen
4. Ready : process is waiting for CPU to be allocated
5. Terminated : CPU execution is finished.

![](pic_folder/process_state_diagram.JPG)

After creation of processes, they are at ready state before allocation of CPU and after allocation, it changes its state to running. When events such as I/O takes place, it changes its state to waiting. Thus, these processes usually wait at ready queue or waiting queue when they are in ready or running state. **CPU Scheduling** schedules the sequence of these proecesses in single process environment to avoid conflict and this is where **Scheduling algorithm** comes to play to effectively schedule the sequences.


**FCFS**
First Come First Serve (FCFS) is an operating system scheduling algorithm that automatically executes queued requests and processes in order of their arrival.

**Shortest Job First Scheduling**
Shortest job first (SJF) or shortest job next, is a scheduling policy that selects the waiting process with the smallest execution time to execute next.

**Priority Scheduling**
Each process is assigned a priority. Process with highest priority is to be executed first and so on. Processes with same priority are executed on first come first served basis.

**Round Robin Scheduling**
time slices (also known as time quanta) are assigned to each process in equal portions and in circular order, handling all processes without priority (also known as cyclic executive).

&nbsp;
## Structure
Below is a system structure and flow of modules.
![](pic_folder/structure.JPG)

&nbsp;

**FCFS_Schedule(readyQueue, waitingQueue, finishedQueue, process_array[])**
![](pic_folder/fcfs.JPG)

**NSJF_Schedule(readyQueue, waitingQueue, finishedQueue, process_array[], preemptive)**
![](pic_folder/nsjf.JPG)

**pp_Schedule(readyQueue, waitingQueue, finishedQueue, process_array[], preemptive)**
![](pic_folder/pp.JPG)

**RR_Schedule(readyQueue, waitingQueue, finishedQueue, process_array[], TIME_QUANTUM)**
![](pic_folder/rr.JPG)

&nbsp;

## Results
Below is a table comparing average waiting time and average turnaround time of each algorithm.

|Algorithm|Average Waiting time|Average turnaround time|
|---|---|---|
FCFS|76|90
Non-Preemptive SJF|36|49
Preemptive SJF	|53|52
Non-Preemptive Priority|48|61
Preemptive Priority|72|69
Round-Robin|93|81


## Tech/framework used
<b>Built with</b>
- C

&nbsp;
