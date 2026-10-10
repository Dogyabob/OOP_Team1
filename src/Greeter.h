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
    RecipeDB& recipeDB;            // database used for insert, search and sort option
    int sortOption = SORT_BY_NAME; // sort option last set on RecipeDB (shown to the user)

    // Prints the main menu.
    void showMenu();

    // Reads a menu number from the user and returns it.
    int getUserChoice();

    // Reads name, ingredients, steps and cooking time from the user into recipe,
    // then asks the user to confirm.
    // Returns false if the user cancels (empty name or 'n' at the confirmation).
    bool inputRecipe(Recipe& recipe);

    // Asks for a search type and a keyword, searches RecipeDB
    // and prints the matching recipes.
    void searchRecipes();

    // Shows each sort option with its order (ascending / descending),
    // asks for one and sets it on RecipeDB.
    void selectSortOption();

    // Prints the given recipes. Prints a message instead if the list is empty.
    void showResults(const std::vector<Recipe>& recipes);
};