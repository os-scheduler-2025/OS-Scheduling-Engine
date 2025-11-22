// src/main.c (Version finale avec IHM GTK)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h> 
#include <gtk/gtk.h> // Ajout de l'inclusion GTK pour la compilation

#include "../includes/process.h"
#include "../includes/parser.h"
#include "../includes/simulation.h"
#include "../includes/utils.h"

// Déclaration de la fonction IHM (implémentée dans src/gui.c)
extern void run_gui_simulation(Process processes[], int num_processes, SchedulerFunc scheduler, int quantum);

// Structure pour gérer les options de politique dans le menu
typedef struct {
    char name[50];
    char so_filename[100]; // Nom du fichier .so
} PolicyOption;

int main(int argc, char *argv[]) {
    // Il faut appeler gtk_init si on veut afficher des messages d'erreur GTK.
    // Cependant, dans ce cas, nous appelons run_gui_simulation qui s'en charge.

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
    void *handle = dlopen(selected_policy.so_filename, RTLD_LAZY | RTLD_GLOBAL); // Utiliser RTLD_GLOBAL pour la stabilité GTK
    if (!handle) {
        fprintf(stderr, "Erreur de dlopen pour %s: %s\n", selected_policy.so_filename, dlerror());
        return EXIT_FAILURE;
    }

    char func_name[100];
    char base_name[50];
    strcpy(base_name, selected_policy.so_filename + strlen("policies/"));
    base_name[strlen(base_name) - 3] = '\0'; 
    sprintf(func_name, "schedule_%s", base_name);


    SchedulerFunc scheduler_func = (SchedulerFunc)dlsym(handle, func_name);
    if (!scheduler_func) {
        fprintf(stderr, "Erreur de dlsym pour %s: %s\n", func_name, dlerror());
        dlclose(handle);
        return EXIT_FAILURE;
    }

    // --- Appel de la SIMULATION GRAPHIQUE ---
    printf("\nLancement de l'IHM Graphique pour la politique: %s\n", selected_policy.name);
    run_gui_simulation(processes, num_processes, scheduler_func, quantum);

    // --- DÉCHARGEMENT DYNAMIQUE ---
    dlclose(handle);

    // Le programme se termine après gtk_main_quit dans run_gui_simulation
    return EXIT_SUCCESS;
}