# 📚 Explication complète des 3 exercices pour la correction

## 🔍 **Ex00 : easyfind**

### Concept
Créer une fonction template qui trouve un entier dans un conteneur STL.

### Code clé
```cpp
template <typename T>
typename T::iterator easyfind(T &container, int value)
{
    typename T::iterator it = std::find(container.begin(), container.end(), value);
    if (it == container.end())
        throw std::invalid_argument("argument not found");
    return it;
}
```

### Algorithme STL utilisé : `std::find`
- **Rôle** : Cherche une valeur dans un range [begin, end)
- **Retour** : Itérateur vers l'élément trouvé, ou `end()` si non trouvé
- **Header** : `<algorithm>`

### Questions potentielles à la correction
**Q: Pourquoi utiliser un template ?**
- Pour fonctionner avec tous les conteneurs STL (vector, list, deque, set, etc.)

**Q: Pourquoi retourner un itérateur ?**
- Permet de connaître la position de l'élément dans le conteneur
- Plus flexible qu'un simple bool

**Q: Pourquoi `typename T::iterator` ?**
- `typename` est nécessaire car le compilateur ne sait pas encore que `T::iterator` est un type (dépend du template)

---

## 📊 **Ex01 : Span**

### Concept
Classe qui stocke N entiers et peut calculer le plus petit et le plus grand "span" (écart).

### Attributs privés
```cpp
std::vector<int> _tab;      // Stocke les nombres
unsigned int _size;         // Capacité maximale
```

### Algorithmes STL utilisés

#### 1️⃣ **`shortestSpan()`** - Le plus petit écart

```cpp
int Span::shortestSpan() {
  std::vector<int> sorted(_tab);
  std::sort(sorted.begin(), sorted.end());
  
  std::vector<int> differences(sorted.size() - 1);
  std::adjacent_difference(sorted.begin(), sorted.end(), differences.begin());
  
  int shortest = *std::min_element(differences.begin() + 1, differences.end());
  return shortest;
}
```

**Algorithmes :**
- **`std::sort`** : Trie le vecteur en ordre croissant
- **`std::adjacent_difference`** : Calcule les différences entre éléments consécutifs
  - Exemple : `[3, 6, 9, 11, 17]` → `[3, 3, 3, 2, 6]` (premier élément = lui-même, puis différences)
- **`std::min_element`** : Trouve le minimum (on skip le 1er élément avec `+ 1`)

**Pourquoi cette approche ?**
- ❌ **FAUX** : Soustraire les 2 plus petits nombres (`sorted[1] - sorted[0]`)
- ✅ **CORRECT** : Vérifier TOUTES les différences consécutives
- Exemple : `[1, 5, 100, 101]` → shortest = 1 (entre 100 et 101), pas 4 (entre 1 et 5)

#### 2️⃣ **`longestSpan()`** - Le plus grand écart

```cpp
int Span::longestSpan() {
  int min = *std::min_element(_tab.begin(), _tab.end());
  int max = *std::max_element(_tab.begin(), _tab.end());
  return max - min;
}
```

**Algorithmes :**
- **`std::min_element`** : Trouve l'itérateur vers le plus petit élément
- **`std::max_element`** : Trouve l'itérateur vers le plus grand élément

**Pourquoi cette approche ?**
- Plus efficace que trier (O(n) vs O(n log n))
- Ne modifie pas `_tab` (contrairement à `std::sort`)

#### 3️⃣ **`addManyNumbers()`** - Ajouter plusieurs nombres

```cpp
void Span::addManyNumbers(std::vector<int> &vec) {
  if (_size - _tab.size() < vec.size())
    throw std::invalid_argument("not enough space");
  _tab.insert(_tab.end(), vec.begin(), vec.end());
}
```

**Algorithme :**
- **`std::vector::insert`** : Insère un range d'éléments à la fin
- Beaucoup plus efficace que d'appeler `addNumber()` en boucle

### Questions potentielles

**Q: Pourquoi créer une copie dans `shortestSpan()` ?**
- Pour ne pas modifier `_tab` original avec `std::sort`
- Permet d'appeler `shortestSpan()` plusieurs fois avec le même résultat

**Q: Que fait `std::adjacent_difference` exactement ?**
- Calcule `result[i] = input[i] - input[i-1]` (sauf pour i=0 où `result[0] = input[0]`)
- C'est pourquoi on commence à `differences.begin() + 1` dans `min_element`

**Q: Pourquoi pas juste `sorted[1] - sorted[0]` pour shortest ?**
- Contre-exemple : `[1, 100, 101]`
  - `sorted[1] - sorted[0]` = 99
  - Mais le vrai shortest = 1 (entre 100 et 101)

---

## 🧬 **Ex02 : MutantStack**

### Concept
Créer une stack qui a des itérateurs (normalement std::stack n'en a pas).

### Héritage
```cpp
template <typename T>
class MutantStack : public std::stack<T>
```

### Le secret : `std::stack::c`

`std::stack` est un **adaptateur de conteneur**. Il utilise un conteneur sous-jacent (par défaut `std::deque`) accessible via le membre **protégé** `c`.

### Implémentation des itérateurs

```cpp
typedef typename std::deque<T>::iterator iterator;

iterator begin() { return this->c.begin(); }
iterator end()   { return this->c.end(); }
```

**Pourquoi `this->c` ?**
- `c` est le conteneur sous-jacent de `std::stack`
- Il est **protégé**, donc accessible depuis la classe dérivée
- `c` est un `std::deque<T>` par défaut, qui possède des itérateurs

### Types d'itérateurs implémentés

```cpp
iterator              // Itérateur normal
const_iterator        // Itérateur constant (lecture seule)
reverse_iterator      // Itérateur inverse
const_reverse_iterator // Itérateur inverse constant
```

### Forme canonique orthodoxe

```cpp
MutantStack()                                    // Constructeur par défaut
MutantStack(const MutantStack &other)           // Constructeur de copie
MutantStack &operator=(const MutantStack &other) // Opérateur d'assignation
~MutantStack()                                   // Destructeur
```

### Questions potentielles

**Q: Qu'est-ce qu'un adaptateur de conteneur ?**
- Une classe qui "enveloppe" un autre conteneur pour changer son interface
- `std::stack`, `std::queue`, `std::priority_queue` sont des adaptateurs
- Ils utilisent `std::deque` ou `std::vector` en interne

**Q: Pourquoi `std::stack` n'a pas d'itérateurs par défaut ?**
- Une stack est censée respecter LIFO (Last In First Out)
- Donner accès aux éléments du milieu viole ce principe
- `MutantStack` est une "mutation" qui brise cette règle

**Q: Pourquoi utiliser `typename` devant `std::deque<T>::iterator` ?**
- Car `std::deque<T>` dépend du template `T`
- Le compilateur a besoin du mot-clé `typename` pour savoir que `iterator` est un type

**Q: Comment tester que ça fonctionne ?**
- Comparer avec `std::list` qui a les mêmes opérations
- Les deux doivent produire le même résultat

---

## 🎯 **Points clés pour la correction**

### Ex00
✅ Utilise `std::find` (pas de boucle manuelle)
✅ Fonctionne avec différents conteneurs

### Ex01
✅ `shortestSpan()` utilise STL algorithms (`std::adjacent_difference`, `std::min_element`)
✅ `longestSpan()` utilise `std::min_element` et `std::max_element`
✅ `addManyNumbers()` pour éviter les appels répétés
✅ Ne modifie pas `_tab` dans les fonctions de calcul

### Ex02
✅ Hérite de `std::stack`
✅ Utilise `this->c` pour accéder au conteneur sous-jacent
✅ Itérateurs normal, const, reverse implémentés
✅ Forme canonique orthodoxe respectée

---

## 📝 **Exemple de réponses courtes pour la correction**

**"Pourquoi utilisez-vous std::adjacent_difference ?"**
→ "Pour calculer automatiquement les différences entre tous les éléments consécutifs du tableau trié, ce qui me permet ensuite de trouver le minimum avec std::min_element."

**"Pourquoi MutantStack hérite de std::stack ?"**
→ "Pour garder toutes les fonctionnalités de std::stack (push, pop, top, etc.) tout en ajoutant des itérateurs via l'accès au conteneur sous-jacent 'c'."

**"Qu'est-ce que std::find retourne ?"**
→ "Un itérateur vers l'élément trouvé, ou end() si l'élément n'existe pas dans le conteneur."

Bon courage pour ta correction ! 🚀
