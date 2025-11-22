# Makefile (Version finale avec Chargement Dynamique et -rdynamic)

CC = gcc
# Ajout de -fPIC pour créer des objets relogeables (nécessaire pour les .so)
CFLAGS = -Wall -Wextra -g -std=c99 -fPIC

# Flags de lien
LDFLAGS = 
# Librairie pour le chargement dynamique (dlopen, dlsym, dlclose)
DYN_LINK_FLAGS = -ldl

# Répertoires
SRCDIR = src
INCLDIR = includes
POLDIR = policies

# Fichiers sources à compiler (ne contient plus les politiques)
SRC_FILES = main.c parser.c simulation.c utils.c
SRC = $(addprefix $(SRCDIR)/, $(SRC_FILES))

# Fichiers politiques à transformer en .so
POL_BASE_NAMES = fifo roundrobin priority mlq
POL_SRC = $(addprefix $(POLDIR)/, $(addsuffix .c, $(POL_BASE_NAMES)))
POL_SO = $(addprefix $(POLDIR)/, $(addsuffix .so, $(POL_BASE_NAMES)))

# Fichiers objets de la source principale
OBJECTS = $(SRC:.c=.o)
EXECUTABLE = ordonnanceur

.PHONY: all clean install dynamic_libs

all: $(EXECUTABLE) $(POL_SO)

# Règle pour l'exécutable final
# AJOUT DE -rdynamic ici: Il expose les symboles de l'exécutable aux bibliothèques chargées par dlopen
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -rdynamic $^ -o $@ $(DYN_LINK_FLAGS)

# Règle générique pour les fichiers .o dans src/
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLDIR)

# ----------------------------------------------------
# RÈGLES POUR LES BIBLIOTHÈQUES PARTAGÉES (.so)
# ----------------------------------------------------

# Règle pour créer un .so à partir d'un .c
$(POLDIR)/%.so: $(POLDIR)/%.c
	@echo "Création de la bibliothèque partagée $@"
	# Compiler en objet position-independent (avec -fPIC)
	$(CC) $(CFLAGS) -c $< -o $*.o -I$(INCLDIR)
	# Créer la bibliothèque partagée (avec -shared)
	$(CC) -shared $*.o -o $@
	# Nettoyer le fichier objet intermédiaire
	rm -f $*.o

dynamic_libs: $(POL_SO)

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	rm -f $(POLDIR)/*.so
	rm -f $(POLDIR)/*.o
	
# Règle d'installation
INSTALL_DIR = /usr/local/bin
install: $(EXECUTABLE) $(POL_SO)
	@echo "Tentative d'installation dans $(INSTALL_DIR)..."
	@if test -w $(INSTALL_DIR); then \
		cp $(EXECUTABLE) $(INSTALL_DIR); \
		cp $(POL_SO) $(INSTALL_DIR); \
		echo "Installation reussie dans $(INSTALL_DIR) (Exécutable + .so)."; \
	else \
		echo "Avertissement: Vous n'avez pas les droits d'ecriture sur $(INSTALL_DIR)."; \
		echo "Installation locale dans le repertoire courant."; \
		cp $(EXECUTABLE) .; \
		cp $(POL_SO) .; \
	fi