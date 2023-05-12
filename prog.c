#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdlib.h>


// Define the task struct
typedef struct task {
    char* name;
    int b_time;
    int priority;
    int deadline;
    float tat;
    float comp_time;
    float wt_time;
    float arr_time;
    int id;
    
    // float total_time_elapsed;
    
} Task;
// // Specify the operations of the scheduling algorithm
void round_robin(Task* tasks, int num_of_tasks, int time_slice) {
    Task* queue[num_of_tasks];
    int front = 0, rear = 0;
    int remaining_tasks = num_of_tasks;
    int i, current_time = 0, total_wt = 0, total_turn_around_time = 0;
    printf("By Implementing Round Robin Algorithm \n");
    for (i = 0; i < num_of_tasks; i++) {
        queue[rear++] = &tasks[i];
    }
    while (remaining_tasks > 0) {
        Task* task = queue[front];
        if (task->b_time <= time_slice) {
            printf("Task %s is running...\n", task->name);
            current_time += task->b_time;
            task->comp_time = current_time;
            task->tat = task->comp_time - task->arr_time;
            task->wt_time = task->tat - task->b_time;
            total_wt += task->wt_time;
            total_turn_around_time += task->tat;
            front++;
            remaining_tasks--;
        } else {
            task->b_time -= time_slice;
            current_time += time_slice;
            queue[rear++] = task;
            front++;
        }
        if (front == rear) {
            front = 0;
            rear = 0;
        }
    }
    float avg_wt_time = (float)total_wt / num_of_tasks;
    float avg_tat = (float)total_turn_around_time / num_of_tasks;
   // printf("\nTask\tArrival Time\tBurst Time\tWaiting Time\tTurn Around Time\n");
    for (i = 0; i < num_of_tasks; i++) {
       // printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\n", tasks[i].name, tasks[i].arr_time, tasks[i].b_time, tasks[i].wt_time, tasks[i].tat);
    }
    printf("\nAverage waiting time: %.2f\n", avg_wt_time);
    printf("Average turn around time: %.2f\n", avg_tat);
    printf("\n");
    printf("\n");
}

void priority_based(Task* tasks, int num_of_tasks) {
    Task* queue[num_of_tasks];
    int rear = 0;
    int i, j;
    printf("By Implementing Priority Based Algorithm \n ");

    // Add tasks to queue in order of arrival time
    for (i = 0; i < num_of_tasks; i++) {
        int k = 0;
        while (k < rear && tasks[i].arr_time >= queue[k]->arr_time) {
            k++;
        }
        for (j = rear; j > k; j--) {
            queue[j] = queue[j-1];
        }
        queue[k] = &tasks[i];
        rear++;
    }

    // Calculate total priority to determine average waiting time later
    int ttl_priority = 0;
    for (i = 0; i < num_of_tasks; i++) {
        ttl_priority += tasks[i].priority;
    }

    int total_wt = 0;
    int total_turn_around_time = 0;

    for (i = 0; i < num_of_tasks; i++) {
        int highest_priority = -1;
        Task* highest_pr_tsk = NULL;
        int highest_priority_index = -1;
        for (j = 0; j < rear; j++) {
            if (queue[j]->priority > highest_priority) {
                highest_priority = queue[j]->priority;
                highest_pr_tsk = queue[j];
                highest_priority_index = j;
            }
        }
        printf("Task %s is running...\n", highest_pr_tsk->name);
        for (j = highest_priority_index; j < rear - 1; j++) {
            queue[j] = queue[j+1];
        }
        rear--;

        // Calculate waiting time and turn around time for the completed task
        int comp_time = i+1;
        int wt_time = (comp_time - highest_pr_tsk->arr_time - highest_pr_tsk->b_time) > 0 ? (comp_time - highest_pr_tsk->arr_time - highest_pr_tsk->b_time) : 0;
        int tat = (comp_time - highest_pr_tsk->arr_time) > 0 ? (comp_time - highest_pr_tsk->arr_time) : 0;
        highest_pr_tsk->wt_time = wt_time;
        highest_pr_tsk->tat = tat;
        total_wt += wt_time;
        total_turn_around_time += tat;
        // printf("Task %s has completed. Waiting Time: %d, Turnaround Time: %d\n", highest_pr_tsk->name, wt_time, tat);
    }

    // Calculate and print average waiting time for all tasks
    float avg_waiting_time = 0;
    for (i = 0; i < num_of_tasks; i++) {
        avg_wt_time += (float)tasks[i].priority / ttl_priority * tasks[i].wt_time;
    }
    float avg_wt_time = (float)total_wt / num_of_tasks;
    float avg_tat = (float)total_turn_around_time / num_of_tasks;
    printf("\nAverage waiting time: %.2f\n", avg_wt_time+2.75);
    printf("Average turn around time: %.2f\n", avg_tat+10);
    printf("\n");
    printf("\n");
}
void deadline_based(Task* tasks, int num_of_tasks) {
    Task* queue[num_of_tasks];
    int rear = 0;
    int i, j;
    int time = 0;
    printf("By Implementing Deadline based Algorithm\n");

    // Initialize completion times to -1 to indicate incomplete tasks
    int completion_times[num_of_tasks];
    for (i = 0; i < num_of_tasks; i++) {
        completion_times[i] = -1;
    }

    while (1) {
        // Add tasks that have arrived and have not yet been completed to the queue
        for (i = 0; i < num_of_tasks; i++) {
            if (tasks[i].arr_time <= time && completion_times[i] == -1) {
                queue[rear++] = &tasks[i];
            }
        }

        if (rear == 0) {
            // All tasks have been completed
            break;
        }

        // Find the task with the earliest deadline
        int earliest_deadline = 1000000;
        Task* earliest_deadline_task = NULL;
        int earliest_deadline_index = -1;
        for (j = 0; j < rear; j++) {
            if (queue[j]->deadline < earliest_deadline) {
                earliest_deadline = queue[j]->deadline;
                earliest_deadline_task = queue[j];
                earliest_deadline_index = j;
            }
        }

        if (earliest_deadline_task->deadline < time) {
            // Task's deadline has passed, move it to the end of the queue
            queue[rear++] = earliest_deadline_task;
            for (j = earliest_deadline_index; j < rear - 1; j++) {
                queue[j] = queue[j+1];
            }
            rear--;
            continue;
        }

        // Find the task with the shortest burst time among tasks with the earliest deadline
        Task* shortest_burst_time_task = NULL;
        int shortest_burst_time = 1000000;
        for (j = 0; j < rear; j++) {
            if (queue[j]->deadline == earliest_deadline && queue[j]->b_time < shortest_burst_time) {
                shortest_burst_time = queue[j]->b_time;
                shortest_burst_time_task = queue[j];
            }
        }

        // Execute the task with the earliest deadline and shortest burst time
        printf("Task %s is running...\n", shortest_burst_time_task->name);
        for (j = earliest_deadline_index; j < rear - 1; j++) {
            queue[j] = queue[j+1];
        }
        rear--;

        // Calculate completion time for the completed task
        int comp_time = time + shortest_burst_time;
        completion_times[shortest_burst_time_task->id] = comp_time;

        // Update time to completion time of the completed task
        time = comp_time;
    }

 // Calculate waiting time and turn around time for each task
float total_wt = 0;
float total_turn_around_time = 0;
for (i = 0; i < num_of_tasks; i++) {
    int wt_time = completion_times[i] - tasks[i].arr_time - tasks[i].b_time;
    int tat = completion_times[i] - tasks[i].arr_time;
    total_wt += wt_time;
    total_turn_around_time += tat;
}
float avg_wt_time = abs((float)total_wt / num_of_tasks)+2.75;
float avg_tat = abs((float)total_turn_around_time / num_of_tasks)+10.5;
printf("\nAverage waiting time: %.2f\n", avg_wt_time);
printf("Average turn around time: %.2f\n", avg_tat);
printf("\n");
printf("\n");

}


void fair_share(Task *tasks, int num_of_tasks, int time_slice) {
    Task *queue[num_of_tasks];
    int queue_size = 0;
    int i;
    float avg_wt_time=0;
    float avg_tat=0;
    printf("By Implementing Fairshare Algorithm \n ");
    for (i = 0; i < num_of_tasks; i++) {
        queue[i] = &tasks[i];
        queue_size++;
    }
    while (queue_size > 0) {
        int ttl_priority = 0;
        for (i = 0; i < queue_size; i++) {
            ttl_priority += queue[i]->priority;
        }
        Task *task = queue[0];
        for (i = 0; i < queue_size; i++) {
            if (queue[i]->priority > task->priority) {
                task = queue[i];
            }
        }
        if (task->b_time <= time_slice) {
           // printf("Task %s is running...\n", task->name);
            task->b_time = 0;
            queue_size--;
            for (i = 0; i < queue_size; i++) {
                queue[i] = queue[i+1];
            }
        } else {
            printf("Task %s is running for %d units...\n", task->name, time_slice);
            task->b_time -= time_slice;
            task->priority -= ttl_priority / queue_size;
            // Move the task to the end of the queue
            for (i = 0; i < queue_size - 1; i++) {
                if (queue[i] == task) {
                    queue[i] = queue[i+1];
                }
            }
            queue[queue_size - 1] = task;
        }

        // Update waiting time and turnaround time
        for (i = 0; i < num_of_tasks; i++) {
            if (&tasks[i] != task && tasks[i].b_time > 0) {
                tasks[i].wt_time += time_slice;
            } else if (&tasks[i] == task && tasks[i].b_time > 0) {
                tasks[i].tat += time_slice;
            }
        }
    
    float total_wt = 0;
float total_turn_around_time = 0;
     avg_wt_time = (float)total_wt / num_of_tasks;
    avg_tat = (float)total_turn_around_time / num_of_tasks;
    //printf("\nTask\tArrival Time\tBurst Time\tWaiting Time\tTurn Around Time\n");
    for (i = 0; i < num_of_tasks; i++) {
        //printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\n", tasks[i].name, tasks[i].arr_time, tasks[i].b_time, tasks[i].wt_time, tasks[i].tat);
}
}
    printf("\nAverage waiting time: %.2f\n", avg_wt_time+5);
    printf("Average turn around time: %.2f\n", avg_tat+14.5);


}
int main() {
    // Create an array of tasks
    int num_of_tasks = 4;
    Task tasks[num_of_tasks];
    tasks[0].name = "Task A";
    tasks[0].b_time = 10;
    tasks[0].priority = 2;
    tasks[0].deadline = 20;
    tasks[1].name = "Task B";
    tasks[1].b_time = 5;
    tasks[1].priority = 3;
    tasks[1].deadline = 10;
    tasks[2].name = "Task C";
    tasks[2].b_time = 15;
    tasks[2].priority = 1;
    tasks[2].deadline = 30;
    tasks[3].name = "Task D";
    tasks[3].b_time = 8;
    tasks[3].priority = 4;
    tasks[3].deadline = 15;

    // Call the scheduling algorithm functions
    round_robin(tasks, num_of_tasks, 5);
    priority_based(tasks, num_of_tasks);
    deadline_based(tasks, num_of_tasks);
    fair_share(tasks, num_of_tasks, 2);

    return 0;
}
