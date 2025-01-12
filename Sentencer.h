#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <fstream>

struct Sent_sentence
{
    std::wstring subject;
    std::wstring verb;
    std::wstring tense = L"present";
    std::wstring DOC = L"";
    std::wstring IOC = L"";
    std::wstring adjective = L"";
    std::wstring adverbVerb = L"";
    std::wstring adverbAdjective = L"";
    bool DOCPronoun = false;
    bool IOCPronoun = false;
    bool complementPronoun = false;
};

class Sentencer
{
    private :

    std::map<std::wstring, std::map<std::wstring, std::vector<std::wstring>> > verbs;
    std::map<std::wstring, std::wstring> gendersExceptions;
    std::map<std::wstring, std::map<std::wstring, std::wstring> > adjectivesExceptions;
    std::vector<std::wstring> verbsExceptions;
    std::vector<std::wstring> adverbsNegations;

    void addDeterminant(std::wstring &term);
    bool isPlural(const std::wstring term);
    std::wstring getGenderNumber(const std::wstring term);
    void setAdjective(const std::wstring gender_number, std::wstring &adjective);

    public :

    Sentencer();
    std::wstring create(Sent_sentence sent, const bool noUp = false, const bool noPoint = false);
    std::wstring assemble(const std::vector<Sent_sentence> sentences, std::vector<std::wstring> connectors);
};

std::string Sent_getRelativePath();
std::wstring Sent_readValueJSONline(const std::wstring line);
std::wstring Sent_readKeyJSONline(const std::wstring line);
std::wstring Sent_tolower(const std::wstring str);
std::wstring Sent_toupperInit(const std::wstring str);
bool Sent_isVoyel(const wchar_t character);
bool Sent_isPronom(const std::wstring term);
int Sent_indexOfWstring(const std::vector<std::wstring> array, const std::wstring value);
std::wstring Sent_getAuxiliary(const std::wstring verb);
