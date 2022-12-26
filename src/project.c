#include "project.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE sizeof(Student)

/* HUOMIOITA
    1. Ohjelmani testaa asioita joskus eri järjestyksessä kuin
   palautusjärjestelmä, esim. käskyt A 123 A B ja A 123 A palauttaa minulla
   virheen argumenttien määrästä kun järjestelmä huomauttaa että 123 on jo
   olemassa, järjestelmä siis tarkastaa ensin opiskelijanumeron. Kokeilin
   poistaa määrätarkistuken ja silloin ohjelmani palautti saman.
*/

/*
    Peruslogiikka: fgets() raakaversio komennosta, poistetaan osia jotta argsLen() (strtok apuna käyttäen) saadaan argumenttien määrä.
    Ennen funktiota tarkistetaan argumenttien määrä, jos se on oikea parseArgs() palauttaa char** josta validointi on helppoa.
*/

int main() {
    int size = 0;  // index where to place next student
    int* sPtr = &size;

    char input[1001];
    char inputCopy[1001];

    Student* list = malloc(1);
    char** args;

    printf("Enter first command:\n");
    while (1) {
        fgets(input, 1001, stdin);
        char c = input[0];
        input[0] = ' ';  // poistetaan komentomerkki niin strtok toimii kivasti
        int comLen = 0;  // command length
        memcpy(inputCopy, input,
               1001);  // strtok muuttaa pointtereita, tarvitaan kopio

        comLen = argsLen(inputCopy, input);

        input[strlen(input) - 1] = 0;  // this just works, don't understand why

        if (c == 'A') {
            if (comLen != 3) {
                printf("A should be followed by exactly 3 arguments.\n");
                continue;
            }
            args = parseArgs(input, 3);
            if (validAdd(list, size, args[0])) {
                int pts[] = {0, 0, 0, 0, 0, 0, 0};
                list = A_addStudent(list, size, args[0], args[1], args[2], pts);
                size++;
            }
        } else if (c == 'U') {
            if (comLen != 3) {
                printf("U should be followed by exactly 3 arguments.\n");
                continue;
            }
            args = parseArgs(input, 3);
            if (validUpdate(args[1], args[2])){
                U_updateStudent(list, size, args[0], atoi(args[1]), atoi(args[2]));
            }
        } else if (c == 'L') {
            L_list(list, size);
        } else if (c == 'W') {
            args = parseArgs(input, 1);
            W_writefile(args[0], list, size);
        } else if (c == 'O') {
            args = parseArgs(input, 1);
            list = O_fromfile(args[0], list, sPtr);
        } else if (c == 'Q') {
            Q_quit(list, size);
            break;
        } else {
            printf("Invalid command %c\n", c);
        }

        if (comLen > 0) freeArgs(args, comLen);
        
    }

    return 0;
}

Student* A_addStudent(Student* ret, int len, char* num, char* lastN,
                      char* firstN, int* points) {
    ret = realloc(ret, len * SIZE + SIZE);

    Student stu;

    stu.lastName = malloc(strlen(lastN) + 1);
    stu.firstName = malloc(strlen(firstN) + 1);

    strncpy(stu.studentNumber, num, 7);
    strncpy(stu.lastName, lastN, strlen(lastN) + 1);
    strncpy(stu.firstName, firstN, strlen(firstN) + 1);

    for (int i = 0; i < 7; i++){
        stu.points[i] = points[i];
    }

    ret[len] = stu;

    printf("SUCCESS\n");
    return ret;
}

void U_updateStudent(Student* list, int len, char* num, int round, int points) {

    for (int i = 0; i < len; i++){
        if (strcmp(num, list[i].studentNumber) == 0){
            list[i].points[6] -= list[i].points[round-1];
            list[i].points[round-1] = points;
            list[i].points[6] += points;
            printf("SUCCESS\n");
            return;
        }
    }

    printf("Student %s is not in the database.\n", num);
}

void L_list(Student* list, int len) {
    if (list == NULL) {
        printf("Error, database is NULL\n");
        return;
    }

    qsort(list, len, SIZE, compareInts);

    for (int i = 0; i < len; i++) {
        printf("%s %s %s %d %d %d %d %d %d %d\n", list[i].studentNumber,
               list[i].lastName, list[i].firstName, list[i].points[0],
               list[i].points[1], list[i].points[2], list[i].points[3],
               list[i].points[4], list[i].points[5], list[i].points[6]);
    }

    printf("SUCCESS\n");
}

void W_writefile(char* filename, Student* list, int len) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Opening the file failed\n");
        return;
    }

    for (int i = 0; i < len; i++) {
        fprintf(file, "%s %s %s %d %d %d %d %d %d %d", list[i].studentNumber,
                list[i].lastName, list[i].firstName, list[i].points[0],
                list[i].points[1], list[i].points[2], list[i].points[3],
                list[i].points[4], list[i].points[5], list[i].points[6]);
        fputc('\n', file);
    }

    fclose(file);

    printf("SUCCESS\n");
}

Student* O_fromfile(char* filename, Student* list, int* lenPtr) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s for reading.\n", filename);
        return list;
    }
    int s = *lenPtr;

    int len = line_count(
        filename);  // laskee rivimäärän laskemalla \n määrän tiedostossa
    *lenPtr = len; // päivitä uuden taulukon koko pointterilla

    Student* ret = malloc(SIZE * len);

    for (int i = 0; i < s; i++) {  // vapauta vanha taulukko
        free(list[i].lastName);
        free(list[i].firstName);
    }

    free(list);

    char lastName[1000];
    char firstName[1000];

    for (int i = 0; i < len; i++) {
        fscanf(file, "%s%s%s%d%d%d%d%d%d%d", ret[i].studentNumber, lastName,
               firstName, &ret[i].points[0], &ret[i].points[1],
               &ret[i].points[2], &ret[i].points[3], &ret[i].points[4],
               &ret[i].points[5], &ret[i].points[6]);

        ret[i].lastName = malloc(strlen(lastName) + 1);
        ret[i].firstName = malloc(strlen(firstName) + 1);

        strncpy(ret[i].lastName, lastName, strlen(lastName) + 1);
        strncpy(ret[i].firstName, firstName, strlen(firstName) + 1);
    }

    fclose(file);

    printf("SUCCESS\n");
    return ret;
}

void Q_quit(Student* list, int len) {
    for (int i = 0; i < len; i++) {
        free(list[i].lastName);
        free(list[i].firstName);
    }

    free(list);

    printf("SUCCESS\n");
}

// validation functions

int validAdd(Student* list, int len, char* num) {
    // return 1 if ok, else 0

    int numLen = strlen(num);

    if (numLen > 6){
        printf("Student number %s cannot be more than 6 digits.\n", num);
        return 0;
    }

    for (int i = 0; i < numLen; i++){
        if (isdigit(num[i]) == 0){
            printf("Error, student number is not numeric.\n");
            return 0;
        }
    }

    for (int i = 0; i < len; i++){
        if (strcmp(num, list[i].studentNumber) == 0){
            printf("Student %s is already in the database.\n", num);
            return 0;
        }
    }

    return 1;
}

int validUpdate(char* round, char* points) {
    // return 1 if ok, else 0

    if (round[0] == '-'){
        printf("Rounds cannot be negative.\n");
        return 0;
    }

    for (int i = 0; i < (int) strlen(round); i++){
        if (isdigit(round[i]) == 0){
            printf("Error, round is not numeric.\n");
            return 0;
        }
    }


    if (points[0] == '-'){
        printf("Student cannot have negative points.\n");
        return 0;
    }

    for (int i = 0; i < (int) strlen(points); i++){
        if (isdigit(points[i]) == 0){
            printf("Error, points is not numeric.\n");
            return 0;
        }
    }

    int r = atoi(round);

    if (r < 1 || 6 < r){
        printf("Round cannot be less than 1 or larger than 6\n");
        return 0;
    }

    
    return 1;
}

// auxiliary functions

int compareInts(const void* a, const void* b) {
    Student* aa = (Student*)a;
    Student* bb = (Student*)b;

    return bb->points[6] - aa->points[6];
}

int line_count(const char* filename) {
    int ret = 0;

    FILE* file = fopen(filename, "r");
    if (!file) {
        return 0;
    }
    char c = fgetc(file);
    char prev;

    if (c == EOF) return 0;

    while (c != EOF) {
        if (c == '\n') ret++;

        prev = c;
        c = fgetc(file);
    }

    if (prev != '\n') ret++;
    fclose(file);
    return ret;
}

char** parseArgs(char* s, unsigned char count) {
    char** ret = malloc(8);
    char* aux = strtok(s, " ");

    ret[0] = malloc(strlen(aux) + 1);
    strcpy(ret[0], aux);

    for (int i = 1; i < count; i++) {
        aux = strtok(NULL, " ");
        ret = realloc(ret, i * 8 + 8);

        ret[i] = malloc(strlen(aux) + 1);
        strcpy(ret[i], aux);
    }

    return ret;
}

void freeArgs(char** args, int len) {
    for (int i = 0; i < len; i++) {
        free(args[i]);
    }

    free(args);
}

int argsLen(char* s, char* i) {
    int ret = 0;

    char* aux = strtok(s, " ");
    while (aux != NULL) {
        aux = strtok(NULL, " ");
        ret++;
    }
    if (i[strlen(i) - 2] == ' ')
        ret--;  // strktok toimii väärin jos viimeinen merkki on väli,
                // tämä ratkaisee sen ongelman

    return ret;
}
