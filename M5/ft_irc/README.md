Qu'est-ce que epoll ?
epoll est une API fournie par le noyau Linux pour surveiller plusieurs descripteurs de fichiers et gérer les événements d'entrée/sortie (E/S) de manière efficace. Il est conçu pour remplacer l'ancienne méthode de sélection par select et poll, offrant une meilleure performance, surtout avec un grand nombre de connexions simultanées.

Concepts Clés
1. Descripteurs de Fichiers (FD)
Un descripteur de fichier est un identifiant unique pour un fichier ouvert ou un socket. Dans le cas de la communication réseau, vous surveillez généralement des sockets pour détecter quand ils sont prêts pour lire ou écrire des données.

2. Événements
Les événements que vous pouvez surveiller avec epoll incluent :

EPOLLIN : le descripteur est prêt à lire.
EPOLLOUT : le descripteur est prêt à écrire.
EPOLLERR : une erreur est survenue sur le descripteur.
EPOLLHUP : la connexion a été interrompue.
3. epoll_create
Pour commencer à utiliser epoll, vous devez créer un descripteur epoll avec epoll_create ou epoll_create1, qui retourne un identifiant pour cet epoll.

4. epoll_ctl
Vous utilisez epoll_ctl pour ajouter, modifier ou supprimer des descripteurs de fichiers de l'instance epoll. Vous spécifiez le type d'opération (ajout, modification, ou suppression), le descripteur de fichier, et les événements à surveiller.

5. epoll_wait
epoll_wait est appelé pour attendre que l'un des descripteurs surveillés émette un événement. Il bloque et attend jusqu'à ce qu'un ou plusieurs événements se produisent, ou jusqu'à ce qu'un délai spécifié soit atteint.

Cycle de Vie d'epoll
Étapes pour utiliser epoll
Création de l'instance epoll
Créez une instance avec epoll_create1.
Ajout de descripteurs de fichiers
Utilisez epoll_ctl pour ajouter les descripteurs de fichiers à surveiller, comme des sockets de serveur.
Boucle d'attente des événements
Appel de epoll_wait dans une boucle pour surveiller les événements. Quand un événement se produit, vous lisez ou écrivez des données à partir des descripteurs correspondants.
Gestion des événements
Déterminez quel descripteur a déclenché l'événement et effectuez les actions nécessaires, comme accepter un nouveau client ou lire un message.
Nettoyage
Fermez les descripteurs et l'instance epoll lorsque vous n'en avez plus besoin.
