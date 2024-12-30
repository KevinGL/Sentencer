Le sentencer permet de créer des phrases structurées à partir de mots-clés, voici comment l'utiliser :

1) Placer le contenu du repo dans un dossier "Sentencer" et ce dernier dans le répertoire de travail
2) Si vous voulez placer le répertoire ailleurs créer un fichier "relative_path.ini" dans le répertoire de travail et y ajouter :

```ini
[Paths]

SentencerPath=(Chemin relatif vers le sentencer)
```

3) Ajouter les fichiers source et d'en-tête à votre projet


Exemple :

```cpp
#include <iostream>
#include "Sentencer.h"

int main()
{
    Sent_sentence sentence;

    sentence.subject = L"lui";
    sentence.verb = L"allumer";
    sentence.tense = L"past tense";
    sentence.DOC = L"feu";

    std::wcout << sentencer.create(sentence) << std::endl;

    return 0;
}
```

Ceci donnera "Il a allumé le feu."

Si vous voulez vous pouvez basculer le COD en pronom :

```cpp
Sent_sentence sentence;

sentence.subject = L"lui";
sentence.verb = L"allumer";
sentence.tense = L"past tense";
sentence.DOC = L"feu";
sentence.adverbVerb = L"pas";
sentence.DOCPronoun = true;    //COD devient pronom

std::wcout << sentencer.create(sentence) << std::endl;
```

Ce qui donnera : "Il l'a allumé."

Il est aussi possible d'ajouter des adjectifs :

```cpp
Sent_sentence sentence;

sentence.subject = L"eau";
sentence.verb = L"Être";
sentence.tense = L"imperfect";
sentence.adjective = L"frais";
sentence.adverbVerb = L"vraiment pas";

std::wcout << sentencer.create(sentence) << std::endl;
```

Ce qui donnera : "L'eau n'était vraiment pas fraîche."

La fonction create() crée une phrase mais il est aussi possible d'assembler plusieurs phrases avec des connecteurs en utilisant la fonction assemble().

Voici les temps de conjugaison supportés :

- Présent ("present")
- Passé composé ("past tense")
- Imparfait ("imperfect")
- Futur ("future")
- Passé simple ("simple past")

Cette liste s'agrandira bien sûr

C'est loin d'être au point pour le moment, il manque encore énormément de données tels que des verbes, des noms irréguliers ...
