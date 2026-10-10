#pragma once

#include <vector>
#include "RecipeDB.h"
#include "Recipe.h"

// Greeter : handles all interaction with the user.
//           Shows the menu, reads the user's input and prints the results.
// Uses RecipeDB (insert, search, sort option) and Recipe (input, output).
class Greeter {
public:
    // Creates a Greeter that works with the given RecipeDB.
    // The RecipeDB is not copied, so it must stay alive while the Greeter is used.
    Greeter(RecipeDB& db);

    // Starts the main loop: shows the menu and runs the chosen action
    // until the user chooses to exit.
    void run();
private:
    RecipeDB& recipeDB;  // database used for insert, search and sort option

    // Prints the main menu.
    void showMenu();

    // Reads a menu number from the user and returns it.
    int getUserChoice();

    // Reads name, ingredients, steps and cooking time from the user
    // and returns them as a new Recipe.
    Recipe inputRecipe();

    // Asks for a search type and a keyword, searches RecipeDB
    // and prints the matching recipes.
    void searchRecipes();

    // Asks for a sort option and sets it on RecipeDB.
    void selectSortOption();

    // Prints the given recipes. Prints a message instead if the list is empty.
    void showResults(const std::vector<Recipe>& recipes);
};