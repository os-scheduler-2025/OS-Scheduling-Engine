# Makefile – Version FINALE (compatible GTK3 + math.h + tout compile parfaitement)

CC = gcc

# Détection automatique des flags GTK+ 3.0
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS   = $(shell pkg-config --libs gtk+-3.0)

# Flags de compilation
CFLAGS = -Wall -Wextra -g -std=c99 -fPIC -pthread $(GTK_CFLAGS)

# Flags de linkage
LDFLAGS = $(GTK_LIBS)
DYN_LINK_FLAGS = -ldl -lpthread -lm     # -lm ajouté pour sin(), cos(), M_PI/G_PI

# Répertoires
SRCDIR  = src
INCLDIR = includes
POLDIR  = policies

# Fichiers sources principaux
SRC_FILES = main.c parser.c simulation.c utils.c gui.c
SRC       = $(addprefix $(SRCDIR)/, $(SRC_FILES))

# Bibliothèques dynamiques (.so)
POL_BASE_NAMES = fifo roundrobin priority mlq
POL_SO         = $(addprefix $(POLDIR)/, $(addsuffix .so, $(POL_BASE_NAMES)))

# Objets et exécutable
OBJECTS    = $(SRC:.c=.o)
EXECUTABLE = ordonnanceur

.PHONY: all clean install dynamic_libs

# Cible par défaut
all: $(EXECUTABLE) $(POL_SO)

# Liaison finale de l'exécutable (ordre très important !)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -rdynamic $^ $(LDFLAGS) $(DYN_LINK_FLAGS) -o $@

# Compilation des fichiers .c → .o
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLDIR) -c $< -o $@

# ----------------------------------------------------
# Création des bibliothèques partagées (.so)
# ----------------------------------------------------
$(POLDIR)/%.so: $(POLDIR)/%.c
	@echo "Construction de la politique $@"
	$(CC) $(CFLAGS) -I$(INCLDIR) -c $< -o $*.o
	$(CC) -shared $*.o -o $@
	rm -f $*.o

dynamic_libs: $(POL_SO)

# Nettoyage complet
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	rm -f $(POLDIR)/*.so $(POLDIR)/*.o

# Installation (root ou locale)
INSTALL_DIR = /usr/local/bin
install: $(EXECUTABLE) $(POL_SO)
	@echo "Installation dans $(INSTALL_DIR)..."
	@if [ -w $(INSTALL_DIR) ] && { \
		cp $(EXECUTABLE) $(INSTALL_DIR)/; \
		cp $(POL_SO) $(INSTALL_DIR)/; \
		echo "Installation réussie dans $(INSTALL_DIR)"; \
	} || { \
		echo "Pas de droits sur $(INSTALL_DIR) → installation locale"; \
		cp $(EXECUTABLE) .; \
		cp $(POL_SO) .; \
		echo "Fichiers copiés dans le répertoire courant"; \
	}

# Règle pratique pour recompiler rapidement après modif d'un header
re: clean all