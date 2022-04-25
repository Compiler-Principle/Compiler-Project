# Compiler-Project
## 项目简介

## 项目日程安排
+ Lexical Analysis 2022.4.24 lsh
+ Parsing 2022.5.2 fzy,jwz

## 运行方法
+ flex scanner.l
+ bison -d parser.y
+ gcc main.c parser.tab.c -o parser
+ ./parser test.txt