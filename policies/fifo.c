// policies/fifo.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/simulation.h"
#include "../includes/utils.h" // Ajouté pour compare_arrival_time

void schedule_fifo(Process processes[], int num_processes, int quantum) {
    // 1. Trier les processus par temps d'arrivée (FIFO)
    qsort(processes, num_processes, sizeof(Process), compare_arrival_time);

    int current_time = 0; 

    printf("--- Execution FIFO ---\n");

    for (int i = 0; i < num_processes; i++) {
        Process *p = &processes[i];

        // Attente si le processus n'est pas encore arrivé
        if (current_time < p->arrival_time) {
            current_time = p->arrival_time;
        }

        // Le processus est prêt, il commence immédiatement
        p->start_time = current_time;
        
        // Exécution complète (FIFO est non-préemptive)
        current_time += p->burst_time;
        
        // Le processus termine
        p->finish_time = current_time;
        p->remaining_time = 0; 

        printf("Temps %d: Le processus '%s' demarre.\n", p->start_time, p->name);
        printf("Temps %d: Le processus '%s' termine.\n", p->finish_time, p->name);
    }
}