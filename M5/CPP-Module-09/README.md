# CPP Module 09 - STL Containers

## Table des matières
- [Exercise 00: Bitcoin Exchange](#exercise-00-bitcoin-exchange)
- [Exercise 01: Reverse Polish Notation](#exercise-01-reverse-polish-notation)
- [Exercise 02: PmergeMe](#exercise-02-pmergeme)

---

## Exercise 00: Bitcoin Exchange

### 📋 Description
Programme qui calcule la valeur de bitcoins à une date donnée en utilisant une base de données historique.

### 🔧 Compilation
```bash
cd ex00
make
```

### 🚀 Utilisation
```bash
./btc input.txt
```

### 📦 Conteneur utilisé : `std::map<Date, double>`

**Pourquoi std::map ?**
- **Stockage ordonné** : Les dates sont automatiquement triées par ordre chronologique
- **Recherche efficace** : Complexité O(log n) pour trouver une date
- **lower_bound()** : Permet de trouver facilement la date la plus proche inférieure si la date exacte n'existe pas
- **Clé unique** : Chaque date n'apparaît qu'une fois, ce qui évite les doublons

### ✅ Gestion des erreurs
- ❌ Date invalide (format incorrect, dates impossibles comme 2001-42-42)
- ❌ Valeur négative
- ❌ Valeur > 1000
- ❌ Format de ligne invalide

### 📝 Exemple
```bash
./btc input.txt
# Output:
# 2011-01-03 => 3 = 0.9
# 2011-01-09 => 1 = 0.32
```

---

## Exercise 01: Reverse Polish Notation

### 📋 Description
Calculatrice en notation polonaise inversée (RPN) - les opérateurs suivent les opérandes.

### 🔧 Compilation
```bash
cd ex01
make
```

### 🚀 Utilisation
```bash
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
# Result: 42
```

### 📦 Conteneur utilisé : `std::stack<int>`

**Pourquoi std::stack ?**
- **Structure LIFO** (Last In First Out) : Parfait pour RPN où on empile les nombres et dépile pour les opérations
- **Push/Pop en O(1)** : Opérations très rapides
- **Simplicité** : Interface claire et adaptée au problème
- **Différent de ex00** : Utilise un conteneur différent (stack vs map)

### 🧮 Fonctionnement
1. **Nombres** : Empilés sur la pile
2. **Opérateur** (+, -, *, /) : Dépile 2 nombres, applique l'opération, empile le résultat
3. **Résultat final** : Le dernier élément de la pile

### ✅ Tests de validation
```bash
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
# 8*9=72, 72-9=63, 63-9=54, 54-9=45, 45-4=41, 41+1=42 ✓

./RPN "9 8 * 4 * 4 / 2 + 9 - 8 - 8 - 1 - 6 -"
# Result: 42 ✓

./RPN "1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 /"
# Result: 15 ✓
```

---

## Exercise 02: PmergeMe

### 📋 Description
Implémentation de l'algorithme de tri **Ford-Johnson** (merge-insert sort) avec comparaison de performances entre deux conteneurs STL.

### 🔧 Compilation
```bash
cd ex02
make
```

### 🚀 Utilisation
```bash
./PmergeMe 3 5 9 7 4
# Before: 3 5 9 7 4
# After: 3 4 5 7 9
# Time to process a range of 5 elements with std::vector : 7.00000 us
# Time to process a range of 5 elements with std::deque : 12.00000 us
```

### 📦 Conteneurs utilisés : `std::vector<int>` et `std::deque<int>`

**Pourquoi std::vector ?**
- **Mémoire contiguë** : Tous les éléments sont stockés côte à côte en mémoire
- **Cache-friendly** : Meilleure utilisation du cache CPU
- **Accès aléatoire en O(1)** : Parfait pour l'algorithme de tri
- **Performance** : Généralement le plus rapide pour ce type d'opérations

**Pourquoi std::deque ?**
- **Comparaison de performances** : Permet de mesurer la différence avec vector
- **Insertion en début/fin efficace** : O(1) aux deux extrémités
- **Mémoire non contiguë** : Stockage par blocs
- **Différent des exercices précédents** : Conteneurs non utilisés dans ex00/ex01

### 🎯 Algorithme Ford-Johnson (Merge-Insert Sort)

#### Principe général
L'algorithme Ford-Johnson minimise le nombre de comparaisons nécessaires pour trier en combinant :
1. **Merge sort** pour trier les paires
2. **Séquence de Jacobsthal** pour un ordre d'insertion optimal
3. **Insertion binaire** pour placer les éléments

#### Étapes détaillées

**1. Création des paires**
```cpp
[3, 5, 9, 7, 4] → [(5,3), (9,7), straggler=4]
```
- Groupe les éléments par paires
- Le plus grand de chaque paire est placé en premier
- Si nombre impair d'éléments, le dernier est mis de côté (straggler)

**2. Tri des paires avec merge sort**
```cpp
[(5,3), (9,7)] → merge_sort → [(5,3), (9,7)]
```
- Tri récursif basé sur le premier élément (le plus grand) de chaque paire
- Utilise la fonction `merge_sort()` qui divise et fusionne
- Complexité : O(n log n)

**3. Construction de la chaîne principale**
```cpp
main_chain = [3, 5, 9]  // Premier petit + tous les grands
```
- Prend le petit élément de la première paire
- Ajoute tous les grands éléments des paires triées

**4. Séquence de Jacobsthal**
```cpp
Jacobsthal: J(0)=0, J(1)=1, J(n)=J(n-1)+2*J(n-2)
Séquence: 0, 1, 1, 3, 5, 11, 21, 43, 85...
```
- Séquence mathématique qui minimise les comparaisons
- Définit l'ordre optimal d'insertion des petits éléments
- Implémentée dans `fill_jacobsthal_array()`

**5. Insertion binaire avec Jacobsthal**
```cpp
// Insertion dans l'ordre de Jacobsthal
final.insert(lower_bound(begin, end, value), value)
```
- Insère les petits éléments restants selon la séquence de Jacobsthal
- Utilise `std::lower_bound()` pour trouver la position en O(log n)
- Garantit un nombre minimal de comparaisons

**6. Gestion du straggler**
```cpp
if (stack.size() % 2 != 0)
    final.insert(lower_bound(...), straggler)
```
- Si nombre impair, insère le dernier élément mis de côté

#### Fonctions clés

```cpp
// Validation et parsing
template <typename T>
T fill_stack(int ac, char **av, T &stack)
// - Parse les arguments
// - Valide les nombres (positifs, pas de overflow)
// - Détecte les doublons

// Création et tri des paires
template <typename T, typename P>
P create_pairs(T &stack, P &pairs)
// - Crée les paires d'éléments
// - Trie chaque paire (grand, petit)

// Tri par merge sort
template <typename P>
void merge_sort(P &pairs, int left, int right)
// - Tri récursif des paires
// - Basé sur le premier élément (le plus grand)

// Séquence de Jacobsthal
template <typename T>
void fill_jacobsthal_array(T &jacobsthalArray)
// - Génère la séquence J(n) = J(n-1) + 2*J(n-2)

// Insertion optimale
template <typename T, typename P>
void jacobsthal_insert(T &jacobsthalArray, P &pairs, T &final)
// - Insère selon l'ordre de Jacobsthal
// - Utilise lower_bound pour insertion binaire
```

### ✅ Validation des entrées

- ❌ Nombres négatifs
- ❌ Doublons
- ❌ Caractères invalides
- ❌ Overflow (> INT_MAX)

### 📊 Tests de performance

```bash
# Test avec 5 éléments
./PmergeMe 3 5 9 7 4
# vector: ~7 µs, deque: ~12 µs

# Test avec 100 éléments
shuf -i 1-1000 -n 100 | tr '\n' ' ' | xargs ./PmergeMe
# vector: ~62 µs, deque: ~89 µs

# Test avec 3000 éléments (test de correction)
shuf -i 1-10000 -n 3000 | tr '\n' ' ' | xargs ./PmergeMe
# vector: ~1028 µs, deque: ~2108 µs
```

### 📈 Différence de performance : vector vs deque

**std::vector est environ 2x plus rapide**

**Raisons :**
1. **Localité mémoire** : Vector stocke tout en mémoire contiguë → meilleur cache hit
2. **Accès direct** : `vector[i]` accède directement à la mémoire
3. **Deque par blocs** : Deque stocke par blocs séparés → indirection supplémentaire
4. **Cache CPU** : Vector profite mieux du prefetching du processeur

**Quand deque est meilleur :**
- Insertions fréquentes au début/fin
- Pas besoin de mémoire contiguë
- Éviter les réallocations de vector

---

## 🎓 Points clés pour la soutenance

### Pourquoi ces conteneurs ?

| Exercice | Conteneur | Raison |
|----------|-----------|--------|
| ex00 | `std::map` | Recherche de dates, ordre automatique, lower_bound |
| ex01 | `std::stack` | Structure LIFO parfaite pour RPN |
| ex02 | `std::vector` + `std::deque` | Comparaison de performances, accès aléatoire |

### Complexités

- **ex00** : O(log n) par recherche (map)
- **ex01** : O(n) pour n opérations (stack)
- **ex02** : O(n log n) Ford-Johnson optimal

### Norme C++98

✅ Tous les exercices compilent avec `-std=c++98`
✅ Pas de features C++11 (auto, range-for, nullptr, etc.)
✅ Utilisation de `std::make_pair` au lieu de `{}`

---

## 🧪 Commandes de test rapides

```bash
# ex00
cd ex00 && make && ./btc input.txt

# ex01
cd ex01 && make
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
./RPN "9 8 * 4 * 4 / 2 + 9 - 8 - 8 - 1 - 6 -"
./RPN "1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 /"

# ex02
cd ex02 && make
./PmergeMe 3 5 9 7 4
shuf -i 1-10000 -n 3000 | tr '\n' ' ' | xargs ./PmergeMe
```

---

## 📚 Ressources

- [Ford-Johnson Algorithm](https://en.wikipedia.org/wiki/Merge-insertion_sort)
- [Jacobsthal Numbers](https://en.wikipedia.org/wiki/Jacobsthal_number)
- [STL Containers](https://cplusplus.com/reference/stl/)

---