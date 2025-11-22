// src/gui.c (CORRIGÉ pour le multithreading GTK)

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <pthread.h> 
#include <cairo.h> // Nécessaire pour le dessin

#include "../includes/process.h"
#include "../includes/simulation.h"

// Définition d'une structure globale des données de simulation
typedef struct {
    Process *processes;
    int num_processes;
    SchedulerFunc scheduler;
    int quantum;
    GtkWidget *status_label;
    GtkWidget *drawing_area; // Zone pour dessiner le Gantt
} SimulationData;

// Instance globale des données
SimulationData g_sim_data;

// --- STRUCTURES ET WRAPPERS POUR MISE À JOUR MULTITHREAD ---

// Structure pour passer les données de mise à jour du label
typedef struct {
    GtkWidget *label;
    gchar *text;
} LabelUpdateContext;

// Wrapper pour mettre à jour le label (exécuté dans le thread principal GTK)
// La fonction doit retourner gboolean et prendre un seul gpointer argument.
gboolean update_status_label_wrapper(gpointer data) {
    LabelUpdateContext *context = (LabelUpdateContext *)data;
    gtk_label_set_text(GTK_LABEL(context->label), context->text);
    g_free(context->text); // Libérer la mémoire allouée dans le thread
    g_free(context);
    return G_SOURCE_REMOVE; // Supprime la source après l'exécution
}

// Wrapper pour demander un redessin (exécuté dans le thread principal GTK)
gboolean queue_draw_wrapper(gpointer data) {
    GtkWidget *widget = (GtkWidget *)data;
    gtk_widget_queue_draw(widget);
    return G_SOURCE_REMOVE;
}

// --- DESSIN DU DIAGRAMME DE GANTT (GtkDrawingArea) ---
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
    
    if (g_sim_data.processes == NULL || g_sim_data.processes[0].finish_time == 0) {
        return FALSE;
    }

    int max_finish_time = 0;
    for (int i = 0; i < g_sim_data.num_processes; i++) {
        if (g_sim_data.processes[i].finish_time > max_finish_time) {
            max_finish_time = g_sim_data.processes[i].finish_time;
        }
    }
    
    if (max_finish_time == 0) return FALSE;

    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    double width = allocation.width;
    double start_x = 100.0;
    double bar_height = 20.0;
    
    // Assurer que l'échelle est gérable (diviser max_finish_time - 1)
    double time_scale = (width - start_x - 10.0) / max_finish_time; 

    cairo_set_line_width(cr, 1.0);

    // Dessin de la légende (à gauche)
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12.0);
    
    for (int i = 0; i < g_sim_data.num_processes; i++) {
        double y_pos = 50.0 + i * (bar_height + 5.0);
        cairo_move_to(cr, 10, y_pos + bar_height * 0.7);
        cairo_show_text(cr, g_sim_data.processes[i].name);
    }
    
    // Dessin des barres d'exécution (Gantt)
    for (int i = 0; i < g_sim_data.num_processes; i++) {
        Process *p = &g_sim_data.processes[i];
        double y_pos = 50.0 + i * (bar_height + 5.0);

        // Couleur de la barre
        unsigned int color_seed = p->name[4] * 123 + p->name[5] * 79;
        cairo_set_source_rgb(cr, (color_seed % 10 + 5) / 15.0, ((color_seed + 3) % 10 + 5) / 15.0, ((color_seed + 7) % 10 + 5) / 15.0);
        
        // Intervalle complet d'exécution (Début à Fin)
        double bar_start = start_x + (p->start_time * time_scale);
        double full_width = (p->finish_time - p->start_time) * time_scale;
        
        cairo_rectangle(cr, bar_start, y_pos, full_width, bar_height);
        cairo_fill_preserve(cr);
        
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Contour noir
        cairo_stroke(cr);
    }

    // Dessin des échelles de temps
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_font_size(cr, 10.0);
    
    // Axe horizontal
    cairo_move_to(cr, start_x, 30.0);
    cairo_line_to(cr, width - 10, 30.0);
    cairo_stroke(cr);
    
    for (int t = 0; t <= max_finish_time; t += 5) {
        double x_pos = start_x + t * time_scale;
        cairo_move_to(cr, x_pos, 30.0);
        cairo_line_to(cr, x_pos, 35.0);
        cairo_stroke(cr);
        
        gchar *time_str = g_strdup_printf("%d", t);
        cairo_move_to(cr, x_pos - 5, 45.0);
        cairo_show_text(cr, time_str);
        g_free(time_str);
    }

    return FALSE;
}

// --- FONCTION DE SIMULATION POUR LE THREAD (Exécution Complète) ---
void* run_scheduler_thread(void* data) {
    SimulationData *s_data = (SimulationData*)data;
    
    // Exécution de la simulation complète
    s_data->scheduler(s_data->processes, s_data->num_processes, s_data->quantum);
    
    // Calcul de l'heure de fin pour le message
    int max_finish_time = 0;
    for (int i = 0; i < s_data->num_processes; i++) {
        if (s_data->processes[i].finish_time > max_finish_time) {
            max_finish_time = s_data->processes[i].finish_time;
        }
    }

    // Préparation du message à afficher
    gchar *message_text = g_strdup_printf("Simulation terminée à t=%d. Voir les résultats ci-dessous.", max_finish_time);
    
    // Préparation du contexte pour le wrapper (mise à jour du label)
    LabelUpdateContext *context = g_new(LabelUpdateContext, 1);
    context->label = s_data->status_label;
    context->text = message_text;

    // Mise à jour de l'IHM (via le wrapper et g_idle_add)
    g_idle_add(update_status_label_wrapper, context);
    
    // Demander le redessin de la zone de Gantt (via le wrapper)
    g_idle_add(queue_draw_wrapper, s_data->drawing_area);

    // Affichage des résultats finaux dans la console (pour le tableau des métriques)
    display_results(s_data->processes, s_data->num_processes);

    return NULL;
}

// --- INITIALISATION DE L'IHM ---
void run_gui_simulation(Process processes[], int num_processes, SchedulerFunc scheduler, int quantum) {
    
    // Copie des données dans la structure globale
    g_sim_data.processes = processes;
    g_sim_data.num_processes = num_processes;
    g_sim_data.scheduler = scheduler;
    g_sim_data.quantum = quantum;

    // 1. Initialisation de GTK
    gtk_init(NULL, NULL);

    // 2. Création de la Fenêtre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Ordonnanceur Multitâche - Simulation Graphique Avancée");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 900, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 3. Conteneurs
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // 4. Élément d'affichage (Label pour l'état)
    GtkWidget *status_label = gtk_label_new("Lancement de la simulation en cours...");
    gtk_box_pack_start(GTK_BOX(vbox), status_label, FALSE, FALSE, 0);
    g_sim_data.status_label = status_label;

    // 5. Zone de dessin (Diagramme de Gantt)
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, -1, 400); 
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL);
    g_sim_data.drawing_area = drawing_area;
    
    // 6. Lancement de la simulation dans un thread
    pthread_t sim_thread;
    if (pthread_create(&sim_thread, NULL, run_scheduler_thread, &g_sim_data) != 0) {
        perror("Erreur de création du thread de simulation");
        return;
    }
    
    // 7. Affichage et Lancement de la boucle principale
    gtk_widget_show_all(window);
    gtk_main(); 
    
    pthread_join(sim_thread, NULL);
}