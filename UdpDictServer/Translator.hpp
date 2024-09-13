#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unordered_map>

const std::string DefaultPath = "./words.txt";
const std::string SEP = ": ";

class Translator
{
private:
    void LoadDict()
    {
        std::ifstream in(_DictPath);
        if(!in.is_open())
        {
            std::cerr << "open file:" << std::endl;
        }

        std::cout << "Begin Load file...!" << std::endl;
        std::string line;
        while(std::getline(in, line))
        {
            size_t pos = line.find(SEP);
            if(pos == std::string::npos) continue;

            std::string key = line.substr(0, pos);
            if(key.empty()) continue;

            std::string val = line.substr(pos + SEP.size());
            if(val.empty()) continue;

            _Dict.insert(std::make_pair(key, val));
            std::cout << "Successfull load " << key << "->" << val << std::endl;
        }

        in.close();
        std::cout << "Successfull load all words!" << std::endl;
    }

public:
    Translator(std::string Path = DefaultPath)
        : _DictPath(Path)
        {
            LoadDict();
        }
    
    std::string translate(const std::string& word)
    {
        if(word.empty()) return "";
        
        auto iter = _Dict.find(word);
        std::cout << word << std::endl;
        if(iter != _Dict.end()) return iter->second;
        else return "None!";
    }

private:
    std::string _DictPath;
    std::unordered_map<std::string, std::string> _Dict;
};