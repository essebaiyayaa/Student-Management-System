
# Student Management System (C Project)

A simple yet functional console-based student information management system written in C. This project allows users to manage student records, handle their grades, and generate academic reports using linked lists and file persistence.

## Project Structure

```
Cproject/
├── main.c             # Core implementation of the system
├── CMakeLists.txt     # CMake build configuration
├── .git/              # Git version control directory
```
## Features

-  Add, modify, and delete student records
-  Manage grades for multiple modules per student
-  Search students by ID, name, or age
-  Generate detailed student reports
-  Display academic performance summaries
-  Save and load data from files to ensure persistence

---

## Data Structures

- `Student`: ID, name, surname, age, average, list of notes
- `Note`: module name, grade
- `StudentList`: linked list to manage dynamic entries

---

## ⚙️ How to Build and Run

### Requirements
- GCC compiler
- CMake (optional but recommended)

### Build using CMake (Recommended)

```bash
cd Cproject
cmake .
make
./Cproject
```

### Or Compile manually

```bash
cd Cproject
gcc main.c -o student_manager
./student_manager
```

---

## Sample Functionalities in `main.c`

- `createStudent()`, `addStudent()`, `modifyStudent()`, `deleteStudent()`
- `addNote()`, `modifyNote()`, `deleteNote()`
- `searchById()`, `searchByAge()`, `searchByName()`
- `generateReport()`, `summarizePerformance()`
- File persistence using `saveToFile()` and `loadFromFile()`

---

## Future Enhancements

- Add GUI (e.g., with GTK or Qt)
- Migrate to database backend (e.g., SQLite)
- Implement authentication system
- Export reports to PDF

---

## Supervised by
**Prof. Hachchane**

## Developed by

- **Aya Essebaiy**
- **Fatima El Messaoudi**

## 📅 Academic Year: 2024/2025
