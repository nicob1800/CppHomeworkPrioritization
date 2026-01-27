#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <regex>
#include <vector>
#include "task.h"
#include "TaskManager.h"


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

    cout << "Difficulty (/10): ";
    while (!(cin >> diff)) {
        cout << "Invalid input! Please enter a number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cout << "Importance (/10): ";
    while (!(cin >> importance)) {
        cout << "Invalid input! Please enter a number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }



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
    TaskManager manager;

    manager.loadTasksFromDB(); //HYDRATE TASK MANAGER
    std::cout << "System started. " << manager.taskCount() << " tasks loaded.\n";

    int choice = 0;
    while (choice!=5) {
        std::cout << "\n---MAIN MENU ---\n";
        std::cout << "1. Add New Task\n";
        std::cout << "2. View Priority List\n";
        std::cout << "3. Complete Task\n";
        std::cout << "4. Delete Task (Permanent)\n";
        std::cout << "5. Exit\n";
        std::cout << "Selection: ";

        if (!(std::cin >> choice)) {
            // Handle noninteger bad input to prevent infinite loops
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            // Use helper to create task
            task t = createNewTask();
            manager.addTask(t);
            std::cout << ">> Task saved to database successfully.\n";

        } else if (choice == 2) {
            // Get the sorted vector from the manager
            std::vector<task> list = manager.getSortedTasks();

            std::cout << "\n--- CURRENT PRIORITIES ---\n";
            if (list.empty()) {
                std::cout << "(No active tasks)\n";
            } else {
                for (const auto& t : list) {
                    // Only show tasks that are NOT completed
                    if (!t.getCompleted()) {
                        std::cout << "[Priority: " << t.getPriority() << "] "
                                  << t.getName()
                                  << " (Due: " << t.getTimeLeft() << " hrs)\n";
                    }
                }
            }

        } else if (choice == 3) {
            std::cout << "Enter exact name of task to complete: ";
            std::string target;
            std::cin >> std::ws; // Consume whitespace
            std::getline(std::cin, target);

            // This triggers the Soft Delete -> Rebuild -> Hydrate cycle
            manager.completeTask(target);
            std::cout << ">> Task status updated.\n";

        } else if (choice == 4) {
            std::cout << "Enter exact name of task to DELETE: ";
            std::string target;
            std::cin >> std::ws;
            std::getline(std::cin, target);

            manager.deleteTaskByName(target);
            std::cout << ">> Task removed from database.\n";
        }
    }

    return 0;
}

