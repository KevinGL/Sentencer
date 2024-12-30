#include "Sentencer.h"

std::string Sent_getRelativePath()
{
    std::ifstream file("./relative_path.ini");

    if(!file.is_open())
    {
        return "./";
    }

    bool dataPath = false;
    std::string res = "";

    while(1)
    {
        std::string line;

        if(!getline(file, line))
        {
            break;
        }

        if(line.find("[Paths]") != std::string::npos)
        {
            dataPath = true;
        }

        else
        if(dataPath)
        {
            res = line;
            res.erase(0, res.find("=") + 1);

            break;
        }
    }

    file.close();

    return res;
}

std::wstring Sent_readValueJSONline(const std::wstring line)
{
    bool inValue = false;
    bool inQuotes = false;
    std::wstring res = L"";

    for(const wchar_t ch : line)
    {
        if(ch == L'"')
        {
            inQuotes = !inQuotes;
        }

        else
        if(ch == L':')
        {
            inValue = true;
        }

        else
        if(inQuotes && inValue)
        {
            res += ch;
        }
    }

    return res;
}

std::wstring Sent_readKeyJSONline(const std::wstring line)
{
    bool inValue = false;
    bool inQuotes = false;
    std::wstring res = L"";

    for(const wchar_t ch : line)
    {
        if(ch == L'"')
        {
            inQuotes = !inQuotes;
        }

        else
        if(ch == L':')
        {
            inValue = true;
        }

        else
        if(inQuotes && !inValue)
        {
            res += ch;
        }
    }

    return res;
}

std::wstring Sent_tolower(const std::wstring str)
{
    std::wstring res = L"";

    for(const wchar_t ch : str)
    {
        res += std::tolower(ch);
    }

    return res;
}

std::wstring Sent_toupperInit(const std::wstring str)
{
    std::wstring res = L"";

    for(size_t i = 0 ; i < str.length() ; i++)
    {
        if(i == 0)
        {
            res += std::toupper(str.at(i));
        }

        else
        {
            res += str.at(i);
        }
    }

    return res;
}

bool Sent_isVoyel(const wchar_t character)
{
    if(std::tolower(character) == L'a' || std::tolower(character) == L'e' || std::tolower(character) == L'i' || std::tolower(character) == L'o' || std::tolower(character) == L'u' || std::tolower(character) == L'y' ||
       std::tolower(character) == L'à' || std::tolower(character) == L'é' || std::tolower(character) == L'î' || std::tolower(character) == L'ô' || std::tolower(character) == L'ù' ||
       std::tolower(character) == L'â' || std::tolower(character) == L'è' || std::tolower(character) == L'ï' || std::tolower(character) == L'ö' || std::tolower(character) == L'û')
    {
        return true;
    }

    return false;
}

bool Sentencer::isPlural(const std::wstring term)
{
    if(Sent_tolower(term).back() == L's' || Sent_tolower(term).back() == L'x' || term.find(L" et ") != std::string::npos)
    {
        return true;
    }

    return false;
}

std::wstring Sentencer::getGenderNumber(const std::wstring term)
{
    std::wstring res = L"ms";
    bool feminine;

    if(gendersExceptions.count(Sent_tolower(term)) != 0)
    {
        if(gendersExceptions[Sent_tolower(term)] == L"f")
        {
            feminine = true;
        }

        else
        {
            feminine = false;
        }
    }

    else
    {
        if(Sent_tolower(term).back() == L'e')
        {
            feminine = true;
        }

        else
        {
            feminine = false;
        }
    }

    if(feminine && !isPlural(term))
    {
        res = L"fs";
    }

    else
    if(!feminine && isPlural(term))
    {
        res = L"mp";
    }

    else
    if(feminine && isPlural(term))
    {
        res = L"fp";
    }

    return res;
}

bool Sent_isPronom(const std::wstring term)
{
    if(Sent_tolower(term) == L"moi" || Sent_tolower(term) == L"toi" || Sent_tolower(term) == L"lui" ||
       Sent_tolower(term) == L"elle" || Sent_tolower(term) == L"nous" || Sent_tolower(term) == L"vous" ||
       Sent_tolower(term) == L"eux" || Sent_tolower(term) == L"elles" || Sent_tolower(term) == L"ça")
    {
        return true;
    }

    return false;
}

void Sentencer::addDeterminant(std::wstring &term)
{
    if(term.find(L"Le ") != 0 && term.find(L"La ") != 0 && term.find(L"Les ") != 0 &&
       term.find(L"le ") != 0 && term.find(L"la ") != 0 && term.find(L"les ") != 0 &&
       term.find(L"Un ") != 0 && term.find(L"Une ") != 0 && term.find(L"Des ") != 0 &&
       term.find(L"un ") != 0 && term.find(L"une ") != 0 && term.find(L"des ") != 0 && !Sent_isPronom(term))
    {
        if(!isPlural(term))
        {
            if(!Sent_isVoyel(term[0]))
            {
                if(getGenderNumber(term) == L"ms")
                {
                    term = L"le " + term;
                }

                else
                if(getGenderNumber(term) == L"fs")
                {
                    term = L"la " + term;
                }
            }

            else
            {
                term = L"l'" + term;
            }
        }

        else
        {
            term = L"les " + term;
        }
    }
}

void Sentencer::setAdjective(const std::wstring gender_number, std::wstring &adjective)
{
    if(adjectivesExceptions.count(adjective) == 0)
    {
        if(gender_number == L"mp")
        {
            adjective += L"s";
        }

        else
        if(gender_number == L"fs")
        {
            adjective += L"e";
        }

        else
        if(gender_number == L"fp")
        {
            adjective += L"es";
        }
    }

    else
    {
        adjective = adjectivesExceptions[adjective][gender_number];
    }
}

int Sent_indexOfWstring(const std::vector<std::wstring> array, const std::wstring value)
{
    for(size_t i = 0 ; i < array.size() ; i++)
    {
        if(array[i] == value)
        {
            return i;
        }
    }

    return -1;
}

std::wstring Sent_getAuxiliary(const std::wstring verb)
{
    if(verb.find(L" ") == std::string::npos)
    {
        return L"";
    }

    std::wstring auxiliary = verb;

    auxiliary.erase(auxiliary.find(L" "));

    if(auxiliary == L"ai" || auxiliary == L"as" || auxiliary == L"a" || auxiliary == L"avons" || auxiliary == L"avez" || auxiliary == L"ont" ||
       auxiliary == L"avais" || auxiliary == L"avais" || auxiliary == L"avait" || auxiliary == L"avions" || auxiliary == L"aviez" || auxiliary == L"avaient")
    {
        return L"Avoir";
    }

    return L"Être";
}
