#include<stdio.h>

int name[105]; // 课程们
int credit[105]; // 学分
std::string prereq;
int grade[105];

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


int main(){

}