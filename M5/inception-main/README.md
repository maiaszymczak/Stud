# 🐳 Inception - Infrastructure Docker

> Projet d'infrastructure système avec Docker Compose - École 42

## 📋 Table des matières

- [Description du projet](#-description-du-projet)
- [Architecture](#-architecture)
- [Prérequis](#-prérequis)
- [Installation](#-installation)
- [Utilisation](#-utilisation)
- [Structure du projet](#-structure-du-projet)
- [Services détaillés](#-services-détaillés)
- [Configuration](#-configuration)
- [Commandes utiles](#-commandes-utiles)
- [Débogage](#-débogage)
- [Soutenance - Points clés](#-soutenance---points-clés)

---

## 🎯 Description du projet

**Inception** est un projet d'administration système qui consiste à créer une infrastructure complète avec Docker. L'objectif est de containeriser une stack LEMP (Linux, Nginx, MariaDB, PHP) avec WordPress, en respectant les bonnes pratiques DevOps.

### Objectifs pédagogiques

- Maîtriser Docker et Docker Compose
- Comprendre la containerisation et l'orchestration de services
- Configurer des services réseau (HTTPS, FastCGI, SQL)
- Gérer la persistance des données avec des volumes
- Sécuriser une infrastructure web

---

## 🏗 Architecture

```
┌─────────────────────────────────────────┐
│          Client (Browser)               │
└──────────────┬──────────────────────────┘
               │ HTTPS (443)
               ▼
┌──────────────────────────────────────────┐
│           NGINX (TLSv1.3)                │
│      Container: debian:bullseye          │
│    SSL Certificate auto-signé            │
└──────────────┬───────────────────────────┘
               │ FastCGI (9000)
               ▼
┌──────────────────────────────────────────┐
│      WordPress + PHP-FPM 7.4             │
│      Container: debian:bullseye          │
│         WP-CLI installé                  │
└──────────────┬───────────────────────────┘
               │ MySQL (3306)
               ▼
┌──────────────────────────────────────────┐
│          MariaDB 10.5                    │
│      Container: debian:bullseye          │
│      Base de données persistante         │
└──────────────────────────────────────────┘

Volumes persistants:
📁 ~/data/wordpress  ← Fichiers WordPress
📁 ~/data/mariadb    ← Base de données
```

### Réseau

- **Network**: `inception` (bridge)
- **Communication inter-containers**: via noms de services DNS
- **Exposition externe**: Port 443 (HTTPS uniquement)

---

## ✅ Prérequis

### Système

- **OS**: Linux (Debian/Ubuntu recommandé) ou WSL2
- **Docker**: version 20.10+
- **Docker Compose**: version 2.0+
- **Make**: pour utiliser le Makefile
- **Droits sudo**: pour la gestion des volumes

### Vérification

```bash
docker --version
docker compose version
make --version
```

---

## 🚀 Installation

### 1. Configuration des secrets

Créer les fichiers de secrets avec vos propres valeurs :

```bash
mkdir -p secrets
```

**secrets/db_password.txt**
```
votre_mot_de_passe_user_db
```

**secrets/db_root_password.txt**
```
votre_mot_de_passe_root_db
```

**secrets/credentials.txt**
```
DOMAIN_NAME=mszymcza.42.fr

# MariaDB
MYSQL_DATABASE=wordpress
MYSQL_USER=wpuser
MYSQL_PASSWD=votre_mot_de_passe_user_db

# WordPress Admin
ADMIN_USER=admin
ADMIN_PASSWD=admin_password
ADMIN_MAIL=admin@example.com

# WordPress Database
WORDPRESS_DB_NAME=wordpress
WORDPRESS_DB_USER=wpuser
WORDPRESS_DB_PASSWD=votre_mot_de_passe_user_db

# WordPress User
WP_USER=user
WP_USER_MAIL=user@example.com
WP_USER_PASSWD=user_password
```

### 2. Configuration du domaine

Ajouter votre domaine au fichier `/etc/hosts` :

```bash
sudo nano /etc/hosts
```

Ajouter la ligne :
```
127.0.0.1   mszymcza.42.fr
```

### 3. Lancement de l'infrastructure

```bash
make
```

Cette commande va :
1. Créer les répertoires de volumes (`~/data/wordpress` et `~/data/mariadb`)
2. Builder les images Docker personnalisées
3. Lancer tous les containers en arrière-plan

### 4. Vérification

Accéder à votre site :
```
https://mszymcza.42.fr
```

⚠️ Accepter le certificat auto-signé dans votre navigateur.

---

## 💻 Utilisation

### Commandes Make disponibles

| Commande | Description |
|----------|-------------|
| `make` ou `make all` | Lance toute l'infrastructure |
| `make down` | Arrête tous les containers |
| `make restart` | Redémarre l'infrastructure |
| `make logs` | Affiche les logs en temps réel |
| `make clean` | Arrête et supprime les volumes de données |
| `make prune` | Nettoyage complet (containers, images, volumes) |
| `make fclean` | Clean + prune |
| `make re` | Reconstruction complète (fclean + all) |

### Commandes Docker utiles

```bash
# Voir les containers actifs
docker ps

# Voir toutes les images
docker images

# Voir les volumes
docker volume ls

# Voir les réseaux
docker network ls

# Logs d'un service spécifique
docker compose -f srcs/docker-compose.yml logs nginx
docker compose -f srcs/docker-compose.yml logs wordpress
docker compose -f srcs/docker-compose.yml logs mariadb

# Entrer dans un container
docker exec -it <container_name> bash

# Redémarrer un service spécifique
docker compose -f srcs/docker-compose.yml restart nginx
```

---

## 📁 Structure du projet

```
inception/
│
├── Makefile                      # Automatisation des commandes
│
├── secrets/                      # Variables sensibles (gitignored)
│   ├── credentials.txt
│   ├── db_password.txt
│   └── db_root_password.txt
│
└── srcs/
    ├── docker-compose.yml        # Orchestration des services
    │
    └── requirements/
        │
        ├── nginx/                # Serveur web HTTPS
        │   ├── Dockerfile
        │   └── default.conf
        │
        ├── wordpress/            # CMS + PHP-FPM
        │   ├── Dockerfile
        │   ├── entrypoint.sh
        │   └── www.conf
        │
        └── mariadb/              # Base de données
            ├── Dockerfile
            ├── entrypoint.sh
            └── my.cnf
```

---

## 🔧 Services détaillés

### 1. NGINX

**Rôle**: Serveur web reverse proxy avec SSL/TLS

**Caractéristiques**:
- Image de base: `debian:bullseye`
- Port exposé: `443` (HTTPS uniquement)
- Protocole: TLSv1.3
- Certificat SSL auto-signé généré au build

**Configuration**:
- Reverse proxy vers WordPress (FastCGI)
- Gestion des fichiers statiques
- Redirection des requêtes PHP vers le container WordPress

**Dockerfile highlights**:
```dockerfile
FROM debian:bullseye
RUN apt update && apt install -y nginx openssl
RUN openssl req -x509 -nodes -days 365 \
    -newkey rsa:2048 \
    -keyout /etc/ssl/private/nginx.key \
    -out /etc/ssl/certs/nginx.crt \
    -subj "/C=FR/ST=Paris/L=Paris/O=42/OU=Inception/CN=mszymcza.42.fr"
```

---

### 2. WordPress + PHP-FPM

**Rôle**: CMS WordPress avec PHP FastCGI Process Manager

**Caractéristiques**:
- Image de base: `debian:bullseye`
- PHP version: 7.4
- Port interne: `9000` (FastCGI)
- WP-CLI installé pour l'automatisation

**Installation automatisée** (via entrypoint.sh):
1. Téléchargement de WordPress
2. Création de `wp-config.php`
3. Installation du core WordPress
4. Création de l'utilisateur admin
5. Création d'un utilisateur standard
6. Installation et activation d'un thème
7. Configuration HTTPS

**Dépendances**:
- Attend que MariaDB soit disponible
- Partage le volume `wordpress_data` avec NGINX

---

### 3. MariaDB

**Rôle**: Système de gestion de base de données

**Caractéristiques**:
- Image de base: `debian:bullseye`
- Port interne: `3306`
- Données persistées dans `/var/lib/mysql`

**Initialisation** (via entrypoint.sh):
1. Installation de la base de données
2. Création de la database WordPress
3. Création de l'utilisateur avec privilèges
4. Sécurisation des permissions

**Configuration**:
- Écoute sur toutes les interfaces (`0.0.0.0`)
- Accessible uniquement depuis le réseau Docker interne

---

## ⚙️ Configuration

### Variables d'environnement

Les variables sont chargées depuis `secrets/credentials.txt` via `env_file` dans le docker-compose.

**Variables obligatoires**:

| Variable | Description | Exemple |
|----------|-------------|---------|
| `DOMAIN_NAME` | Nom de domaine du site | `mszymcza.42.fr` |
| `MYSQL_DATABASE` | Nom de la base de données | `wordpress` |
| `MYSQL_USER` | Utilisateur MySQL | `wpuser` |
| `MYSQL_PASSWD` | Mot de passe MySQL | `secure_password` |
| `ADMIN_USER` | Admin WordPress | `admin` |
| `ADMIN_PASSWD` | Mot de passe admin | `admin_password` |
| `ADMIN_MAIL` | Email admin | `admin@example.com` |
| `WP_USER` | Utilisateur WP standard | `user` |
| `WP_USER_MAIL` | Email utilisateur | `user@example.com` |
| `WP_USER_PASSWD` | Mot de passe utilisateur | `user_password` |

### Volumes

Les volumes sont mappés sur le système hôte pour la persistance :

```yaml
volumes:
  wordpress_data:
    driver: local
    driver_opts:
      type: none
      o: bind
      device: ~/data/wordpress
  
  mariadb_data:
    driver: local
    driver_opts:
      type: none
      o: bind
      device: ~/data/mariadb
```

**Emplacement**: `~/data/wordpress` et `~/data/mariadb`

---

## 🛠 Commandes utiles

### Accès aux containers

```bash
# NGINX
docker exec -it <nginx_container_id> bash
nginx -t  # Tester la config
cat /var/log/nginx/error.log

# WordPress
docker exec -it <wordpress_container_id> bash
wp --info --allow-root
ls -la /var/www/html

# MariaDB
docker exec -it <mariadb_container_id> bash
mariadb -u wpuser -p
# Puis:
SHOW DATABASES;
USE wordpress;
SHOW TABLES;
```

### Tests de connexion

```bash
# Tester NGINX
curl -k https://mszymcza.42.fr

# Tester la connectivité entre containers
docker exec <wordpress_container_id> ping mariadb
docker exec <wordpress_container_id> nc -zv mariadb 3306
```

### Vérification des volumes

```bash
# Voir le contenu des volumes
ls -la ~/data/wordpress
ls -la ~/data/mariadb

# Taille des volumes
du -sh ~/data/wordpress
du -sh ~/data/mariadb
```

---

## 🐛 Débogage

### Problèmes courants

#### 1. Erreur de connexion à la base de données

**Symptômes**: WordPress affiche "Error establishing database connection"

**Solutions**:
```bash
# Vérifier que MariaDB est démarré
docker ps | grep mariadb

# Vérifier les logs MariaDB
docker compose -f srcs/docker-compose.yml logs mariadb

# Tester la connexion depuis WordPress
docker exec <wordpress_container_id> nc -zv mariadb 3306
```

#### 2. Certificat SSL refusé

**Symptômes**: Le navigateur bloque l'accès

**Solutions**:
- Accepter manuellement le certificat dans le navigateur
- Vérifier que le certificat est bien généré:
```bash
docker exec <nginx_container_id> ls -la /etc/ssl/certs/nginx.crt
docker exec <nginx_container_id> openssl x509 -in /etc/ssl/certs/nginx.crt -text -noout
```

#### 3. Permission denied sur les volumes

**Symptômes**: Erreurs de permissions dans les logs

**Solutions**:
```bash
# Donner les bonnes permissions
sudo chown -R $USER:$USER ~/data/wordpress
sudo chown -R $USER:$USER ~/data/mariadb
sudo chmod -R 755 ~/data
```

#### 4. Port 443 déjà utilisé

**Symptômes**: "bind: address already in use"

**Solutions**:
```bash
# Trouver le processus utilisant le port
sudo lsof -i :443
# ou
sudo netstat -tulpn | grep 443

# Arrêter le service conflictuel
sudo systemctl stop apache2  # exemple
```

### Logs détaillés

```bash
# Logs de tous les services
make logs

# Logs d'un service spécifique avec horodatage
docker compose -f srcs/docker-compose.yml logs -f --timestamps nginx
docker compose -f srcs/docker-compose.yml logs -f --timestamps wordpress
docker compose -f srcs/docker-compose.yml logs -f --timestamps mariadb

# Dernières 100 lignes
docker compose -f srcs/docker-compose.yml logs --tail=100
```

---

## 🎓 Soutenance - Points clés

### Questions attendues et réponses

#### **1. Qu'est-ce que Docker ?**

Docker est une plateforme de containerisation qui permet d'empaqueter une application et ses dépendances dans un container isolé et portable. Contrairement aux machines virtuelles, les containers partagent le kernel de l'hôte, ce qui les rend légers et rapides.

#### **2. Différence entre Docker et Docker Compose ?**

- **Docker**: Outil pour gérer des containers individuels
- **Docker Compose**: Outil d'orchestration pour gérer plusieurs containers simultanément avec un fichier de configuration YAML

#### **3. Pourquoi Debian Bookworm comme base ?**

Debian Bookworm est la version stable avant la dernière. C'est une distribution fiable, bien documentée, et compatible avec les exigences du projet.

#### **4. Comment fonctionne la communication entre containers ?**

Les containers communiquent via le réseau Docker bridge `inception`. Docker fournit un DNS interne qui résout les noms de services (ex: `wordpress`, `mariadb`) vers leurs adresses IP respectives.

#### **5. Pourquoi TLSv1.3 uniquement ?**

TLSv1.3 est la version la plus récente et sécurisée du protocole TLS. Elle élimine les algorithmes cryptographiques obsolètes et améliore les performances.

#### **6. Comment les données persistent-elles ?**

Les volumes Docker mappent des répertoires du container vers des répertoires de l'hôte (`~/data/wordpress` et `~/data/mariadb`). Même si les containers sont supprimés, les données restent sur l'hôte.

#### **7. Qu'est-ce que FastCGI ?**

FastCGI (Fast Common Gateway Interface) est un protocole permettant à un serveur web (NGINX) de communiquer avec un processeur PHP (PHP-FPM) de manière efficace et persistante.

#### **8. Rôle des entrypoints ?**

Les scripts d'entrypoint (`entrypoint.sh`) s'exécutent au démarrage du container. Ils permettent d'initialiser les services (création de DB, installation de WordPress, etc.) de manière automatique et idempotente.

#### **9. Pourquoi pas d'images Docker Hub toutes faites ?**

Le projet exige de construire ses propres images pour comprendre les processus d'installation, de configuration et de sécurisation des services. C'est un objectif pédagogique.

#### **10. Comment vérifier que tout fonctionne ?**

```bash
# 1. Vérifier les containers
docker ps
# Doit montrer 3 containers actifs: nginx, wordpress, mariadb

# 2. Vérifier les volumes
docker volume ls
# Doit montrer wordpress_data et mariadb_data

# 3. Tester l'accès web
curl -k https://mszymcza.42.fr
# Doit retourner du HTML

# 4. Accéder au site dans le navigateur
# https://mszymcza.42.fr
```

### Démonstration pour la soutenance

1. **Présenter l'architecture** (schéma ci-dessus)
2. **Montrer le code source** (Dockerfiles, docker-compose.yml)
3. **Lancer l'infrastructure**: `make`
4. **Vérifier les containers**: `docker ps`
5. **Montrer les logs**: `make logs`
6. **Accéder au site** dans le navigateur
7. **Se connecter à WordPress** (voir section ci-dessous)
8. **Entrer dans un container**: `docker exec -it <container> bash`
9. **Tester la persistance**: 
   - Créer un article WordPress
   - `make down`
   - `make`
   - Vérifier que l'article existe toujours
10. **Nettoyage**: `make fclean`

### 🔐 Accès administrateur WordPress

#### Connexion à l'interface d'administration

1. **Accéder à la page de login** :
   ```
   https://mszymcza.42.fr/wp-admin
   ```
   Ou directement :
   ```
   https://mszymcza.42.fr/wp-login.php
   ```

2. **Identifiants administrateur** :
   - **Nom d'utilisateur** : Valeur de `ADMIN_USER` dans `secrets/credentials.txt` (par défaut: `admin`)
   - **Mot de passe** : Valeur de `ADMIN_PASSWD` dans `secrets/credentials.txt`

3. **Identifiants utilisateur standard** :
   - **Nom d'utilisateur** : Valeur de `WP_USER` dans `secrets/credentials.txt` (par défaut: `user`)
   - **Mot de passe** : Valeur de `WP_USER_PASSWD` dans `secrets/credentials.txt`

#### Exemple de connexion

Si votre fichier `secrets/credentials.txt` contient :
```bash
ADMIN_USER=admin
ADMIN_PASSWD=admin123secure
WP_USER=john
WP_USER_PASSWD=userpass456
```

Alors :
- **Admin** : `admin` / `admin123secure`
- **User** : `john` / `userpass456`

#### Que faire après connexion ?

**En tant qu'administrateur**, vous pouvez :
- ✍️ Créer/modifier des articles et pages
- 🎨 Changer le thème (zigcy-lite est installé par défaut)
- 🔌 Installer des plugins
- 👥 Gérer les utilisateurs
- ⚙️ Modifier les paramètres du site
- 📊 Voir les statistiques

**Pour tester la persistance** :
1. Connectez-vous en admin
2. Allez dans **Articles** > **Ajouter**
3. Créez un article de test
4. Publiez-le
5. Arrêtez l'infrastructure : `make down`
6. Relancez : `make`
7. Reconnectez-vous → l'article doit toujours être là !

#### Récupérer les identifiants

Si vous avez oublié vos identifiants, consultez le fichier :
```bash
cat secrets/credentials.txt
```

Ou utilisez WP-CLI dans le container WordPress :
```bash
# Lister tous les utilisateurs
docker exec <wordpress_container_id> wp user list --allow-root

# Réinitialiser le mot de passe admin
docker exec <wordpress_container_id> wp user update admin --user_pass=nouveaumotdepasse --allow-root
```

### Checklist de validation

- [ ] 3 containers fonctionnent correctement
- [ ] NGINX écoute uniquement sur le port 443
- [ ] TLSv1.3 est configuré (vérifier avec `openssl s_client`)
- [ ] Les volumes persistent les données
- [ ] WordPress est accessible et fonctionnel
- [ ] Pas d'images Docker Hub (uniquement debian:bullseye)
- [ ] Les containers redémarrent automatiquement en cas de crash
- [ ] Les variables d'environnement sont sécurisées (pas en clair dans le code)
- [ ] Le domaine est accessible via le nom configuré
- [ ] Les logs sont accessibles

---

## 🗄️ Explication détaillée : Script MariaDB entrypoint.sh

### Script complet

```bash
#!/bin/bash

# create db
if [ ! -f "/var/lib/mysql/$MYSQL_DATABASE" ]; then 
    echo CREATING MARIADB
    mariadb-install-db --user=mysql --datadir=/var/lib/mysql
    /etc/init.d/mariadb start
    mariadb -e "CREATE DATABASE IF NOT EXISTS \`${MYSQL_DATABASE}\`;"
    mariadb -e "CREATE USER IF NOT EXISTS '${MYSQL_USER}'@'%' IDENTIFIED BY '${MYSQL_PASSWD}';"
    mariadb -e "GRANT ALL PRIVILEGES ON \`${MYSQL_DATABASE}\`.* TO '${MYSQL_USER}'@'%';"
    mariadb -e "FLUSH PRIVILEGES;"
    /etc/init.d/mariadb stop
fi
exec mariadbd --datadir=/var/lib/mysql
```

### 🔍 Analyse ligne par ligne

#### 1. Condition d'idempotence
```bash
if [ ! -f "/var/lib/mysql/$MYSQL_DATABASE" ]; then
```
- **Rôle** : Vérifie si la base de données existe déjà
- `-f` teste l'existence d'un fichier
- À la **première exécution** : le dossier n'existe pas → initialisation
- Aux **redémarrages** : le dossier existe → on saute l'initialisation
- **Évite de recréer la base et perdre les données à chaque redémarrage**

#### 2. Initialisation de MariaDB
```bash
mariadb-install-db --user=mysql --datadir=/var/lib/mysql
```
- Crée la structure de base de MariaDB (tables système, utilisateurs)
- `--user=mysql` : exécute avec l'utilisateur système `mysql`
- `--datadir=/var/lib/mysql` : où stocker les fichiers de la base

**Résultat** : Création de `/var/lib/mysql/` avec `mysql/`, `performance_schema/`, etc.

#### 3. Démarrage temporaire
```bash
/etc/init.d/mariadb start
```
- Démarre MariaDB en **mode temporaire** pour exécuter des commandes SQL
- **Nécessaire** car on a besoin d'un serveur actif pour créer la base et l'utilisateur

#### 4. Création de la base de données
```bash
mariadb -e "CREATE DATABASE IF NOT EXISTS \`${MYSQL_DATABASE}\`;"
```
- `-e` : exécute une commande SQL directement (sans interface interactive)
- `IF NOT EXISTS` : évite une erreur si elle existe déjà
- `${MYSQL_DATABASE}` : variable du `.env` (ex: `wordpress_db`)

**Équivalent manuel** :
```sql
mysql> CREATE DATABASE IF NOT EXISTS `wordpress_db`;
```

#### 5. Création de l'utilisateur
```bash
mariadb -e "CREATE USER IF NOT EXISTS '${MYSQL_USER}'@'%' IDENTIFIED BY '${MYSQL_PASSWD}';"
```
- `${MYSQL_USER}` : nom de l'utilisateur (ex: `wpuser`)
- **`@'%'`** : **CRITIQUE** - autorise les connexions depuis **n'importe quel hôte**
  - `%` = wildcard = tous les hôtes
  - **Nécessaire** car WordPress est dans un **autre conteneur Docker**
  - Sans ça : `Access denied for user 'wpuser'@'wordpress'`
- `IDENTIFIED BY` : définit le mot de passe

**Pourquoi `'%'` et pas `'localhost'` ?**

Dans Docker, les conteneurs communiquent via le réseau :
```
wordpress conteneur → mariadb conteneur
     (IP: 172.18.0.3)      (IP: 172.18.0.2)
```
WordPress se connecte **depuis une autre machine** → connexions distantes obligatoires !

#### 6. Attribution des privilèges
```bash
mariadb -e "GRANT ALL PRIVILEGES ON \`${MYSQL_DATABASE}\`.* TO '${MYSQL_USER}'@'%';"
```
- `GRANT ALL PRIVILEGES` : donne tous les droits (SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, etc.)
- `ON \`${MYSQL_DATABASE}\`.*` : sur **toutes les tables** (`.*`) de la base `wordpress_db`
- `TO '${MYSQL_USER}'@'%'` : à l'utilisateur `wpuser` depuis n'importe quel hôte

**Résultat** : L'utilisateur `wpuser` peut tout faire sur `wordpress_db`, mais **rien** sur les autres bases (sécurité).

#### 7. Application des changements
```bash
mariadb -e "FLUSH PRIVILEGES;"
```
- Force MariaDB à **recharger les tables de privilèges** depuis la base `mysql`
- Sans ça, les nouveaux droits ne sont pas pris en compte immédiatement
- **Bonne pratique** après tout `GRANT` ou `CREATE USER`

#### 8. Arrêt du serveur temporaire
```bash
/etc/init.d/mariadb stop
```
- On a démarré MariaDB **temporairement** pour la configuration
- Maintenant on va le relancer **proprement** avec `exec mariadbd`
- Évite d'avoir 2 processus MariaDB en conflit

#### 9. Démarrage définitif
```bash
exec mariadbd --datadir=/var/lib/mysql
```
**Décomposition** :
- `exec` : **remplace** le processus actuel (bash) par `mariadbd`
  - Le processus `mariadbd` devient le **PID 1** du conteneur
  - Quand `mariadbd` s'arrête → le conteneur s'arrête
  - Reçoit correctement les signaux Docker (SIGTERM, SIGKILL)
- `mariadbd` : démon MariaDB (version moderne de `mysqld`)
- `--datadir=/var/lib/mysql` : utilise les données qu'on vient de créer

**Sans `exec`** :
```
PID 1: bash (script)
  └── PID 42: mariadbd (processus enfant)
```
→ Docker envoie les signaux à bash, pas à mariadbd → **problèmes d'arrêt** !

**Avec `exec`** :
```
PID 1: mariadbd (remplace bash)
```
→ MariaDB reçoit directement les signaux → **arrêt propre** ✅

### 🔄 Flux d'exécution complet

#### Première exécution (base n'existe pas)
```
1. Script démarre
2. Vérifie /var/lib/mysql/wordpress_db → N'EXISTE PAS
3. ✅ Entre dans le if
4. Initialise la structure MariaDB
5. Démarre MariaDB temporairement
6. Crée la base wordpress_db
7. Crée l'utilisateur wpuser avec le mot de passe
8. Donne tous les droits à wpuser sur wordpress_db
9. Recharge les privilèges
10. Arrête MariaDB temporaire
11. Lance MariaDB définitivement
```

#### Redémarrage (base existe déjà)
```
1. Script démarre
2. Vérifie /var/lib/mysql/wordpress_db → EXISTE
3. ❌ Saute le if (ne pas recréer la base)
4. Lance MariaDB définitivement avec les données existantes
```

### 🔑 Variables d'environnement utilisées

Depuis le fichier `.env` :
```bash
MYSQL_DATABASE=wordpress_db      # Nom de la base à créer
MYSQL_USER=wpuser                # Nom de l'utilisateur
MYSQL_PASSWD=securepassword123   # Mot de passe de l'utilisateur
```

Ces variables sont automatiquement injectées dans le conteneur via :
```yaml
# docker-compose.yml
services:
  mariadb:
    env_file: .env  # ← Charge les variables
```

### 🧪 Test manuel

Pour vérifier que ça fonctionne :
```bash
# Se connecter au conteneur
docker exec -it inception-mariadb-1 bash

# Tester la connexion avec l'utilisateur créé
mysql -u wpuser -p
# Entrer le mot de passe du .env

# Vérifier les bases accessibles
SHOW DATABASES;
# Résultat :
# +--------------------+
# | Database           |
# +--------------------+
# | information_schema |
# | wordpress_db       |  ← Notre base
# +--------------------+

# Utiliser la base
USE wordpress_db;

# Vérifier les privilèges
SHOW GRANTS;
# GRANT ALL PRIVILEGES ON `wordpress_db`.* TO 'wpuser'@'%'

EXIT;
```

### 🚨 Erreurs courantes

#### "Can't connect to MySQL server"
- **Cause** : MariaDB n'écoute pas sur `0.0.0.0`
- **Solution** : Vérifier `my.cnf` :
  ```ini
  bind-address = 0.0.0.0  # Pas 127.0.0.1 !
  ```

#### "Access denied for user 'wpuser'@'wordpress'"
- **Cause** : L'utilisateur existe mais avec `@'localhost'` au lieu de `@'%'`
- **Solution** : Recréer l'utilisateur avec `@'%'`

#### Base recréée à chaque redémarrage
- **Cause** : Volume non monté ou mauvais chemin
- **Solution** : Vérifier `docker-compose.yml` :
  ```yaml
  volumes:
    - mariadb_data:/var/lib/mysql  # Doit être persistant !
  ```

### 🛡️ Sécurité

**Ce qui est sécurisé** ✅
- Mot de passe **jamais en clair** dans le code (variable `$MYSQL_PASSWD`)
- Utilisateur limité à **une seule base** (pas de droits sur `mysql`, `sys`, etc.)
- Pas de `root` utilisé pour WordPress

**Ce qui pourrait être amélioré** 🔶
- `@'%'` autorise **tous les hôtes** → en prod, préciser l'IP : `@'172.18.0.3'`
- Pas de backup automatique configuré

---

## 📝 Explication détaillée : Script WordPress entrypoint.sh

### Script complet

```bash
#!/bin/bash

# Création de wp-config.php si absent
if [ ! -f /var/www/html/wp-config.php ]; then
  cd /var/www/html 
  wp core download --allow-root --path="/var/www/html" 
  wp config create --allow-root \
          --dbname=$WORDPRESS_DB_NAME \
          --dbuser=$WORDPRESS_DB_USER  \
          --dbpass=$WORDPRESS_DB_PASSWD \
          --url=$DOMAIN_NAME  \
          --dbhost="mariadb" \
          --skip-check   \
          --path="/var/www/html" 
  wp core install --allow-root \
          --url=$DOMAIN_NAME  \
          --title=$DOMAIN_NAME  \
          --admin_user=$ADMIN_USER  \
          --admin_password=$ADMIN_PASSWD  \
          --admin_email=$ADMIN_MAIL  \
          --path="/var/www/html"
  wp user create --allow-root $WP_USER $WP_USER_MAIL --user_pass=$WP_USER_PASSWD --path="/var/www/html"
  wp theme install zigcy-lite  --allow-root --path="/var/www/html"
  wp theme activate zigcy-lite  --allow-root --path="/var/www/html"
  wp config set WP_HOME "https://$DOMAIN_NAME" --allow-root
  wp config set WP_SITEURL "https://$DOMAIN_NAME" --allow-root
  wp search-replace "http://$DOMAIN_NAME" "https://$DOMAIN_NAME" --all-tables --allow-root
  wp cache flush --allow-root
fi
chown -R www-data:www-data /var/www/html
mkdir -p /run/php
exec php-fpm7.4 -F
```

### 🔍 Analyse ligne par ligne

#### 1. Condition d'idempotence
```bash
if [ ! -f /var/www/html/wp-config.php ]; then
```
- **Rôle** : Vérifie si WordPress est déjà installé
- `-f` teste l'existence du fichier `wp-config.php`
- À la **première exécution** : fichier absent → installation complète
- Aux **redémarrages** : fichier présent → on saute l'installation
- **Évite de réinstaller WordPress et perdre le contenu**

#### 2. Changement de répertoire
```bash
cd /var/www/html
```
- Se place dans le répertoire web racine
- Requis pour que WP-CLI travaille au bon endroit

#### 3. Téléchargement de WordPress
```bash
wp core download --allow-root --path="/var/www/html"
```
- **`wp`** : WP-CLI, outil officiel WordPress en ligne de commande
- `core download` : télécharge les fichiers WordPress (dernière version stable)
- `--allow-root` : autorise l'exécution en tant que root (nécessaire dans Docker)
- `--path` : où installer les fichiers

**Résultat** : Télécharge `wp-admin/`, `wp-content/`, `wp-includes/`, `index.php`, etc.

#### 4. Création du fichier de configuration
```bash
wp config create --allow-root \
        --dbname=$WORDPRESS_DB_NAME \
        --dbuser=$WORDPRESS_DB_USER  \
        --dbpass=$WORDPRESS_DB_PASSWD \
        --url=$DOMAIN_NAME  \
        --dbhost="mariadb" \
        --skip-check   \
        --path="/var/www/html"
```
- `config create` : génère le fichier `wp-config.php`
- `--dbname` : nom de la base de données (ex: `wordpress_db`)
- `--dbuser` : utilisateur MySQL (ex: `wpuser`)
- `--dbpass` : mot de passe de la base
- `--url` : URL du site (ex: `mszymcza.42.fr`)
- **`--dbhost="mariadb"`** : **IMPORTANT** - nom du service Docker, pas `localhost` !
  - Docker résout automatiquement `mariadb` en adresse IP du conteneur
- `--skip-check` : ne teste pas la connexion maintenant (MariaDB peut ne pas être prêt)

**Équivalent manuel** :
```php
// wp-config.php
define('DB_NAME', 'wordpress_db');
define('DB_USER', 'wpuser');
define('DB_PASSWORD', 'securepassword123');
define('DB_HOST', 'mariadb');
```

#### 5. Installation de WordPress
```bash
wp core install --allow-root \
        --url=$DOMAIN_NAME  \
        --title=$DOMAIN_NAME  \
        --admin_user=$ADMIN_USER  \
        --admin_password=$ADMIN_PASSWD  \
        --admin_email=$ADMIN_MAIL  \
        --path="/var/www/html"
```
- `core install` : initialise la base de données et crée le compte admin
- `--url` : URL du site
- `--title` : titre du site WordPress
- `--admin_user` : nom d'utilisateur administrateur
- `--admin_password` : mot de passe admin
- `--admin_email` : email de l'admin

**Ce que ça fait** :
- Crée les tables WordPress dans la base (`wp_posts`, `wp_users`, etc.)
- Insère l'utilisateur administrateur
- Configure les paramètres de base

**Sans WP-CLI**, il faudrait :
1. Ouvrir le navigateur
2. Remplir le formulaire d'installation
3. Créer l'admin manuellement

#### 6. Création d'un utilisateur standard
```bash
wp user create --allow-root $WP_USER $WP_USER_MAIL --user_pass=$WP_USER_PASSWD --path="/var/www/html"
```
- `user create` : crée un nouvel utilisateur WordPress
- `$WP_USER` : nom d'utilisateur (ex: `normaluser`)
- `$WP_USER_MAIL` : email de l'utilisateur
- `--user_pass` : mot de passe de l'utilisateur

**Pourquoi ?** Le sujet impose de créer **au moins 2 utilisateurs** :
- 1 admin (créé par `core install`)
- 1 utilisateur standard (créé ici)

#### 7. Installation d'un thème
```bash
wp theme install zigcy-lite --allow-root --path="/var/www/html"
```
- `theme install` : télécharge un thème depuis le répertoire WordPress.org
- `zigcy-lite` : nom du thème (gratuit et léger)

#### 8. Activation du thème
```bash
wp theme activate zigcy-lite --allow-root --path="/var/www/html"
```
- `theme activate` : active le thème installé
- **Résultat** : Le site utilisera ce thème au lieu du thème par défaut

#### 9. Configuration HTTPS - WP_HOME
```bash
wp config set WP_HOME "https://$DOMAIN_NAME" --allow-root
```
- `config set` : modifie `wp-config.php`
- `WP_HOME` : URL d'accès au site
- **Force HTTPS** : `https://` au lieu de `http://`

**Pourquoi ?** Sans ça, WordPress génère des liens HTTP et provoque des erreurs mixtes content.

#### 10. Configuration HTTPS - WP_SITEURL
```bash
wp config set WP_SITEURL "https://$DOMAIN_NAME" --allow-root
```
- `WP_SITEURL` : URL des fichiers WordPress
- Généralement identique à `WP_HOME`

**Différence WP_HOME vs WP_SITEURL** :
- `WP_HOME` : URL du site pour les visiteurs
- `WP_SITEURL` : URL des fichiers WordPress (admin, assets)

#### 11. Remplacement HTTP → HTTPS dans la base
```bash
wp search-replace "http://$DOMAIN_NAME" "https://$DOMAIN_NAME" --all-tables --allow-root
```
- `search-replace` : cherche et remplace dans toutes les tables
- `--all-tables` : parcourt **toutes** les tables de la base
- **Rôle** : Convertit tous les liens HTTP en HTTPS

**Pourquoi ?** WordPress peut avoir créé des liens HTTP dans :
- Les articles
- Les pages
- Les options
- Les widgets

#### 12. Vidage du cache
```bash
wp cache flush --allow-root
```
- `cache flush` : vide tous les caches WordPress
- **Important** après les modifications pour qu'elles soient prises en compte

#### 13. Sortie du bloc if
```bash
fi
```
- Fin de la condition `if [ ! -f /var/www/html/wp-config.php ]`
- Le code suivant s'exécute **à chaque démarrage** (pas seulement la première fois)

#### 14. Attribution des permissions
```bash
chown -R www-data:www-data /var/www/html
```
- `chown -R` : change le propriétaire récursivement
- `www-data:www-data` : utilisateur et groupe utilisés par NGINX et PHP-FPM
- **Nécessaire** pour que PHP-FPM puisse lire/écrire les fichiers WordPress

**Sans ça** : Erreurs de permissions, impossible de téléverser des médias, installer des plugins, etc.

#### 15. Création du répertoire runtime PHP
```bash
mkdir -p /run/php
```
- Crée le dossier `/run/php` si absent
- `-p` : ne renvoie pas d'erreur si le dossier existe
- **Requis** pour que PHP-FPM puisse créer son socket Unix

#### 16. Démarrage de PHP-FPM
```bash
exec php-fpm7.4 -F
```
- `exec` : **remplace** le processus bash par PHP-FPM
  - PHP-FPM devient le **PID 1** du conteneur
  - Quand PHP-FPM s'arrête → le conteneur s'arrête
  - Reçoit correctement les signaux Docker
- `php-fpm7.4` : démon PHP FastCGI Process Manager
- `-F` : mode **foreground** (ne se met pas en arrière-plan)

**Sans `-F`** : PHP-FPM se lance en daemon et le conteneur s'arrête immédiatement.

### 🔄 Flux d'exécution complet

#### Première exécution (WordPress non installé)
```
1. Script démarre
2. Vérifie /var/www/html/wp-config.php → N'EXISTE PAS
3. ✅ Entre dans le if
4. Télécharge WordPress (fichiers core)
5. Crée wp-config.php avec les infos de connexion DB
6. Installe WordPress (crée les tables, admin)
7. Crée l'utilisateur standard
8. Installe le thème zigcy-lite
9. Active le thème
10. Configure HTTPS dans wp-config.php
11. Remplace tous les liens HTTP → HTTPS dans la DB
12. Vide le cache
13. Change les permissions des fichiers
14. Crée /run/php
15. Lance PHP-FPM définitivement
```

#### Redémarrage (WordPress déjà installé)
```
1. Script démarre
2. Vérifie /var/www/html/wp-config.php → EXISTE
3. ❌ Saute le if (ne pas réinstaller)
4. Change les permissions des fichiers
5. Crée /run/php
6. Lance PHP-FPM avec la config existante
```

### 🔑 Variables d'environnement utilisées

Depuis le fichier `.env` :
```bash
DOMAIN_NAME=mszymcza.42.fr

# Base de données WordPress
WORDPRESS_DB_NAME=wordpress_db
WORDPRESS_DB_USER=wpuser
WORDPRESS_DB_PASSWD=securepassword123

# Administrateur WordPress
ADMIN_USER=mszymcza
ADMIN_PASSWD=admin_password123
ADMIN_MAIL=mszymcza@student.42.fr

# Utilisateur standard WordPress
WP_USER=normaluser
WP_USER_MAIL=user@example.com
WP_USER_PASSWD=user_password123
```

Ces variables sont injectées via :
```yaml
# docker-compose.yml
services:
  wordpress:
    env_file: .env
```

### 🧪 Test manuel

Pour vérifier que ça fonctionne :
```bash
# Vérifier que WordPress est installé
docker exec inception-wordpress-1 ls -la /var/www/html/
# Doit contenir : wp-config.php, wp-admin/, wp-content/, etc.

# Vérifier les utilisateurs WordPress
docker exec inception-wordpress-1 wp user list --allow-root --path="/var/www/html"
# Doit afficher 2 utilisateurs : admin et normaluser

# Vérifier le thème actif
docker exec inception-wordpress-1 wp theme list --allow-root --path="/var/www/html"
# zigcy-lite doit être marqué "active"

# Vérifier les URLs HTTPS
docker exec inception-wordpress-1 wp option get home --allow-root --path="/var/www/html"
# Résultat : https://mszymcza.42.fr

docker exec inception-wordpress-1 wp option get siteurl --allow-root --path="/var/www/html"
# Résultat : https://mszymcza.42.fr
```

### 🚨 Erreurs courantes

#### "Error establishing a database connection"
- **Cause** : WordPress ne peut pas se connecter à MariaDB
- **Solutions** :
  - Vérifier que MariaDB est démarré : `docker ps`
  - Vérifier `--dbhost="mariadb"` (nom du service, pas IP)
  - Vérifier les credentials dans `.env`
  - Tester la connexion : `docker exec wordpress-1 nc -zv mariadb 3306`

#### "Sorry, you are not allowed to access this page"
- **Cause** : Problèmes de permissions
- **Solution** : Relancer `chown -R www-data:www-data /var/www/html`

#### WordPress se réinstalle à chaque redémarrage
- **Cause** : Volume non monté, `wp-config.php` disparaît
- **Solution** : Vérifier `docker-compose.yml` :
  ```yaml
  volumes:
    - wordpress_data:/var/www/html
  ```

#### Le thème par défaut est utilisé au lieu de zigcy-lite
- **Cause** : Le thème n'a pas été activé
- **Solution** : Activer manuellement :
  ```bash
  docker exec wordpress-1 wp theme activate zigcy-lite --allow-root
  ```

#### Erreur "mixed content" (HTTP/HTTPS)
- **Cause** : Certains liens sont encore en HTTP
- **Solution** : Relancer le search-replace :
  ```bash
  docker exec wordpress-1 wp search-replace "http://mszymcza.42.fr" "https://mszymcza.42.fr" --all-tables --allow-root
  ```

### 🛡️ Sécurité

**Ce qui est sécurisé** ✅
- Mots de passe **jamais en clair** dans le code (variables d'environnement)
- Utilisateur admin **sans le mot "admin"** dans le nom (requis par le sujet)
- HTTPS forcé partout
- Permissions correctes (`www-data`)

**Ce qui pourrait être amélioré** 🔶
- Désactiver l'éditeur de thèmes/plugins dans `wp-config.php`
- Limiter les tentatives de connexion (plugin de sécurité)
- Configurer les clés de sécurité WordPress (salt)
- Désactiver XML-RPC si non utilisé

### 📦 Pourquoi WP-CLI ?

**Avantages de WP-CLI** :
- ✅ **Automatisation complète** : pas d'interaction manuelle
- ✅ **Idempotent** : peut être relancé sans problème
- ✅ **Rapide** : installation en quelques secondes
- ✅ **Scriptable** : parfait pour Docker et CI/CD
- ✅ **Officiel** : maintenu par WordPress.org

**Sans WP-CLI**, il faudrait :
1. Copier manuellement les fichiers WordPress
2. Créer `wp-config.php` à la main
3. Ouvrir le navigateur pour l'installation
4. Remplir le formulaire
5. Se connecter pour créer le 2e utilisateur
6. Installer et activer le thème manuellement
7. Modifier les URLs en base de données avec SQL

→ **Impossible à automatiser proprement !**

---

## 📚 Ressources supplémentaires

### Documentation officielle

- [Docker Documentation](https://docs.docker.com/)
- [Docker Compose](https://docs.docker.com/compose/)
- [NGINX Documentation](https://nginx.org/en/docs/)
- [WordPress CLI](https://wp-cli.org/)
- [MariaDB Documentation](https://mariadb.org/documentation/)

### Commandes de test SSL/TLS

```bash
# Vérifier le protocole TLS
openssl s_client -connect mszymcza.42.fr:443 -tls1_3

# Voir les détails du certificat
openssl s_client -connect mszymcza.42.fr:443 -showcerts
```

---

## 📝 Notes importantes

### Sécurité

⚠️ **Ce projet est à des fins pédagogiques**. En production, il faudrait :
- Utiliser des certificats SSL valides (Let's Encrypt)
- Sécuriser davantage les mots de passe
- Mettre en place un firewall
- Utiliser des secrets Docker au lieu de fichiers env
- Configurer fail2ban
- Activer les mises à jour automatiques de sécurité

### Performance

Pour améliorer les performances en production :
- Activer le cache Redis pour WordPress
- Configurer le cache NGINX
- Optimiser les paramètres PHP-FPM
- Utiliser un CDN pour les assets statiques

---

## 👤 Auteur

**mszymcza** - Projet Inception - École 42

---

## 📄 Licence

Ce projet est réalisé dans le cadre du cursus de l'École 42.

---

**Bon courage pour la soutenance ! 🚀**
