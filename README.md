# Unizy - University Management System

A C++17 console application

## Features

- Multi-role authentication (Student, Teacher, Admin, Dining Authority, PR Admin)
- Dining system with meal management and token lifecycle
- Time simulation with persistent state
- Public relations notice management
- Binary file persistence

## OOP Features

| Feature | Location |
|---------|----------|
| **Classes & Objects** | [`System/Users/`](https://github.com/thehav0k/unizyy/tree/master/System/Users/), [`System/Modules/Meal/`](https://github.com/thehav0k/unizyy/tree/master/System/Modules/Meal/), [`Core/Models/`](https://github.com/thehav0k/unizyy/tree/master/Core/Models/) |
| **Constructors & Destructors** | All class files |
| **Inheritance** | [`System/Users/`](https://github.com/thehav0k/unizyy/tree/master/System/Users/) - User base class inherited by Student, Teacher, Admin classes |
| **Association** | [`System/Modules/Meal/meal.h`](https://github.com/thehav0k/unizyy/blob/master/System/Modules/Meal/meal.h), [`System/Users/`](https://github.com/thehav0k/unizyy/tree/master/System/Users/) |
| **Aggregation** | [`Core/Models/Notice.h`](https://github.com/thehav0k/unizyy/blob/master/Core/Models/Notice.h) - Notice contains Date object, [`System/Users/student.h`](https://github.com/thehav0k/unizyy/blob/master/System/Users/student.h) - Student contains department, hall enums |
| **File Streams** | [`Core/Database/DatabaseManager.cpp`](https://github.com/thehav0k/unizyy/blob/master/Core/Database/DatabaseManager.cpp) |
| **Multifile Program** | Project structure |
| **Dynamic Binding/Polymorphism** | [`System/Authentication/auth.cpp`](https://github.com/thehav0k/unizyy/blob/master/System/Authentication/auth.cpp), [`System/Interface/`](https://github.com/thehav0k/unizyy/tree/master/System/Interface/) |
| **Virtual Functions** | [`System/Users/user.h`](https://github.com/thehav0k/unizyy/blob/master/System/Users/user.h), [`System/Interface/`](https://github.com/thehav0k/unizyy/tree/master/System/Interface/) |
| **Pointers** | [`Core/Database/DatabaseManager.cpp`](https://github.com/thehav0k/unizyy/blob/master/Core/Database/DatabaseManager.cpp) |
| **STL & Templates** | [`Core/Database/DatabaseManager.h`](https://github.com/thehav0k/unizyy/blob/master/Core/Database/DatabaseManager.h) - Generic template methods for database operations |
| **Templates** | [`Core/Database/DatabaseManager.h`](https://github.com/thehav0k/unizyy/blob/master/Core/Database/DatabaseManager.h) - Template methods for type-safe object persistence, [`Core/Utils/StringHelper.h`](https://github.com/thehav0k/unizyy/blob/master/Core/Utils/StringHelper.h) - Template functions for string operations |
| **Operator Overloading** | [`Core/Models/date.h`](https://github.com/thehav0k/unizyy/blob/master/Core/Models/date.h) - Stream insertion operator overloading |
| **Friend Functions** | [`Core/Models/date.h`](https://github.com/thehav0k/unizyy/blob/master/Core/Models/date.h) - Friend function for `operator<<` overloading |
| **Enumerations (enum class)** | [`System/Modules/Meal/meal.h`](https://github.com/thehav0k/unizyy/blob/master/System/Modules/Meal/meal.h) - MealType, MealRating, TokenStatus; [`Core/Models/`](https://github.com/thehav0k/unizyy/tree/master/Core/Models/) - Designation, Department, Hall, AdminType |
