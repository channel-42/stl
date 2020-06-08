#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* DATABASE STYLE
g GROUP NAME
t prio taskname p
EG:
g main
t 1 do this thing
*/

/* a task
struct task {
  char *name;
  int prio;
};
*/

// terminal colors
const char *red = "\e[92m";
const char *green = "\e[31m";
const char *white = "\e[39m";

// main task struct
typedef struct task {
  int prio;
  char *name;
  struct task *next;
} task_t;

void AddTask(task_t *head, char *name, int prio);
void PrintTasks(task_t *head);
int RemoveTask(task_t **head, int index);
void ReadDB(task_t *head, char *path);

int main() {
  char *homedir = getenv("HOME");
  char *DBPath = strdup(strcat(homedir, "/.cache/stldb"));

  task_t *head = NULL;

  head = (task_t *)malloc(sizeof(task_t));
  ReadDB(head, DBPath);
  RemoveTask(&head, 2);
  PrintTasks(head);
}

// remove a task from struct
// TODO: remove tast from database by removing the corresponding line
int RemoveTask(task_t **head, int index) {
  int i;
  task_t *now = *head;
  task_t *temp_node = NULL;
  for (i = 0; i < index - 1; i++) {
    if (now->next == NULL) {
      return 1;
    }
    now = now->next;
  }
  temp_node = now->next;
  free(temp_node);
  return 0;
}

// read database for tasks and load into struct
void ReadDB(task_t *head, char *path) {
  FILE *file;
  file = fopen(path, "r");
  // create file if it doesnt exist
  if (file == NULL) {
    file = fopen(path, "w+");
    fclose(file);
  }
  char line[1024];
  // read every line
  while (fgets(line, 1024, file) != NULL) {
    if (line[0] == 't') {
      char *name = strndup(line + 4, strlen(line) - 4);
      // remove trailing newline
      strtok(name, "\n");
      char *prioChar = strndup(line + 2, 1);
      // convert string to int
      int prio = atoi(prioChar);
      AddTask(head, name, prio);
      free(prioChar);
    }
  }
  fclose(file);
}

// TODO: write to db function that inserts a new line and writes
void WriteDB(task_t *head, char *path) {
  FILE *file;
  file = fopen(path, "w+");
}

// Adds a tast to the local struct
void AddTask(task_t *head, char *name, int prio) {
  task_t *now = head;
  while (now->next != NULL) {
    now = now->next;
  }
  now->next = (task_t *)malloc(sizeof(task_t));
  now->next->name = name;
  now->next->prio = prio;
  now->next->next = NULL;
}

// main output funtions for displaying tasks
void PrintTasks(task_t *head) {
  int i = 1;
  task_t *now = head->next;
  while (now != NULL) {
    if (now->next != NULL) {
      printf("├── ");
    } else {
      printf("└── ");
    }
    if (now->prio == 1) {
      printf("%s%d  %s\n", green, i, now->name);
    } else if (now->prio == 2) {
      printf("%s%d  %s\n", red, i, now->name);
    } else {
      printf("%s%d  %s\n", white, i, now->name);
    }
    // reset color
    printf("%s", white);
    now = now->next;
    i++;
  }
  // reset color
  printf("%s", white);
}
