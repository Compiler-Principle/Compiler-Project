#include<stdio.h>
#include<ctype.h>
#define MAX_LINE 400
int name[105]; // 课程们
int credit[105]; // 学分
char prereq[105][MAX_LINE];
int grade[105];
int courses_len;
int nextCourses_len = 0;

void myscanf(char s[]){
    int i = 0;
    char c;
    while(1){
        scanf("%c", &c);
        if(c == 10 || c == 0){
            break;
        }
        s[i++] = c;
    }
    s[i] = '\0';
}

int parseGrade(char g){
    int ret;
    ret = -1;
    if(g == 65){
        ret = 4;
    }
    if(g == 66){
        ret = 3;
    }
    if(g == 67){
        ret = 2;
    }
    if(g == 68){
        ret = 1;
    }
    if(g == 70){
        ret =0;
    }
    return ret;
}

int strCount(char s[], char c){
    int count, i;
    i = 0;
    count = 0;

    while(s[i] != 0){
        if(s[i] == c){
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

void summary(double gpa, int hAttempt, int hComplete, int cRemain, int courses[]){
    int i;
    i = 0;

    printf("GPA: %.1f\n", gpa);
    printf("Hours Attempted: %d\n", hAttempt);
    printf("Hours Completed: %d\n", hComplete);
    printf("Credits Remaining: %d\n", cRemain);
    printf("\nPossible Courses to Take Next\n");
    while(i < nextCourses_len){
        printf("  c%d\n", courses[i]);
        i = i + 1;
    }
    if(nextCourses_len == 0){
        if(cRemain == 0){
            printf("  None - Congratulations!\n");
        }
    }
}

int strContains(char basicString[], char s) {
    int i;
    i = 0;
    while(basicString[i] != 0){
        if(basicString[i] == s){
            return 1;
        }
        i = i + 1;
    }
    return 0;
}

int a2i(char p[], int start){
    int cyc, ret;
    int i, t;
    i = 0;
    t = 0;
    while(i < start){
        t++;
        start--;
    }
    cyc = (int)p[t];
    t = t + 1;
    ret = 0;

    while (cyc >= 48 && cyc <= 57){
        ret = 10 * ret + (cyc - 48);
        cyc = (int)p[t];
        t = t + 1;
    }

    return ret;
}

int strlen(char s[]){
    int i;
    i = 0;
    while(s[i] != 0){
        i = i + 1;
    }
    return i;
}

void memset(char i[], char target, int len){
    int j;
    j = 0;
    while( j < len){
        i[j] = target;
        j = j + 1;
    }
}

void strcpy(char dest[], char src[]){
    int i;
    i = 0;
    while(src[i] != 0){
        dest[i] = src[i];
        i = i + 1;
    }
    dest[i] = 0;
}

void substr(char s[], char sub[], int start, int len){
    int i;
    i = 0;
    while(i < len){
        sub[i] = s[start + i];
        i = i + 1;
    }
    sub[i] = 0;
}

int lineSplit(char input[], char deli, char res[][MAX_LINE]){
    int len, i, cnt, deli_count, ii;
    int delimiter_idx[100];
    char tmp[MAX_LINE];

    deli_count = strCount(input, deli);
    ii = 1;
    cnt = 0;

    delimiter_idx[deli_count+1] = strlen(input);
    i = 0;
    while(i < strlen(input)){
        if(input[i] == deli){
            delimiter_idx[ii] = i;
            ii = ii + 1;
        }
        i = i + 1;
    }

    i= 0;
    while( i < deli_count+1){
        if(i == 0){
            substr(input, tmp, delimiter_idx[i], delimiter_idx[i+1]-delimiter_idx[i]);
        }
        else{
            substr(input, tmp, delimiter_idx[i]+1, delimiter_idx[i+1]-delimiter_idx[i]-1);
        }
        strcpy(res[cnt], tmp);
        cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;

}

int main(){
    int course_cnt;
    char line[MAX_LINE];
    char t[105][MAX_LINE];
    int line_slice, hAttempt, hComplete, cRemain;
    float credit_score;

    int nextCourse[105];
    int canTake;
    int found;
    int i, ii;



    int pres_id;
    int l, k, this_course;



    char valid_pre_strs[105][MAX_LINE];
    int valid_pre_strs_len;
    char pre_strs[105][MAX_LINE];
    int pre_strs_len;


    hAttempt = 0;
    hComplete = 0;
    cRemain = 0;
    credit_score = 0.0;
    course_cnt = 0;
    valid_pre_strs_len = 0;
    pre_strs_len = 0;


    while(1){
        myscanf(line);
        line_slice = lineSplit(line, '|', t);
        if(line_slice == 1){
            break;
        }
        name[course_cnt] = a2i(t[0], 1);
        credit[course_cnt] = a2i(t[1], 0);
        strcpy(prereq[course_cnt], t[2]);
        grade[course_cnt] = parseGrade(t[3][0]);
        course_cnt = course_cnt + 1;
    }
    i = 0;
    while( i < course_cnt){
        if(grade[i] == -1){
            // -1 代表未修过
            cRemain = cRemain + credit[i];
        }
        else if(grade[i] == 0){
            // 0 代表Fail
//            credit_score += 0.0;
            hAttempt = hAttempt + credit[i];
            cRemain = cRemain + credit[i];

        }
        else{
            // 正常得分
            credit_score = credit_score + grade[i] * credit[i];
            hAttempt = hAttempt + credit[i];
            hComplete = hComplete + credit[i];
        }
        i = i + 1;
    }
    i = 0;
    ii = 0;
    while(ii < course_cnt){
        // 判断当前课程是否能够被推荐
        if(grade[ii] == -1 || grade[ii] == 0){
            // 未修过 或者已经挂科了
            if(strlen(prereq[ii]) == 0){
                // 没有先修课程
                nextCourse[nextCourses_len] = name[ii];
                nextCourses_len++;
                ii = ii + 1;
                continue;
            }
            else{
                canTake = 1;
                // 有先修课程


                if(strContains(prereq[ii], ';') == 1){
//                    valid_pre_strs = lineSplit(prereq[i], ";");
                    valid_pre_strs_len = lineSplit(prereq[ii], ';', valid_pre_strs);
                }
                else{
                    strcpy(valid_pre_strs[valid_pre_strs_len], prereq[ii]);
                    valid_pre_strs_len = valid_pre_strs_len + 1;
                }
                pres_id = 0;
                while(pres_id < valid_pre_strs_len){

                    if(strContains(valid_pre_strs[pres_id], ',') == 1){
                        // 有多个先修课程
                        pre_strs_len = lineSplit(valid_pre_strs[pres_id], ',', pre_strs);
                    }
                    else{
                        strcpy(pre_strs[pre_strs_len], valid_pre_strs[pres_id]);
                        pre_strs_len = pre_strs_len + 1;
                    }

                    // 如果本个先修课程的要求都满足了，那么就可以推荐
                    canTake = 1;
                    // 接下来检测某个要求中的每一门课
                    l = 0;
                    while( l < pre_strs_len){
                        this_course = a2i(pre_strs[l], 1);
                        // 对某个this_course 需要检测在列表中的状态
                        found = 0;
                        k = 0;
                        while( k < course_cnt){
                            if(name[k] == this_course){
                                found = 1;
                                if(grade[k] == -1 || grade[k] == 0){
                                    // 如果先修课程未修过，那么就不能推荐
                                    // 如果先修课程Fail，那么就不能推荐
                                    canTake = 0;
                                    break;
                                }
                            }
                            k = k + 1;
                        }
                        if (found == 0){
                            // 如果在cnt这么中没有找到这个先修课程，那么就不能推荐
                            canTake = 0;
                            break;
                        }
                        if(canTake == 0){
                            break;
                        }
                        l = l + 1;
                    }

                    // 某一个先修课完全符合要求，也就是canTake没有被改成false，那么就可以推荐
                    if(canTake == 1){
                        nextCourse[nextCourses_len] = name[ii];
                        nextCourses_len = nextCourses_len + 1;
                        break; // 也不用看后面的可能预修要求了
                    }
                    memset(pre_strs, 0, sizeof(pre_strs));
                    pre_strs_len = 0;
                    pres_id = pres_id + 1;

                }
                // reset the arrays
                memset(valid_pre_strs, 0, sizeof(valid_pre_strs));
                valid_pre_strs_len = 0;

            }
        }
        ii = ii + 1;
    }
    if(hAttempt==0){
        summary( 0.0 , hAttempt, hComplete, cRemain, nextCourse);

    }
    else{
        summary(credit_score/hAttempt, hAttempt, hComplete, cRemain, nextCourse);

    }
    return 0;
}