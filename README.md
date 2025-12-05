🚀 Ordonnanceur Multi-Tâche - Simulateur d'Ordonnancement de Processus Linux


🎯 Équipe Scrum :
🏆 Wiem Hamzaoui - Scrum Master (SM)
🎨 Nour Chaker - Product Owner (PO)
🔍 Mazen Khoualdi - QA & Test Engineer

👨‍💻 Équipe de Développement (DEV)
Membres : Mazen Khoualdi, Wiem Hamzaoui, Nour Chaker

💻 Concevoir et implémenter les fonctionnalités selon les User Stories
🐛 Corriger les bugs et optimiser les performances
📚 Documenter, intégrer et maintenir le code pour un produit évolutif

👨‍🏫 Encadrement
Professeur de Systèmes d'exploitation - [Yousra Najar]

📋 Aperçu du Projet:

📁 Structure du Projet
ordonnanceur/
├── src/                 # Code source principal
├── policies/            # Implémentations des politiques d'ordonnancement
├── include/             # Fichiers d'en-tête
├── config/              # Fichiers de configuration
├── Makefile             # Fichier de construction
├── Licence              # Fichier de droits
├── README.md            # Ce fichier

Ce projet simule l'ordonnancement multitâche de processus sous un système d'exploitation Linux, en implémentant diverses politiques d'ordonnancement. Il vise à démontrer une compréhension approfondie des concepts d'ordonnancement, tout en intégrant des fonctionnalités avancées pour une simulation réaliste et interactive.
Caractéristiques principales :
✨ Simulation textuelle des résultats avec diagrammes de Gantt
🔧 Politiques d'ordonnancement variées (FIFO, Round-Robin, Priorité)
🎯 Interface graphique pour visualiser les simulations
🏗️ Architecture modulaire avec chargement dynamique des politiques
📊 Métriques de performance (temps d'attente moyen, temps de rotation)

🎯 Fonctionnalités:

✅ Fonctionnalités Minimales:

📊 Simulation Textuelle : Affichage en console des diagrammes de Gantt, temps d'attente moyen, temps de rotation moyen
🔄 Politiques d'Ordonnancement Supportées :

    🎪 FIFO (First-In-First-Out) - Ordonnancement non préemptif basé sur l'ordre d'arrivée

    🔄 Round-Robin - Ordonnancement préemptif avec quantum de temps configurable (défaut : 2 unités)

    ⚡ Priorité Préemptive - Basé sur des priorités statiques avec préemption

🔨 Makefile Automatisé : Compilation dynamique et génération de bibliothèques partagées

📝 Fichier de Configuration Flexible : Lecture de fichiers texte décrivant les processus

🚀 Fonctionnalités Avancées:

🏗️ Multi-Level Queue (MLQ) avec Priorité Statique et Dynamique (Aging)
🔼 Mécanisme d'Aging pour éviter la famine
⚡ Préemptif au sein des files avec Round-Robin par niveau
📈 Affichage Graphique des Résultats
🖥️ IHM Graphique Complète pour une visualisation interactive


🛠️ Installation et Guide d'Utilisation
📋 Prérequis
Le simulateur nécessite :

🐧 Un environnement Linux (natif ou WSL sous Windows)
🔧 Un compilateur GCC ou Clang
🛠️ L'utilitaire make
💻 Un terminal Linux fonctionnel

Note : Tous ces outils peuvent être utilisés en mode utilisateur, sans installation système.

📥 Téléchargement du Projet:

Téléchargez le projet dans un répertoire où vous avez les droits d'écriture :

Option 1 — Avec Git 🐙:

git clone https://github.com/os-scheduler-2025/OS-Scheduling-Engine
cd OS-Scheduling-Engine

Option 2 — Sans Git 📦

Téléchargez l'archive ZIP depuis GitHub/GitLab

Extrayez le dossier dans votre espace utilisateur

Aucune installation système n'est requise. ✅

🔧 Compilation du Projet:

Depuis le dossier du projet, exécutez simplement :

make
Cette commande va :
✅ Compiler tous les fichiers source
📚 Compiler les politiques d'ordonnancement
🏗️ Générer l'exécutable ordonnanceur

Si make n'est pas installé :
# Si vous avez les droits sudo :
sudo apt install make

# Sinon, utilisez WSL ou installez manuellement
📝 Format du Fichier de Configuration
Le fichier de configuration liste les processus à simuler.

Exemple (processes.txt) :
# name arrival_time burst_time priority
Proc_A 0 12 1
Proc_B 1 6 3
Proc_C 2 8 2
Proc_D 4 3 4
Caractéristiques :

💬 Les lignes commençant par # sont des commentaires

📄 Les lignes vides sont ignorées

🔢 Le nombre de processus est libre

🚀 Exécution du Simulateur
Pour lancer le programme :
./ordonnanceur
Visualiser les résultats :
📊 Diagramme de Gantt dans le terminal
📈 Métriques de performance calculées automatiquement


🗑️ Désinstallation:
Aucune désinstallation particulière n'est nécessaire. Il suffit simplement de supprimer le dossier :
rm -rf OS-Scheduling-Engine/

📄 Licence
Ce projet est sous licence MIT.
Voir le fichier LICENSE pour plus de détails.

⭐ N'hésitez pas à donner une étoile au projet si vous le trouvez utile ! ⭐
