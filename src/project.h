#ifndef _PROJECT__H_
#define _PROJECT__H_

typedef struct {
    char studentNumber[7];
    char *firstName;
    char *lastName;
    int points[7];


} Student;

Student* A_addStudent(Student* ret, int len, char* num, char* lastN, char* firstN, int* points);
void U_updateStudent(Student* list, int len, char* num, int round, int points);
void L_list(Student* list, int len);
void W_writefile(char* filename, Student* list, int len);
Student* O_fromfile(char* filename, Student* list, int* lenPtr);
void Q_quit(Student* list, int len);

int validAdd(Student* list, int len, char* num);
int validUpdate(char* round, char* points);

int compareInts(const void* a, const void* b);
int line_count(const char* filename);

char** parseArgs(char* s, unsigned char count);
void freeArgs(char** args, int len);
int argsLen(char* s, char* i);

#endif //! _PROJECT__H_