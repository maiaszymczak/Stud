# Algorithme Ford-Johnson (Merge-Insertion Sort)

## Introduction

L'algorithme **Ford-Johnson**, également connu sous le nom de **merge-insertion sort**, est un algorithme de tri qui minimise le nombre de comparaisons nécessaires pour trier une séquence d'éléments. Il a été développé par Lester R. Ford Jr. et Selmer M. Johnson en 1959.

Cet algorithme combine intelligemment les techniques de **tri par fusion** (merge sort) et d'**insertion binaire** pour atteindre une complexité optimale en nombre de comparaisons.

## Principe général

L'algorithme Ford-Johnson fonctionne en plusieurs étapes clés :

1. **Groupement par paires** : Diviser la séquence en paires d'éléments
2. **Tri des paires** : Trier chaque paire individuellement
3. **Tri récursif** : Trier récursivement les plus grands éléments de chaque paire
4. **Insertion optimisée** : Insérer les plus petits éléments en utilisant la suite de Jacobsthal

## Étapes détaillées

### 1. Création des paires

On divise la séquence initiale en paires d'éléments :

```
Exemple: [5, 2, 8, 1, 9, 3, 7, 4]
Paires:  (5,2) (8,1) (9,3) (7,4)
```

### 2. Tri des paires

Pour chaque paire, on s'assure que le premier élément est plus grand que le second :

```
Après tri: (5,2) (8,1) (9,3) (7,4)
           ↓
          (5,2) (8,1) (9,3) (7,4)
          grand,petit
```

### 3. Tri récursif des plus grands éléments

On trie récursivement les plus grands éléments de chaque paire en utilisant un **tri par fusion** (merge sort) :

```
Grands éléments: [5, 8, 9, 7]
Après merge sort: [5, 7, 8, 9]
Avec leurs petits associés: (5,2) (7,4) (8,1) (9,3)
```

### 4. Construction de la séquence initiale

On crée une séquence `final` avec tous les plus grands éléments déjà triés :

```
final = [5, 7, 8, 9]
```

### 5. Insertion selon la suite de Jacobsthal

C'est l'étape la plus importante et innovante de l'algorithme.

#### La suite de Jacobsthal

La suite de Jacobsthal est définie par :
- $J_0 = 0$
- $J_1 = 1$  
- $J_n = J_{n-1} + 2 \times J_{n-2}$

Dans l'implémentation, on utilise une variante qui commence à 3 :
```
Suite: 3, 5, 11, 21, 43, 85, 171, ...
```

#### Pourquoi Jacobsthal ?

La suite de Jacobsthal détermine l'**ordre optimal d'insertion** des petits éléments pour minimiser le nombre de comparaisons. Elle garantit que lors de chaque insertion :
- On sait déjà où chercher (bornes connues)
- On minimise la profondeur de recherche binaire

#### Processus d'insertion

On insère les petits éléments dans l'ordre dicté par Jacobsthal :

```
Petits éléments associés: [2, 4, 1, 3]
Indices Jacobsthal: [3, 1, 0, 2] (en partant de l'index 0)

1. Insérer l'élément à l'index Jacobsthal[0] = 3 → insérer 3
   final = [3, 5, 7, 8, 9]

2. Insérer les éléments entre 0 et 3 → insérer 2, 4, 1
   final = [1, 2, 3, 4, 5, 7, 8, 9]
```

### 6. Gestion de l'élément impair

Si la séquence initiale a un nombre impair d'éléments, l'élément restant (qui n'a pas pu être mis en paire) est inséré à la fin en utilisant une recherche binaire.

## Complexité

- **Nombre de comparaisons** : Proche de $\log_2(n!)$ (optimal théorique)
- **Complexité temporelle** : $O(n \log n)$
- **Complexité spatiale** : $O(n)$

## Avantages

1. **Minimal en comparaisons** : Pour de petites séquences (n ≤ 20), c'est l'algorithme qui fait le moins de comparaisons
2. **Stable** : Préserve l'ordre relatif des éléments égaux
3. **Prédictible** : Le nombre de comparaisons est déterministe

## Inconvénients

1. **Complexe à implémenter** : Plus difficile que quicksort ou mergesort classique
2. **Overhead** : Pour de grandes séquences, l'overhead dépasse les gains en comparaisons
3. **Pas en place** : Nécessite de la mémoire supplémentaire

## Implémentation dans l'exercice

Dans notre implémentation C++, l'algorithme est utilisé avec deux conteneurs différents :
- `std::vector` : Accès rapide par index
- `std::deque` : Insertion et suppression efficaces aux extrémités

Les étapes clés dans le code :

1. **`create_pairs`** : Création et tri initial des paires
2. **`merge_sort`** : Tri récursif des paires par leurs plus grands éléments
3. **`final_stack`** : Construction de la séquence avec les grands éléments
4. **`fill_jacobsthal_array`** : Génération de la suite de Jacobsthal
5. **`jacobsthal_insert`** : Insertion optimale des petits éléments

## Exemple complet

```
Entrée: [5, 2, 8, 1, 9, 3, 7, 4, 6]

Étape 1 - Paires:
  (5,2) (8,1) (9,3) (7,4) + élément impair: 6

Étape 2 - Tri des paires:
  Déjà ok: (5,2) (8,1) (9,3) (7,4)

Étape 3 - Merge sort sur les grands:
  [5, 8, 9, 7] → [5, 7, 8, 9]
  Paires: (5,2) (7,4) (8,1) (9,3)

Étape 4 - Final initial:
  [5, 7, 8, 9]

Étape 5 - Insertion de l'impair:
  [5, 6, 7, 8, 9]

Étape 6 - Jacobsthal insertion:
  Petits: [2, 4, 1, 3]
  Suite Jacobsthal: [3, 1, 0, 2]
  
  - Insérer index 3 (valeur 3): [3, 5, 6, 7, 8, 9]
  - Insérer 0→3 (valeurs 2, 4, 1):
    * Insérer 4: [3, 4, 5, 6, 7, 8, 9]
    * Insérer 2: [2, 3, 4, 5, 6, 7, 8, 9]
    * Insérer 1: [1, 2, 3, 4, 5, 6, 7, 8, 9]

Résultat final: [1, 2, 3, 4, 5, 6, 7, 8, 9]
```

## Références

- Ford, L. R., & Johnson, S. M. (1959). "A Tournament Problem". *American Mathematical Monthly*, 66(5), 387-389.
- Knuth, D. E. (1998). *The Art of Computer Programming, Volume 3: Sorting and Searching* (2nd ed.).
