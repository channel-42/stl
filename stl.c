#include "stl.h"

#include <stdlib.h>
#include <string.h>

char* getDBPath(void) { return strcat(getenv("HOME"), "/.cache/tasks.db"); }

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
#ifdef DEBUG
    RED;
    fputs("Reading database to task_ptr\n", stdout);
    printf("task_ptr: %p\n", task_array);
    RST;
#endif
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
      RED;
      printf("TASK FOUND: %s:%s:%d:%d\n", TASK(cnt).task_name,
             TASK(cnt).category, TASK(cnt).prio, TASK(cnt).done);
      RST;
#endif
      cnt++;
    }
    *task_array_size = cnt;
    return task_array;
  }
}

// export all task_array to database, file has to be reopened or rewound
int exportDB(FILE* dbptr, Task_t* task_array, int task_array_size) {
#ifdef DEBUG
  RED;
  fputs("Exporting tasks to database:", stdout);
  printf("Tasknum: %d, task_ptr_addr: %p", task_array_size, task_array);
  RST;
#endif
  if (dbptr != NULL) {
    for (int i = 0; i < task_array_size; i++) {
      fprintf(dbptr, "%s:%s:%d:%u\n", TASK(i).task_name, TASK(i).category,
              TASK(i).prio, TASK(i).done);
    }
    return 0;
  } else {
    return 1;
  }
}
// print colorcoded taskname
void taskPrinter(Task_t* task_array, int i) {
  // check prio
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
int printTasks(Task_t* task_array, int task_array_size, char* category) {
  if (!strcmp(category, "")) {
    // loop over loaded whole tasklist
    FAT;
    fputs("all tasks:\n", stdout);
    RST;
    for (int i = 0; i < task_array_size; i++) {
      if (!i) {
        // first element
        printf("┌ ");
        taskPrinter(task_array, i);
      } else if (i + 1 == task_array_size) {
        // last element
        printf("└ ");
        taskPrinter(task_array, i);
      } else {
        // elements in between
        printf("├ ");
        taskPrinter(task_array, i);
      }
    }
  } else {
    // loop over tasklist but only print specified category
    FAT;
    printf("all tasks in '%s':\n", category);
    RST;
    int cat_cnt = 0;
    int cnt = 0;
    // get num of matches
    while (cnt < task_array_size) {
      if (!strcmp(TASK(cnt).category, category)) {
        cat_cnt++;
      }
      cnt++;
    }
    cnt = 0;
    for (int i = 0; i < task_array_size; i++) {
      if (!strcmp(TASK(i).category, category)) {
        if (!cnt) {
          // first element
          printf("┌ ");
          taskPrinter(task_array, i);
        } else if (cnt + 1 == cat_cnt) {
          // last element
          printf("└ ");
          taskPrinter(task_array, i);
        } else {
          // elements in between
          printf("├ ");
          taskPrinter(task_array, i);
        }
        cnt++;
      }
    }
  }
  return 0;
}

// adds a task to the current tasklist
int addTask(Task_t* task_array, int* task_array_size, char* task_name,
            char* category, priority_t prio, unsigned int done) {
  // expand mem
  task_array =
      (Task_t*)realloc(task_array, sizeof(task_array) + sizeof(Task_t));
  //update array size
  (*task_array_size)++;
  //add new task
  int i = *task_array_size - 1;
  strcpy(TASK(i).task_name, task_name);
  strcpy(TASK(i).category, category);
  TASK(i).prio = prio;
  TASK(i).done = done;
  return 0;
}
