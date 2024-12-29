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
}

std::wstring Sentencer::create(Sent_sentence sent, const bool noUp, const bool noPoint)
{
    if(verbs.count(sent.verb) == 0)
    {
        return L"Error : Verb \"" + sent.verb + L"\" does not exist";
    }

    if(verbs[sent.verb].count(sent.tense) == 0)
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

    std::wstring verbConjugued = verbs[sent.verb][sent.tense][person];
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

    std::wofstream file("aa.txt", std::ios::app);

    file << verbConjugued << std::endl;

    file.close();

    //if(sent.adverb == L"pas" || sent.adverb == L"plus" || sent.adverb == L"jamais" || sent.adverb == L"rien")
    if(sent.adverb.find(L"pas") != std::string::npos || sent.adverb.find(L"plus") != std::string::npos || sent.adverb.find(L"jamais") != std::string::npos || sent.adverb.find(L"rien") != std::string::npos)
    {
        if(Sent_isVoyel(verbConjugued[0]))
        {
            res += L"n'";
        }

        else
        {
            res += L"ne ";
        }
    }

    if(sent.adverb != L"" && sent.tense == L"past tense")
    {
        std::wstring auxiliary = verbConjugued;
        std::wstring past = verbConjugued;

        auxiliary.erase(auxiliary.find(L" "));
        past.erase(0, past.find(L" ") + 1);

        verbConjugued = auxiliary + L" " + sent.adverb + L" " + past;
    }

    res += verbConjugued;

    if(sent.adverb != L"" && sent.tense != L"past tense")
    {
        res += L" " + Sent_tolower(sent.adverb);
    }

    if(sent.complement != L"")
    {
        addDeterminant(sent.complement);
        res += L" " + sent.complement;
    }

    if(sent.adjective != L"" && sent.complement == L"")
    {
        const std::wstring gender_number = getGenderNumber(baseSubject);

        //std::wcout << subject << L": " << gender_number << std::endl;

        if(Sent_indexOfWstring(verbsExceptions, Sent_tolower(sent.verb)) == -1)
        {
            setAdjective(gender_number, sent.adjective);
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

std::wstring Sentencer::createTwo(const Sent_sentence s1, const Sent_sentence s2, std::wstring connector)
{
    std::wstring res = L"";

    if(connector != L",")
    {
        res = create(s1, false, true) + L" " + Sent_tolower(connector) + L" " + create(s2, true, false);
    }

    else
    {
        res = create(s1, false, true) + Sent_tolower(connector) + L" " + create(s2, true, false);
    }

    return res;
}
