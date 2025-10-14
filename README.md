```markdown
# unizyy - University Management System

A C++17 console application for managing university operations including multi-role authentication, dining services, meal tokenization, time simulation, and persistent file storage.

## Features

- Multi-role authentication (Student, Teacher, Admin, Dining Authority, PR Admin)
- Dining system with meal management and token lifecycle
- Time simulation with persistent state
- Public relations notice management
- Binary file persistence

## OOP Features

| Feature | Location |
|---------|----------|
| **Classes & Objects** | [`System/Users/`](System/Users/), [`System/Modules/Meal/`](System/Modules/Meal/), [`Core/Models/`](Core/Models/) |
| **Constructors & Destructors** | All class files |
| **Inheritance** | [`System/Users/`](System/Users/), [`System/Users/h`](System/Users/) |
| **Association** | [`System/Modules/Meal/meal.h`](System/Modules/Meal/meal.h), [`System/Users/`](System/Users/) |
| **File Streams** | [`Core/Database/DatabaseManager.cpp`](Core/Database/DatabaseManager.cpp) |
| **Multifile Program** | Project structure |
| **Dynamic Binding/Polymorphism** | [`System/Authentication/auth.cpp`](System/Authentication/auth.cpp), [`System/Interface/`](System/Interface/) |
| **Virtual Functions** | [`System/Users/user.h`](System/Users/user.h), [`System/Interface/`](System/Interface/) |
| **Pointers** | [`Core/Database/DatabaseManager.cpp`](Core/Database/DatabaseManager.cpp) |
| **STL & Templates** | [`Core/Database/DatabaseManager.h`](Core/Database/DatabaseManager.h) |
| **Operator Overloading** | [`Core/Models/date.h`](Core/Models/date.h) |



