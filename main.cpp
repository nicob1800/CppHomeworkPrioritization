#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <regex>
#include <vector>
#include <queue>
#include "task.h"


using namespace std;



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

    int day, month, year;
    while (true) {
        cout << "Date (DD-MM-YYYY): ";
        cin >> dueDRaw;

        // 1. Tokenize the input
        vector<string> theseTokens;
        sregex_token_iterator thisBegin(dueDRaw.begin(), dueDRaw.end(), re, -1);
        copy(begin, end, back_inserter(theseTokens));

        // 2. Validate format and attempt conversion
        try {
            if (theseTokens.size() != 3 || theseTokens[0].length() != 2 ||
                theseTokens[1].length() != 2 || theseTokens[2].length() != 4) {
                throw std::invalid_argument("Format error");
                }

            day = stoi(theseTokens[0]);
            month = stoi(theseTokens[1]);
            year = stoi(theseTokens[2]);

            // If we reached here, inputs are valid numbers. Break the loop!
            break;
        } catch (...) {
            cout << "ERROR! Invalid date or format. Please use DD-MM-YYYY (e.g., 19-01-2026)." << endl;
            // The loop continues, asking for the date again.
        }
    }
    return {name, diff, task::makeDue(year, month, day), importance};



}
int main() {
    //max heap priority queue

    priority_queue<task> pq;

    cout<<pq.size()<<endl;
    if (pq.empty()) {
        cout << "priority queue is empty!" << endl;
    }else {
        const task topTask = pq.top();
        cout<<topTask.getName()<<endl;
    }

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

