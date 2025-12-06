# 🚀 Ordonnanceur Multi-Tâche - Simulateur d'Ordonnancement de Processus Linux


---

## 🎯 Équipe Scrum
- 🏆 **Wiem Hamzaoui** — Scrum Master (SM)  
- 🎨 **Nour Chaker** — Product Owner (PO)  
- 🔍 **Mazen Khoualdi** — QA & Test Engineer  

---

## 👨‍💻 Équipe de Développement (DEV)
**Membres :** Mazen Khoualdi, Wiem Hamzaoui, Nour Chaker

---

## 👨‍🏫 Encadrement
Professeur de Systèmes d'exploitation — **Yousra Najar**

---

## 📋 Aperçu du Projet

### 📁 Structure du Projet
ordonnanceur/
        
        ├── src/ # Code source principal
        
        ├── policies/ # Implémentations des politiques d'ordonnancement
        
        ├── include/ # Fichiers d'en-tête
        
        ├── config/ # Fichiers de configuration
        
        ├── Makefile # Fichier de construction
        
        ├── Licence # Fichier de droits
        
        ├── README.md # Ce fichier


Ce projet simule l'ordonnancement multitâche de processus sous Linux en implémentant plusieurs politiques d'ordonnancement.  
Il vise à illustrer les concepts fondamentaux de l’ordonnancement tout en offrant une simulation réaliste et interactive.

---

## ✨ Caractéristiques principales

- ✨ Simulation textuelle avec diagrammes de Gantt  
- 🔧 Politiques d'ordonnancement : FIFO, Round-Robin, Priorité  
- 🎯 Interface graphique pour visualiser les simulations  
- 🏗️ Architecture modulaire (politiques dynamiques)  
- 📊 Métriques : temps d'attente moyen, temps de rotation  

---

## 🎯 Fonctionnalités

### ✅ Fonctionnalités Minimales

#### 🔄 Politiques d'Ordonnancement supportées
- 🎪 **FIFO** — Ordonnancement non préemptif basé sur l’ordre d’arrivée  
- 🔄 **Round-Robin** — Préemptif avec quantum configurable (défaut : 2)  
- ⚡ **Priorité Préemptive** — Basé sur des priorités statiques  

#### 🔧 Autres fonctionnalités
- 🔨 Makefile automatisé (génération des bibliothèques partagées)  
- 📝 Fichier de configuration flexible (lecture de `.txt`)  
- 📊 Affichage console des Gantt + métriques  

---

### 🚀 Fonctionnalités Avancées

- 🏗️ **Multi-Level Queue (MLQ)** statique + dynamique  
- 🔼 Mécanisme d’**Aging** contre la famine  
- ⚡ Préemption par file (Round-Robin interne)  
- 📈 Affichage graphique des résultats  
- 🖥️ IHM graphique complète  

---

## 🛠️ Installation & Guide d'Utilisation

### 📋 Prérequis

Le simulateur nécessite :
- 🐧 Linux natif ou WSL  
- 🔧 GCC ou Clang  
- 🛠️ make  
- 💻 Un terminal Linux  

---

## 📥 Téléchargement du Projet

### Option 1 — Avec Git 🐙

git clone https://github.com/os-scheduler-2025/OS-Scheduling-Engine
cd OS-Scheduling-Engine


### Option 2 — Sans Git 📦

*Téléchargez l'archive ZIP depuis GitHub/GitLab

*Extrayez le dossier dans votre espace utilisateur

Aucune installation système n'est requise. ✅ 

---



### 🔧 Compilation du Projet:


Depuis le dossier du projet, exécutez simplement :

make

Cette commande va :

✅ Compiler tous les fichiers source

📚 Compiler les politiques d'ordonnancement

🏗️ Générer l'exécutable ordonnanceur

Si make n'est pas installé :

    - Si vous avez les droits sudo :sudo apt install make
    
    - Sinon, utilisez WSL ou installez manuellement 

---

    
### 📝 Format du Fichier de Configuration:


Le fichier de configuration liste les processus à simuler.

Exemple (processes.txt) :
name arrival_time burst_time priority

Proc_A 0 12 1

Proc_B 1 6 3

Proc_C 2 8 2

Proc_D 4 3 4

Caractéristiques :

💬 Les lignes commençant par # sont des commentaires

📄 Les lignes vides sont ignorées

🔢 Le nombre de processus est libre 

---


### 🚀 Exécution du Simulateur:


Pour lancer le programme :

./ordonnanceur

Visualiser les résultats :

📊 Diagramme de Gantt dans le terminal

📈 Métriques de performance calculées automatiquement 

---



### 🗑️ Désinstallation:
Aucune désinstallation particulière n'est nécessaire. Il suffit simplement de supprimer le dossier :

rm -rf OS-Scheduling-Engine/ 

---


### 📄 Licence:

Ce projet est sous licence MIT.

Voir le fichier LICENSE pour plus de détails. 

---


# ⭐ N'hésitez pas à donner une étoile au projet si vous le trouvez utile ,cela nous motive énormément! ⭐
