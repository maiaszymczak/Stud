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

#### **3. Pourquoi Debian Bullseye comme base ?**

Debian Bullseye est la version stable avant la dernière (Bookworm). C'est une distribution fiable, bien documentée, et compatible avec les exigences du projet.

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
