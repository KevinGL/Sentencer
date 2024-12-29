﻿#include "Sentencer.h"

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

bool Sent_isVoyel(const wchar_t character)
{
    if(std::tolower(character) == 'a' || std::tolower(character) == 'e' || std::tolower(character) == 'i' || std::tolower(character) == 'o' || std::tolower(character) == 'u' || std::tolower(character) == 'y')
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
                term = L"le " + term;       //Ou "la" si féminin
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