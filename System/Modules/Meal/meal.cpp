//
// Created by Md. Asif Khan on 11/8/25.
//

#include "meal.h"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <random>
#include "../../../Core/Utils/StringHelper.h"
#include "../../../Core/Database/DatabaseManager.h"

using namespace std;

// Constants - centralized for DRY
const string TokenManager::TOKEN_FOLDER = "Meal Tokens/";

// Static member definitions
vector<Meal> Meal::cachedMeals;
bool Meal::mealsLoaded = false;
