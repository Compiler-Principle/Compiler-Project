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
    course(){}

};

int parseGrade(std::string g){
    if(g == "A") return 4;
    else if(g == "B") return 3;
    else if(g == "C") return 2;
    else if(g == "D") return 1;
    else if(g == "F") return 0;
    else return -1;
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


void summary(double gpa, int hAttempt, int hComplete, int cRemain, std::vector<std::string> courses){

    printf("GPA: %.1f\n", gpa);
    printf("Hours Attempted: %d\n", hAttempt);
    printf("Hours Completed: %d\n", hComplete);
    printf("Credits Remaining: %d\n", cRemain);
    printf("Possible Courses to Take Next\n");
    for(const std::string& c : courses){
        printf("%s\n", c.c_str());
    }
}

int main(){
//    std::string name, prereq;
    char name_c[100], prereq_c[100], credit_c[100], grade_c[100];
    int credit, grade;
    std::string line;
    course cs[105];
    int cnt = 0;
    while(getline(std::cin, line)){
        std::vector<std::string> t;
        lineSplit(line, t);
        cs[cnt] = course(t[0], atoi(t[1].c_str()), t[2], parseGrade(t[3]));
        cnt++;
    }
    int hAttempt = 0;
    int hComplete = 0;
    int cRemain = 0;
    double credit_score = 0.0;
    std::vector<std::string> nextCourse;
    for(int i = 0; i < cnt; i++){
        if(cs[i].grade == -1){
            // -1 代表未修过
            cRemain += cs[i].credit;
        }
        else if(cs[i].grade == 0){
            // 0 代表Fail
            credit_score += 0.0;
            hAttempt += cs[i].credit;

        }
        else{
            // 正常得分
            credit_score += cs[i].grade * cs[i].credit;
            hAttempt += cs[i].credit;
            hComplete += cs[i].credit;
        }
    }
    summary(hAttempt==0 ? 0.0 : credit_score/hAttempt, hAttempt, hComplete, cRemain, nextCourse);

    return 0;

}
