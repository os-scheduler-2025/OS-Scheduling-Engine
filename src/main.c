// src/main.c (Mise à jour pour le Chargement Dynamique)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h> // Nécessaire pour dlopen, dlsym, dlclose

#include "../includes/process.h"
#include "../includes/parser.h"
#include "../includes/simulation.h"
#include "../includes/utils.h"

// Le prototype de la fonction de politique n'a plus besoin de 'extern'
// Il est redéfini via le pointeur de fonction 'SchedulerFunc' dans simulation.h

// Structure pour gérer les options de politique dans le menu (mise à jour)
typedef struct {
    char name[50];
    char so_filename[100]; // Nom du fichier .so
} PolicyOption;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Utilisation: %s <fichier_configuration>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Process processes[MAX_PROCESSES];
    int num_processes = 0;
    const char *config_file = argv[1];

    if (parse_processes(config_file, processes, &num_processes) != 0 || num_processes == 0) {
        fprintf(stderr, "Erreur: Aucun processus valide n'a pu être chargé.\n");
        return EXIT_FAILURE;
    }

    PolicyOption policies[] = {
        {"FIFO", "policies/fifo.so"},
        {"Round-Robin", "policies/roundrobin.so"},
        {"Priorite Preemptive", "policies/priority.so"},
        {"Multi-Level Queue (MLQ)", "policies/mlq.so"},
    };
    int num_policies = sizeof(policies) / sizeof(PolicyOption);

    // --- Menu de sélection ---
    int choice = 1; 
    
    printf("\n--- Selection de la Politique d'Ordonnancement ---\n");
    for (int i = 0; i < num_policies; i++) {
        printf("%d. %s %s\n", i + 1, policies[i].name, (i == 0) ? "(Par defaut)" : "");
    }
    printf("Entrez votre choix (1-%d): ", num_policies);
    
    char input[10];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        int user_choice = atoi(input);
        if (user_choice >= 1 && user_choice <= num_policies) {
            choice = user_choice;
        }
    }

    PolicyOption selected_policy = policies[choice - 1];
    int quantum = 0;

    // Demander le quantum si Round-Robin ou MLQ est sélectionné
    if (strcmp(selected_policy.name, "Round-Robin") == 0 || strcmp(selected_policy.name, "Multi-Level Queue (MLQ)") == 0) {
        printf("Entrez la valeur du Quantum (ms, > 0): ");
        if (scanf("%d", &quantum) != 1 || quantum <= 0) {
            fprintf(stderr, "Quantum invalide ou manquant, utilisation de la valeur par defaut (10).\n");
            quantum = 10;
        }
        while (getchar() != '\n'); 
    }

    // --- CHARGEMENT DYNAMIQUE ---
    void *handle = dlopen(selected_policy.so_filename, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Erreur de dlopen pour %s: %s\n", selected_policy.so_filename, dlerror());
        return EXIT_FAILURE;
    }

    // Le nom de la fonction est construit (e.g., "schedule_fifo" -> "schedule_fifo")
    char func_name[100];
    // Retirer le préfixe "policies/" et le suffixe ".so" pour obtenir le nom de base
    char base_name[50];
    strcpy(base_name, selected_policy.so_filename + strlen("policies/"));
    base_name[strlen(base_name) - 3] = '\0'; // Supprimer ".so"
    sprintf(func_name, "schedule_%s", base_name);


    SchedulerFunc scheduler_func = (SchedulerFunc)dlsym(handle, func_name);
    if (!scheduler_func) {
        fprintf(stderr, "Erreur de dlsym pour %s: %s\n", func_name, dlerror());
        dlclose(handle);
        return EXIT_FAILURE;
    }

    // --- Exécution ---
    printf("\nExecution de la simulation avec la politique: %s\n", selected_policy.name);
    run_simulation(processes, num_processes, scheduler_func, quantum);

    display_results(processes, num_processes);

    // --- DÉCHARGEMENT DYNAMIQUE ---
    dlclose(handle);

    return EXIT_SUCCESS;
}