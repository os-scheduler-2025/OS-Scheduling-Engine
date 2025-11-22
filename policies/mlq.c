// policies/mlq.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "../includes/simulation.h"
#include "../includes/utils.h"

// Note: Le défilement d'une file sans supprimer le premier élément
// est complexe avec une implémentation tableau simple. L'Aging ici
// fonctionne en comptant l'attente mais la promotion ne change pas
// instantanément l'ordre d'exécution dans le code simple ci-dessous.

void schedule_mlq(Process processes[], int num_processes, int quantum) {    if (quantum <= 0) {
        quantum = 5;
    }

    // 1. Initialisation des deux files
    ProcessQueue rr_queue;   // File 1: Round-Robin (Haute Priorité)
    ProcessQueue fifo_queue; // File 2: FIFO (Basse Priorité)
    init_queue(&rr_queue);
    init_queue(&fifo_queue);
    
    // Taux d'Aging: Promotion après 10 unités de temps en attente
    const int AGING_THRESHOLD = 10; 
    
    int current_time = 0;
    int completed_processes = 0;
    
    // Trier initialement par temps d'arrivée
    qsort(processes, num_processes, sizeof(Process), compare_arrival_time);
    int next_arrival_index = 0;

    printf("--- Execution Multi-Level Queue (RR Quantum: %d, Aging: %d) ---\n", quantum, AGING_THRESHOLD);

    // Initialisation des files MLQ (Priorité 1-2 -> RR, 3+ -> FIFO)
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].priority <= 2) { 
            processes[i].queue_level = 1;
        } else { 
            processes[i].queue_level = 2;
        }
    }
    
    // --- BOUCLE PRINCIPALE ---
    while (completed_processes < num_processes) {
        
        // 1. Ajouter les processus arrivés aux files appropriées (en respectant le niveau actuel)
        while (next_arrival_index < num_processes && processes[next_arrival_index].arrival_time <= current_time) {
            Process *p = &processes[next_arrival_index];
            if (p->queue_level == 1) {
                enqueue(&rr_queue, p);
            } else {
                enqueue(&fifo_queue, p);
            }
            next_arrival_index++;
        }
        
        // 2. Logique d'Aging (Promotion vers RR) - S'applique aux processus en FIFO
        // On parcourt la liste complète des processus non terminés pour vérifier l'aging
        for (int i = 0; i < num_processes; i++) {
            Process *p = &processes[i];
            
            if (p->remaining_time > 0 && p->queue_level == 2) {
                // Si le processus est arrivé mais n'est pas en cours d'exécution
                if (p->arrival_time <= current_time) {
                    p->aging_counter++;
                }

                if (p->aging_counter >= AGING_THRESHOLD) {
                    printf("Temps %d: AGING - Processus '%s' promu de FIFO a RR (Compteur: %d).\n", 
                           current_time, p->name, p->aging_counter);
                    p->queue_level = 1; // Promotion
                    p->aging_counter = 0; 
                    // Pour que l'algorithme fonctionne correctement, le processus
                    // doit être déplacé de fifo_queue à rr_queue.
                    // Dans cette implémentation simple, on le laisse en fifo_queue
                    // et on le "récupère" dans la prochaine étape (3.)
                }
            }
        }
        
        Process *current_process = NULL;
        int execution_time = 0;
        int process_from_queue = 0; // 1 pour RR, 2 pour FIFO

        // 3. Choix du Processus (MLQ Préemptif)
        if (!is_empty(&rr_queue)) {
            // File 1 (RR) a la priorité absolue
            current_process = dequeue(&rr_queue);
            process_from_queue = 1;
            execution_time = (current_process->remaining_time < quantum) ? 
                             current_process->remaining_time : quantum;

        } else if (!is_empty(&fifo_queue)) {
            // File 2 (FIFO) s'exécute seulement si RR est vide
            current_process = dequeue(&fifo_queue);
            process_from_queue = 2;
            // FIFO est non-préemptif (exécute jusqu'à la fin ou l'arrivée d'un RR)
            execution_time = current_process->remaining_time; 
        }

        // 4. Exécution
        if (current_process != NULL) {
            
            if (current_process->is_started == 0) {
                current_process->start_time = current_time;
                current_process->is_started = 1;
                printf("Temps %d: '%s' demarre (Niveau: %d, Durée: %d).\n", 
                       current_time, current_process->name, current_process->queue_level, current_process->burst_time);
            }

            // Gérer la préemption par un nouvel arrivant de haute priorité (si FIFO est en cours)
            int next_rr_arrival_time = INT_MAX;
            for (int i = next_arrival_index; i < num_processes; i++) {
                if (processes[i].priority <= 2) { // Processus qui va en RR (Prio <= 2)
                    next_rr_arrival_time = processes[i].arrival_time;
                    break;
                }
            }
            
            // Si la FIFO exécute, elle peut être limitée par l'arrivée du RR
            if (process_from_queue == 2 && next_rr_arrival_time < current_time + execution_time) {
                 execution_time = next_rr_arrival_time - current_time;
            }

            // Mettre à jour le temps et l'état
            current_time += execution_time;
            current_process->remaining_time -= execution_time;

            // 5. Terminaison ou Retour en File
            if (current_process->remaining_time == 0) {
                current_process->finish_time = current_time;
                completed_processes++;
                printf("Temps %d: '%s' termine.\n", current_time, current_process->name);
                
            } else if (process_from_queue == 1) {
                // RR est préempté par le Quantum, il retourne en RR
                enqueue(&rr_queue, current_process);
                printf("Temps %d: '%s' preempte par Quantum, retourne en RR (restant: %d).\n", 
                       current_time, current_process->name, current_process->remaining_time);
            } else if (process_from_queue == 2) {
                // FIFO est préempté par l'arrivée d'un RR, il retourne en FIFO
                enqueue(&fifo_queue, current_process);
                printf("Temps %d: '%s' preempte par arrivée RR, retourne en FIFO (restant: %d).\n", 
                       current_time, current_process->name, current_process->remaining_time);
            }
        } else {
            // CPU inactif, avancer jusqu'à la prochaine arrivée
            int next_arrival_time = INT_MAX;
            for (int i = next_arrival_index; i < num_processes; i++) {
                if (processes[i].arrival_time < next_arrival_time) {
                    next_arrival_time = processes[i].arrival_time;
                }
            }
            if (next_arrival_time != INT_MAX) {
                current_time = next_arrival_time;
                // Pas de message "CPU inactif" pour alléger la trace
            } else {
                break;
            }
        }
    }
}