# Task Prioritization System

A C++ command-line application designed for ranking and managing tasks using a custom priority algorithm. The system utilizes a priority queue for task ordering and integrates SQLite for local data persistence.

## Features

* **Priority Algorithm:** Dynamically ranks tasks based on a weighted combination of importance and urgency.
* **Persistent Storage:** Integrates the SQLite C API to ensure tasks and metadata are saved across sessions.
* **Data Structures:** Utilizes a priority queue for efficient task management and retrieval.
* **CLI Interface:** A fully text-based command-prompt application for rapid data entry and processing.

## Project Structure

* `main.cpp`: Handles the application entry point and user interface logic.
* `TaskManager.h`: Manages the collection of tasks and database interactions.
* `task.h`: Defines the task object model and priority calculation logic.
* `CMakeLists.txt`: Build configuration file for cross-platform compilation.
* `sqlite3.c / sqlite3.h`: SQLite database engine source files.

## Technical Implementation

### Priority Logic
The system distinguishes between "urgent" and "important" tasks using a custom weight-based formula. This ensures that long-term critical goals are not overshadowed by immediate but low-impact tasks.

### Persistence Layer
To avoid external dependencies during installation, the SQLite database engine is included as an amalgamation. The `TaskManager` class handles the SQL queries required to create tables, insert records, and sync the local priority queue with the disk.

## Build Instructions

### Prerequisites
* CMake 3.10 or higher
* C++17 compatible compiler (GCC, Clang, or MSVC)

### Compilation
1. Create a build directory:
   `mkdir build && cd build`
2. Generate build files:
   `cmake ..`
3. Compile the project:
   `make`

### Execution
Run the generated executable:
`./TaskPrioritizer`
