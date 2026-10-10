#include "Greeter.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

// Menu numbers shown by showMenu().
enum MenuChoice {
    MENU_EXIT = 0,
    MENU_ADD_RECIPE = 1,
    MENU_SEARCH = 2,
    MENU_SORT_OPTION = 3,
    MENU_DELETE_RECIPE = 4
};

// Prints the prompt and returns one line typed by the user.
// Returns "" if there is no more input.
static string readLine(const string& prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) {
        return "";
    }
    return line;
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

// Returns a copy of the text with every letter in lower case.
static string toLowerCase(string text) {
    for (char& letter : text) {
        letter = static_cast<char>(tolower(static_cast<unsigned char>(letter)));
    }
    return text;
}

// Returns true if the text contains '|' or ';', which FileManager uses as separators.
static bool hasSeparator(const string& text) {
    return text.find('|') != string::npos || text.find(';') != string::npos;
}

// Converts text such as "30" to a number.
// Returns -1 if the text is not a non-negative whole number.
static int toNumber(const string& text) {
    if (text.empty() || text.size() > 6) {
        return -1;
    }
    for (char letter : text) {
        if (!isdigit(static_cast<unsigned char>(letter))) {
            return -1;
        }
    }
    return stoi(text);
}

// Reads lines until the user enters an empty line and returns them as a list.
// Lines with '|' or ';' are rejected and must be typed again.
static vector<string> readList(const string& title) {
    vector<string> items;
    cout << title << " (press Enter on an empty line to finish)" << endl;
    while (cin) {
        string line = readLine("- ");
        if (line.empty()) {
            break;
        }
        if (hasSeparator(line)) {
            cout << "'|' and ';' cannot be used. Please type it again." << endl;
            continue;
        }
        items.push_back(line);
    }
    return items;
}

// Returns a description of the sort option, including its order.
static string sortOptionLabel(int option) {
    if (option == SORT_BY_COOK_TIME) {
        return "Cooking time (ascending)";
    }
    return "Name (ascending)";
}

// Asks a yes/no question until the user answers y or n.
// Returns false for 'n' or if there is no more input.
static bool askYesNo(const string& question) {
    while (cin) {
        string answer = readLine(question + " (y/n): ");
        if (answer == "y" || answer == "Y") {
            return true;
        }
        if (answer == "n" || answer == "N") {
            return false;
        }
        if (cin) {
            cout << "Please enter y or n." << endl;
        }
    }
    return false;
}

Greeter::Greeter(RecipeDB& db) : recipeDB(db) {}

void Greeter::run() {
    cout << "Welcome to the Recipe Manager!" << endl;

    while (true) {
        showMenu();
        int choice = getUserChoice();

        switch (choice) {
        case MENU_ADD_RECIPE: {
            Recipe recipe;
            if (!inputRecipe(recipe)) {
                cout << "Adding the recipe was cancelled." << endl;
            } else if (recipeDB.insertRecipe(recipe)) {
                cout << "Recipe \"" << recipe.getName() << "\" was added." << endl;
            } else {
                cout << "Could not add the recipe. "
                     << "The name may be empty or already used, or saving failed." << endl;
            }
            break;
        }
        case MENU_SEARCH:
            searchRecipes();
            break;
        case MENU_SORT_OPTION:
            selectSortOption();
            break;
        case MENU_DELETE_RECIPE:
            deleteRecipe();
            break;
        case MENU_EXIT:
            cout << "Goodbye!" << endl;
            return;
        default:
            cout << "Invalid choice. Please enter a number from the menu." << endl;
            break;
        }
    }
}

void Greeter::showMenu() {
    cout << endl;
    cout << "========== MENU ==========" << endl;
    cout << MENU_ADD_RECIPE << ". Add a recipe" << endl;
    cout << MENU_SEARCH << ". Search recipes" << endl;
    cout << MENU_SORT_OPTION << ". Change sort order (now: " << sortOptionLabel(sortOption) << ")" << endl;
    cout << MENU_DELETE_RECIPE << ". Delete a recipe" << endl;
    cout << MENU_EXIT << ". Exit" << endl;
    cout << "==========================" << endl;
}

int Greeter::getUserChoice() {
    string line = readLine("Select: ");

    // No more input (e.g. end of file) : exit instead of looping forever.
    if (!cin) {
        return MENU_EXIT;
    }
    return toNumber(line);  // -1 for invalid input, handled as an invalid choice
}

bool Greeter::inputRecipe(Recipe& recipe) {
    string name;
    while (true) {
        name = readLine("Recipe name (press Enter on an empty line to cancel): ");
        if (!cin || isBlank(name)) {
            return false;
        }
        if (hasSeparator(name)) {
            cout << "'|' and ';' cannot be used in the name." << endl;
        } else {
            break;
        }
    }

    vector<string> ingredients = readList("Ingredients");
    vector<string> steps = readList("Cooking steps");

    int cookTime = -1;
    while (cin && cookTime < 0) {
        cookTime = toNumber(readLine("Cooking time (minutes): "));
        if (cookTime < 0) {
            cout << "Please enter a whole number of minutes (e.g. 30)." << endl;
        }
    }
    if (cookTime < 0) {
        cookTime = 0;
    }

    recipe = Recipe(name, ingredients, steps, cookTime);

    // Last chance to cancel after seeing everything that was typed.
    cout << "--------------------------" << endl;
    recipe.print();
    cout << "--------------------------" << endl;
    return askYesNo("Save this recipe?");
}

void Greeter::deleteRecipe() {
    string name = readLine("Name of the recipe to delete (press Enter on an empty line to cancel): ");
    if (!cin || isBlank(name)) {
        cout << "Deleting was cancelled." << endl;
        return;
    }

    // Name search matches parts of names, so pick the recipe with exactly this name.
    for (const Recipe& recipe : recipeDB.search(SEARCH_BY_NAME, name)) {
        if (toLowerCase(recipe.getName()) != toLowerCase(name)) {
            continue;
        }

        cout << "--------------------------" << endl;
        recipe.print();
        cout << "--------------------------" << endl;
        if (!askYesNo("Delete this recipe?")) {
            cout << "Deleting was cancelled." << endl;
        } else if (recipeDB.deleteRecipe(recipe.getName())) {
            cout << "Recipe \"" << recipe.getName() << "\" was deleted." << endl;
        } else {
            cout << "Could not delete the recipe. Saving failed." << endl;
        }
        return;
    }

    cout << "No recipe is named \"" << name << "\"." << endl;
}

void Greeter::searchRecipes() {
    cout << endl;
    cout << "Search by:" << endl;
    cout << SEARCH_BY_NAME << ". Name" << endl;
    cout << SEARCH_BY_INGREDIENT << ". Ingredient" << endl;
    cout << SEARCH_BY_MAX_COOK_TIME << ". Maximum cooking time" << endl;
    cout << SEARCH_LIST_ALL << ". Show all recipes" << endl;

    int type = getUserChoice();
    string keyword;

    switch (type) {
    case SEARCH_BY_NAME:
        keyword = readLine("Part of the name: ");
        break;
    case SEARCH_BY_INGREDIENT:
        keyword = readLine("Ingredient: ");
        break;
    case SEARCH_BY_MAX_COOK_TIME:
        keyword = readLine("Maximum cooking time (minutes): ");
        if (toNumber(keyword) < 0) {
            cout << "Please enter a whole number of minutes (e.g. 30)." << endl;
            return;
        }
        break;
    case SEARCH_LIST_ALL:
        break;  // no keyword needed
    default:
        cout << "Invalid search type." << endl;
        return;
    }

    showResults(recipeDB.search(type, keyword));
}

void Greeter::selectSortOption() {
    cout << endl;
    cout << "Current sort order: " << sortOptionLabel(sortOption) << endl;
    cout << "Sort results by:" << endl;
    cout << SORT_BY_NAME << ". " << sortOptionLabel(SORT_BY_NAME) << endl;
    cout << SORT_BY_COOK_TIME << ". " << sortOptionLabel(SORT_BY_COOK_TIME) << endl;

    int option = getUserChoice();
    if (option == SORT_BY_NAME || option == SORT_BY_COOK_TIME) {
        recipeDB.setSortOption(option);
        sortOption = option;
        cout << "Sort order changed to: " << sortOptionLabel(sortOption) << endl;
    } else {
        cout << "Invalid sort option. The sort order was not changed." << endl;
    }
}

void Greeter::showResults(const vector<Recipe>& recipes) {
    if (recipes.empty()) {
        cout << "No recipes found." << endl;
        return;
    }

    cout << recipes.size() << " recipe(s) found. "
         << "Sorted by " << sortOptionLabel(sortOption) << "." << endl;
    for (const Recipe& recipe : recipes) {
        cout << "--------------------------" << endl;
        recipe.print();
    }
    cout << "--------------------------" << endl;
}
