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

