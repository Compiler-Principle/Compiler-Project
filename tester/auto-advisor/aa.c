#include<stdio.h>
#include<string.h>
#include<ctype.h>
#define MAX_LINE 400
int name[105]; // 课程们
int credit[105]; // 学分
char prereq[105][MAX_LINE];
int grade[105];

int courses_len;
int nextCourses_len = 0;

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

void summary(double gpa, int hAttempt, int hComplete, int cRemain, int courses[]){

    printf("GPA: %.1f\n", gpa);
    printf("Hours Attempted: %d\n", hAttempt);
    printf("Hours Completed: %d\n", hComplete);
    printf("Credits Remaining: %d\n", cRemain);
    printf("\nPossible Courses to Take Next\n");
    for(int i = 0; i < nextCourses_len; i++){
        printf("  c%d\n", courses[i]);
    }
    if(nextCourses_len == 0 && cRemain == 0) printf("  None - Congratulations!\n");
}

int strContains(char basicString[], char s) {
    for(int i = 0; basicString[i] != 0; i++){
        if(basicString[i] == s) return 1;
    }
    return 0;
}

int a2i(const char *p, int start){
    int cyc, ret, sign;
    int t;
    t = 0;

    while(start--){
        p++;
    }
    cyc = (int)*(p++);
    sign = cyc;
    if (cyc == '-' || cyc == '+')
        cyc = (int)*(p++);

    ret = 0;

    while (isdigit(cyc)) {
        ret = 10 * ret + (cyc - '0');
        cyc = (int)*(p++);
    }

    return sign == '-' ? -ret : ret;
}

//int strlen(char s[]){
//    int i;
//    i = 0;
//    while(s[i] != 0){
//        i++;
//    }
//    return i;
//}

//void memset(char i[], char target, int len){
//    int j;
//    j = 0;
//    for(; j < len; j++){
//        i[j] = target;
//    }
//}

//void strcpy(char dest[], char src[]){
//    int i;
//    i = 0;
//    while(src[i] != 0){
//        dest[i] = src[i];
//        i++;
//    }
//    dest[i] = 0;
//}

void substr(char s[], char sub[], int start, int len){
    int i;
    i = 0;
    for(; i < len; i++){
        sub[i] = s[start + i];
    }
    sub[i] = 0;
}

int lineSplit(char input[], char deli, char res[][MAX_LINE]){
    int len, i, cnt;
//    len = 0;
//    i = 0;
    cnt = 0;
    int deli_count = strCount(input, deli);
    int delimiter_idx[100] = {0}; // 至少deli_count+2个元素
    int ii = 1;
    char tmp[MAX_LINE];
    delimiter_idx[deli_count+1] = strlen(input);
    for(i = 0; i < strlen(input); i++){
        if(input[i] == deli){
            delimiter_idx[ii] = i;
            ii++;
        }
    }
    for(i = 0; i < deli_count+1; i++){
        if(i == 0){
            substr(input, tmp, delimiter_idx[i], delimiter_idx[i+1]-delimiter_idx[i]);
        }
        else{
            substr(input, tmp, delimiter_idx[i]+1, delimiter_idx[i+1]-delimiter_idx[i]-1);
        }
        strcpy(res[cnt], tmp);
        cnt++;
    }
    return cnt;

}

int main(){
    int course_cnt;
    course_cnt = 0;
    char line[MAX_LINE];
    char t[105][MAX_LINE];
    int line_slice;
    while(scanf("%s", line) != EOF){
        line_slice = lineSplit(line, '|', t);
        if(line_slice == 1) break;
        name[course_cnt] = a2i(t[0], 1);
        credit[course_cnt] = a2i(t[1], 0);
        strcpy(prereq[course_cnt], t[2]);
        grade[course_cnt] = parseGrade(t[3][0]);
        course_cnt++;
    }
    int hAttempt = 0; // 尝试学分
    int hComplete = 0; // 已修学分
    int cRemain = 0;
    float credit_score = 0.0;

    int nextCourse[105];
    int canTake;
    int k;
    int found;

    for(int i = 0; i < course_cnt; i++){
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

    for(int i = 0; i < course_cnt; i++){
        if(name[i] == 76){
            int a = 0;
        }

        // 判断当前课程是否能够被推荐
        if(grade[i] == -1 || grade[i] == 0){
            // 未修过 或者已经挂科了
            if(strlen(prereq[i]) == 0){
                // 没有先修课程
                nextCourse[nextCourses_len] = name[i];
                nextCourses_len++;
                continue;
            }
            else{
                canTake = 1;
                // 有先修课程

                // 这门课的valid的先修课程们
                char valid_pre_strs[105][MAX_LINE];
                //c1, c2, c3
                //c4, c5, c6
                int valid_pre_strs_len = 0;
                // 2
                char pre_strs[105][MAX_LINE];// 某一条先修课程们的名字, [105][5]就够
                // c1
                // c2
                // c3
                int pre_strs_len = 0;
                // 3

                if(strContains(prereq[i], ';')){
//                    valid_pre_strs = lineSplit(prereq[i], ";");
                    valid_pre_strs_len = lineSplit(prereq[i], ';', valid_pre_strs);
                }
                else{
                    strcpy(valid_pre_strs[valid_pre_strs_len], prereq[i]);
                    valid_pre_strs_len++;
//                    valid_pre_strs[valid_pre_strs_len] = prereq[i];
                }

                for(int pres_id = 0; pres_id < valid_pre_strs_len; pres_id++){

                    if(strContains(valid_pre_strs[pres_id], ',')){
                        // 有多个先修课程
//                        pre_strs = lineSplit(valid_pre_strs[k], ",");
                        pre_strs_len = lineSplit(valid_pre_strs[pres_id], ',', pre_strs);
                    }
                    else{
                        strcpy(pre_strs[pre_strs_len], valid_pre_strs[pres_id]);
//                        pre_strs[pre_strs_len] = valid_pre_strs[k];
                        pre_strs_len++;
                    }

                    // 如果本个先修课程的要求都满足了，那么就可以推荐
                    canTake = 1;
                    // 接下来检测某个要求中的每一门课
                    for(int l = 0; l < pre_strs_len; l++){
                        int this_course = a2i(pre_strs[l], 1);
                        // 对某个this_course 需要检测在列表中的状态
                        found = 0;
                        for(k = 0; k < course_cnt; k++){
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
                    memset(pre_strs, 0, sizeof(pre_strs));
                    pre_strs_len = 0;

                }
                // reset the arrays
                memset(valid_pre_strs, 0, sizeof(valid_pre_strs));
                valid_pre_strs_len = 0;

            }
        }
    }

    summary(hAttempt==0 ? 0.0 : credit_score/hAttempt, hAttempt, hComplete, cRemain, nextCourse);

    return 0;
}