// src/simulation.c (Mise à jour pour Diagramme de Gantt)

#include "../includes/simulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Nouvelle fonction pour afficher le Diagramme de Gantt
void display_gantt_chart(Process processes[], int num_processes) {
    if (num_processes == 0) return;

    // Déterminer la fin maximale de tous les processus
    int max_finish_time = 0;
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].finish_time > max_finish_time) {
            max_finish_time = processes[i].finish_time;
        }
    }

    if (max_finish_time <= 0) {
        printf("\nAucun processus terminé pour afficher le Diagramme de Gantt.\n");
        return;
    }

    printf("\n--- DIAGRAMME DE GANTT (Simplifie) ---\n");

    // Ligne supérieure de la boîte
    printf(" ");
    for (int i = 0; i < max_finish_time; i++) {
        printf("---");
    }
    printf("-\n");

    // Ligne des noms de processus
    printf("|");
    int last_time = 0;

    // Cette reconstruction est une simplification : elle parcourt le temps
    // pour déterminer quel processus s'exécutait à quel moment.
    for (int t = 0; t < max_finish_time; t++) {
        Process *running_process = NULL;
        int count_running = 0;

        // Trouver le processus en cours d'exécution à l'instant t
        for (int i = 0; i < num_processes; i++) {
            // Un processus s'exécute si son temps d'exécution s'étend sur 't'
            // Cette méthode suppose que la simulation a correctement initialisé
            // le temps restant et est moins précise que l'historique de segments,
            // mais fonctionne bien pour les politiques non-préemptives.
            // Pour RR et Prio, nous utilisons la trace console pour la précision.

            if (processes[i].start_time != -1 && 
                t >= processes[i].start_time && t < processes[i].finish_time) {
                // Pour les politiques préemptives, nous devons vérifier que le processus n'est pas "en attente" à t.
                // Une simplification est de considérer le processus comme exécutant s'il n'est pas Idle et n'est pas terminé.
                // Cependant, sans stocker les segments, nous allons nous appuyer sur une approximation par intervalle.
                running_process = &processes[i];
                count_running++;
                break; // Simplement prendre le premier trouvé pour t
            }
        }

        if (running_process != NULL) {
            printf(" %s", running_process->name); 
            // Nous allons afficher seulement les premières lettres des noms pour que cela tienne
            // Mettre la première lettre
            printf(" %c", running_process->name[4]); // E.g., 'Proc_A' -> 'A'
        } else {
            // CPU Idle (inactif)
            printf(" I ");
        }
    }
    printf("|\n");

    // Ligne inférieure de la boîte
    printf(" ");
    for (int i = 0; i < max_finish_time; i++) {
        printf("---");
    }
    printf("-\n");

    // Ligne des temps
    printf("0");
    for (int t = 1; t <= max_finish_time; t++) {
        // Afficher le temps à l'extrémité de chaque segment
        if (t % 5 == 0 || t == max_finish_time) {
            printf("%*d", (t < 10) ? 3 : 3, t); // Alignement
        } else {
            printf("   ");
        }
    }
    printf("\n");
}


// La fonction display_results est la même, mais appelle display_gantt_chart
void display_results(Process processes[], int num_processes) {
    printf("\n--- RESULTATS DE LA SIMULATION ---\n");

    float total_waiting_time = 0;
    float total_turnaround_time = 0;

    printf("\n| %-10s | %-8s | %-8s | %-8s | %-8s | %-14s | %-12s |\n",
           "Processus", "Arrivee", "Execution", "Debut", "Fin", "Temps Rotation", "Temps Attente");
    printf("|------------|----------|-----------|----------|----------|----------------|--------------|\n");

    for (int i = 0; i < num_processes; i++) {
        // Calcul des métriques
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;

        printf("| %-10s | %-8d | %-8d | %-8d | %-8d | %-14d | %-12d |\n",
               processes[i].name,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].start_time,
               processes[i].finish_time,
               processes[i].turnaround_time,
               processes[i].waiting_time);
    }
    printf("---------------------------------------------------------------------------------------\n");

    printf("Temps d'Attente Moyen: %.2f\n", total_waiting_time / num_processes);
    printf("Temps de Rotation Moyen: %.2f\n", total_turnaround_time / num_processes);

    // NOUVEL APPEL
    display_gantt_chart(processes, num_processes);
}


// run_simulation reste inchangée
void run_simulation(Process processes[], int num_processes, SchedulerFunc scheduler, int quantum) {
    if (scheduler != NULL) {
        scheduler(processes, num_processes, quantum);
    }
}