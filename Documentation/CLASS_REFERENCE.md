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
| `designation` | Professor, AssociateProfessor, AssistantProfessor, Lecturer | Teacher rank (in Designation.h) |
| `Halls` | Al_Beruni_Hall, Meer_Mosharraf_Hossain_Hall, Shaheed_Salam_Barkat_Hall, AFM_Kamaluddin_Hall, Moulana_Bhasani_Hall, Bangabondhu_Sheikh_Majibur_Rahman_Hall, Jatiya_Kabi_Kazi_Nazrul_Islam_Hall, Rabindra_Nath_Tagore_Hall, Shahid_Tajuddin_Ahmed_Hall, Shahid_Sheikh_Russel_Hall, Shaheed_Rafiq_Jabbar_Hall, Nawab_Faizunnesa_Hall, Fazilatunnesa_Hall, Jahanara_Imam_Hall, Preetilata_Hall, Begum_Khaleda_Zia_Hall, Sheikh_Hasina_Hall, Bir_Pratik_Taramon_Bibi_Hall | University residential halls (18 total) |
| `MealType` | BREAKFAST, LUNCH, DINNER | Token & meal classification |
| `MealRating` | POOR–EXCELLENT (1–5) | Review scale |
| `TokenStatus` | ACTIVE, USED, EXPIRED, REVIEWED | Token lifecycle |

---
## Utility Classes
### StringHelper (static)
| Method | Purpose |
|--------|---------|
| `stringToCharArray<N>(src, dest)` (templated) | Safe copy with null termination |
| `stringToCharArray(src, dest, maxSize)` (overloaded) | Non-template variant with size parameter |
| `safeStringToCharArray<N>(src, dest, fieldName)` | Copy + warning if truncated |
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
| Comparators (`== != < > <= >=`) | | Date ordering (operator overloading) |
| `getNextDay() / getPreviousDay()` | | Date arithmetic |
| `isToday/Tomorrow/Yesterday()` | | Relative checks |
| `getCurrentDate()` | S | Real or simulated date |
| `getTomorrowDate()` | S | Convenience wrapper |
| `SimulateDate(n)` | S | Advance n days |
| `SimulateMonths(n)` | S | Advance n months (clamps day) |
| `SimulateHours(n)` | S | Advance hours (auto day roll) |
| `setSimulatedDate(d)` | S | Activate simulation with date |
| `setSimulatedDateTime(d,h)` | S | Set date + hour |
| `getSimulatedHour()` | S | Returns simulated hour or -1 |
| `isSimulationActive()` | S | Flag query |
| `resetSimulation()` | S | Disable simulation |
| `getCurrentTimeString()` | S | Returns YYYY-MM-DD HH:MM format (no seconds) |
| `getCurrentDateTimeString()` | S | Returns YYYY-MM-DD HH:MM:SS format |
| `operator<<` | Friend | Stream insertion operator for display |

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
| `age` | int | Age |
| `studentID` | char[15] | Unique student ID |
| `classRoll` | int | Class roll number |
| `dept` | department | Academic department |
| `batch` | int | Cohort year / batch |
| `hall` | Halls | Assigned residential hall |
| `Gender` | int | 1 for male / 0 for female |
| `balance` | double | Financial balance (future logic) |

| Method | Description |
|--------|-------------|
| Constructors (full / default) | Initialize fields |
| `get/set` for all members | Accessors & mutators |
| `display()` | Implements User::display |
| `ChangePassword()` | Password change functionality |
| `BuyMealToken()` / `TakeMeal()` / `ReviewMeal()` | High-level meal operations |
| `ViewResult()` / `ViewNotice()` / `ViewClassSchedule()` / `ViewTransportSchedule()` | Module stubs |
| Static CRUD: `loadAllStudents`, `add/update/deleteStudentInDB`, `findStudentByEmail/ID` | DatabaseManager wrappers |

### Teacher : User
| Member | Type | Notes |
|--------|------|-------|
| `name` | char[100] | Teacher name |
| `dept` | department | Department enum |
| `dg` | designation | Academic rank/designation |

| Method | Description |
|--------|-------------|
| Constructors | Full & default |
| `set/getName` | Name management |
| `set/getDepartment` | Department enum access |
| `getDepartmentName()` | Friendly string conversion |
| `set/getDesignation()` | Designation/rank operations |
| `display()` | Role details |
| `gradeStudent(Student&, Course&)` | Grading flow (TBD) |
| Static CRUD: `loadAllTeachers`, `addTeacherToDB`, `updateTeacherInDB`, `deleteTeacherFromDB`, `findTeacherByEmail` | Persistence wrappers |

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
| Constructors | Default & full parameter constructor |
| Getters/Setters | `getName/setName`, `getHallName/setHallName`, `getHall/setHall` |
| Meal CRUD | `createMeal`, `updateMeal`, `deleteMeal`, `viewAllMeals` |
| Review Management | `viewFoodReviews` |
| Display | `display()` - override from User |
| Static CRUD | add/update/delete/find operations |

### PublicRelationsAdmin : Admin
| Member | Type | Notes |
|--------|------|-------|
| (inherits from Admin) | | Inherits name, email, password, adminType |

| Method | Description |
|--------|-------------|
| Constructors | Default + credential/name constructor (sets AdminType::PublicRelations) |
| `display()` | Override display method |
| `createNotice()` | Interactive notice creation |
| `viewAllNotices()` | Display all notices |
| `editNotice()` | Modify existing notice |
| `deleteNotice()` | Remove notice |
| `viewNoticeDetails()` | Detailed notice view |
| `searchNotices()` | Search functionality |
| `displayNoticeStatistics()` | Notice analytics |
| Static CRUD: `loadAllNotices`, `addNoticeToDB`, `updateNoticeInDB`, `saveNoticesToDB` | Notice persistence operations |

---
## Meal & Token System
### Meal
| Member | Type | Notes |
|--------|------|-------|
| `MealPackage` | char[300] | "Name - Description" combined storage |
| `mealType` | MealType | Category (BREAKFAST/LUNCH/DINNER) |
| `price` | double | Cost (BDT) |
| `availableQuantity` | int | Remaining portions |
| `isAvailable` | bool | Availability flag |
| `date` | char[12] | `DD-MM-YYYY` |
| `time` | char[10] | Scheduled time string |
| `hallName` | Halls | Associated hall |

| Static Members | Type | Notes |
|---------------|------|-------|
| `cachedMeals` | vector<Meal> | In-memory cache |
| `mealsLoaded` | bool | Guard to ensure single load |

| Method Group | Highlights |
|--------------|-----------|
| Constructors | Full parameter & default constructor |
| Accessors | `getMealName`, `getDescription`, `getMealType`, `getPrice`, `getAvailableQuantity`, `getIsAvailable`, `getDate`, `getTime`, `getHallName` |
| Mutators | `setMealName/Description/Type/Price/AvailableQuantity/IsAvailable/Date/Time/HallName` |
| Operations | `orderMeal(quantity)` - decrease available quantity |
| Display | `displayMeal()` - console output |
| Static Utils | `mealTypeToString`, `stringToMealType` |
| Persistence (Static) | `loadAllMealsFromDatabase`, `loadMealsByHall/Date/Type`, `deleteMealFromDatabase`, `initializeMealDatabase`, `displayAllMeals`, `getMealCount` |
| Persistence (Instance) | `saveMealToDatabase`, binary write/read helpers |
| Lifecycle | `isExpired()` - check if meal date has passed |

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
| Getters | `getTokenNumber`, `getStudentEmail`, `getMealName`, `getMealType`, `getHallName`, `getPaidAmount`, `getPurchaseDate`, `getValidDate`, `getStatus`, `getPurchaseTime` |
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
| `batch` | int | Student academic batch |
| `department` | department | Student department |

| Method | Description |
|--------|-------------|
| Constructors | Default + full parameter |
| Getters | `getStudentEmail`, `getTokenNumber`, `getMealName`, `getRating`, `getComment`, `getReviewDate`, `getHallName` |
| Display | `displayReview()` |
| Static | `ratingToString()` |

### TokenManager
| Member | Type | Notes |
|--------|------|-------|
| `activeTokens` | vector<MealToken> | In-memory active set |
| `usedTokens` | vector<MealToken> | Used/reviewed tokens |
| `reviews` | vector<MealReview> | In-memory reviews |
| `TOKEN_FOLDER` | string (static const) | Receipt export path |

| Method | Description |
|--------|-------------|
| Constructor | Loads tokens & ensures folder exists |
| `buyToken(email, hall, type, meal)` | Creates & stores token |
| `useToken(tokenNumber, email)` | Validates & moves to used list |
| `canBuyToken(email, type, date)` | Prevent duplicate daily type |
| `addReview(email, tokenNumber, rating, comment)` | Marks token reviewed + persists |
| `getStudentTokens(email)` | Combined active+used list |
| `getActiveTokens()` / `getUsedTokens()` | Vector accessors |
| `getMealReviews(mealName)` / `getHallReviews(hallName)` | Filter reviews |
| `displayStudentTokens(email)` | Grouped listing |
| Lifecycle | `cleanupExpiredTokens()` to mark expirations |
| Persistence | `saveAllTokens()` / `loadAllTokens()`, `saveAllReviews()` / `loadAllReviews()` |
| Private | `createTokenFolder()` - ensure directory exists |

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

### Notice
| Member | Type | Notes |
|--------|------|-------|
| `title` | char[90] | Notice title |
| `message` | char[300] | Text body |
| `date` | Date | Notice date (composition/aggregation) |

| Method | Description |
|--------|-------------|
| Constructors | Default & full parameter (title, message, date) |
| `getTitle/setTitle` | Title access |
| `getMessage/setMessage` | Message content access |
| `getDate/setDate` | Date access |

---
## DatabaseManager (Static Utility)
| Purpose | Central binary file persistence manager using templates |
|---------|----------------------------------------------------------|

| Static Constants | File Paths |
|-----------------|------------|
| `STUDENTS_DB` / `TEACHERS_DB` / `ADMINS_DB` / `DINING_AUTH_DB` | User role databases |
| `MEALS_DB` | Meal database |
| `ACTIVE_TOKENS_DB` / `USED_TOKENS_DB` / `REVIEWS_DB` | Token system databases |
| `NOTICES_DB` | Notice database |
| `DATABASE_DIR` | Base directory path |

| Static Cache Vectors | Holds in-memory data |
|---------------------|----------------------|
| `cachedStudents` / `cachedTeachers` / `cachedAdmins` / `cachedDiningAuthorities` | User caches |
| `cachedMeals` / `cachedActiveTokens` / `cachedUsedTokens` / `cachedReviews` / `cachedNotices` | Domain object caches |

| Template Methods | Description |
|-----------------|-------------|
| `writeObjectToBinary<T>(out, obj)` | Write single object |
| `readObjectFromBinary<T>(in, obj)` | Read single object |
| `loadObjects<T>(filePath)` | Load all objects of type T |
| `saveObjects<T>(objects, filePath)` | Save vector of type T |
| `findObjectByKey<T,KeyType>(container, key, memberFunc)` | Find by key using member function pointer |
| `addObject<T>(cache, object, filePath)` | Add to cache & persist |
| `updateObject<T,KeyType>(cache, key, updatedObject, filePath, memberFunc)` | Update in cache & persist |
| `deleteObject<T,KeyType>(cache, key, filePath, memberFunc)` | Delete from cache & persist |

| Specialized Methods | Per-Class CRUD Operations |
|--------------------|---------------------------|
| Student/Teacher/Admin/DiningAuthority | load/save/add/update/delete/find operations |
| Meal/Token/Review/Notice | Specialized persistence operations |
| `initializeDatabase()` | Load all data into caches at startup |

---
## Future / Placeholder Modules
| Module | Location | Status |
|--------|----------|--------|
| Results | `System/Modules/Results/` | Empty - planned for academic results |
| Transport | `System/Modules/Transport/` | Empty - planned for transport scheduling |

---
## Persistence Files (Binary & Text Formats)
| File | Classes Affected | Format |
|------|------------------|--------|
| `Database/students.dat` | Student | Binary fixed-size records |
| `Database/teachers.dat` | Teacher | Binary |
| `Database/admins.dat` | Admin | Binary |
| `Database/dining_authorities.dat` | DiningAuthority | Binary |
| `Database/meals.dat` | Meal | Binary |
| `Database/active_tokens.dat` | MealToken (ACTIVE) | Binary |
| `Database/used_tokens.dat` | MealToken (USED/REVIEWED) | Binary |
| `Database/meal_reviews.dat` | MealReview | Binary |
| `Database/notices.dat` | Notice | Binary |
| `Meal Tokens/` | MealToken | Text receipts (individual files) |

---
## Key OOP Features Demonstrated
| Feature | Location | Example |
|---------|----------|---------|
| **Inheritance** | User hierarchy | Student, Teacher, Admin, DiningAuthority, PublicRelationsAdmin inherit from User/Admin |
| **Polymorphism** | `display()` method | Virtual function overridden in each User subclass |
| **Encapsulation** | All classes | Private members with public getters/setters |
| **Abstraction** | User class | Pure virtual `display()` method |
| **Aggregation** | Notice class | Notice contains Date object (has-a relationship) |
| **Composition** | Student class | Student contains department and Halls enums |
| **Operator Overloading** | Date class | Comparison operators, stream insertion operator |
| **Friend Functions** | Date class | `operator<<` as friend function |
| **Templates** | DatabaseManager, StringHelper | Generic type-safe operations |
| **Enumerations** | Multiple files | Type-safe enum classes for domain concepts |
| **Static Members** | Meal, Date, DatabaseManager | Class-level data and operations |
| **STL Containers** | Throughout | vector for collections |

---
## Extension Guidance
| Area | Recommendations |
|------|----------------|
| Security | Hash passwords before binary persistence |
| Serialization | Add version headers to binary files for backward compatibility |
| Testing | Unit tests for date simulation & token transitions |
| Validation | Enhanced input validation across all user inputs |
| Results Module | Implement grade management system |
| Transport Module | Implement bus/transport scheduling |
| Notice System | Add categories, priorities, expiration dates |

---
## Quick Cross-Reference
| Class | Key Enums Used | Key Dependencies |
|-------|----------------|------------------|
| Student | department, Halls | User, StringHelper, DatabaseManager |
| Teacher | department, designation | User, Course, DatabaseManager |
| Admin | AdminType | User, StringHelper, DatabaseManager |
| DiningAuthority | Halls, MealType | User, Meal, Notice, DatabaseManager |
| PublicRelationsAdmin | (inherits AdminType) | Admin, Notice, Date, DatabaseManager |
| Meal | MealType, Halls | StringHelper, Date, DatabaseManager |
| MealToken | MealType, Halls, TokenStatus | Date, MealUtils |
| MealReview | MealRating, Halls, department | Date |
| TokenManager | MealType, TokenStatus | Meal, MealToken, MealReview, DatabaseManager |
| Notice | (none) | Date, StringHelper |
| Date | (none) | Standard library only |
| DatabaseManager | (all user/domain classes) | Student, Teacher, Admin, DiningAuthority, Meal, MealToken, MealReview, Notice |

---
**Generated:** Documentation snapshot for unizyy University Management System  
**Last Updated:** Based on project inspection
