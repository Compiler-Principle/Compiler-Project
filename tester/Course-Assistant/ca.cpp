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


std::vector<std::string> lineSplit(const std::string& s,  const std::string& delimiters = "|") {
    std::vector<std::string> tokens;
    int delimiter_idx[5] = {0, 0, 0, 0, 0};
    int ii = 1;
    delimiter_idx[4] = s.size();
    for(int i = 0; i < s.size(); i++){
        if(s[i] == delimiters[0]){
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
    return tokens;
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

bool strContains(std::string basicString, const char *string) {
    return basicString.find(string) != std::string::npos;
}

int main(){
//    std::string name, prereq;
    char name_c[100], prereq_c[100], credit_c[100], grade_c[100];
    int credit, grade;
    std::string line;
    course cs[105];
    int cnt = 0;
    while(getline(std::cin, line)){
        std::vector<std::string> t = lineSplit(line);
        cs[cnt] = course(t[0], atoi(t[1].c_str()), t[2], parseGrade(t[3]));
        cnt++;
    }
    int hAttempt = 0; // 尝试学分
    int hComplete = 0; // 已修学分
    int cRemain = 0;
    double credit_score = 0.0;
    std::vector<std::string> nextCourse;
    printf("cnt: %d\n", cnt);
    for(int i = 0; i < cnt; i++){
        if(cs[i].grade == -1){
            // -1 代表未修过
            cRemain += cs[i].credit;
        }
        else if(cs[i].grade == 0){
            // 0 代表Fail
            credit_score += 0.0;
            hAttempt += cs[i].credit;
            cRemain += cs[i].credit;

        }
        else{
            // 正常得分
            credit_score += cs[i].grade * cs[i].credit;
            hAttempt += cs[i].credit;
            hComplete += cs[i].credit;
        }

    }

    for(int i = 0; i < cnt; i++){
        // 判断当前课程是否能够被推荐
        if(cs[i].grade == -1 || cs[i].grade == -1){
            // 未修过 或者已经挂科了
            if(cs[i].prereq == ""){
                // 没有先修课程
                nextCourse.push_back(cs[i].name);
                continue;
            }
            else{
                bool canTake = true;
                // 有先修课程
                std::vector<std::vector<std::string>> valid_pres;
                // 这门课的valid的先修课程们
                std::vector<std::string> valid_pre_strs;
                if(strContains(cs[i].prereq, ";")){
                    valid_pre_strs = lineSplit(cs[i].prereq, ";");
                }
                else{
                    valid_pre_strs.push_back(cs[i].prereq);
                }
                for(std::string& s : valid_pre_strs){
                    std::cout << "s: " << s << std::endl;
                    // 如果本个先修课程的要求都满足了，那么就可以推荐
                    std::vector<std::string> pre_strs = lineSplit(s, ",");
                    canTake = true;
                    for(int k = 0; k < cnt; k++){
                        for(int l = 0; l < pre_strs.size(); l++){
                            if(cs[k].name == pre_strs[l]){
                                if(cs[k].grade == -1 || cs[k].grade == 0){
                                    // 如果先修课程未修过，那么就不能推荐
                                    // 如果先修课程Fail，那么就不能推荐
                                    canTake = false;
                                    break;
                                }


                            }
                        }
                        if(!canTake){
                            break;
                        }

                    }
                    // 某一个先修课完全符合要求，也就是canTake没有被改成false，那么就可以推荐
                    if(canTake){
                        nextCourse.push_back(cs[i].name);
                        break; // 也不用看后面的可能预修要求了
                    }
                }

            }
        }
    }
    summary(hAttempt==0 ? 0.0 : credit_score/hAttempt, hAttempt, hComplete, cRemain, nextCourse);

    return 0;

}
