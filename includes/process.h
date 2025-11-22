// includes/process.h

#ifndef PROCESS_H
#define PROCESS_H

#define MAX_NAME_LENGTH 50
#define MAX_PROCESSES 100 // Taille maximale du tableau de processus

// Structure pour décrire un processus
typedef struct {
    char name[MAX_NAME_LENGTH]; 
    int arrival_time;          
    int burst_time;           
    int priority;             

    // Variables pour la simulation
    int remaining_time;        
    int start_time;            
    int finish_time;           
    int waiting_time;          
    int turnaround_time;       
    
    // CHAMPS POUR LES POLITIQUES PRÉEMPTIVES
    int is_started; // 0 ou 1: pour savoir si start_time a été initialisé

    // NOUVEAUX CHAMPS POUR MLQ et AGING
    int queue_level;    // 1 (RR) ou 2 (FIFO)
    int aging_counter;  // Compteur de temps passé en attente

} Process;


// Structure simple de File (Queue) 
typedef struct {
    Process* elements[MAX_PROCESSES]; // Pointeur vers les processus
    int front; 
    int rear;  
} ProcessQueue;

// Prototypes des fonctions de gestion de la File (implémentées dans utils.c)
void init_queue(ProcessQueue *q);
int is_empty(ProcessQueue *q);
void enqueue(ProcessQueue *q, Process *p);
Process* dequeue(ProcessQueue *q);

#endif // PROCESS_H