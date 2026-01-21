//
// Created by Nicol on 2026-01-20.
//
#pragma once
#include <queue>
#include <vector>
#include <memory>
#include <iostream>
#include "task.h"
#include "sqlite3.h"

class TaskManager {
private:
    std::priority_queue<task> m_workQueue;
    sqlite3 *m_db = nullptr;

public:

    TaskManager() {
        if (sqlite3_open("homework.db", &m_db) != SQLITE_OK) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(m_db) << std::endl;
        }
        initDatabase();
    }

    ~TaskManager() {
        sqlite3_close(m_db);
    }

    void initDatabase() const {
        const auto sql = "CREATE TABLE IF NOT EXISTS tasks ("
                          "ID INTEGER  PRIMARY KEY AUTOINCREMENT,"
                          "NAME TEXT NOT NULL,"
                          "DIFFICULTY INT NOT NULL,"
                          "IMPORTANCE INT NOT NULL,"
                          "DUE_DATE INT NOT NULL,"
                          "COMPLETED BOOL NOT NULL);";
        char* errMsg = nullptr;
        if (sqlite3_exec(m_db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cerr << "SQL Table Error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
    }

    void addTask(const task& t) {
        m_workQueue.push(t);
        saveTaskToDB(t);
    }

    void saveTaskToDB(const task& t) const {
        const std::string sql = "INSERT INTO TASKS(NAME, DIFFICULTY, IMPORTANCE,  DUE_DATE) VALUES ('" +
                            t.getName() + "', " +
                            std::to_string(t.getDiff()) + ", " +
                            std::to_string(t.getImportance()) + ", " +
                            std::to_string(static_cast<long>(t.getTimeLeft())) + ", " +
                            std::to_string(t.getCompleted()) + ");";
        char* errMsg = nullptr;
        if (sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cerr << "Save Error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
    }

    void loadTasksFromDB() {
        auto sql = "SELECT NAME, DIFFICULTY, IMPORTANCE, DUE_DATE, COMPLETED FROM TASKS;";

        auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
            auto * queue = static_cast<std::priority_queue<task>*>(data);

            std::string name = argv[0];
            int diff = std::stoi(argv[1]);
            int imp = std::stoi(argv[2]);
            bool comp = std::stoi(argv[3]);

            queue->emplace(name, diff, time(nullptr) + (std::stoi(argv[3]) * 3600), imp, comp);
            return 0;
        };

        char* errMsg = nullptr;
        sqlite3_exec(m_db, sql, callback, &m_workQueue, &errMsg);
    }

    [[nodiscard]] task getTopTask() const {
        if (m_workQueue.empty()) {
            return task {"No Tasks", 0, 0, 0, false};
        }
        return m_workQueue.top();
    }

    void popTop() {
        if (!m_workQueue.empty()) {
            m_workQueue.pop();
        }
    }

    [[nodiscard]] bool isEmpty() const {
        return m_workQueue.empty();
    }
    [[nodiscard]] size_t taskCount() const {
        return m_workQueue.size();
    }

    [[nodiscard]] std::vector<task> getSortedTasks() const {
        std::vector<task> sortedList;
        auto tempQueue = m_workQueue;
        while (!tempQueue.empty()) {
            sortedList.push_back(tempQueue.top());
            tempQueue.pop();
        }
        return sortedList;
    }
};