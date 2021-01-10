#ifndef STL_H
#define STL_H

#define TASK(i) task_array[i]
#define _STRINGIFY(s) #s
#define STRINGIFY(s) _STRINGIFY(s)

// text effects
#define WHITE fputs("\e[39m", stdout)
#define GREEN fputs("\e[92m", stdout)
#define RED fputs("\e[31m", stdout)
#define RST fputs("\e[0m", stdout)
#define FAT fputs("\e[1m", stdout)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum { low, medium, high } priority_t;

typedef struct {
  char task_name[255];
  char category[255];
  priority_t prio;
  unsigned int done;
} Task_t;

//load all
Task_t* loadDB(FILE* dbptr, int* task_array_size);
char* getDBPath(void);
int exportDB(FILE* dbptr, Task_t* task_array, int task_array_size);
int printTasks(Task_t* task_array, int task_array_size, char* category);
int addTask(Task_t * task_array, int *task_array_size, char* task_name, char* category, priority_t pro, unsigned int done);
void taskPrinter(Task_t* task_array, int i);

#endif
