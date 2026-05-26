# Grocery Expense Tracker

A console-based Grocery Expense Tracker developed in modern C++ for managing grocery inventory, monitoring expenses, and maintaining persistent storage using JSON serialization.

The application demonstrates practical implementation of Object-Oriented Programming, STL containers, filesystem handling, structured data persistence, and input validation in C++17.

---

## Overview

This project was developed to provide a lightweight and efficient grocery management system capable of:

- Tracking grocery items and expenses
- Categorizing purchases
- Generating budget summaries
- Predicting low-stock items
- Persisting data using JSON files

The application automatically stores and reloads user data, making it suitable for continuous day-to-day usage.

---

## Core Features

- Add grocery items with price, category, and stock duration
- View complete inventory in a formatted table
- Delete existing grocery items
- Generate category-wise budget reports
- Predict items running out within a specified period
- Automatic JSON-based data persistence
- Filesystem-based directory management
- Robust input validation and exception handling

---

## Technologies Used

- **Language:** C++17
- **Libraries & Tools:**
  - STL Containers
  - `<filesystem>`
  - `<fstream>`
  - `<iomanip>`
  - `nlohmann/json`

---

## Dependency

This project uses the following JSON library:

### JSON for Modern C++

Repository:  
https://github.com/nlohmann/json

---



## Functional Modules

### Grocery Item Management
Allows users to add, store, display, and remove grocery items dynamically.

### Budget Reporting
Calculates:
- Total expenditure
- Category-wise spending
- Percentage distribution across categories

### Runout Prediction
Identifies grocery items expected to run out within seven days.

### Persistent Storage
All data is stored automatically in:

```text
data/groceries.json
```

The application restores saved data during startup.

---

## Software Engineering Concepts Demonstrated

- Object-Oriented Programming
- Data Serialization
- Exception Handling
- Input Validation
- File Handling
- Filesystem Operations
- Structured Program Design
- STL Containers and Iteration
- Console UI Formatting

---

## Future Enhancements

Potential improvements include:

- Search and filtering functionality
- Edit/update item support
- Monthly analytics and visual reports
- GUI implementation
- SQLite database integration
- Authentication system
- Barcode scanning support

---

## Author

### Waqas Izhaan  
Computer System Engineer  
University of Engineering and Technology (UET) Peshawar

---

## License

This project is released under the MIT License.
