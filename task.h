//
// Created by Nico on 2026-01-19.
//
#pragma once

#ifndef HOMEWORKPRIORITIZER_TASK_H
#define HOMEWORKPRIORITIZER_TASK_H

#include <cmath>
#include <string>
#include <ctime>
#include <utility>

using std::string;

class task {
private:
    string name;
    int diff;
    time_t dueD;
    int importance;
    bool completed;
public:
    double timeLeft;
    double priorityVal;

    task(string name, const int diff, const time_t dueD, const int importance, const bool completed)
        : name(std::move(name)), diff(diff), dueD(dueD), importance(importance), completed(completed)
    {

        const double hoursLeft = difftime(dueD, time(nullptr)) / 3600.0;
        this->timeLeft = hoursLeft;
        if (completed) {this -> priorityVal = 0;}
        else if (hoursLeft <= 0) {
            //OVERDUE
            this->priorityVal = 1000000.0 + (static_cast<double>(importance) * std::abs(hoursLeft));
        } else {
            //FUTURE

            this->priorityVal = static_cast<double>(importance) / (hoursLeft + 0.1);

            //FOR EASY TASKS FIRST REPLACE ABOVE WITH BELOW

            // this->priorityVal = importance / ((hoursLeft + 0.1) * (diff + 1));
        }
    }

    [[nodiscard]] string getName() const { return name; }
    void setName(const string &newN) {this->name = newN;}

    [[nodiscard]] int getImportance() const { return importance; }
    void setImportance(const int newImportance) { this->importance = newImportance; }

    [[nodiscard]] int getDiff() const { return diff; }
    void setDiff(const int newD) {this->diff = newD;}

    [[nodiscard]] double getTimeLeft() const { return timeLeft; }
    void setTimeLeft(const double newTimeLeft) { this->timeLeft = newTimeLeft; }

    [[nodiscard]] double getPriority() const { return priorityVal; }
    void setPriority(const int newP) {this->priorityVal = newP;}

    [[nodiscard]] bool getCompleted() const { return completed; }
    void setCompleted(const bool newCompleted) { this->completed = newCompleted; }

    static time_t makeDue(const int year, const int month, const int day, const int hour = 0) {
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
#endif //HOMEWORKPRIORITIZER_TASK_H
