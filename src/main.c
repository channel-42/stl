//#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//#define DEBUG

#include "../include/stl.h"

const char *USAGE = "\
usage: stl [OPTION] [VALUE] ...\n\
try 'stl -h' for more information.\n\
";

const char *HELP = "\
usage: stl [OPTION] [VALUE] ...\n\
Control a WLED host through the HTTP API.\n\n\
  -h            display this help\n\
  -l (GROUP)    list all tasks.\n\
  -a            add a new task.\n\
  -g            print sorted groups.\n\
  -r NUM        remove task by index.\n\
";

int main(int argc, char *argv[]) {
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
  // open db
  FILE *db = fopen(path, "r");
  if (db != NULL) {
    // create and load task array and close db
    Task_t *task_array = loadDB(db, &task_array_size);
    fclose(db);
    char temp_name[255];
    char temp_category[255];
    int temp_prio;

    // main ui loop
    extern char *optarg;
    int opt;
    while ((opt = getopt(argc, argv, ":al:r:gh")) != -1) {
      switch (opt) {
      case 'h':
        fprintf(stdout, "%s", HELP);
        break;
      case 'l':
        printTasks(task_array, task_array_size, optarg);
        break;
      case 'a':
        fprintf(stdout, "task name: ");
        scanf(" %[^\n]", temp_name);
        fprintf(stdout, "category: ");
        scanf(" %[^\n]", temp_category);
        fprintf(stdout, "priority (0-2): ");
        scanf("%d", &temp_prio);
        // add task
#ifdef DEBUG
        printf("MAIN:%s:%s:%d:0\n", temp_name, temp_category, temp_prio);
#endif
        addTask(&task_array, &task_array_size, temp_name, temp_category,
                temp_prio, 0);
        fprintf(stdout, "\nnew task added!\n");
        break;
      case 'r':
        if (!removeTask(&task_array, &task_array_size, atoi(optarg))) {
          fprintf(stdout, "\ntask removed!\n");
        } else {
          fprintf(stdout, "\ninvalid index!\n");
        }
        break;
      case 'g':
        printGroups(task_array, task_array_size);
        break;
      // catch optional args
      case ':':
        switch (optopt) {
        case 'l':
          printTasks(task_array, task_array_size, "");
          break;
        default:
          fprintf(stderr, "option -%c is missing a required argument\n",
                  optopt);
          exit(EXIT_FAILURE);
          break;
        }
        break;
      default:
        fprintf(stderr, "%s", USAGE);
        exit(EXIT_FAILURE);
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
    fprintf(stderr, "Error! Check if DB-File exists\n");
  }
  return 0;
}
