#include <iostream>
#include <cstring>
#include <vector>

class course{
public:
    std::string name;
    int credit;
    std::string prereq;
    int grade;
    course(std::string n, int c, std::string p, int g):name(n), credit(c), prereq(p), grade(g){}

};

int parseGrade(std::string g){
    if(g == "A") return 4;
    else if(g == "B") return 3;
    else if(g == "C") return 2;
    else if(g == "D") return 1;
    else if(g == "F") return 0;
    else return -1;
}



void split(const std::string& s, std::vector<std::string>& tokens, const std::string& delimiters = "|") {
    std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
    std::string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos) {
        tokens.push_back(s.substr(lastPos, pos - lastPos));//use emplace_back after C++11
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
        printf("%d %d\n", pos, lastPos);

    }
}

void lineSplit(const std::string& s, std::vector<std::string>& tokens, const std::string& delimiters = "|") {
    int delimiter_idx[5] = {0, 0, 0, 0, 0};
    int ii = 1;
    delimiter_idx[4] = s.size();
    for(int i = 0; i < s.size(); i++){
        if(s[i] == '|'){
            delimiter_idx[ii] = i;
            ii++;
        }
    }
    for(int i = 0; i < 4; i++){
        if(i == 0){
            tokens.push_back(s.substr(delimiter_idx[i], delimiter_idx[i+1]-delimiter_idx[i]));

        }
       else{
            tokens.push_back(s.substr(delimiter_idx[i]+1, delimiter_idx[i+1]-delimiter_idx[i]-1));
       }
    }
//    for(int i = 0; i < 4; i++){
//        printf("%d %d\n", delimiter_idx[i], delimiter_idx[i+1]);
//    }
//    for(std::string t : tokens){
//        printf("%s###\n", t.c_str());
//    }

}

int main(){
//    std::string name, prereq;
    char name_c[100], prereq_c[100], credit_c[100], grade_c[100];
    int credit, grade;
    std::string line;
//    while(scanf("%s|%s|%s|%s", name_c, credit_c, prereq_c, grade_c) != EOF){
    while(getline(std::cin, line)){
        std::vector<std::string> t;
        lineSplit(line, t);

        std::cout << t.size() << std::endl;
    }

    return 0;

}
