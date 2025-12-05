// policies/mlq.c
// Multi-Level Queue (MLQ) Scheduler
// Niveau 1 : Priorité haute → Round-Robin (quantum)
// Niveau 2 : Priorité basse → FIFO
// Pas d’aging (comme demandé : "éventuellement")

#include <stdio.h>
#include <stdlib.h>
#include "../includes/process.h"
#include "../includes/utils.h"

void schedule_mlq(Process processes[], int num_processes, int quantum) {
    if (quantum <= 0) quantum = 4;  // Quantum par défaut

    // Initialisation des champs de simulation
    for (int i = 0; i < num_processes; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].num_slices = 0;
        processes[i].is_started = 0;
        // On classe simplement : priorité <= 2 → haute (RR), > 2 → basse (FIFO)
        // Tu peux changer cette règle comme tu veux
        processes[i].queue_level = (processes[i].priority <= 2) ? 1 : 2;
    }

    qsort(processes, num_processes, sizeof(Process), compare_arrival_time);

    ProcessQueue high_queue;  // File pour les processus haute priorité (RR)
    ProcessQueue low_queue;   // File pour les processus basse priorité (FIFO)
    init_queue(&high_queue);
    init_queue(&low_queue);

    int current_time = 0;
    int completed = 0;
    int next_arrival_idx = 0;

    printf("\n=== Multi-Level Queue (MLQ) Scheduling ===\n");
    printf("    Niveau 1 (Priorité ≤ 2) → Round-Robin (Quantum = %d)\n", quantum);
    printf("    Niveau 2 (Priorité > 2) → FIFO\n\n");

    while (completed < num_processes) {
        // Ajouter les processus arrivés
        while (next_arrival_idx < num_processes &&
               processes[next_arrival_idx].arrival_time <= current_time) {
            Process *p = &processes[next_arrival_idx];
            if (p->queue_level == 1) {
                enqueue(&high_queue, p);
            } else {
                enqueue(&low_queue, p);
            }
            next_arrival_idx++;
        }

        Process *current = NULL;
        int exec_time = 0;

        // Priorité absolue à la file haute (RR)
        if (!is_empty(&high_queue)) {
            current = dequeue(&high_queue);
            exec_time = (current->remaining_time < quantum) ? current->remaining_time : quantum;
        }
        // Sinon, on prend dans la file basse (FIFO)
        else if (!is_empty(&low_queue)) {
            current = dequeue(&low_queue);
            exec_time = current->remaining_time;  // Exécution complète
        }
        // CPU idle
        else if (next_arrival_idx < num_processes) {
            printf("[T=%3d → T=%3d] CPU inactif (en attente d'arrivée)\n",
                   current_time, processes[next_arrival_idx].arrival_time);
            current_time = processes[next_arrival_idx].arrival_time;
            continue;
        } else {
            break;
        }

        // Première exécution ?
        if (!current->is_started) {
            current->start_time = current_time;
            current->is_started = 1;
            printf("[T=%3d] Début   → %s (Niveau %d, Prio=%d)\n",
                   current_time, current->name, current->queue_level, current->priority);
        }

        // Ajouter la tranche (slice) pour le Gantt
        if (current->num_slices < MAX_SLICES) {
            current->slices[current->num_slices].start = current_time;
            current->slices[current->num_slices].duration = exec_time;
            current->num_slices++;
        }

        current_time += exec_time;
        current->remaining_time -= exec_time;

        // Gestion de la fin ou remise en file
        if (current->remaining_time <= 0) {
            current->remaining_time = 0;
            current->finish_time = current_time;
            completed++;
            printf("[T=%3d] Fin     → %s\n", current_time, current->name);
        } else {
            // Si c'était un processus RR et pas terminé → remise en file haute
            if (current->queue_level == 1) {
                enqueue(&high_queue, current);
            }
            // Les processus FIFO ne reviennent jamais (ils sont exécutés en entier)
        }
    }

    printf("\n=== Fin de la simulation MLQ ===\n\n");
}