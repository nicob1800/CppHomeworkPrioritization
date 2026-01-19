#include <cmath>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <regex>
#include <vector>
#include <queue>
#include <ctime>

using namespace std;

class task {
private:
    string name;
    int diff = 0;
    time_t dueD = 0;
    int importance = 0;
public:

    double timeLeft;
    double priorityVal;

    task(const string &name, const int diff, const time_t dueD, const int importance) {
        this->name = name;
        this->diff = diff;
        this->dueD = dueD;
        this->importance = importance;
        const double hoursLeft = difftime(dueD, time(nullptr)) / 3600.0;
        this->timeLeft = hoursLeft;

        if (hoursLeft <= 0) {
            //OVERDUE
            this->priorityVal = 1000000.0 + (importance * std::abs(hoursLeft));
        } else {
            //FUTURE

            this->priorityVal = importance / (hoursLeft + 0.1);

            //FOR EASY TASKS FIRST REPLACE ABOVE WITH BELOW

            // this->priorityVal = importance / ((hoursLeft + 0.1) * (diff + 1));
        }
    }

    [[nodiscard]] string getName() const {
        return name;
    }

    [[nodiscard]] int getImportance() const {
        return importance;
    }

    [[nodiscard]] double getDiff() const {
        return diff;
    }

    [[nodiscard]] double getTimeLeft() const {
        return timeLeft;
    }


    static time_t makeDue(int year, int month, int day, int hour = 0) {
        struct tm t = {0};
        t.tm_year = year-1900;
        t.tm_mon = month-1;
        t.tm_mday = day;
        t.tm_hour = hour;
        t.tm_isdst = -1;
        return mktime(&t);
    }


    bool operator<(const task &other) const {
        return this->priorityVal < other.priorityVal;
    }
};

task createNewTask() {
    //Makes a new task programmatically from inputs from console

    //#### Variables ####
    string name;
    int diff, importance;
    string dueDRaw;

    //#### INPUTS ####
    cout << "Task name: ";
    cin >> ws;
    getline(cin, name);

    cout << "Difficulty: ";
    while (!(cin >> diff)) {
        cout << "Invalid input! Please enter a number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cout << "Importance: ";
    while (!(cin >> importance)) {
        cout << "Invalid input! Please enter a number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cout << "Date (DD-MM-YYYY): ";
    cin >> dueDRaw; //Raw text due date DD-MM-YYYY

    //DATE PROCESSING
    vector<string> tokens;
    const regex re("-"); //Separator
    sregex_token_iterator end;

    sregex_token_iterator begin(dueDRaw.begin(), dueDRaw.end(), re, -1);
    copy(begin, end, back_inserter(tokens));

    //#### FORMAT AUTHENTICATION ####
    while (tokens.size() != 3 || tokens[0].length() != 2 || tokens[1].length() != 2 || tokens[2].length() != 4) {
        cout << "ERROR! Format must be DD-MM-YYYY. Try again: ";
        cin >> dueDRaw; //Raw text due date DD-MM-YYYY

        tokens.clear();
        sregex_token_iterator newBegin(dueDRaw.begin(), dueDRaw.end(), re, -1);
        copy(newBegin, end, back_inserter(tokens));
    }

    int day = stoi(tokens[0]);
    int month = stoi(tokens[1]);
    int year = stoi(tokens[2]);
    cout << "Day: " << day << " Month: " << month << " Year: " << year << endl;

    return {name, diff, task::makeDue(year, month, day), importance};



}
int main() {
    //max heap priority queue

    priority_queue<task> pq;
    task newTask = task("NEW TASK", 2, task::makeDue(2026, 2, 2, 12), 11);
    task newTask1 = task("URGENT TASK", 5, task::makeDue(2026, 1, 22, 12), 15);
    task newTask2 = task("SUPER HARD", 2, task::makeDue(2026, 1, 22, 12), 12);

    pq.push(newTask2);
    pq.push(newTask);
    pq.push(newTask1);

    cout<<pq.size()<<endl;
    task topTask = pq.top();
    cout<<topTask.getName()<<endl;

    int times;
    cout<< "Tasks: ";
    cin >> times;
    while (times>0) {
        task thisTask = createNewTask();
        pq.push(thisTask);
        --times;
    }
    while (!pq.empty()) {
        cout << pq.top().getName() <<endl;
        pq.pop();
    }


}
void insert(priority_queue<task>& pq, const task& t) {
    pq.push(t);
}

