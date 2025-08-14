# Class & Enum Reference

Comprehensive reference for the core domain model, user roles, meal/token system, utilities, and supporting enums.

---
## Legend
- C = Constructor
- S = Static Method
- V = Virtual Method (overridden in subclass)
- [] = return type
- ( ) = parameters (overloaded ctors grouped)
- Char arrays are used for POD-friendly binary persistence.

---
## Core Enums
| Enum | Values | Notes |
|------|--------|-------|
| `AdminType` | Transport, HallDining, PublicRelations, Department, SystemAdmin | Admin role specialization |
| `department` | Department_of_Computer_Science_and_Engineering, Department_of_physics, Department_of_Mathematics | Academic departments with helper string converters |
| `AcademicPosition` (in Designation.h) | Professor, AssociateProfessor, AssistantProfessor, Lecturer | Teacher rank |
| `Halls` | Al_Beruni_Hall, Meer_Mosharraf_Hossain_Hall, ... , Bir_Pratik_Taramon_Bibi_Hall | University residential halls |
| `MealType` | BREAKFAST, LUNCH, DINNER | Token & meal classification |
| `MealRating` | POOR–EXCELLENT (1–5) | Review scale |
| `TokenStatus` | ACTIVE, USED, EXPIRED, REVIEWED | Token lifecycle |

---
## Utility Classes
### StringHelper (static)
| Method | Purpose |
|--------|---------|
| `stringToCharArray(src, dest)` (templated / sized) | Safe copy with truncation guard |
| `safeStringToCharArray(src, dest, fieldName)` | Copy + warning if truncated |
| `charArrayToString(char*)` | Wrap raw char array into std::string |
| `validateEmail(email)` | Basic domain & structure validation (`@juniv.edu`) |
| `validatePassword(pass)` | Length ≥6 with upper/lower/digit |
| `isValidString(str)` | Reject empty / all-space strings |

---
## Date
| Member | Type | Notes |
|--------|------|-------|
| `day` | int | Day (1–31) |
| `month` | int | Month (1–12) |
| `year` | int | 4-digit year |
| `simulationActive` (S) | bool | Global simulation flag |
| `simulatedDate` (S) | Date | Stored simulated date |
| `simulatedHour` (S) | int | Hour (0–23 / -1 disabled) |

| Method | Kind | Description |
|--------|------|-------------|
| `Date()` / `Date(d,m,y)` / `Date("DD-MM-YYYY")` | C | Constructors (parsing variant) |
| `getDay/Month/Year()` | | Accessors |
| `toString()` | | `DD-MM-YYYY` |
| Comparators (`== != < > <= >=`) | | Date ordering |
| `getNextDay() / getPreviousDay()` | | Arithmetic |
| `isToday/Tomorrow/Yesterday()` | | Relative checks |
| `getCurrentDate()` | S | Real or simulated date |
| `getTomorrowDate()` | S | Convenience |
| `SimulateDate(n)` | S | Advance n days |
| `SimulateMonths(n)` | S | Advance n months (clamps day) |
| `SimulateHours(n)` | S | Advance hours (auto day roll) |
| `setSimulatedDate(d)` | S | Activate simulation with date |
| `setSimulatedDateTime(d,h)` | S | Set date + hour |
| `getSimulatedHour()` | S | Returns simulated hour or -1 |
| `isSimulationActive()` | S | Flag query |
| `resetSimulation()` | S | Disable simulation |
| `saveSimulationState()` | S | Persist to `Database/simulation_state.dat` |
| `loadSimulationState()` | S | Restore simulation state |

---
## Base User Hierarchy
### User (abstract)
| Member | Type | Notes |
|--------|------|-------|
| `email` | char[100] | Stored email |
| `password` | char[32] | Stored password (plain text currently) |

| Method | Kind | Description |
|--------|------|-------------|
| `User(email, password)` / `User()` | C | Base initialization |
| `get/setEmail` | | Accessors with char[] bridging |
| `get/setPassword` | | Accessors |
| `display()` | V, pure | Must be implemented by derived roles |
| `~User()` | V | Polymorphic destruction |

### Student : User
| Member | Type | Notes |
|--------|------|-------|
| `name` | char[60] | Student full name |
| `age` | int | Age (16–35 validated externally) |
| `studentID` | char[15] | Unique student ID |
| `classRoll` | int | Class roll number |
| `dept` | department | Academic department |
| `batch` | int | Cohort year / batch |
| `hall` | Halls | Assigned residential hall |
| `balance` | double | Planned financial balance (future logic) |

| Method | Description |
|--------|-------------|
| Constructors (full / default) | Initialize fields |
| `get/set` for all members | Accessors & mutators |
| `display()` | Implements User::display |
| `ChangePassword()` | Placeholder behavior |
| `BuyMealToken()` / `TakeMeal()` / `ReviewMeal()` | High-level meal ops (UI triggers) |
| `ViewResult()` / `ViewNotice()` / `ViewClassSchedule()` / `ViewTransportSchedule()` | Stubs for future modules |
| Static CRUD: `loadAllStudents`, `add/update/deleteStudentInDB`, `findStudentByEmail/ID` | DatabaseManager wrappers |

### Teacher : User
| Member | Type | Notes |
|--------|------|-------|
| `name` | char[100] | Teacher name |
| `dept` | department | Department enum |
| `AP` | AcademicPosition | Rank / designation |

| Method | Description |
|--------|-------------|
| Constructors | Full & default |
| `set/getName` | Name management |
| `set/getDepartment` | Enum access |
| `getDepartmentName()` | Friendly string |
| `set/getRank()` / `getDesignation()` | Rank ops |
| `display()` | Role details |
| `gradeStudent(Student&, Course&)` | TBD grading flow |
| Static CRUD: `loadAllTeachers`, etc. | Persistence wrappers |

### Admin : User
| Member | Type | Notes |
|--------|------|-------|
| `name` | char[100] | Admin name |
| `adminType` | AdminType | Role specialization |

| Method | Description |
|--------|-------------|
| Constructors | Full & default |
| `get/setName` | Name access |
| `get/setAdminType` | Role type |
| `display()` | Shows core admin info |
| Static CRUD same pattern | Persistence wrappers |

### DiningAuthority : User
| Member | Type | Notes |
|--------|------|-------|
| `name` | char[100] | Authority name |
| `hallname` | Halls | Managed hall |

| Method Groups | Key Methods |
|--------------|-------------|
| Meal CRUD | `createMeal`, `updateMealAvailability/Quantity/Price`, `removeMeal` |
| Viewing | `viewAllMeals`, `viewMealsByType/Date`, `viewTodaysMeals`, `viewUpcomingMeals` |
| Tokens & Reviews (stubs) | `viewActiveTokens`, `viewUsedTokens`, `viewFoodReviews`, `viewReviewsByMeal`, `viewHallReviews`, `respondToReview` |
| Notices | `postNotice`, `viewNotices`, `removeNotice` |
| Analytics | `generateDailyReport`, `viewPopularMeals`, `viewRevenueReport` |
| Inventory | `updateInventory`, `viewLowStockMeals`, `restockMeal` |
| Static CRUD | add/update/delete/find |
| Helpers | `getMealsByHall/Type`, `getAvailableMeals`, `canManageHall` |

### PublicRelationsAdmin : User
| Member | Type | Notes |
|--------|------|-------|
| `name` | char[100] | PR admin name |
| (static) `NOTICE_DB_FILE` | string | Text notice storage file |

| Method | Description |
|--------|-------------|
| Constructors | Default + credential/name constructor |
| `get/setName` | Name mutation |
| `postNotice(title, body)` | Append notice with current/sim date |
| `loadNotices()` | Parse notices into vector pairs |
| `removeNotice(title)` | Title-match removal |
| `listNotices()` | Console render |

---
## Meal & Token System
### Meal
| Member | Type | Notes |
|--------|------|-------|
| `MealPackage` | char[300] | "Name - Description" combined storage |
| `mealType` | MealType | Category |
| `price` | double | Cost (BDT) |
| `availableQuantity` | int | Remaining portions |
| `isAvailable` | bool | Availability flag |
| `date` | char[12] | `DD-MM-YYYY` |
| `time` | char[10] | Scheduled time string |
| `hallName` | Halls | Associated hall |

| Method Group | Highlights |
|--------------|-----------|
| Accessors | `getMealName`, `getDescription`, scalar getters |
| Mutators | `setMealName/Description/Type/Price/AvailableQuantity/IsAvailable/Date/Time/HallName` |
| Ops | `orderMeal(quantity)` |
| Display | `displayMeal()` |
| Static Utils | `mealTypeToString`, `stringToMealType` |
| Persistence (Static) | `loadAllMealsFromDatabase`, `loadMealsByHall/Date/Type`, `deleteMealFromDatabase`, `initializeMealDatabase`, `displayAllMeals`, `getMealCount` |
| Persistence (Instance) | `saveMealToDatabase`, binary write/read helpers |
| Lifecycle | `isExpired()` |

### MealToken
| Member | Type | Notes |
|--------|------|-------|
| `tokenNumber` | char[20] | Unique ID "TKN..." |
| `studentEmail` | char[120] | Owner reference |
| `mealName` | char[120] | Cached meal label |
| `mealType` | MealType | Category |
| `hallName` | Halls | Hall origin |
| `paidAmount` | double | Price paid |
| `purchaseDate` | Date | Date purchased |
| `validDate` | Date | Date usable |
| `status` | TokenStatus | ACTIVE/USED/EXPIRED/REVIEWED |
| `purchaseTime` | char[10] | HH:MM (simulation-aware) |

| Method | Description |
|--------|-------------|
| Constructors | Default + full parameter |
| Getters | All field access |
| `setStatus()` | Manual status change |
| `isValid()` | ACTIVE & not expired |
| `canBeUsed()` | Date match & meal window |
| `isExpired()` | `validDate < currentDate` |
| `markAsUsed()` / `markAsReviewed()` | Status transitions |
| `displayToken()` | Console listing |
| `saveToFile(folder)` | Human-readable receipt |
| Static | `generateTokenNumber`, `tokenStatusToString` |

### MealReview
| Member | Type | Notes |
|--------|------|-------|
| `Name` | char[100] | Student email / identifier |
| `tokenNumber` | char[20] | Reviewed token |
| `mealName` | char[100] | Meal label |
| `rating` | MealRating | 1–5 enum |
| `comment` | char[300] | Feedback text |
| `reviewDate` | Date | Submission date |
| `hallName` | Halls | Hall context |
| `batch` | int | Placeholder academic batch |
| `department` | department | Student department |

| Method | Description |
|--------|-------------|
| Constructors | Default + full |
| Getters | For each attribute |
| Display | `displayReview()` |
| Static | `ratingToString()` |

### TokenManager
| Member | Type | Notes |
|--------|------|-------|
| `activeTokens` | vector<MealToken> | In-memory active set |
| `usedTokens` | vector<MealToken> | Used/reviewed tokens |
| `reviews` | vector<MealReview> | In-memory reviews (binary persistence WIP) |
| `TOKEN_FOLDER` | string (static) | Receipt export path |

| Method | Description |
|--------|-------------|
| Constructor | Loads tokens & ensures folder |
| `buyToken(email, hall, type, meal)` | Creates & stores token |
| `useToken(tokenNumber, email)` | Validates & moves to used list |
| `canBuyToken(email, type, date)` | Prevent duplicate daily type |
| `addReview(email, tokenNumber, rating, comment)` | Marks token reviewed + persists |
| `getStudentTokens(email)` | Combined active+used list |
| `displayStudentTokens(email)` | Grouped listing |
| Lifecycle | `cleanupExpiredTokens()` to mark expirations |
| Persistence | `saveAllTokens()` / `loadAllTokens()` (binary) |

### MealUtils (static)
| Method | Description |
|--------|-------------|
| `isWithinMealTime(type)` | Window validation (simulation-aware) |
| `getCurrentTime()` | HH:MM (sim hour if active) |
| `isMealTimeExpired(type, date)` | Same-day window check |
| `getAvailableHalls()` | Short hall list (subset) |
| `displayMealMenu(meals)` | Render meals |
| `displayTokenReceipt(token)` | Render receipt |

---
## Other Domain Classes
### Course
| Member | Type | Notes |
|--------|------|-------|
| `courseName` | char[60] | Course title |
| `courseCode` | char[20] | Code (e.g., CSE101) |
| `totalMarks` | double | Aggregate marks |

| Method | Description |
|--------|-------------|
| Constructors | Full + default |
| `get/setCourseName` | Name management |
| `get/setCourseCode` | Code management |
| `get/setTotalMarks` | Score setter/getter |
| `marksToCGPA()` | Placeholder conversion |
| `display()` | Console output |

### Notice (abstract)
| Member | Type | Notes |
|--------|------|-------|
| `date` | Date | Notice date |
| `description` | char[300] | Text body |

| Method | Description |
|--------|-------------|
| Constructors | Base initialization |
| `get/setDate` | Date access |
| `get/setDescription` | Text access |
| `displayNotice()` | Pure virtual (implementation TBD) |

---
## Future / Placeholder Entities
| File | Purpose | Status |
|------|---------|--------|
| `Results/` (module) | Academic results | Scaffold only |
| `Transport/` (module) | Transport scheduling | Scaffold only |
| `StudentUtility/` | Misc utilities | Scaffold only |

---
## Persistence Files (Relevant to Classes)
| File | Classes Affected | Format |
|------|------------------|--------|
| `Database/students.dat` | Student | Binary fixed-size records |
| `Database/teachers.dat` | Teacher | Binary |
| `Database/admins.dat` | Admin | Binary |
| `Database/dining_authorities.dat` | DiningAuthority | Binary |
| `Database/meals.dat` | Meal | Binary |
| `Database/active_tokens.dat` | MealToken (ACTIVE) | Binary |
| `Database/used_tokens.dat` | MealToken (USED/REVIEWED) | Binary |
| `Database/meal_reviews.dat` | MealReview | (Planned/partial) |
| `Database/simulation_state.dat` | Date | Binary trio (bool, Date, int) |
| `Database/pr_notices.txt` | PublicRelationsAdmin | Delimited text |

---
## Extension Guidance
| Area | Additions |
|------|-----------|
| Reviews | Serialize MealReview fully (append + load) |
| Notice | Implement concrete subclasses & `displayNotice()` |
| Security | Hash passwords before binary persistence |
| Serialization | Add version headers to binary files |
| Testing | Introduce unit tests for date simulation & token transitions |

---
## Quick Cross-Reference
| Class | Key Enums Used | Depends On |
|-------|----------------|------------|
| Student | department, Halls | User, StringHelper |
| Teacher | department, AcademicPosition | User, Course |
| Admin | AdminType | User |
| DiningAuthority | Halls, MealType | User, Meal, Date |
| PublicRelationsAdmin | (none) | User, Date, StringHelper |
| Meal | MealType, Halls | StringHelper, Date |
| MealToken | MealType, Halls, TokenStatus | Date, MealUtils |
| MealReview | MealRating, Halls, department | Date |
| TokenManager | MealType, TokenStatus | Meal, MealToken, MealReview |

---
Generated: (auto documentation snapshot)

