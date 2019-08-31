#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PROCESS_NUM 10
#define FALSE 0
#define TRUE 1

struct Process{
	int pid;
	int cpu_burst_time;
	int ori_cpu_burst_time;
	int io_burst_time;
	int arrival_time;
	int priority;
	int io_operation;
	int waiting_time;
	int turnaround_time;
	int io_operation_count;
	int io_operation_finish_time;
	int time_quantum;
};

typedef struct Process process;

struct node
{
    process *process;
    struct node *next;
};
typedef struct node node;

struct queue
{
    int count;
    node *front;
    node *rear;
};
typedef struct queue queue;

void initialize(queue *q)
{
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}

int isempty(queue *q)
{
    return (q->rear == NULL);
}

void enqueue(queue *q, process *process)
{
    node *tmp;
    tmp = (node*)malloc(sizeof(node));
    tmp->process = process;
    
	tmp->next = NULL;
	
    if(!isempty(q))
    {
    	q->rear->next = tmp;
        q->rear = tmp;
    }
    else
    {	
        q->front = q->rear = tmp;
    }
    q->count++;
}

process* dequeue(queue *q)
{
    node *tmp;
    process *process = q->front->process;
    tmp = q->front;
    q->front = q->front->next;
    q->count--;
    if(q->count == 0){
    	q->rear = NULL;
	}
    free(tmp);
    return(process);
}

process* dequeue_process(queue *q, process* dequeue_process){
	
	process *front_process;
	process *rear_process;
	process *dump_process;
	
	front_process = q->front->process;
	rear_process = q->rear->process;
	
	if(front_process->pid == dequeue_process->pid){
		return dequeue(q);
	}else if(rear_process->pid == dequeue_process->pid){
	
		node *back = q->front;
	    
		while(back->next->process->pid != dequeue_process->pid){
	    	back = back->next;
		}
		
		dump_process = back->next->process;
		
		node* dump_node = back->next;
		q->rear = back;
		
		q->count--;
	    if(q->count == 0){
	    	q->rear = NULL;
		}
	    free(dump_node);
	    
	    return(dump_process);
		
	}else{
		node *back = q->front;
	    
		while(back->next->process->pid != dequeue_process->pid){
	    	back = back->next;
		}
		
		
		dump_process = back->next->process;
		
		node* dump_node = back->next;
		
		back->next = back->next->next;	
		
		q->count--;
	    if(q->count == 0){
	    	q->rear = NULL;
		}
	    free(dump_node);
	    return(dump_process);
	}
}

process createProcess(int pid, int priority, int arrival_time, int cpu_burst_time, int io_burst_time, int io_operation){
	
	process process;
	process.pid= pid;
	process.priority = priority;
	process.arrival_time = arrival_time;
	process.cpu_burst_time = cpu_burst_time;
	process.ori_cpu_burst_time = cpu_burst_time;
	process.io_burst_time = io_burst_time;
	process.io_operation = io_operation;
	process.waiting_time = 0;
	process.turnaround_time = 0;
	process.io_operation_count = 0;
	process.io_operation_finish_time = 0;
	process.time_quantum = 10;
	
	
	return process;
}

void get_arrival_time(int *arrival_time_array){
	
	srand(time(NULL));
	
	int i;
	int j;
	for(i = 0; i < PROCESS_NUM; i++){
		arrival_time_array[i] = rand() % 15 + 1;
	}
	
	for (i = 0; i < PROCESS_NUM-1; i++)
    {
        for (j = 0; j < PROCESS_NUM-1 - i; j++)
        {
            if (arrival_time_array[j] > arrival_time_array[j + 1])
            {
                int temp = arrival_time_array[j];
                arrival_time_array[j] = arrival_time_array[j + 1];
                arrival_time_array[j + 1] = temp;
            }
        }
    }
}
void swap (int *a, int *b) 
{ 
    int temp = *a; 
    *a = *b; 
    *b = temp; 
} 

// A function to generate a random permutation of arr[] 
void randomize ( int arr[], int n ) 
{ 
	int i;
    // Use a different seed value so that we don't get same 
    // result each time we run this program 
    srand ( time(NULL) ); 
  
    // Start from the last element and swap one by one. We don't 
    // need to run for the first element that's why i > 0 
    for (i = n-1; i > 0; i--) 
    { 
        // Pick a random index from 0 to i 
        int j = rand() % (i+1); 
  
        // Swap arr[i] with the element at random index 
        swap(&arr[i], &arr[j]); 
    } 
} 


void printQueue(queue *queue){
	
	printf("pid    priority    arrival_time    cpu_burst_time    io_burst_time    io_operation\n");
	printf("=====================================================================================\n");
	
	node *tempNode;
	int i;
	for(i = 0; i < PROCESS_NUM; i++){
		if(i == 0){
			tempNode = queue->front;
			printf("%2d      %4d     %8d       %10d       %9d      %13d\n", tempNode->process->pid,tempNode->process->priority, tempNode->process->arrival_time, tempNode->process->cpu_burst_time, tempNode->process->io_burst_time, tempNode->process->io_operation);		
		}else{
			tempNode = tempNode->next;
			printf("%2d      %4d     %8d       %10d       %9d      %13d\n", tempNode->process->pid,tempNode->process->priority, tempNode->process->arrival_time, tempNode->process->cpu_burst_time, tempNode->process->io_burst_time, tempNode->process->io_operation);		
		}
	}

	printf("=====================================================================================\n");
	}
	
void printResult(queue *finishedQueue){
	
	node *currentFinishedNode = finishedQueue->front;
	process *currentFinishedProcess = currentFinishedNode->process;
	int sum_waitingtime = 0;
	int sum_turnaroundtime = 0;
	
	while(!isempty(finishedQueue)){
		
		printf("pid : %2d\n", currentFinishedProcess->pid);
		printf("waiting time = %2d \t turnaround time = %2d\n", currentFinishedProcess->waiting_time, currentFinishedProcess->turnaround_time);
		printf("==================================================================\n");
		sum_waitingtime += currentFinishedProcess->waiting_time;
		sum_turnaroundtime += currentFinishedProcess->turnaround_time;
		currentFinishedNode = currentFinishedNode->next;
		if(currentFinishedNode != NULL){
			currentFinishedProcess = currentFinishedNode->process;
		}
		dequeue(finishedQueue);
	}
	
	float average_waitingtime = sum_waitingtime/PROCESS_NUM;
	float average_turnaroundtime = sum_turnaroundtime/PROCESS_NUM;
	printf("Average waiting time = %2f\n", average_waitingtime);
	printf("Average turnaround time = %2f\n", average_turnaroundtime);
}

void process_creation(queue *queue, process process_array[PROCESS_NUM]){
	
	int arrival_time_array[PROCESS_NUM]; //array for storing arrival time
    int pid_array[PROCESS_NUM];
    int i;
    for(i = 0; i < PROCESS_NUM; i++){
    	pid_array[i] = i+1;
	}
    
    randomize(pid_array,PROCESS_NUM);
    
    get_arrival_time(arrival_time_array); //get random arrival time and store them in descending order
    
    srand(time(NULL));
    
    for(i = 0; i < PROCESS_NUM; i++){

	int priority = rand() % PROCESS_NUM;
	int arrival_time = *(arrival_time_array + i);
	int cpu_burst_time = rand() % 20 + 1;
	int io_burst_time = rand() % 5;
	int io_operation = rand() % 10;
	while(io_operation > cpu_burst_time){
		io_operation = rand() % 10;
	}
	
    process_array[i] = createProcess(pid_array[i],priority,arrival_time,cpu_burst_time, io_burst_time, io_operation);
	enqueue(queue, &process_array[i]);
	}
}

void fcfs_Schedule(queue *FCFSreadyQueue,queue *FCFSwaitingQueue,queue *FCFSfinishedQueue, process FCFS_process_array[PROCESS_NUM]){
	
	printf("\n\n======================================FCFS START=====================================\n\n");
	
	process_creation(FCFSreadyQueue, FCFS_process_array); //create process and store them in FCFSreadyQueue
	
	printQueue(FCFSreadyQueue); //print processes in FCFSreadyQueue
	
	int time = 0;
	
	process *executingProcess = NULL; //process that is executed by cpu
	node *currentNode = FCFSreadyQueue->front; //node that is pointing the foremost node of readyqueue
	process *currentProcess = NULL; //process of the currentnode
	node *currentIONode = NULL; //node that is pointing the foremost node of waitingqueue
	process *currentIOProcess = NULL; //process of the currentIONode
	
	while(!isempty(FCFSreadyQueue) || !isempty(FCFSwaitingQueue)){ //till there is process in the readyqueue 
		
		currentProcess = currentNode->process;
		
		if(!isempty(FCFSwaitingQueue)){//if there is process waiting for i/o in waiting queue
			
			if(currentIONode== NULL){//if currentIONode is NULL, make sure currentIONode  points to the front of waitingQueue.
				currentIONode = FCFSwaitingQueue->front;
				currentIOProcess = currentIONode->process;
			}
			
			if(currentIOProcess->io_burst_time > 0){ //if process is in the middle of i/o operation
				printf("%3d : (pid : %2d) I/O running\t", time, currentIOProcess->pid);
				currentIOProcess->io_burst_time--; //i/o burst time decreases by one
			}else{ //if process finishes its i/o operation
				printf("%3d : (pid : %2d) I/O finished\t", time, currentIOProcess->pid);	
				currentIOProcess->io_operation_finish_time = time;
				currentIONode = NULL;
				process *readyQueue_back = dequeue(FCFSwaitingQueue); //dequeue process that finished its i/o opeation from waitingQueue
				enqueue(FCFSreadyQueue, readyQueue_back); //enqueue the process that finished its i/o operation back to readyQueue
				}
		}
		
		
		if(executingProcess == NULL){
			if(currentProcess->arrival_time <= time){
				executingProcess = currentProcess;
				printf("%3d : (pid : %2d) execution running start\n", time, executingProcess->pid);
				executingProcess->cpu_burst_time--;
				executingProcess->io_operation = time + executingProcess->io_operation;
				if(executingProcess->io_operation_count == 0){//add the waiting_time of process that is executed for the first time.
					executingProcess->waiting_time += time - executingProcess->arrival_time; 	
				}else{//add the waiting_time of process that was waiting in the ready after io operation has finished.
					executingProcess->waiting_time += time - executingProcess->io_operation_finish_time; 		
				}
				
				if(time >= executingProcess->io_operation){
					printf("%3d : (pid : %2d) -> I/O Operation\n",time ,executingProcess->pid);
					currentNode = currentNode->next;
					process* io_process = dequeue(FCFSreadyQueue);
					io_process->io_operation_count++;
					enqueue(FCFSwaitingQueue, io_process);
					executingProcess = NULL;
				}
						
			}else{
				printf("%3d : idle\n", time);	
			}
		}else{	
			if(executingProcess->cpu_burst_time > 0){
				printf("%3d : (pid : %2d) execution running\n", time, executingProcess->pid);
				executingProcess->cpu_burst_time--;
				
				if(time >= currentProcess->io_operation){
					printf("%3d : (pid : %2d) -> I/O Operation\n",time, executingProcess->pid, time, executingProcess->pid);
					currentNode = currentNode->next;
					process* io_process = dequeue(FCFSreadyQueue);
					io_process->io_operation_count++;
					enqueue(FCFSwaitingQueue, io_process);
					executingProcess = NULL;
				}	
			}else if (executingProcess->cpu_burst_time <= 0){ //when execution of ongoing process in readyQueue is finished
				printf("%3d : (pid : %2d) execution finished\n", time, executingProcess->pid);
				currentNode = currentNode->next;
				process* finished_process = dequeue(FCFSreadyQueue);
				finished_process->turnaround_time = time - finished_process->arrival_time;
				executingProcess = NULL;
				enqueue(FCFSfinishedQueue, finished_process);
			}
		}
		
		time++;
	}
	
	printf("\n==========================FCFS Finished==========================\n");
	printResult(FCFSfinishedQueue);	

}

int get_smallest_cpu_burst_pid(queue *NSJFreadyQueue, int preemptive, int time){ //find the shortest cpu_burst_time
		if(!isempty(NSJFreadyQueue)){
	
			int shortest_cpu_burst_time	= 100;
			int shortest_cpu_pid = -1;
			int i;
			
			node *currentNode = NSJFreadyQueue->front; //node that is pointing the foremost node of readyqueue
			process *currentProcess = currentNode->process; //process of the currentnode
			
			for(i = 0; i < NSJFreadyQueue->count; i++){ //go over each node in readyQueue
				
				if(currentProcess->ori_cpu_burst_time <= shortest_cpu_burst_time && time > currentProcess->arrival_time){ //if current process's cpu_burst_time is smaller
					shortest_cpu_burst_time = currentProcess->ori_cpu_burst_time; //change it with the smallest_cpu_burst
					shortest_cpu_pid = currentProcess->pid;
				}
				
				currentNode = currentNode->next; 
				
				if(currentNode != NULL){
					currentProcess = currentNode->process;	
				}
			}
			
			return shortest_cpu_pid;	
		}else{
			return -1;
		}
}

void nsjf_Schedule(queue *readyQueue,queue *waitingQueue,queue *finishedQueue, process process_array[PROCESS_NUM], int preemptive){
	
	if(preemptive == TRUE){
		printf("\n\n======================================SJF START=====================================\n\n");		
	}else{
		printf("\n\n======================================NSJF START=====================================\n\n");
	}
		
	process_creation(readyQueue, process_array); //create process and store them in NSJFreadyQueue
	
	printQueue(readyQueue); //print processes in NSJFreadyQueue
	
	int time = 0;
	
	process *executingProcess = NULL; //process that is executed by cpu
	node *currentNode = readyQueue->front; //node that is pointing the foremost node of readyqueue
	process *currentProcess = currentNode->process; //process of the currentnode
	node *currentIONode = NULL; //node that is pointing the foremost node of waitingqueue
	process *currentIOProcess = NULL; //process of the currentIONode
	int shortest_cpu_pid = 0;
	int preemptived = FALSE;
	
	while(!isempty(readyQueue) || !isempty(waitingQueue)){ //till there is process in the readyqueue 
		
		if(!isempty(waitingQueue)){ //if there is process waiting for i/o in waiting queue
			
			if(currentIONode== NULL){ //if currentIONode is NULL, make sure currentIONode  points to the front of waitingQueue.
				currentIONode = waitingQueue->front;
				currentIOProcess = currentIONode->process;
			}
			
			if(currentIOProcess->io_burst_time > 0){ //if process is in the middle of i/o operation
				printf("%3d : (pid : %2d) I/O running\t", time, currentIOProcess->pid);
				currentIOProcess->io_burst_time--; //i/o burst time decreases by one
			}else{ //if process finishes its i/o operation
				printf("%3d : (pid : %2d) I/O finished\t", time, currentIOProcess->pid);	
				currentIOProcess->io_operation_finish_time = time;
				currentIONode = NULL;
				process *readyQueue_back = dequeue(waitingQueue); //dequeue process that finished its i/o opeation from waitingQueue
				enqueue(readyQueue, readyQueue_back); //enqueue the process that finished its i/o operation back to readyQueue
				
				if(currentNode == NULL){
					currentNode = readyQueue->front;
					currentProcess = currentNode->process;
				}
			}	
		}
		
		START:
		if(executingProcess == NULL){
			
			if(time < currentProcess->arrival_time || currentNode == NULL){
				printf("%3d : idle\n", time);	
			}else{
				executingProcess = currentProcess;
				
				if(preemptived == TRUE){
					printf("%3d : (pid : %2d) execution running start (preemptived)\n", time, executingProcess->pid);	
				}else{
					printf("%3d : (pid : %2d) execution running start\n", time, executingProcess->pid);
				}	
				
				executingProcess->cpu_burst_time--; //executing process's cpu_brust_time decreases
				
				if(executingProcess->io_operation_count == 0){//add the waiting_time of process that is executed for the first time.
					executingProcess->io_operation = time + executingProcess->io_operation; //executing process's io_operation time is decided by adding the time it started to its originial oo_operation time
					executingProcess->waiting_time += time - executingProcess->arrival_time; 	
				}else{//add the waiting_time of process that was waiting in the ready after io operation has finished.
					executingProcess->waiting_time += time - executingProcess->io_operation_finish_time; 		
				}
				
				if(time >= executingProcess->io_operation && executingProcess->io_burst_time > 0){ //if there is i/o operation occurs		
					printf("%3d : (pid : %2d) -> I/O Operation\n",time ,executingProcess->pid);
					process* io_process = dequeue_process(readyQueue, executingProcess); //dequeue the current process
					io_process->io_operation_count++; //its operation_cout increases
					enqueue(waitingQueue, io_process); //enqueue the current process into waitingqueue
					executingProcess = NULL; //no process is executing	
					
					shortest_cpu_pid = get_smallest_cpu_burst_pid(readyQueue, preemptive, time); //get the shortest_cpu_burst_time
					printf("shortest : %d\n", shortest_cpu_pid);
					if(shortest_cpu_pid == -1){
						executingProcess = NULL;
						currentNode = NULL;
						//currentProcess = currentNode->process;
					}else{
						node *frontNode = readyQueue->front;
						
						while(frontNode->process->pid != shortest_cpu_pid){ //find the node with a process that has shortest_cpu_burst_time
							frontNode = frontNode->next;
						}
						
						currentNode = frontNode;
						currentProcess = currentNode->process;
					}
				}
			}
		}else{
			if(preemptive == TRUE){
				shortest_cpu_pid = get_smallest_cpu_burst_pid(readyQueue, preemptive, time); //get the shortest_cpu_burst_time		
				
				if(executingProcess->pid != shortest_cpu_pid){
					node *frontNode = readyQueue->front;
						
					while(frontNode->process->pid != shortest_cpu_pid){ //find the node with a process that has shortest_cpu_burst_time
						frontNode = frontNode->next;
					}
					
					executingProcess = NULL;
					currentNode = frontNode;
					currentProcess = currentNode->process;
					preemptived = TRUE;
					goto START;
					
				}
			}
			
			if(executingProcess->cpu_burst_time > 0){ //if process is still executing
				
				printf("%3d : (pid : %2d) execution running\n", time, executingProcess->pid);
				executingProcess->cpu_burst_time--;
				
				if(time >= executingProcess->io_operation && executingProcess->io_burst_time > 0){ //if there is i/o operation occurs		
					printf("%3d : (pid : %2d) -> I/O Operation\n",time ,executingProcess->pid);
					process* io_process = dequeue_process(readyQueue, executingProcess); //dequeue the current process
					io_process->io_operation_count++; //its operation_cout increases
					enqueue(waitingQueue, io_process); //enqueue the current process into waitingqueue
					executingProcess = NULL; //no process is executing	
					
					shortest_cpu_pid = get_smallest_cpu_burst_pid(readyQueue, preemptive, time); //get the shortest_cpu_burst_time
					if(shortest_cpu_pid == -1){
						executingProcess = NULL;
						currentNode = NULL;
						//currentProcess = currentNode->process;
					}else{
						node *frontNode = readyQueue->front;
						
						while(frontNode->process->pid != shortest_cpu_pid){ //find the node with a process that has shortest_cpu_burst_time
							frontNode = frontNode->next;
						}
						
						currentNode = frontNode;
						currentProcess = currentNode->process;
					}								
				}
			}else{
				printf("%3d : (pid : %2d) execution finished\n", time, executingProcess->pid);
				
				process* finished_process = dequeue_process(readyQueue, executingProcess);
				finished_process->turnaround_time = time - finished_process->arrival_time;
				executingProcess = NULL;
				enqueue(finishedQueue, finished_process);
				shortest_cpu_pid = get_smallest_cpu_burst_pid(readyQueue, preemptive, time); //get the shortest_cpu_burst_time
				
				if(shortest_cpu_pid == -1){
					executingProcess = NULL;
					currentNode = NULL;
					//currentProcess = currentNode->process;
				}else{
					node *frontNode = readyQueue->front;
					
					while(frontNode->process->pid != shortest_cpu_pid){ //find the node with a process that has shortest_cpu_burst_time
						frontNode = frontNode->next;
					}
					
					currentNode = frontNode;
					currentProcess = currentNode->process;
					preemptived = FALSE;
				}
			}
		}
		time++;
	}
	
	printf("\n==========================NSJF Finished==========================\n");
	printResult(finishedQueue);	
}

int get_smallest_priority_pid(queue *NSJFreadyQueue, int preemptive, int time){ //find the shortest cpu_burst_time
		if(!isempty(NSJFreadyQueue)){
	
			int smallest_priority = 100;
			int smallest_priority_pid = -1;
			int i;
			node *currentNode = NSJFreadyQueue->front; //node that is pointing the foremost node of readyqueue
			process *currentProcess = currentNode->process; //process of the currentnode
			
			for(i = 0; i < NSJFreadyQueue->count; i++){ //go over each node in readyQueue
				
				if(currentProcess->pid <= smallest_priority && time > currentProcess->arrival_time){ //if current process's cpu_burst_time is smaller
					smallest_priority = currentProcess->priority; //change it with the smallest_cpu_burst
					smallest_priority_pid = currentProcess->pid;
				}
				
				currentNode = currentNode->next; 
				
				if(currentNode != NULL){
					currentProcess = currentNode->process;	
				}
			}
			
			return smallest_priority_pid;	
	}else{
		return -1;
	}
}

void pp_Schedule(queue *readyQueue,queue *waitingQueue,queue *finishedQueue, process process_array[PROCESS_NUM], int preemptive){
	
	if(preemptive == TRUE){
		printf("\n\n======================================PP START=====================================\n\n");		
	}else{
		printf("\n\n======================================NPP START=====================================\n\n");
	}
		
	process_creation(readyQueue, process_array); //create process and store them in NSJFreadyQueue
	
	printQueue(readyQueue); //print processes in NSJFreadyQueue
	
	int time = 0;
	
	process *executingProcess = NULL; //process that is executed by cpu
	node *currentNode = readyQueue->front; //node that is pointing the foremost node of readyqueue
	process *currentProcess = currentNode->process; //process of the currentnode
	node *currentIONode = NULL; //node that is pointing the foremost node of waitingqueue
	process *currentIOProcess = NULL; //process of the currentIONode
	int smallest_priority_pid = 0;
	int preemptived = FALSE;
	
	while(!isempty(readyQueue) || !isempty(waitingQueue)){ //till there is process in the readyqueue 
		
		if(!isempty(waitingQueue)){ //if there is process waiting for i/o in waiting queue
			
			if(currentIONode== NULL){ //if currentIONode is NULL, make sure currentIONode  points to the front of waitingQueue.
				currentIONode = waitingQueue->front;
				currentIOProcess = currentIONode->process;
			}
			
			if(currentIOProcess->io_burst_time > 0){ //if process is in the middle of i/o operation
				printf("%3d : (pid : %2d) I/O running\t", time, currentIOProcess->pid);
				currentIOProcess->io_burst_time--; //i/o burst time decreases by one
			}else{ //if process finishes its i/o operation
				printf("%3d : (pid : %2d) I/O finished\t", time, currentIOProcess->pid);	
				currentIOProcess->io_operation_finish_time = time;
				currentIONode = NULL;
				process *readyQueue_back = dequeue(waitingQueue); //dequeue process that finished its i/o opeation from waitingQueue
				enqueue(readyQueue, readyQueue_back); //enqueue the process that finished its i/o operation back to readyQueue
				
				if(currentNode == NULL){
					currentNode = readyQueue->front;
					currentProcess = currentNode->process;
				}
			}	
		}
		
		START:
		if(executingProcess == NULL){
			
			if(time < currentProcess->arrival_time || currentNode == NULL){
				printf("%3d : idle\n", time);	
			}else{
				executingProcess = currentProcess;
				
				if(preemptived == TRUE){
					printf("%3d : (pid : %2d) execution running start (preemptived)\n", time, executingProcess->pid);	
				}else{
					printf("%3d : (pid : %2d) execution running start\n", time, executingProcess->pid);
				}
				
				executingProcess->cpu_burst_time--; //executing process's cpu_brust_time decreases
				
				if(executingProcess->io_operation_count == 0){//add the waiting_time of process that is executed for the first time.
					executingProcess->io_operation = time + executingProcess->io_operation; //executing process's io_operation time is decided by adding the time it started to its originial oo_operation time
					executingProcess->waiting_time += time - executingProcess->arrival_time; 	
				}else{//add the waiting_time of process that was waiting in the ready after io operation has finished.
					executingProcess->waiting_time += time - executingProcess->io_operation_finish_time; 		
				}
				
				if(time >= executingProcess->io_operation && executingProcess->io_burst_time > 0){ //if there is i/o operation occurs		
					printf("%3d : (pid : %2d) -> I/O Operation\n",time ,executingProcess->pid);
					process* io_process = dequeue_process(readyQueue, executingProcess); //dequeue the current process
					io_process->io_operation_count++; //its operation_cout increases
					enqueue(waitingQueue, io_process); //enqueue the current process into waitingqueue
					executingProcess = NULL; //no process is executing	
					
					smallest_priority_pid = get_smallest_priority_pid(readyQueue, preemptive, time); //get the shortest_cpu_burst_time
					if(smallest_priority_pid == -1){
						executingProcess = NULL;
						currentNode = NULL;
					}else{
						node *frontNode = readyQueue->front;
						
						while(frontNode->process->pid != smallest_priority_pid){ //find the node with a process that has shortest_cpu_burst_time
							frontNode = frontNode->next;
						}
						
						currentNode = frontNode;
						currentProcess = currentNode->process;
					}
				}
			}
		}else{
			if(preemptive == TRUE){
				smallest_priority_pid = get_smallest_priority_pid(readyQueue, preemptive, time); //get the shortest_cpu_burst_time		
				
				if(executingProcess->pid != smallest_priority_pid){
					node *frontNode = readyQueue->front;
						
					while(frontNode->process->pid != smallest_priority_pid){ //find the node with a process that has shortest_cpu_burst_time
						frontNode = frontNode->next;
					}
					
					executingProcess = NULL;
					currentNode = frontNode;
					currentProcess = currentNode->process;
					preemptived = TRUE;
					goto START;		
				}
			}
			
			if(executingProcess->cpu_burst_time > 0){ //if process is still executing
				
				printf("%3d : (pid : %2d) execution running\n", time, executingProcess->pid);
				executingProcess->cpu_burst_time--;
				
				if(time >= executingProcess->io_operation && executingProcess->io_burst_time > 0){ //if there is i/o operation occurs		
					printf("%3d : (pid : %2d) -> I/O Operation\n",time ,executingProcess->pid);
					process* io_process = dequeue_process(readyQueue, executingProcess); //dequeue the current process
					io_process->io_operation_count++; //its operation_cout increases
					enqueue(waitingQueue, io_process); //enqueue the current process into waitingqueue
					executingProcess = NULL; //no process is executing	
					
					smallest_priority_pid = get_smallest_priority_pid(readyQueue, preemptive, time); //get the shortest_cpu_burst_time
					if(smallest_priority_pid == -1){
						executingProcess = NULL;
						currentNode = NULL;
					}else{
						node *frontNode = readyQueue->front;
						
						while(frontNode->process->pid != smallest_priority_pid){ //find the node with a process that has shortest_cpu_burst_time
							frontNode = frontNode->next;
						}
						
						currentNode = frontNode;
						currentProcess = currentNode->process;
					}								
				}
			}else{
				printf("%3d : (pid : %2d) execution finished\n", time, executingProcess->pid);
				
				process* finished_process = dequeue_process(readyQueue, executingProcess);
				finished_process->turnaround_time = time - finished_process->arrival_time;
				executingProcess = NULL;
				enqueue(finishedQueue, finished_process);
				smallest_priority_pid = get_smallest_priority_pid(readyQueue, preemptive, time); //get the shortest_cpu_burst_time
				
				if(smallest_priority_pid == -1){
					executingProcess = NULL;
					currentNode = NULL;
				}else{
					node *frontNode = readyQueue->front;
					
					while(frontNode->process->pid != smallest_priority_pid){ //find the node with a process that has shortest_cpu_burst_time
						frontNode = frontNode->next;
					}
					
					currentNode = frontNode;
					currentProcess = currentNode->process;
					preemptived = FALSE;
				}
			}
		}
		time++;
	}
	printf("\n==========================PP Finished==========================\n");
	printResult(finishedQueue);		
}

void rr_Schedule(queue *readyQueue,queue *waitingQueue,queue *finishedQueue, process process_array[PROCESS_NUM], int TIMEQUANTUM){

	printf("\n\n======================================RR START=====================================\n\n");
		
	process_creation(readyQueue, process_array); //create process and store them in NSJFreadyQueue
	
	printQueue(readyQueue); //print processes in NSJFreadyQueue
	
	int time = 0;
	
	process *executingProcess = NULL; //process that is executed by cpu
	node *currentNode = readyQueue->front; //node that is pointing the foremost node of readyqueue
	process *currentProcess = currentNode->process; //process of the currentnode
	node *currentIONode = NULL; //node that is pointing the foremost node of waitingqueue
	process *currentIOProcess = NULL; //process of the currentIONode
	
	while(!isempty(readyQueue) || !isempty(waitingQueue)){ //till there is process in the readyqueue 
		
		if(!isempty(waitingQueue)){ //if there is process waiting for i/o in waiting queue
			
			if(currentIONode== NULL){ //if currentIONode is NULL, make sure currentIONode  points to the front of waitingQueue.
				currentIONode = waitingQueue->front;
				currentIOProcess = currentIONode->process;
			}
			
			if(currentIOProcess->io_burst_time > 0){ //if process is in the middle of i/o operation
				printf("%3d : (pid : %2d) I/O running\t", time, currentIOProcess->pid);
				currentIOProcess->io_burst_time--; //i/o burst time decreases by one
			}else{ //if process finishes its i/o operation
				printf("%3d : (pid : %2d) I/O finished\t", time, currentIOProcess->pid);	
				currentIOProcess->io_operation_finish_time = time;
				currentIONode = NULL;
				process *readyQueue_back = dequeue(waitingQueue); //dequeue process that finished its i/o opeation from waitingQueue
				enqueue(readyQueue, readyQueue_back); //enqueue the process that finished its i/o operation back to readyQueue
				
				if(currentNode == NULL){
					currentNode = readyQueue->front;
					currentProcess = currentNode->process;
				}
			}	
		}
		
		START:
		if(executingProcess == NULL){
			
			if(time < currentProcess->arrival_time || currentNode == NULL){
				printf("%3d : idle\n", time);	
			}else{
				executingProcess = currentProcess;
				executingProcess->time_quantum = 10;
				
				printf("%3d : (pid : %2d) execution running start (TIMEQUANTUM = %d)\n", time, executingProcess->pid, executingProcess->time_quantum);
				
				executingProcess->cpu_burst_time--; //executing process's cpu_brust_time decreases
				executingProcess->time_quantum--;
				
				if(executingProcess->io_operation_count == 0){//add the waiting_time of process that is executed for the first time.
					executingProcess->io_operation = time + executingProcess->io_operation; //executing process's io_operation time is decided by adding the time it started to its originial oo_operation time
					executingProcess->waiting_time += time - executingProcess->arrival_time; 	
				}else{//add the waiting_time of process that was waiting in the ready after io operation has finished.
					executingProcess->waiting_time += time - executingProcess->io_operation_finish_time; 		
				}
				
				if(time >= executingProcess->io_operation && executingProcess->io_burst_time > 0){ //if there is i/o operation occurs		
					printf("%3d : (pid : %2d) -> I/O Operation\n",time ,executingProcess->pid);
					
					if(isempty(readyQueue)){
						executingProcess = NULL;
						currentNode = NULL;
					}else{
						currentNode = currentNode->next;
						currentProcess = currentNode->process;
					}
					
					process* io_process = dequeue_process(readyQueue, executingProcess); //dequeue the current process
					io_process->io_operation_count++; //its operation_cout increases
					enqueue(waitingQueue, io_process); //enqueue the current process into waitingqueue
					executingProcess = NULL; //no process is executing	
					
				}
			}
		}else{

			if(executingProcess->cpu_burst_time > 0 && executingProcess->time_quantum > 0){	
				printf("%3d : (pid : %2d) execution running (TIMEQUANTUM = %d)\n", time, executingProcess->pid, executingProcess->time_quantum);
				executingProcess->cpu_burst_time--;
				executingProcess->time_quantum--;
				
				if(time >= executingProcess->io_operation && executingProcess->io_burst_time > 0){ //if there is i/o operation occurs		
					printf("%3d : (pid : %2d) -> I/O Operation\n",time ,executingProcess->pid);
					
					if(isempty(readyQueue)){
						executingProcess = NULL;
						currentNode = NULL;
					}else{
						currentNode = currentNode->next;
						if(currentNode != NULL){
							currentProcess = currentNode->process;		
						}
					}
					
					process* io_process = dequeue_process(readyQueue, executingProcess); //dequeue the current process
					io_process->io_operation_count++; //its operation_cout increases
					enqueue(waitingQueue, io_process); //enqueue the current process into waitingqueue
					executingProcess = NULL; //no process is executing	
													
				}
			}else if(executingProcess->time_quantum == 0){
				printf("=====Time Quantum Over=====\n");
				
				if(isempty(readyQueue)){
					executingProcess = NULL;
					currentNode = NULL;
				}else{
					if(readyQueue->count == 1){
						currentNode = readyQueue->front;
						currentProcess = currentNode->process;
					}else{
						currentNode = currentNode->next;
						currentProcess = currentNode->process;	
					}	
				}
				
				process* quantum_process = dequeue_process(readyQueue, executingProcess); //dequeue the current process
				enqueue(readyQueue, quantum_process); //enqueue the current process into waitingqueue
				executingProcess = NULL; //no process is executing		
			
			}else{
				printf("%3d : (pid : %2d) execution finished\n", time, executingProcess->pid);
				
				if(isempty(readyQueue)){
					executingProcess = NULL;
					currentNode = NULL;
				}else{
					currentNode = currentNode->next;
					if(currentNode != NULL){		
						currentProcess = currentNode->process;
					}
				}
				
				process* finished_process = dequeue_process(readyQueue, executingProcess);
				finished_process->turnaround_time = time - finished_process->arrival_time;
				executingProcess = NULL;
				enqueue(finishedQueue, finished_process);
			}
		}
		time++;
	}	
	printf("\n==========================RR Finished==========================\n");
	printResult(finishedQueue);		
}


int main()
{
	
	//FCFS Queue
    queue *FCFSreadyQueue; //queue for process that are executing in cpu
    FCFSreadyQueue = (queue*)malloc(sizeof(queue));
    initialize(FCFSreadyQueue);
    
    queue *FCFSwaitingQueue; //queue for processes that are operating in i/o operation
    FCFSwaitingQueue = (queue*)malloc(sizeof(queue));
    initialize(FCFSwaitingQueue);
    
    queue *FCFSfinishedQueue; //queue for calculating waiting time and turnaournd time.
    FCFSfinishedQueue = (queue*)malloc(sizeof(queue));
    initialize(FCFSfinishedQueue);
    
    //NSJF Queue
    queue *NSJFreadyQueue; //queue for process that are executing in cpu
    NSJFreadyQueue = (queue*)malloc(sizeof(queue));
    initialize(NSJFreadyQueue);
    
    queue *NSJFwaitingQueue; //queue for processes that are operating in i/o operation
    NSJFwaitingQueue = (queue*)malloc(sizeof(queue));
    initialize(NSJFwaitingQueue);
    
    queue *NSJFfinishedQueue; //queue for calculating waiting time and turnaournd time.
    NSJFfinishedQueue = (queue*)malloc(sizeof(queue));
    initialize(NSJFfinishedQueue);
    
    //SJF Queue
    queue *SJFreadyQueue; //queue for process that are executing in cpu
    SJFreadyQueue = (queue*)malloc(sizeof(queue));
    initialize(SJFreadyQueue);
    
    queue *SJFwaitingQueue; //queue for processes that are operating in i/o operation
    SJFwaitingQueue = (queue*)malloc(sizeof(queue));
    initialize(SJFwaitingQueue);
    
    queue *SJFfinishedQueue; //queue for calculating waiting time and turnaournd time.
    SJFfinishedQueue = (queue*)malloc(sizeof(queue));
    initialize(SJFfinishedQueue);
    
    //PP Queue
    queue *PPreadyQueue; //queue for process that are executing in cpu
    PPreadyQueue = (queue*)malloc(sizeof(queue));
    initialize(PPreadyQueue);
    
    queue *PPwaitingQueue; //queue for processes that are operating in i/o operation
    PPwaitingQueue = (queue*)malloc(sizeof(queue));
    initialize(PPwaitingQueue);
    
    queue *PPfinishedQueue; //queue for calculating waiting time and turnaournd time.
    PPfinishedQueue = (queue*)malloc(sizeof(queue));
    initialize(PPfinishedQueue);
    
    //NPP Queue
	queue *NPPreadyQueue; //queue for process that are executing in cpu
    NPPreadyQueue = (queue*)malloc(sizeof(queue));
    initialize(NPPreadyQueue);
    
    queue *NPPwaitingQueue; //queue for processes that are operating in i/o operation
    NPPwaitingQueue = (queue*)malloc(sizeof(queue));
    initialize(NPPwaitingQueue);
    
    queue *NPPfinishedQueue; //queue for calculating waiting time and turnaournd time.
    NPPfinishedQueue = (queue*)malloc(sizeof(queue));
    initialize(NPPfinishedQueue);
    
    //RR Queue
	queue *RRreadyQueue; //queue for process that are executing in cpu
    RRreadyQueue = (queue*)malloc(sizeof(queue));
    initialize(RRreadyQueue);
    
    queue *RRwaitingQueue; //queue for processes that are operating in i/o operation
    RRwaitingQueue = (queue*)malloc(sizeof(queue));
    initialize(RRwaitingQueue);
    
    queue *RRfinishedQueue; //queue for calculating waiting time and turnaournd time.
    RRfinishedQueue = (queue*)malloc(sizeof(queue));
    initialize(RRfinishedQueue);
    
	//process creation
    process FCFS_process_array[PROCESS_NUM]; //array used for enqueuing process into FCFSreadyQueue.
    process NSJF_process_array[PROCESS_NUM]; //array used for enqueuing process into NSJPreadyQueue.
	process SJF_process_array[PROCESS_NUM]; //array used for enqueuing process into NSJPreadyQueue.
	process PP_process_array[PROCESS_NUM]; //array used for enqueuing process into PPreadyQueue.
	process NPP_process_array[PROCESS_NUM]; //array used for enqueuing process into NPPreadyQueue.
	process RR_process_array[PROCESS_NUM]; //array used for enqueuing process into RRreadyQueue.
	
	fcfs_Schedule(FCFSreadyQueue, FCFSwaitingQueue, FCFSfinishedQueue, FCFS_process_array);
	nsjf_Schedule(SJFreadyQueue, SJFwaitingQueue, SJFfinishedQueue, SJF_process_array,TRUE);
	nsjf_Schedule(NSJFreadyQueue, NSJFwaitingQueue, NSJFfinishedQueue, NSJF_process_array, FALSE);
	pp_Schedule(PPreadyQueue, PPwaitingQueue, PPfinishedQueue, PP_process_array, TRUE);
	pp_Schedule(NPPreadyQueue, NPPwaitingQueue, NPPfinishedQueue, NPP_process_array, FALSE);
	rr_Schedule(RRreadyQueue, RRwaitingQueue, RRfinishedQueue, RR_process_array, 10);
	
	return 0;
}
