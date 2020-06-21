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
const char *red = "\e[92m";
const char *green = "\e[31m";
const char *white = "\e[39m";

class Task {
 public:
  int priority;
  string name;
  // string category;

  // Constructor
  Task(string n, int p) {
    priority = p;
    name = n;
  }

  // create new Task from UI
  void newTask() {
    cout << "Name: ";
    cin >> name;
    cout << "Prio (0-2): ";
    cin >> priority;
  }
};

// define list iterator
list<Task>::iterator it;

// returns a list of task objects
void importFromDB(list<Task> &listOfTasks, string path);
// shows all tasks after import
void showTasks(list<Task> listOfTasks);

// TODO: main operations
// only used for testing at the moment
int main() {
  list<Task>::iterator it;
  list<Task> testList;
  importFromDB(testList, PATH);
  showTasks(testList);
}

// shows tasks with colorcoding according to priority
void showTasks(list<Task> listOfTasks) {
  cout << white << "You have " << listOfTasks.size() << " tasks to do:" << endl;
  for (it = listOfTasks.begin(); it != listOfTasks.end(); it++) {
    if (it->priority == 0) {
      cout << white << "└── " << it->name << endl;
    }
    if (it->priority == 1) {
      cout << green << "└── " << it->name << endl;
    }
    if (it->priority == 2) {
      cout << red << "└── " << it->name << endl;
    }
  }
}
// Create instances for each entry in database
void importFromDB(list<Task> &listOfTasks, string path) {
  ifstream DB(PATH);
  if (DB.is_open()) {
    string line;
    string temp;
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
      Task aTask(name, prio);
      listOfTasks.push_back(aTask);
    }
    DB.close();
  }
}
