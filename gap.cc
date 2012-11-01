#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
//

#include <stdio.h>
#include <stdlib.h>

class gap{
public:
     gap();
     ~gap();
     void read_lm();
     std::vector<std::string> add_line(std::string s);
     int get_id(std::string s);
     int add_string(std::string s);
     std::string get_string(int i);
     std::vector<std::string> inline tokenize(const std::string &source, const char *delimiter);
private:
     std::istream* inputSource;
     std::unordered_map<std::vector<int>, double> logPHash;
     std::unordered_map<std::vector<int>, double> logBHash;
     std::unordered_map<std::string, int> voc;
     std::vector<std::string> cov;
};

gap::gap():inputSource(&std::cin){};
gap::~gap(){}

int gap::add_string(std::string s){
     std::cout << "Add:" << voc.size() << std::endl;
     int id = voc.size();
     voc[s] = id;
     cov.push_back(s);
     return id;
}

int gap::get_id(std::string s){
     if(voc.find(s) != voc.end()) return voc[s];
     else return add_string(s);
}

std::string gap::get_string(int i){
     if (i < cov.size()) return cov[i];
     else std::cerr << "Index missmatch\n";
}

std::vector<std::string> inline gap::tokenize(const std::string &source, const char *delimiter = " "){
     std::vector<std::string> results;
     size_t prev = 0, next = 0;
     while ((next = source.find_first_of(delimiter, prev)) != std::string::npos)
     {
          if (next - prev != 0){
               results.push_back(source.substr(prev, next - prev));
          }
          prev = next + 1;
     }
     if (prev < source.size())
          results.push_back(source.substr(prev));
     return results;
}

std::vector<std::string> gap::add_line(std::string s){
     std::vector<std::string> results = tokenize(s, "\t");
     bool back_off = false;
     double logP = -99, logB = -99;
     std::cout << s << std::endl;
     logP = atof(results[0].c_str());
     if (results.size() == 3) {
          logB = atof(results[results.size() - 1].c_str());
          back_off = true;
     }
     printf("logB:%lf logP:%lf\n", logB, logP);
     results = tokenize(results[1], " ");
     std::vector<int> ng;
     for(int i = 0 ; i < results.size() ; i++){          
          ng.push_back(get_id(results[i]));
          std::cout << "w:" << results[i] << " ng:" << ng[i] << std::endl;
     }
     //     logPHash[ng] = logP;
     //     if(back_off) logBHash[ng] = logB;
     return results;
}

void gap::read_lm(){
     std::string line;
     int flag = 0;
     while(getline(*inputSource, line)){          
          if(line.empty() || line[0] == '\n') continue;          
          else if(line == "\\data\\"){
               flag = 1;
          }
          else if(line[0] == '\\'){
               int norder = 0;
               sscanf(&line[0],"\\%d-grams:", &norder);
               flag = norder == 1 ? 2 : 3;
               printf("Reading %d-grams\n", norder);
          }
          else if(flag == 1 && line[0] == 'n'){
               int norder = 0, ncount = 0;
               sscanf(&line[0],"ngram %d=%d", &norder, &ncount);
               std::cout << "read:" << norder << " " << ncount << std::endl;
          }
          else{
               add_line(line);
          }
     }
     std::cout << "Vocabulary:" << voc.size() << std::endl;
}

int main(){
     std::cout << "Reading LM" << std::endl;
     gap g;
     g.read_lm();
     return 0;
}

