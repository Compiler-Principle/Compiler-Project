#include<stdio.h>

int name[105]; // 课程们
int credit[105]; // 学分
char prereq[105][500];
int grade[105];
char t[105][105];
int courses_len;

int parseGrade(char g){
    if(g == 'A') return 4;
    else if(g == 'B') return 3;
    else if(g == 'C') return 2;
    else if(g == 'D') return 1;
    else if(g == 'F') return 0;
    else return -1;
}

int strCount(char s[], char c){
    int count = 0;
    for(int i = 0; s[i] != 0; i++){
        if(s[i] == c) count++;
    }
    return count;
}

void summary(double gpa, int hAttempt, int hComplete, int cRemain, char courses[][105]){

    printf("GPA: %.1f\n", gpa);
    printf("Hours Attempted: %d\n", hAttempt);
    printf("Hours Completed: %d\n", hComplete);
    printf("Credits Remaining: %d\n", cRemain);
    printf("\nPossible Courses to Take Next\n");
    for(int i = 0; i < courses_len; i++){
        printf("  %s\n", courses[i]);
    }
    if(courses_len == 0 && cRemain == 0) printf("  None - Congratulations!\n");
}

int strContains(char basicString[], char s) {
    for(int i = 0; basicString[i] != 0; i++){
        if(basicString[i] == s) return 1;
    }
    return 0;
}

int main(){
    char name_c[100], prereq_c[100], credit_c[100], grade_c[100];
    int cnt;
    cnt = 0;
    while(getline(std::cin, line)){
        std::vector<std::string> t = lineSplit(line);
        if(t.size() == 1) break;
        cs[cnt] = course(t[0], atoi(t[1].c_str()), t[2], parseGrade(t[3]));
        cnt++;
    }
    int hAttempt = 0; // 尝试学分
    int hComplete = 0; // 已修学分
    int cRemain = 0;
    float credit_score = 0.0;

    int nextCourse[105];
    int nextCourses_len = 0;
    int canTake;
    int k;
    int found;

    for(int i = 0; i < cnt; i++){
        if(grade[i] == -1){
            // -1 代表未修过
            cRemain += credit[i];
        }
        else if(grade[i] == 0){
            // 0 代表Fail
            credit_score += 0.0;
            hAttempt += credit[i];
            cRemain += credit[i];

        }
        else{
            // 正常得分
            credit_score += grade[i] * credit[i];
            hAttempt += credit[i];
            hComplete += credit[i];
        }
    }

    for(int i = 0; i < cnt; i++){
        // 判断当前课程是否能够被推荐
        if(grade[i] == -1 || grade[i] == 0){
            // 未修过 或者已经挂科了
            if(prereq[i] == ""){
                // 没有先修课程
                nextCourse[nextCourses_len] = name[i];
                nextCourses_len++;
                continue;
            }
            else{
                canTake = 1;
                // 有先修课程
//                int valid_pres[105][50][100];
//                int valid_pres_len = 0;
                int valid_pre_strs[105][100];
                int valid_pre_strs_len = 0;
                int pre_strs[105];
                int pre_strs_len = 0;
//                std::vector<std::vector<std::string>> valid_pres;
                // 这门课的valid的先修课程们
//                std::vector<std::string> valid_pre_strs;
                if(strContains(prereq[i], ";")){
                    valid_pre_strs = lineSplit(prereq[i], ";");
                }
                else{
                    valid_pre_strs[valid_pre_strs_len] = prereq[i];
                }

                for(std::string& s : valid_pre_strs){
//                    std::vector<std::string> pre_strs; // 某一条先修课程们的名字
                    if(strContains(s, ",")){
                        // 有多个先修课程
                        pre_strs = lineSplit(s, ",");
                    }
                    else{
                        pre_strs.push_back(s);
                    }

                    // 如果本个先修课程的要求都满足了，那么就可以推荐
                    canTake = 1;
                    // 接下来检测某个要求中的每一门课
                    for(int l = 0; l < pre_strs_len; l++){
                        int this_course = pre_strs[l];
                        // 对某个this_course 需要检测在列表中的状态
                        found = 0;
                        for(k = 0; k < cnt; k++){
                            if(name[k] == this_course){
                                found = 1;
                                if(grade[k] == -1 || grade[k] == 0){
                                    // 如果先修课程未修过，那么就不能推荐
                                    // 如果先修课程Fail，那么就不能推荐
                                    canTake = 0;
                                    break;
                                }
                            }
                        }
                        if (!found){
                            // 如果在cnt这么中没有找到这个先修课程，那么就不能推荐
                            canTake = 0;
                            break;
                        }
                        if(!canTake){
                            break;
                        }
                    }

                    // 某一个先修课完全符合要求，也就是canTake没有被改成false，那么就可以推荐
                    if(canTake){
                        nextCourse[nextCourses_len] = name[i];
                        nextCourses_len++;
                        break; // 也不用看后面的可能预修要求了
                    }
                }

            }
        }
    }

    summary(hAttempt==0 ? 0.0 : credit_score/hAttempt, hAttempt, hComplete, cRemain, nextCourse);

    return 0;
}