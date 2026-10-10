#include "RecipeDB.h"
#include "FileManager.h"

#include <algorithm>
#include <cctype>

using namespace std;

// Returns a copy of the text with every letter in lower case.
static string toLowerCase(string text) {
    for (char& letter : text) {
        letter = static_cast<char>(tolower(static_cast<unsigned char>(letter)));
    }
    return text;
}

// Returns true if the text contains the keyword, ignoring upper/lower case.
static bool containsIgnoreCase(const string& text, const string& keyword) {
    return toLowerCase(text).find(toLowerCase(keyword)) != string::npos;
}

// Returns true if the text has no visible character.
static bool isBlank(const string& text) {
    for (char letter : text) {
        if (!isspace(static_cast<unsigned char>(letter))) {
            return false;
        }
    }
    return true;
}

// Converts a keyword such as "30" to minutes.
// Returns -1 if the keyword is not a non-negative whole number.
static int parseMinutes(const string& keyword) {
    if (keyword.empty() || keyword.size() > 6) {
        return -1;
    }
    for (char letter : keyword) {
        if (!isdigit(static_cast<unsigned char>(letter))) {
            return -1;
        }
    }
    return stoi(keyword);
}

// Ordering rule for SORT_BY_NAME: alphabetical, ignoring upper/lower case.
static bool nameComesFirst(const Recipe& first, const Recipe& second) {
    return toLowerCase(first.getName()) < toLowerCase(second.getName());
}

// Ordering rule for SORT_BY_COOK_TIME: shortest first, then by name.
static bool cookTimeComesFirst(const Recipe& first, const Recipe& second) {
    if (first.getCookTime() != second.getCookTime()) {
        return first.getCookTime() < second.getCookTime();
    }
    return nameComesFirst(first, second);
}

RecipeDB::RecipeDB(const string& fileName) : fileName(fileName) {
    // FileManager returns an empty list when the file is empty or new.
    FileManager fileManager;
    recipeList = fileManager.load(this->fileName);
}

bool RecipeDB::insertRecipe(const Recipe& recipe) {
    if (isBlank(recipe.getName())) {
        return false;
    }

    // Recipe names must be unique in the database.
    string newName = toLowerCase(recipe.getName());
    for (const Recipe& stored : recipeList) {
        if (toLowerCase(stored.getName()) == newName) {
            return false;
        }
    }

    recipeList.push_back(recipe);

    // Save right away so the recipe survives after the program ends.
    FileManager fileManager;
    if (!fileManager.save(fileName, recipeList)) {
        recipeList.pop_back();  // keep memory and file consistent
        return false;
    }
    return true;
}

bool RecipeDB::deleteRecipe(const string& name) {
    string targetName = toLowerCase(name);

    for (size_t index = 0; index < recipeList.size(); ++index) {
        if (toLowerCase(recipeList[index].getName()) != targetName) {
            continue;
        }

        Recipe removedRecipe = recipeList[index];
        recipeList.erase(recipeList.begin() + index);

        // Save right away so the recipe stays deleted after the program ends.
        FileManager fileManager;
        if (!fileManager.save(fileName, recipeList)) {
            recipeList.insert(recipeList.begin() + index, removedRecipe);  // undo
            return false;
        }
        return true;
    }

    return false;  // no recipe with that name
}

void RecipeDB::setSortOption(int option) {
    if (option == SORT_BY_NAME || option == SORT_BY_COOK_TIME) {
        sortOption = option;
    }
}

vector<Recipe> RecipeDB::search(int type, const string& keyword) const {
    vector<Recipe> results;
    int maxMinutes = parseMinutes(keyword);

    for (const Recipe& recipe : recipeList) {
        bool isMatched = false;

        switch (type) {
        case SEARCH_BY_NAME:
            isMatched = containsIgnoreCase(recipe.getName(), keyword);
            break;
        case SEARCH_BY_INGREDIENT:
            // The recipe itself knows whether it uses an ingredient.
            isMatched = recipe.hasIngredient(keyword);
            break;
        case SEARCH_BY_MAX_COOK_TIME:
            isMatched = (maxMinutes >= 0 && recipe.getCookTime() <= maxMinutes);
            break;
        case SEARCH_LIST_ALL:
            isMatched = true;
            break;
        default:
            isMatched = false;
            break;
        }

        if (isMatched) {
            results.push_back(recipe);
        }
    }

    sortRecipes(results);
    return results;
}

void RecipeDB::sortRecipes(vector<Recipe>& recipes) const {
    if (sortOption == SORT_BY_COOK_TIME) {
        sort(recipes.begin(), recipes.end(), cookTimeComesFirst);
    } else {
        sort(recipes.begin(), recipes.end(), nameComesFirst);
    }
}
