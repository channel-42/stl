//#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DEBUG

#include "../include/stl.h"

int main(int argc, char *argv[]) {
  // STUFF FOR TESTING, NOT FINAL
  int task_array_size = 0;
  char *path = getDBPath();

#ifdef DEBUG
  RED;
  printf("DB_PATH:%s\n", path);
  for (int i = 0; i < argc; i++) {
    printf("argc: %d -> %s\n", i, argv[i]);
  }
  RST;
#endif
  FILE *db = fopen(path, "r");

  if (db != NULL) {
    // open and read
    Task_t *task_array = loadDB(db, &task_array_size);
    fclose(db);
    if (argc == 2) {
      char temp_name[255];
      char temp_category[255];
      int temp_prio;
      char command = *(argv[1] + 1);
      switch (command) {
      // list command all cats
      case 'l':
        printTasks(task_array, task_array_size, "");
        break;
      // add command
      case 'a':
        fputs("task name: ", stdout);
        scanf("%s", temp_name);
        fputs("category: ", stdout);
        scanf("%s", temp_category);
        fputs("priority (0-2): ", stdout);
        scanf("%d", &temp_prio);
        // add task
        addTask(&task_array, &task_array_size, temp_name, temp_category,
                temp_prio, 0);
        fputs("\nnew task added!\n", stdout);
        break;
      }
    } else if (argc == 3) {
      char command = *(argv[1] + 1);
      switch (command) {
      // list command for specific category
      case 'l':
        printTasks(task_array, task_array_size, argv[2]);
        break;
      // add task command
      case 'a':
        break;
      case 'r':
        if(!removeTask(&task_array, &task_array_size, argv[2])){
          fputs("\ntask removed!\n", stdout);
        }else{
          fputs("\ninvalid index!\n", stdout);
        }
        break;
      }
    }
    // write and close
    db = fopen(path, "w");
    exportDB(db, &task_array, task_array_size);
    // mem stuff
    fclose(db);
    // free(task_array);
  } else {
    printf("Error! Check if DB-File exists\n");
  }
  return 0;
}
