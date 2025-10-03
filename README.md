# Project Status and Progress Report (2025-10-04)

Summary
- Language/Build: C++17 with CMake. Console-based university management system.
- Current state: Architecture and headers are broadly scaffolded; many implementations are partial or stubs. Build currently fails due to missing sources referenced by CMake.

Build status
- Configure fails because CMakeLists.txt lists files that don’t exist:
  - System/Users/PublicRelationsAdmin.cpp
  - System/Interface/MenuInterface.cpp (and other Interface/*.cpp files listed)
- Effect: No targets are generated; compilation doesn’t start.

Source inventory (key files)
- Core
  - Database: DatabaseManager.h (rich API declared), DatabaseManager.cpp (empty)
  - Models: date.{h,cpp}, Notice.{h,cpp}, department.h, hall.h, Course.h, Designation.h, Transaction.h, admintype.h
  - Utils: StringHelper.h (implemented; char[] helpers, validation)
- System
  - Users: user.h (base), student.{h,cpp} (partially implemented; display), Teacher.{h,cpp} (partially implemented), Admin.{h,cpp} (minimal), DiningAuthority.{h,cpp} (stub), NoticeManager.{h,cpp} (stub)
  - Authentication: auth.{h,cpp} (stubs)
  - Modules/Meal: meal.{h,cpp} (large API in header; .cpp only defines constants/static members; most functions unimplemented)
  - Interface: directory referenced in CMake but not present in repo
- Top-level: main.cpp (appears to be a legacy demo using local vectors and different include paths)

What’s implemented vs planned
- Implemented
  - StringHelper utilities (string↔char[], validation rules)
  - Basic domain enums/models headers
  - Student/Teacher minimal objects and display
- Partially implemented
  - Meal system classes and TokenManager (APIs defined; implementations largely missing)
  - Admin model with simple display
- Not implemented / stubs
  - DatabaseManager logic (almost entirely; cpp empty)
  - Authentication flows
  - DiningAuthority, NoticeManager behavior
  - UI/Interface layer (files missing but referenced)

Key mismatches and risks
- CMake lists many non-existent sources, causing configure failure.
- main.cpp includes paths that don’t match the Core/… structure, and its logic duplicates persistence independently of DatabaseManager.
- Persistence design assumes POD-like binary writes with char[] fields; password hashing not yet implemented.

Next steps (recommended order)
1) Fix CMake sources to match reality
   - Temporarily remove missing entries: System/Users/PublicRelationsAdmin.cpp and all System/Interface/*.cpp files.
   - Keep only files that exist: main.cpp, Core/Models/date.cpp, Core/Models/Notice.cpp, Core/Database/DatabaseManager.cpp, System/Users/*.cpp (present), System/Modules/Meal/meal.cpp, System/Authentication/auth.cpp.
2) Make the project compile minimally
   - Option A: Comment-out or stub method bodies in headers/cpps that are referenced by the current build but unimplemented.
   - Option B: Trim main.cpp to a “hello world” or a tiny smoke test, then iteratively wire features.
3) Align main.cpp with project structure
   - Replace incorrect includes (use Core/... and System/... paths) and shift any ad-hoc persistence to DatabaseManager once implemented.
4) Implement DatabaseManager minimum viable set
   - Start with Student/Teacher add/load/update using binary files, then extend to Admin and tokens.
5) Flesh out Meal module
   - Implement Meal, MealToken, TokenManager persistence and basic flows; add simple CLI to exercise buy/use/review.
6) Re-introduce Interface layer gradually
   - Once core compiles, add Interface/*.cpp files and menus incrementally.
7) Security and validation
   - Add password hashing; keep StringHelper for char[] but avoid storing plaintext.
8) Tests and smoke checks
   - Add a small test target to verify token lifecycle and date simulation.

Observed build command and failure
- Ran: configure and build with CMake; configuration failed due to missing sources in CMakeLists.txt (see above).

Progress snapshot
- Repo structure in place; many classes and APIs drafted.
- Core utilities and some models are usable.
- Database and Meal logic need implementation.
- Build is currently red due to CMake source list drift vs repository.

Actionable checklist for “green build”
- [ ] Update CMakeLists.txt to remove/guard missing files.
- [ ] Adjust main.cpp includes and reduce scope until DatabaseManager is ready.
- [ ] Add minimal implementations or stubs for referenced methods.
- [ ] Re-run CMake configure/build and iterate on compile errors.

Once the build turns green, add a “How to Run” section pointing to the produced binary and minimal usage instructions.

---


# University Management System (unizyy)

> Comprehensive C++17 console application (CLion/CMake) for managing core university operations: authentication, users (Students, Teachers, Admins, Dining Authority, Public Relations Admin), dining hall meal publishing & tokenization, reviews, analytics scaffolding, date/time simulation, and persistent storage using simple binary/text files.

---
## 1. Key Highlights
- **Multi‑role authentication** (Student / Teacher / Admin / Dining Authority / PR Admin)
- **Dining / Meal Module** with meals, tokens, usage, reviews, halls
- **Token lifecycle**: Active → Used → Reviewed / Expired (explicit status persistence)
- **Date & Time Simulation** (days, months, hours, set date+hour) with persistence across sessions
- **Persistent storage** via lightweight binary `.dat` files (no external DB required)
- **Hall & Department models** with enum conversions
- **StringHelper utility** for safe `char[]` handling and validation (email/password)
- **Public Relations Admin**: notices (append/remove/list) persisted in text file
- **Cross‑platform (Mac/Linux/Win)** console UI (clear screen abstractions)

---
## 2. Feature Overview
| Area | Capabilities |
|------|--------------|
| Authentication | Login + role detection; registration flows per role with validation |
| Students | View dashboard, buy/use/review meal tokens, simulate time, profile ops |
| Teachers | Basic interface scaffold (extendable) |
| Admins | Simulation, system/report/database/profile placeholders |
| Dining Authority | Create/update/remove meals, adjust price/quantity/availability, view meals, future analytics hooks |
| Meal Tokens | Purchase (next day), validity checks vs simulated time, persistence, status transitions |
| Reviews | Attach rating & comment to used tokens (status → Reviewed) |
| Time Simulation | Advance days/months/hours, set exact date/hour, persistence in `simulation_state.dat` |
| Public Relations | Post/list/remove notices via PR admin (text file storage) |
| Persistence | Binary record sets per entity, simple append/update replacement strategy |
| Validation | Email domain rules, password strength, string sanitation, enum mapping safety |

---
## 3. Architecture
```
Core/
  Models/        -> Fundamental domain models (date, hall, department, admin types, notices)
  Utils/         -> StringHelper (validation & safe char[] conversions)
  Database/      -> DatabaseManager (static CRUD helpers, binary IO)
System/
  Authentication -> auth.{h,cpp} (registration/login wrappers over DatabaseManager)
  Users/         -> Concrete role classes + PR Admin + Dining Authority
  Modules/Meal   -> meal.{h,cpp} (Meals, MealToken, MealReview, TokenManager, MealUtils)
  Interface/     -> Console UI layers (MainMenu + role menus + common helpers)
Database/        -> Runtime binary storage (.dat files) (ignored by VCS except placeholder)
Meal Tokens/     -> Per-token human-readable receipts (.txt)
```
**Separation goals:**
- "Core" is pure data & utilities.
- "System" orchestrates flows and user interactions.
- UI layer drives scenario logic; business logic resides in modules.

---
## 4. Core Modules & Classes
### 4.1 Date Simulation (`date.h/.cpp`)
- Static simulation state: `simulationActive`, `simulatedDate`, `simulatedHour` (0–23 or -1 when not set)
- Methods: `SimulateDate`, `SimulateMonths`, `SimulateHours`, `setSimulatedDateTime`, `resetSimulation`
- Persistent state: `Database/simulation_state.dat`
- All time-sensitive token & meal checks read from simulation when active.

### 4.2 Meal & Token System (`meal.h/.cpp`)
**Classes:**
- `Meal`: name+description packed into `MealPackage` char buffer.
- `MealToken`: holds tokenNumber, studentEmail, mealName, mealType, hall, purchase & valid date, status, purchaseTime.
- `MealReview`: rating, comment, hall, date (attached to token after usage).
- `TokenManager`: in-memory vectors for active/used tokens, persistence to `active_tokens.dat`, `used_tokens.dat`.
- `MealUtils`: time-window logic, current time retrieval (simulation-aware).

**Token Status Flow:**
```
ACTIVE --(use within valid window)--> USED --(review)--> REVIEWED
ACTIVE --(date passes)----------------> EXPIRED
```
Explicit mapping via `MealToken::tokenStatusToString`.

### 4.3 Database Manager (`DatabaseManager.h/.cpp`)
- Provides static CRUD functions for each entity group (students, teachers, admins, dining authorities).
- Raw binary write/read: `writeObjectToBinary`, `readObjectFromBinary` (POD-style struct copies).

### 4.4 Public Relations Admin
- File: `PublicRelationsAdmin.{h,cpp}`
- Persisted notices: `Database/pr_notices.txt` using pipe-delimited lines: `title|body|date`.

### 4.5 Validation (`StringHelper.h`)
- Safe truncation with warnings.
- Email domain rule: must end with `@juniv.edu`, local part length >= 4.
- Password rule: length >= 6 and must contain upper, lower, digit.

---
## 5. Persistence Files (Binary / Text)
| File | Purpose |
|------|---------|
| `Database/students.dat` | Student records |
| `Database/teachers.dat` | Teacher records |
| `Database/admins.dat` | Admin records |
| `Database/dining_authorities.dat` | Dining authority records |
| `Database/active_tokens.dat` | Active meal tokens |
| `Database/used_tokens.dat` | Used / Reviewed tokens |
| `Database/meal_reviews.dat` | (Reserved – review persistence scaffold) |
| `Database/meals.dat` | Meal definitions |
| `Database/simulation_state.dat` | Date & hour simulation state |
| `Database/pr_notices.txt` | Public relations notices (text) |
| `Meal Tokens/*.txt` | Individual token receipt exports |

> NOTE: Some review persistence may still be in-memory; extend `meal_reviews.dat` logic if needed.

---
## 6. Meal Time Windows
| Meal Type | Window (Simulated Hour) | Notes |
|-----------|-------------------------|-------|
| Breakfast | 07–10 | Inclusive hours |
| Lunch     | 12–14 | Approximates 12:00–14:30 |
| Dinner    | 19–22 | Approximates 19:00–22:30 |

Windows enforced in `MealUtils::isWithinMealTime` and token usage via `MealToken::canBeUsed`.

---
## 7. Building & Running
### Prerequisites
- C++17 compiler (Clang / GCC / MSVC)
- CMake ≥ 3.20

### Configure & Build
```bash
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target unizyy -j
```
### Run
```bash
./unizyy
```
Or (CLion): Use the generated run configuration.

### Clean
```bash
rm -rf build
```

---
## 8. Simulation Usage
From Main Menu or any role menu (where available):
1. Choose simulation option.
2. Select: set date, advance days/months/hours, set date+hour, or reset.
3. Simulation persists automatically — restart program and it resumes.
4. Reset restores real time mode.

**Token Validity** relies on: `simulatedDate + simulatedHour` (or real system time if simulation inactive).

---
## 9. Token Lifecycle & Persistence
1. **Purchase**: creates token (valid date usually next day).
2. **Use**: allowed only if date matches current (simulation-aware) and within window.
3. **Review**: only after status = USED; sets status = REVIEWED and persists.
4. **Expiry**: if current date surpasses valid date before usage → status flagged EXPIRED (via cleanup logic before listing or explicit call).

### Potential Extension
- Automatic cleanup on login / periodic check.
- Auto purge REVIEWED / EXPIRED after N days.

---
## 10. Admin Roles
| Type | Intended Responsibilities (Current / Future) |
|------|----------------------------------------------|
| Transport | Placeholder (e.g., bus schedules) |
| HallDining | Oversee dining authority data, audit meals/tokens |
| PublicRelations | Manage PR notices (implemented) |
| Department | Departmental academic operations (scaffold) |
| SystemAdmin | Global configuration / backups / analytics |

---
## 11. Public Relations Notices
- Stored in a plain text file for simplicity.
- Format: `title|body|date` (UTF-8, one per line)
- Extend with: author email, severity, expiration date.

---
## 12. Error Handling & Logging
- Primarily console output (human readable).
- Warnings on string truncation (StringHelper::safeStringToCharArray).
- Minimal exception usage; binary IO uses early returns.
- Potential Improvement: centralized logger + log levels + rotating file logs.

---
## 13. Security & Validation
| Aspect | Current | Future Ideas |
|--------|---------|--------------|
| Password storage | Plain text in memory/files | Hash (bcrypt/Argon2) + salt |
| Email validation | Domain + structure | MX check / stronger regex |
| Input sanitation | Length + simple checks | Central sanitize/escape lib |
| Authorization | Role-based dispatch | Fine-grained permission matrix |
| Data integrity | Binary blob rewrite | Checksums / version headers |

---
## 14. Coding Conventions
- `snake_case.dat` for persistence files.
- `CamelCase` for classes, `UPPER_SNAKE` for constants (future standardization recommended).
- Inline enum->string helpers in header for simplicity & ODR safety (constexpr in future).
- `char[]` used for binary POD-style persistence (avoid std::string direct binary layout issues).

---
## 15. Extensibility Roadmap
| Area | Planned / Suggested Enhancements |
|------|----------------------------------|
| Review Persistence | Serialize reviews to `meal_reviews.dat` |
| Token Cleanup | Automated scheduled purge process |
| Reporting | Revenue & usage analytics (aggregate per hall/date) |
| Notice System | Rich metadata, filtering, expiry |
| Transport Module | Schedules, vehicle assignment, seat booking |
| Results Module | Grade publishing, transcript requests |
| User Interface | Theming, pagination, search/filter menus |
| Error Handling | Structured logging & error codes |
| Security | Password hashing, lockout, audit trails |
| Testing | Unit tests (GoogleTest / Catch2) for core logic |
| Packaging | Dockerfile / cross-compilation presets |

---
## 16. Known Limitations
- No concurrency control (simultaneous process writes could race).
- Entire dataset rewritten for certain operations (not memory-mapped incremental updates).
- Binary layout changes require rebuild + potential migration script.
- Meal reviews partially in-memory (extend persistence layer).

---
## 17. Contributing
1. Fork & clone.
2. Create a feature branch: `git checkout -b feature/my-improvement`.
3. Build & run to ensure no regressions.
4. Submit PR with concise description & rationale.

**Style Suggestions:**
- Keep functions short & single-purpose.
- Prefer pure functions in Core where possible.
- Add comments for non-trivial binary operations.

---
## 18. Quick Reference Commands
```bash
# Build (Debug)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j

# Run
./build/unizyy

# Reset simulation (in-app menu) or remove file
rm -f Database/simulation_state.dat

# Clean
rm -rf build
```

---
## 19. License
*Add your chosen license (e.g., MIT, Apache 2.0).*

---
## 20. Credits
Developed by . Inspiration: modular academic & campus management systems.

---
## 21. Support / Contact
Open an issue or provide feedback via repository issue tracker.

> _Happy hacking!_ 🔧
