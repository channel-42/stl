#include <string.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <ios>
#include <iostream>
#include <list>
#include <ostream>
#include <sstream>
#include <string>

using namespace std;

string HOME = getenv("HOME");
string PATH = HOME + "/.config/stl/db";

// terminal colors
const char *green = "\e[92m";
const char *red = "\e[31m";
const char *white = "\e[39m";

// TODO: ADD A LIST FOR EACH CATEGORY

// main task class
class Task {
 public:
  int priority;
  int index;
  string name;
  // string category;

  // Constructor
  Task(string n, int p, int i) {
    priority = p;
    name = n;
    index = i;
  }
};

string helptext =
    "Usage\nstl [-h] [-s] [-n] [-r int] [-ra]\nOptions:\n-h,      display this "
    "help page\n-s,      show tasks\n-n,      create new task\n-r,      remove "
    "task based on it's index\n-ra,     remove all tasks\n";

// define list iterator
list<Task>::iterator it;

// returns a list of task objects
void importFromDB(list<Task> &listOfTasks, string path);
// shows all tasks after import
void showTasks(list<Task> listOfTasks);
// removes a specific or all tasks
void removeTask(list<Task> &listOfTasks, int index, bool all);
// creates a new task and adds it to the tasklist !NO DUMPING TO DATABASE
void newTask(list<Task> &listOfTasks);
// dumps all open tasks to the database
void dumpTasks(list<Task> listOfTasks);

// main operations parsing
int main(int argc, char *argv[]) {
  list<Task> testList;
  importFromDB(testList, PATH);
  // TODO: Fix segmen.fault when no args given
  if (argc < 1) {
    cout << red << "No arguments provided. See stl -h" << endl;
    _Exit(1);
  } else if (!strcmp(argv[1], "-s")) {
    showTasks(testList);
  }

  else if (!strcmp(argv[1], "-n")) {
    newTask(testList);
    dumpTasks(testList);
  }

  else if (!strcmp(argv[1], "-r")) {
    int index = stoi(argv[2]);
    removeTask(testList, index, false);
    dumpTasks(testList);
  }

  else if (!strcmp(argv[1], "-ra")) {
    removeTask(testList, 0, true);
    dumpTasks(testList);
  }

  else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
    cout << helptext << endl;
  }
}

// dumps all tasks that are in a tasklist
void dumpTasks(list<Task> listOfTasks) {
  // needed for remove()
  const char *chPATH = PATH.c_str();
  // delete old database
  remove(chPATH);
  // create new database and dump data
  ofstream NEWDB(PATH);
  for (it = listOfTasks.begin(); it != listOfTasks.end(); it++) {
    // TODO: edit this line (down) when introducing categories!
    NEWDB << "r " << it->priority << " " << it->name << endl;
  }
  NEWDB.close();
}
// create new Task from UI
void newTask(list<Task> &listOfTasks) {
  string name;
  int index, priority;
  // gather information
  cout << white << "Name: ";
  getline(cin, name);
  cout << white << "Prio (0-2): ";
  cin >> priority;
  index = listOfTasks.size() + 1;
  // add task
  listOfTasks.push_back(Task(name, priority, index));
  cout << white << "New task '" << name << "' created!" << endl;
}

// remove specific item or clear list
void removeTask(list<Task> &listOfTasks, int index, bool all) {
  // delete all items
  if (all) {
    listOfTasks.clear();
    // delete a specific item
  } else {
    for (it = listOfTasks.begin(); it != listOfTasks.end(); it++) {
      if (index == it->index) {
        it = listOfTasks.erase(it);
      }
    }
  }
}

// shows tasks with colorcoding according to priority
void showTasks(list<Task> listOfTasks) {
  // listOfTasks.sort([](Task lh, Task rh) { return lh.category < rh.category;
  // });
  cout << endl
       << white << "You have " << listOfTasks.size() << " tasks to do:" << endl;
  for (it = listOfTasks.begin(); it != listOfTasks.end(); it++) {
    if (it->priority == 0) {
      cout << white << "└── " << it->index << ": " << it->name << endl;
    }
    if (it->priority == 1) {
      cout << green << "└── " << it->index << ": " << it->name << endl;
    }
    if (it->priority == 2) {
      cout << red << "└── " << it->index << ": " << it->name << endl;
    }
  }
  if (listOfTasks.size() == 0) {
    cout << green << "Good job, you've completed all tasks!" << endl;
  }
}

// Create instances for each entry in database
void importFromDB(list<Task> &listOfTasks, string path) {
  int n = 1;
  ifstream DB(PATH);
  // catch missing database
  if (!DB) {
    cout << red << "No databse found in ./config/stl/db, creating new file..."
         << endl;
    ofstream oDB(PATH);
    oDB.close();
    cout << red << "Database was created, exiting..." << endl;
    _Exit(1);
  }
  // normal procedure
  else if (DB.is_open()) {
    string line, temp;
    // loop over lines in file
    while (getline(DB, line)) {
      int i = 0, prio;
      string name;
      istringstream iss(line);
      // loop over "words" in line and filter out name and prio
      while (iss >> temp) {
        i++;
        switch (i) {
          case 1:
            // will be used for category/task filtering later on
            break;
          case 2:
            prio = stoi(temp);
            break;
        }
        // allow names with whitespaces
        if (i >= 3) {
          name += temp;
          name += " ";  // insert missing whitespace
        }
      }
      // create a task with corresponding name and prio
      Task aTask(name, prio, n);
      listOfTasks.push_back(aTask);
      n++;
    }
    DB.close();
  }
}
