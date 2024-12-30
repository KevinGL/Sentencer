#include "Sentencer.h"

Sentencer::Sentencer()
{
    std::wifstream file("../Mini-libs/Sentencer/Conjug.json");

    if(!file.is_open())
    {
        std::cout << "\"../Mini-libs/Sentencer/Conjug.json\" doest not exist" << std::endl;
        exit(-1);
    }

    std::wstring name = L"";
    std::wstring tense = L"";
    bool inTense = false;

    while(1)
    {
        std::wstring line;

        if(!getline(file, line))
        {
            break;
        }

        if(line.find(L"\"verb\"") != std::string::npos)
        {
            name = Sent_readValueJSONline(line);
        }

        else
        if(line.find(L"\"tenses\"") != std::string::npos)
        {
            inTense = true;
        }

        else
        if(line.find(L"}") != std::string::npos && inTense)
        {
            inTense = false;
        }

        else
        if(line.find(L"[") != std::string::npos && inTense)
        {
            tense = Sent_readKeyJSONline(line);
        }

        else
        if(line.find(L":") == std::string::npos && line.find(L"\"") != std::string::npos && inTense)
        {
            std::wstring conjug = line;

            conjug.erase(0, conjug.find(L"\"") + 1);
            conjug.erase(conjug.rfind(L"\""));

            verbs[name][tense].push_back(conjug);
        }
    }

    file.close();

    /*for(const auto& kv1 : verbs)
    {
        std::wcout << kv1.first << L" :" << std::endl;

        for(const auto& kv2 : kv1.second)
        {
            std::wcout << L"    " << kv2.first << L" :" << std::endl;

            for(const std::wstring conjug : kv2.second)
            {
                std::wcout << L"        " << conjug << std::endl;
            }

            getch();
        }
    }*/

    ///////////////////////////////////////////////////////////////////////////////////////////

    file.open("../Mini-libs/Sentencer/Exceptions_names_gender.json");

    if(!file.is_open())
    {
        std::cout << "\"../Mini-libs/Sentencer/Exceptions_names_gender.json\" doest not exist" << std::endl;
        exit(-1);
    }

    std::wstring key = L"";
    std::wstring keyPlural = L"";

    while(1)
    {
        std::wstring line;

        if(!getline(file, line))
        {
            break;
        }

        if(line.find(L"\"name\"") != std::string::npos)
        {
            key = Sent_readValueJSONline(line);
        }

        else
        if(line.find(L"\"plural\"") != std::string::npos)
        {
            keyPlural = Sent_readValueJSONline(line);
        }

        else
        if(line.find(L"\"gender\"") != std::string::npos)
        {
            gendersExceptions[key] = Sent_readValueJSONline(line);
            gendersExceptions[keyPlural] = Sent_readValueJSONline(line);
        }
    }

    file.close();

    /*for(const auto& kv : gendersExceptions)
    {
        std::wcout << kv.first << L" => " << kv.second << std::endl;
    }*/

    ///////////////////////////////////////////////////////////////////////////////////////////

    file.open("../Mini-libs/Sentencer/Exceptions_adjectives.json");

    if(!file.is_open())
    {
        std::cout << "\"../Mini-libs/Sentencer/Exceptions_adjectives.json\" doest not exist" << std::endl;
        exit(-1);
    }

    key = L"";
    std::map<std::wstring, std::wstring> adjective;

    while(1)
    {
        std::wstring line;

        if(!getline(file, line))
        {
            break;
        }

        if(line.find(L"\"ms\"") != std::wstring::npos)
        {
            key = Sent_readValueJSONline(line);
            adjective[L"ms"] = Sent_readValueJSONline(line);
        }

        else
        if(line.find(L"\"mp\"") != std::wstring::npos)
        {
            adjective[L"mp"] = Sent_readValueJSONline(line);
        }

        else
        if(line.find(L"\"fs\"") != std::wstring::npos)
        {
            adjective[L"fs"] = Sent_readValueJSONline(line);
        }

        else
        if(line.find(L"\"fp\"") != std::wstring::npos)
        {
            adjective[L"fp"] = Sent_readValueJSONline(line);
        }

        else
        if(line.find(L"}") != std::wstring::npos)
        {
            adjectivesExceptions[key] = adjective;
        }
    }

    file.close();

    /*for(const auto& kv1 : adjectivesExceptions)
    {
        std::wcout << kv1.first << L" :" << std::endl;

        for(const auto& kv2 : kv1.second)
        {
            std::wcout << L"   " << kv2.first << L": " << kv2.second << std::endl;
        }
    }*/

    ///////////////////////////////////////////////////////////////////////////////////////////

    file.open("../Mini-libs/Sentencer/Exceptions_verbs.json");

    if(!file.is_open())
    {
        std::cout << "\"../Mini-libs/Sentencer/Exceptions_verbs.json\" doest not exist" << std::endl;
        exit(-1);
    }

    while(1)
    {
        std::wstring line;

        if(!getline(file, line))
        {
            break;
        }

        if(line.find(L"\"") != std::string::npos)
        {
            std::wstring verb = line;

            verb.erase(0, verb.find(L"\"") + 1);
            verb.erase(verb.rfind(L"\""));

            verbsExceptions.push_back(Sent_tolower(verb));
        }
    }

    file.close();

    /*for(const std::wstring v : verbsExceptions)
    {
        std::wcout << v << std::endl;
    }*/

    ///////////////////////////////////////////////////////////////////////////////////////////

    file.open("../Mini-libs/Sentencer/Adverbs_negations.json");

    if(!file.is_open())
    {
        std::cout << "\"../Mini-libs/Sentencer/Adverbs_negations.json\" doest not exist" << std::endl;
        exit(-1);
    }

    while(1)
    {
        std::wstring line;

        if(!getline(file, line))
        {
            break;
        }

        if(line.find(L"\"") != std::string::npos)
        {
            std::wstring adverb = line;

            adverb.erase(0, adverb.find(L"\"") + 1);
            adverb.erase(adverb.rfind(L"\""));

            adverbsNegations.push_back(Sent_tolower(adverb));
        }
    }

    file.close();

    /*for(const std::wstring a : adverbsNegations)
    {
        std::wcout << a << std::endl;
    }*/
}

std::wstring Sentencer::create(Sent_sentence sent, const bool noUp, const bool noPoint)
{
    std::wstring verb = Sent_toupperInit(sent.verb);

    if(verbs.count(verb) == 0)
    {
        return L"Error : Verb \"" + verb + L"\" does not exist";
    }

    if(verbs[verb].count(sent.tense) == 0)
    {
        return L"Error : Tense \"" + sent.tense + L"\" does not exist";
    }

    const std::wstring baseSubject = sent.subject;

    addDeterminant(sent.subject);

    size_t person;

    if(Sent_tolower(sent.subject) == L"moi")
    {
        person = 0;
        sent.subject = L"je";
    }

    else
    if(Sent_tolower(sent.subject) == L"toi")
    {
        person = 1;
        sent.subject = L"tu";
    }

    else
    if(Sent_tolower(sent.subject) == L"lui")
    {
        person = 2;
        sent.subject = L"il";
    }

    else
    if(Sent_tolower(sent.subject) == L"ça")
    {
        person = 2;
        sent.subject = L"ce";
    }

    else
    if(Sent_tolower(sent.subject) == L"elle")
    {
        person = 2;
    }

    else
    if(Sent_tolower(sent.subject) == L"nous")
    {
        person = 3;
    }

    else
    if(Sent_tolower(sent.subject) == L"vous")
    {
        person = 4;
    }

    else
    if(Sent_tolower(sent.subject) == L"eux")
    {
        person = 5;
        sent.subject = L"ils";
    }

    else
    if(Sent_tolower(sent.subject) == L"elles")
    {
        person = 5;
    }

    else
    if(isPlural(sent.subject))
    {
        person = 5;
    }

    else
    {
        person = 2;
    }

    std::wstring verbConjugued = verbs[verb][sent.tense][person];
    bool apostroph = false;

    //if(Sent_isVoyel(sent.subject.back()) && Sent_isVoyel(verbConjugued[0]))
    if(sent.subject == L"je" && Sent_isVoyel(verbConjugued[0]))
    {
        sent.subject.back() = L'\'';
        apostroph = true;
    }

    std::wstring res = sent.subject;

    if(!apostroph)
    {
        res += L" ";
    }

    std::wstring pronoun = L"";

    if(sent.complement != L"" && sent.complementPronoun)
    {
        const std::wstring gender_number = getGenderNumber(sent.complement);

        if(gender_number == L"ms")
        {
            if(!Sent_isVoyel(verbConjugued[0]))
            {
                pronoun = L"le ";
            }

            else
            {
                pronoun = L"l'";
            }
        }

        else
        if(gender_number == L"fs")
        {
            if(!Sent_isVoyel(verbConjugued[0]))
            {
                pronoun = L"la ";
            }

            else
            {
                pronoun = L"l'";
            }
        }

        else
        if(gender_number == L"mp" || gender_number == L"fp")
        {
            pronoun = L"les ";
        }
    }

    if(Sent_indexOfWstring(adverbsNegations, Sent_tolower(sent.adverbVerb)) != -1)
    {
        std::wstring next;

        if(!sent.complementPronoun)
        {
            next = verbConjugued;
        }

        else
        {
            next = pronoun;
        }

        if(Sent_isVoyel(next[0]))
        {
            res += L"n'";
        }

        else
        {
            res += L"ne ";
        }
    }

    res += pronoun;

    if(sent.adverbVerb != L"" && sent.tense == L"past tense")
    {
        std::wstring auxiliary = verbConjugued;
        std::wstring past = verbConjugued;

        auxiliary.erase(auxiliary.find(L" "));
        past.erase(0, past.find(L" ") + 1);

        verbConjugued = auxiliary + L" " + sent.adverbVerb + L" " + past;
    }

    if(sent.complementPronoun && sent.tense == L"past tense" && Sent_getAuxiliary(verbConjugued) == L"Avoir")
    {
        const std::wstring gender_number = getGenderNumber(sent.complement);

        if(gender_number == L"mp")
        {
            verbConjugued += L"s";
        }

        else
        if(gender_number == L"fs")
        {
            verbConjugued += L"e";
        }

        else
        if(gender_number == L"fp")
        {
            verbConjugued += L"es";
        }
    }

    res += verbConjugued;

    if(sent.adverbVerb != L"" && sent.tense != L"past tense")
    {
        res += L" " + Sent_tolower(sent.adverbVerb);
    }

    if(sent.complement != L"" && !sent.complementPronoun)
    {
        addDeterminant(sent.complement);
        res += L" " + sent.complement;
    }

    if(sent.adjective != L"" && sent.complement == L"")
    {
        const std::wstring gender_number = getGenderNumber(baseSubject);

        //std::wcout << subject << L": " << gender_number << std::endl;

        if(Sent_indexOfWstring(verbsExceptions, Sent_tolower(verb)) == -1)
        {
            setAdjective(gender_number, sent.adjective);
        }

        if(sent.adverbAdjective != L"")
        {
            res += L" " + Sent_tolower(sent.adverbAdjective);
        }

        res += L" " + sent.adjective;
    }

    if(!noPoint)
    {
        res += L".";
    }

    if(!noUp)
    {
        res[0] = std::toupper(res[0]);
    }

    return res;
}

std::wstring Sentencer::assemble(const std::vector<Sent_sentence> sentences, std::vector<std::wstring> connectors)
{
    std::wstring res = L"";

    /*if(connector != L",")
    {
        res = create(s1, false, true) + L" " + Sent_tolower(connector) + L" " + create(s2, true, false);
    }

    else
    {
        res = create(s1, false, true) + Sent_tolower(connector) + L" " + create(s2, true, false);
    }*/

    if(connectors.size() > sentences.size() - 1)
    {
        return L"Error : Too connectors";
    }

    if(connectors.size() < sentences.size() - 1)
    {
        return L"Error : Not enough connectors";
    }

    for(size_t i = 0 ; i < sentences.size() ; i++)
    {
        if(i == 0)
        {
            res += create(sentences[i], false, true);
        }

        else
        if(i > 0 && i < sentences.size() - 1)
        {
            res += create(sentences[i], true, true);
        }

        else
        {
            res += create(sentences[i], true, false);
        }

        if(i < sentences.size() - 1)
        {
            if(connectors[i] != L",")
            {
                res += L" " + connectors[i] + L" ";
            }

            else
            {
                res += connectors[i] + L" ";
            }
        }
    }

    return res;
}
