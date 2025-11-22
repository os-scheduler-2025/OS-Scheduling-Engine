# Makefile (Version CORRIGÉE pour l'ordre de liaison GTK)

CC = gcc
# Récupération automatique des drapeaux GTK
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)

# CFLAGS inclut -fPIC, -pthread et les drapeaux GTK
CFLAGS = -Wall -Wextra -g -std=c99 -fPIC -pthread $(GTK_CFLAGS)

# Flags de lien
# Les librairies GTK sont incluses ici.
LDFLAGS = $(GTK_LIBS) 
# Librairie pour le chargement dynamique (dlopen) et multithreading (pthread)
DYN_LINK_FLAGS = -ldl -lpthread

# Répertoires
SRCDIR = src
INCLDIR = includes
POLDIR = policies

# Fichiers sources à compiler (avec gui.c)
SRC_FILES = main.c parser.c simulation.c utils.c gui.c
SRC = $(addprefix $(SRCDIR)/, $(SRC_FILES))

# Fichiers politiques à transformer en .so
POL_BASE_NAMES = fifo roundrobin priority mlq
POL_SO = $(addprefix $(POLDIR)/, $(addsuffix .so, $(POL_BASE_NAMES)))

# Fichiers objets de la source principale
OBJECTS = $(SRC:.c=.o)
EXECUTABLE = ordonnanceur

.PHONY: all clean install dynamic_libs

all: $(EXECUTABLE) $(POL_SO)

# Règle pour l'exécutable final
# L'ordre est crucial: OBJETS -> -rdynamic -> LDFLAGS (GTK_LIBS) -> DYN_LINK_FLAGS (-ldl -lpthread)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -rdynamic $^ $(LDFLAGS) $(DYN_LINK_FLAGS) -o $@

# Règle générique pour les fichiers .o dans src/
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLDIR)

# ----------------------------------------------------
# RÈGLES POUR LES BIBLIOTHÈQUES PARTAGÉES (.so)
# ----------------------------------------------------

# Règle pour créer un .so à partir d'un .c
$(POLDIR)/%.so: $(POLDIR)/%.c
	@echo "Création de la bibliothèque partagée $@"
	$(CC) $(CFLAGS) -c $< -o $*.o -I$(INCLDIR)
	$(CC) -shared $*.o -o $@
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