//#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// macro helpers
#define TASK(i) task_array[i]
#define _STRINGIFY(s) #s
#define STRINGIFY(s) _STRINGIFY(s)

// text effects
#define WHITE fputs("\e[39m", stdout)
#define GREEN fputs("\e[92m", stdout)
#define RED fputs("\e[31m", stdout)
#define RST fputs("\e[0m", stdout)
#define FAT fputs("\e[1m", stdout)
//#define DEBUG

typedef enum { low, medium, high } priority_t;
typedef struct {
  char task_name[255];
  char category[255];
  priority_t prio;
  unsigned int done;
} Task_t;

// prototypes
Task_t* loadDB(FILE* dbptr, int* task_array_size);
int printTasks(Task_t* task_array, int* task_array_size);
void taskPrinter(Task_t* task_array, int i);

int main(void) {
  // STUFF FOR TESTING, NOT FINAL
  int task_array_size = 0;
  FILE* db = fopen("tasks.db", "r");
  if (db != NULL) {
    Task_t* task_array = loadDB(db, &task_array_size);
#ifdef DEBUG
    printf("SIZE:%d\nEG: 1. %s\n", task_array_size, TASK(1).task_name);
#endif
    printTasks(task_array, &task_array_size);
  } else {
    printf("Error! Check if DB-File exists\n");
  }
}

// load database into memmory
Task_t* loadDB(FILE* dbptr, int* task_array_size) {
  // catch db error
  if (dbptr == NULL) {
    return NULL;
  } else {
    // read vars
    char temp_task_name[255];
    char temp_category[255];
    unsigned int temp_priority;
    unsigned int temp_done;
    // other vars
    int cnt = 0;

    Task_t* task_array = (Task_t*)malloc(sizeof(Task_t));

    // main read loop
    while (fscanf(dbptr, "%[^:]:%[^:]:%d:%u\n", temp_task_name, temp_category,
                  &temp_priority, &temp_done) != EOF) {
      // expand mem
      task_array = (Task_t*)realloc(task_array, (cnt + 1) * sizeof(Task_t));

      // copy data into mem
      strcpy(task_array[cnt].task_name, temp_task_name);
      strcpy(task_array[cnt].category, temp_category);
      task_array[cnt].prio = temp_priority;
      task_array[cnt].done = temp_done;
#ifdef DEBUG
      printf("%s:%s:%d:%d\n", TASK(cnt).task_name, TASK(cnt).category,
             TASK(cnt).prio, TASK(cnt).done);
#endif
      cnt++;
    }
    *task_array_size = cnt;
    return task_array;
  }
}

// print colorcoded taskname
void taskPrinter(Task_t* task_array, int i) {
  // check piro
  switch (TASK(i).prio) {
    case 0:
      WHITE;
      printf("%s\n", TASK(i).task_name);
      RST;
      break;
    case 1:
      GREEN;
      printf("%s\n", TASK(i).task_name);
      RST;
      break;
    case 2:
      RED;
      printf("%s\n", TASK(i).task_name);
      RST;
      break;
    default:
      WHITE;
      printf("%s\n", TASK(i).task_name);
      RST;
      break;
  }
}

// print all tasks
int printTasks(Task_t* task_array, int* task_array_size) {
  FAT;
  fputs("ALL TASKS:\n", stdout);
  RST;
  // loop over loaded tasklist
  for (int i = 0; i < *task_array_size; i++) {
    if (!i) {
      // first element
      printf("┌ ");
      taskPrinter(task_array, i);
    } else if (i + 1 == *task_array_size) {
      // last element
      printf("└ ");
      taskPrinter(task_array, i);
    } else {
      // elements in between
      printf("├ ");
      taskPrinter(task_array, i);
    }
  }
  return 0;
}
