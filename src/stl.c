#include "../include/stl.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

char *getDBPath(void) { return strcat(getenv("HOME"), "/.cache/tasks.db"); }

// load database into memmory
Task_t *loadDB(FILE *dbptr, int *task_array_size) {
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

    Task_t *task_array = (Task_t *)calloc(1, sizeof(Task_t));
#ifdef DEBUG
    RED;
    fputs("Reading database to task_ptr\n", stdout);
    printf("task_ptr: %p\n", task_array);
    RST;
#endif
    // main read loop
    while (fscanf(dbptr, "%[^:]:%[^:]:%d:%u\n ", temp_task_name, temp_category,
                  &temp_priority, &temp_done) != EOF) {
      // expand mem
      task_array = (Task_t *)realloc(task_array, (cnt + 1) * sizeof(Task_t));
      // ensure memmory is empty
      memset(task_array[cnt].task_name, 0, strlen(task_array[cnt].task_name));
      memset(task_array[cnt].category, 0, strlen(task_array[cnt].category));
      // ensure strings are null terminated
      temp_task_name[254] = '\0';
      temp_category[254] = '\0';
      task_array[cnt].task_name[254] = '\0';
      task_array[cnt].category[254] = '\0';
      // copy data to struct
      strncat(task_array[cnt].task_name, temp_task_name,
              sizeof(temp_task_name) + 1);
      strncat(task_array[cnt].category, temp_category,
              sizeof(temp_task_name) + 1);
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
int exportDB(FILE *dbptr, Task_t **task_array, int task_array_size) {
#ifdef DEBUG
  RED;
  fputs("Exporting tasks to database:", stdout);
  printf("Tasknum: %d, task_ptr_addr: %p\n", task_array_size, *task_array);
  RST;
#endif
  if (dbptr != NULL) {
    for (int i = 0; i < task_array_size; i++) {
#ifdef DEBUG
      RED;
      printf("EXP: %s:%s:%d:%d\n", (*task_array)[i].task_name,
             (*task_array)[i].category, (*task_array)[i].prio,
             (*task_array)[i].done);
      RST;
#endif
      fprintf(dbptr, "%s:%s:%d:%u\n", (*task_array)[i].task_name,
              (*task_array)[i].category, (*task_array)[i].prio,
              (*task_array)[i].done);
    }
    return 0;
  } else {
    return 1;
  }
}
// print colorcoded taskname
void taskPrinter(Task_t *task_array, int i) {
  // check prio
  switch (TASK(i).prio) {
  case 0:
    WHITE;
    printf("%d: %s\n", i + 1, TASK(i).task_name);
    RST;
    break;
  case 1:
    GREEN;
    printf("%d: %s\n", i + 1, TASK(i).task_name);
    RST;
    break;
  case 2:
    RED;
    printf("%d: %s\n", i + 1, TASK(i).task_name);
    RST;
    break;
  default:
    WHITE;
    printf("%d: %s\n", i + 1, TASK(i).task_name);
    RST;
    break;
  }
}

// print all tasks
int printTasks(Task_t *task_array, int task_array_size, char *category) {
  if (!strcmp(category, "")) {
    // loop over loaded whole tasklist
    FAT;
    fputs("all tasks:\n", stdout);
    RST;
    if (!task_array_size) {
      fputs("no tasks to print!\n", stdout);
    } else if (task_array_size == 1) {
      printf("- ");
      taskPrinter(task_array, 0);
    } else {
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
int addTask(Task_t **task_array, int *task_array_size, char *task_name,
            char *category, priority_t prio, unsigned int done) {
  // update array size
  (*task_array_size)++;
  // expand mem
  (*task_array) =
      (Task_t *)realloc((*task_array), ((*task_array_size) * sizeof(Task_t)));
  // add new task
  int i = (*task_array_size) - 1;

  // ensure that memmory is empty
  memset((*task_array)[i].task_name, 0, strlen((*task_array)[i].task_name));
  memset((*task_array)[i].category, 0, strlen((*task_array)[i].category));
  // ensure strings null terminated
  task_name[254] = '\0';
  category[254] = '\0';
  (*task_array)[i].task_name[254] = '\0';
  (*task_array)[i].category[254] = '\0';
  // copy data to array
  strncat((*task_array)[i].task_name, task_name, sizeof(task_name) + 1);
  strncat((*task_array)[i].category, category, sizeof(task_name) + 1);
  (*task_array)[i].prio = prio;
  (*task_array)[i].done = done;

#ifdef DEBUG
  printf("ADDING:READ: %s:%s:%d:%d\n", task_name, category, prio, done);
  printf("ADDING:ARRAY: %s:%s:%d:%d\n", (*task_array)[i].task_name,
         (*task_array)[i].category, (*task_array)[i].prio,
         (*task_array)[i].done);
#endif

  return 0;
}

// remove task from current tasklist
int removeTask(Task_t **task_array, int *task_array_size, char *index) {
  // check if index is valid
  unsigned int iIndex = atoi(index);
#ifdef DEBUG
  printf("%s:%d\n", index, iIndex);
#endif
  if (iIndex - 1 > *task_array_size || iIndex < 1) {
    return 1;
  } else {
    // move all following elements one index down
    for (int i = iIndex - 1; i < (*task_array_size) - 1; i++) {
      strcpy((*task_array)[i].task_name, (*task_array)[i + 1].task_name);
      strcpy((*task_array)[i].category, (*task_array)[i + 1].category);
      (*task_array)[i].prio = (*task_array)[i + 1].prio;
      (*task_array)[i].done = (*task_array)[i + 1].done;
    }
    // shrink memmory
    (*task_array) = (Task_t *)realloc((*task_array),
                                      (*task_array_size - 1) * sizeof(Task_t));
    (*task_array_size)--;
    return 0;
  }
}

int checkCat(char **cat_list, unsigned int list_size, char *cat_check) {
  if (!list_size) {
    return 0;
  } else {
    for (int i = 0; i < list_size; i++) {
      printf("%d:%s\n", i, *(cat_list + i));
      if (!strcmp(*(cat_list + i), cat_check)) {
        return 0;
      } else {
        continue;
      }
    }
    return 1;
  }
}

int printGroups(Task_t *task_array, int task_array_size) {
  char **category_list = (char **)calloc(1, sizeof(char *));
  *category_list = (char *)calloc(1, sizeof(char));
  unsigned int category_count = 0;
  for (int i = 0; i < task_array_size; i++) {
    printf("cat: %s\n", task_array[i].category);
    if (!checkCat(category_list, category_count, task_array[i].category)) {
      printf("UNIQUE!!\n");
      category_count++;
      // this is a mess and doesnt work
      category_list =
          (char **)realloc(category_list, category_count * sizeof(char *));
      *(category_list + (category_count - 1)) =
          (char *)realloc(*(category_list + (category_count - 1)),
                          strlen(task_array[i].category) + 1);
      strncpy(category_list[category_count - 1], task_array[i].category,
              sizeof(task_array[i].category));
    } else {
      continue;
    }
  }
#ifdef DEBUG
  printf("unique categories: %d\n", category_count);
  for (int i = 0; i < category_count; i++) {
    printf("%s\n", category_list[i]);
  }
#endif

  return 0;
}
