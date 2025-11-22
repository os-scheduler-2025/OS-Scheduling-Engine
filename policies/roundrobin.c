// policies/roundrobin.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/simulation.h"
#include "../includes/utils.h" 

void schedule_round_robin(Process processes[], int num_processes, int quantum) {
    // Initialisation
    int current_time = 0;
    int completed_processes = 0;
    ProcessQueue ready_queue;
    init_queue(&ready_queue);
    
    // Trier par temps d'arrivée pour ajouter correctement à la file
    qsort(processes, num_processes, sizeof(Process), compare_arrival_time);
    
    int next_arrival_index = 0;
    
    printf("--- Execution Round-Robin (Quantum: %d) ---\n", quantum);

    // Boucle principale de la simulation
    while (completed_processes < num_processes) {
        
        // 1. Ajouter les processus arrivés à la file d'attente
        while (next_arrival_index < num_processes && processes[next_arrival_index].arrival_time <= current_time) {
            // Vérifier que le processus n'est pas déjà dans la file (pour éviter les doublons si déjà préempté)
            // Dans cette implémentation simple, on utilise le flag is_started pour éviter de remettre le même proc
            // qui est déjà dans la file suite à une préemption. Cependant, la file d'attente devrait gérer cela
            // via une structure plus robuste (comme les listes chaînées). Pour le tableau simple, on se base sur l'arrivée.
            
            // Le processus est ajouté s'il n'est pas encore dans la file et qu'il arrive
            enqueue(&ready_queue, &processes[next_arrival_index]);
            next_arrival_index++;
        }

        // 2. Traiter le processus en tête de file
        if (!is_empty(&ready_queue)) {
            Process *current_process = dequeue(&ready_queue);
            
            // Calculer le temps d'exécution (le minimum entre le quantum et le temps restant)
            int execution_time = (current_process->remaining_time < quantum) ? 
                                 current_process->remaining_time : quantum;
            
            // Temps de début (première fois)
            if (current_process->is_started == 0) {
                current_process->start_time = current_time;
                current_process->is_started = 1;
                printf("Temps %d: Le processus '%s' demarre pour la premiere fois.\n", current_time, current_process->name);
            }

            // Exécution
            current_time += execution_time;
            current_process->remaining_time -= execution_time;

            // 3. Ajouter les nouveaux arrivants pendant l'exécution (si l'arrivée tombe pile à la fin du quantum)
            while (next_arrival_index < num_processes && processes[next_arrival_index].arrival_time <= current_time) {
                enqueue(&ready_queue, &processes[next_arrival_index]);
                next_arrival_index++;
            }
            
            // 4. Terminaison ou Remise en file
            if (current_process->remaining_time == 0) {
                current_process->finish_time = current_time;
                completed_processes++;
                printf("Temps %d: Le processus '%s' termine.\n", current_time, current_process->name);
            } else {
                // Remettre le processus en fin de file
                enqueue(&ready_queue, current_process);
                printf("Temps %d: Le processus '%s' est preempte (restant: %d).\n", 
                       current_time, current_process->name, current_process->remaining_time);
            }
        } else {
            // CPU inactif. Avancer le temps jusqu'à la prochaine arrivée
            if (next_arrival_index < num_processes) {
                printf("Temps %d: CPU inactif.", current_time);
                current_time = processes[next_arrival_index].arrival_time;
                printf(" Avance du temps à %d jusqu'à l'arrivée de '%s'.\n", current_time, processes[next_arrival_index].name);
            } else {
                break;
            }
        }
    }
}