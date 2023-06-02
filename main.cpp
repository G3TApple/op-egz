#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <regex>
#include <vector>
#include <locale>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::map;
using std::left;
using std::right;
using std::fixed;
using std::setw;
using std::regex_replace;
using std::regex_match;
using std::regex;
using std::vector;

bool tinkamas_zodis(string& zodis){
    regex reg("[^a-zA-ZĄČĘĖĮŠŲŪŽąčęėįšųūž\\s]");
    zodis = regex_replace(zodis, reg, "");
    if(!zodis.empty())
        return true;
    return false;
}

void nuskaityti_zodzius(map<string, vector<int>>& zodziai, vector<string>& domenai,vector<string>& tinklalapiai){
    ifstream fd("input.txt");
    if (!fd.is_open()) {
        std::cout << "Nepavyko atidaryti failo.\n";
        exit(0);
    }
    int linijosNr = 1;
    string eilute;
    while(getline(fd,eilute)){
        stringstream ss_eilute(eilute);
        string zodis;
        vector<string> pradzia = {"www","http"};
        while(ss_eilute>>zodis){
            bool rastas = false;
            if(zodis.back() == '.')
                    zodis.pop_back();
            transform(zodis.begin(), zodis.end(), zodis.begin(), ::tolower);
            for(const auto& d : domenai){
                auto it = search(zodis.begin(),zodis.end(),d.begin(),d.end());
                if(zodis.end()-it == d.end()-d.begin()){
                    tinklalapiai.push_back(regex_replace(zodis, regex(R"(^\.)"), ""));
                    rastas=true;
                }
                else {
                    for(const auto& p : pradzia){
                        auto it_ = search(zodis.begin(),zodis.end(),p.begin(),p.end());
                        if(zodis.begin()-it_ == 0){
                            tinklalapiai.push_back(zodis);
                            rastas=true;
                        }
                    }
                }
                if(rastas) break;
            }
            if(!rastas && tinkamas_zodis(zodis)){
                zodziai[zodis].push_back(linijosNr);
            }
        }
        linijosNr++;
    }
    fd.close();
}

void nuskaityti_domenus(vector<string>& domenai){
    ifstream fd("domenai.txt");
    if (!fd.is_open()) {
        std::cout << "Nepavyko atidaryti failo.\n";
        exit(0);
    }
    string domain;
    while (getline(fd, domain)) {
        if (!domain.empty()) {
            domenai.push_back(domain);
        }
    }
    fd.close();
}

void irasyti_i_faila(map<string, vector<int>>& zodziai, vector<string>& tinklalapiai){
    sort( tinklalapiai.begin(), tinklalapiai.end() );
    tinklalapiai.erase( unique( tinklalapiai.begin(), tinklalapiai.end() ), tinklalapiai.end() );
    ofstream fr ("rezultatai.txt");
    fr << "Tekste rasti tinklalapiai:\n";
    for (const auto& t : tinklalapiai){
        fr << t << endl;
    }
    fr << endl << "Žodžių cross-reference lentelė: \n";
    for (const auto& m : zodziai){
        if(m.second.size()>1){
            fr << left << setw(20) << m.first;
            fr << "-- pasikartojo tiek kartų: " << m.second.size() << ", eilutėse: ";
            for(const auto& eilute: m.second)
                fr << eilute << " ";
            fr << endl;
        }
    }
    fr.close();
}


int main()
{
    map<string, vector<int>> zodziai;
    vector<string> tinklalapiai;
    vector<string> domenai;
    nuskaityti_domenus(domenai);
    nuskaityti_zodzius(zodziai,domenai,tinklalapiai);
    irasyti_i_faila(zodziai,tinklalapiai);

    return 0;
}

