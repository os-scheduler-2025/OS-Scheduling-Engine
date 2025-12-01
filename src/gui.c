// src/gui.c – VERSION FINALE PARFAITE (couleurs magnifiques + tout fonctionne)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include <cairo.h>

#include "../includes/process.h"
#include "../includes/simulation.h"

typedef struct {
    Process *processes;
    int num_processes;
    SchedulerFunc scheduler;
    int quantum;
    GtkWidget *status_label;
    GtkWidget *drawing_area;
} SimulationData;

SimulationData g_sim_data;

// Pour le callback thread-safe
typedef struct {
    GtkWidget *label;
    gchar     *text;
} LabelUpdateContext;

gboolean update_status_label_wrapper(gpointer data) {
    LabelUpdateContext *ctx = (LabelUpdateContext *)data;
    gtk_label_set_text(GTK_LABEL(ctx->label), ctx->text);
    g_free(ctx->text);
    g_free(ctx);
    return G_SOURCE_REMOVE;
}

gboolean queue_draw_wrapper(gpointer data) {
    gtk_widget_queue_draw(GTK_WIDGET(data));
    return G_SOURCE_REMOVE;
}

// =====================================================================
// GANTT MAGNIFIQUE – couleurs uniques et très lisibles
// =====================================================================
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
    (void)data;

    if (!g_sim_data.processes || g_sim_data.processes[0].finish_time == 0)
        return FALSE;

    int max_time = 0;
    for (int i = 0; i < g_sim_data.num_processes; i++)
        if (g_sim_data.processes[i].finish_time > max_time)
            max_time = g_sim_data.processes[i].finish_time;

    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    double width  = alloc.width;
    double height = alloc.height;

    double margin_left   = 140.0;
    double margin_top    = 100.0;
    double bar_height    = 42.0;
    double space_between = 18.0;

    double usable_width = width - margin_left - 40;
    double time_scale   = usable_width / (double)max_time;

    // Fond blanc
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // Axe principal
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 4.0);
    cairo_move_to(cr, margin_left, margin_top - 25);
    cairo_line_to(cr, width - 30, margin_top - 25);
    cairo_stroke(cr);

    // Graduations tous les 2 unités
    cairo_set_font_size(cr, 14.0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    for (int t = 0; t <= max_time; t += 2) {
        double x = margin_left + t * time_scale;
        cairo_set_source_rgb(cr, 0.35, 0.35, 0.35);
        cairo_move_to(cr, x, margin_top - 35);
        cairo_line_to(cr, x, margin_top - 10);
        cairo_stroke(cr);

        char buf[12];
        snprintf(buf, sizeof(buf), "%d", t);
        cairo_text_extents_t te;
        cairo_text_extents(cr, buf, &te);
        cairo_move_to(cr, x - te.width/2, margin_top - 45);
        cairo_show_text(cr, buf);
    }

    // Palette de couleurs magnifiques (une par processus)
    const double colors[10][3] = {
        {0.10, 0.55, 0.90},  // Bleu vif
        {0.90, 0.30, 0.10},  // Orange
        {0.20, 0.80, 0.40},  // Vert émeraude
        {0.75, 0.10, 0.75},  // Violet
        {1.00, 0.70, 0.00},  // Jaune or
        {0.00, 0.70, 0.80},  // Cyan
        {0.85, 0.20, 0.20},  // Rouge profond
        {0.50, 0.30, 0.90},  // Indigo
        {1.00, 0.50, 0.30},  // Corail
        {0.20, 0.60, 0.90}   // Bleu ciel
    };

    // Noms des processus
    cairo_set_font_size(cr, 16.0);
    for (int i = 0; i < g_sim_data.num_processes; i++) {
        double y = margin_top + i * (bar_height + space_between);
        cairo_set_source_rgb(cr, 0.05, 0.05, 0.05);
        cairo_move_to(cr, 15, y + bar_height/2 + 7);
        cairo_show_text(cr, g_sim_data.processes[i].name);
    }

    // Dessin des barres
    for (int i = 0; i < g_sim_data.num_processes; i++) {
        Process *p = &g_sim_data.processes[i];
        double y = margin_top + i * (bar_height + space_between);

        double r = colors[i % 10][0];
        double g = colors[i % 10][1];
        double b = colors[i % 10][2];

        for (int s = 0; s < p->num_slices; s++) {
            double x_start = margin_left + p->slices[s].start * time_scale;
            double w       = p->slices[s].duration * time_scale;

            double radius = bar_height / 2.1;

            // Forme arrondie
            cairo_move_to(cr, x_start, y + radius);
            cairo_arc(cr, x_start + radius, y + radius, radius, G_PI, 3*G_PI/2);
            cairo_arc(cr, x_start + w - radius, y + radius, radius, 3*G_PI/2, 0);
            cairo_arc(cr, x_start + w - radius, y + bar_height - radius, radius, 0, G_PI/2);
            cairo_arc(cr, x_start + radius, y + bar_height - radius, radius, G_PI/2, G_PI);
            cairo_close_path(cr);

            // Remplissage
            cairo_set_source_rgb(cr, r, g, b);
            cairo_fill_preserve(cr);

            // Bordure foncée
            cairo_set_source_rgb(cr, r*0.6, g*0.6, b*0.6);
            cairo_set_line_width(cr, 3.0);
            cairo_stroke(cr);

            // Texte blanc au centre si assez large
            if (w > 40) {
                char txt[32];
                snprintf(txt, sizeof(txt), "%d → %d",
                         p->slices[s].start,
                         p->slices[s].start + p->slices[s].duration);

                cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
                cairo_set_font_size(cr, 14.0);
                cairo_text_extents_t te;
                cairo_text_extents(cr, txt, &te);
                cairo_move_to(cr, x_start + w/2 - te.width/2, y + bar_height/2 + 6);
                cairo_show_text(cr, txt);
            }
        }
    }

    return FALSE;
}

// =====================================================================
// Thread + lancement GTK
// =====================================================================
void* run_scheduler_thread(void* data) {
    SimulationData *s = data;
    s->scheduler(s->processes, s->num_processes, s->quantum);

    int max_t = 0;
    for (int i = 0; i < s->num_processes; i++)
        if (s->processes[i].finish_time > max_t)
            max_t = s->processes[i].finish_time;

    gchar *msg = g_strdup_printf("Simulation terminée à t=%d", max_t);

    LabelUpdateContext *ctx = g_new(LabelUpdateContext, 1);
    ctx->label = s->status_label;
    ctx->text  = msg;

    g_idle_add(update_status_label_wrapper, ctx);
    g_idle_add(queue_draw_wrapper, s->drawing_area);

    display_results(s->processes, s->num_processes);
    return NULL;
}

void run_gui_simulation(Process processes[], int num_processes, SchedulerFunc scheduler, int quantum) {
    g_sim_data.processes     = processes;
    g_sim_data.num_processes = num_processes;
    g_sim_data.scheduler     = scheduler;
    g_sim_data.quantum       = quantum;

    gtk_init(NULL, NULL);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Ordonnanceur Multitâche – Gantt Professionnel");
    gtk_window_set_default_size(GTK_WINDOW(window), 1250, 720);
    gtk_container_set_border_width(GTK_CONTAINER(window), 25);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 25);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_sim_data.status_label = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(g_sim_data.status_label), "<span size='x-large' weight='bold' foreground='#2c3e50'>Simulation en cours…</span>");
    gtk_box_pack_start(GTK_BOX(vbox), g_sim_data.status_label, FALSE, FALSE, 0);

    g_sim_data.drawing_area = gtk_drawing_area_new();
    gtk_box_pack_start(GTK_BOX(vbox), g_sim_data.drawing_area, TRUE, TRUE, 0);
    g_signal_connect(g_sim_data.drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);

    pthread_t thread;
    pthread_create(&thread, NULL, run_scheduler_thread, &g_sim_data);

    gtk_widget_show_all(window);
    gtk_main();

    pthread_join(thread, NULL);
}