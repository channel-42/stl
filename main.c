//#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stl.h"


int main(void) {
  // STUFF FOR TESTING, NOT FINAL
  int task_array_size = 0;
  char* path = getDBPath();
#ifdef DEBUG
  RED;
  printf("DB_PATH:%s\n", path);
  REST;
#endif
  FILE* db = fopen(path, "r");

  if (db != NULL) {
    // open and read
    Task_t* task_array = loadDB(db, &task_array_size);
    printTasks(task_array, task_array_size, "");
    fclose(db);

    // write and close
    db = fopen(path, "w");
    exportDB(db, task_array, task_array_size);
    // mem stuff
    fclose(db);
    free(task_array);
  } else {
    printf("Error! Check if DB-File exists\n");
  }
  return 0;
}
