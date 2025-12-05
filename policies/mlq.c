// policies/mlq.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "../includes/simulation.h"
#include "../includes/utils.h"

void schedule_mlq(Process processes[], int num_processes, int quantum) {
    if (quantum <= 0) quantum = 4;

    ProcessQueue rr_queue;   // File 1: Round-Robin (priorité ≤2)
    ProcessQueue fifo_queue; // File 2: FIFO (priorité >2)
    init_queue(&rr_queue);
    init_queue(&fifo_queue);
    
    const int AGING_THRESHOLD = 10; 
    
    int current_time = 0;
    int completed_processes = 0;
    
    qsort(processes, num_processes, sizeof(Process), compare_arrival_time);
    int next_arrival_index = 0;

    // Attribution initiale des files
    for (int i = 0; i < num_processes; i++) {
        processes[i].queue_level = (processes[i].priority <= 2) ? 1 : 2;
        processes[i].aging_counter = 0;
    }
    
    printf("--- Execution Multi-Level Queue (Quantum RR: %d, Aging: %d) ---\n", quantum, AGING_THRESHOLD);

    while (completed_processes < num_processes) {
        
        // Ajouter les processus arrivés
        while (next_arrival_index < num_processes && 
               processes[next_arrival_index].arrival_time <= current_time) {
            Process *p = &processes[next_arrival_index];
            if (p->queue_level == 1) {
                enqueue(&rr_queue, p);
            } else {
                enqueue(&fifo_queue, p);
            }
            next_arrival_index++;
        }
        
        // Aging : promotion après attente prolongée
        for (int i = 0; i < num_processes; i++) {
            Process *p = &processes[i];
            if (p->remaining_time > 0 && p->queue_level == 2 && p->arrival_time <= current_time) {
                p->aging_counter++;
                if (p->aging_counter >= AGING_THRESHOLD) {
                    printf("Temps %d: AGING - '%s' promu en file RR !\n", current_time, p->name);
                    p->queue_level = 1;
                    p->aging_counter = 0;
                    // On le déplacera à la prochaine itération
                }
            }
        }

        Process *current_process = NULL;
        int execution_time = 0;
        int from_rr = 0;

        if (!is_empty(&rr_queue)) {
            current_process = dequeue(&rr_queue);
            from_rr = 1;
            execution_time = (current_process->remaining_time < quantum) ? 
                             current_process->remaining_time : quantum;
        } else if (!is_empty(&fifo_queue)) {
            current_process = dequeue(&fifo_queue);
            from_rr = 0;
            execution_time = current_process->remaining_time;
        }

        if (current_process != NULL) {
            
            if (current_process->is_started == 0) {
                current_process->start_time = current_time;
                current_process->is_started = 1;
                printf("Temps %d: '%s' démarre (Niveau: %d).\n", 
                       current_time, current_process->name, current_process->queue_level);
            }

            // Enregistrement de la slice (indispensable pour le Gantt)
            if (current_process->num_slices < MAX_SLICES) {
                current_process->slices[current_process->num_slices].start = current_time;
                current_process->slices[current_process->num_slices].duration = execution_time;
                current_process->num_slices++;
            }

            current_time += execution_time;
            current_process->remaining_time -= execution_time;

            if (current_process->remaining_time == 0) {
                current_process->finish_time = current_time;
                completed_processes++;
                printf("Temps %d: '%s' termine.\n", current_time, current_process->name);
            } else if (from_rr) {
                enqueue(&rr_queue, current_process);
            } else {
                enqueue(&fifo_queue, current_process);
            }
        } else {
            if (next_arrival_index < num_processes) {
                current_time = processes[next_arrival_index].arrival_time;
            } else {
                break;
            }
        }
    }
}