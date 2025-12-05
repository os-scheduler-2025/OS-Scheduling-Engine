// includes/simulation.h

#ifndef SIMULATION_H
#define SIMULATION_H

#include "process.h"

typedef void (*SchedulerFunc)(Process[], int, int); 

void run_simulation(Process processes[], int num_processes, SchedulerFunc scheduler, int quantum);
void display_results(Process processes[], int num_processes);

#endif // SIMULATION_H