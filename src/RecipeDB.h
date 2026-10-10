#pragma once
#include <string>
#include <vector>
#include "Recipe.h"

// Search types accepted by RecipeDB::search().
enum SearchType {
    SEARCH_BY_NAME = 1,           // keyword is a part of the recipe name
    SEARCH_BY_INGREDIENT = 2,     // keyword is an ingredient name
    SEARCH_BY_MAX_COOK_TIME = 3,  // keyword is the maximum cooking time (minutes)
    SEARCH_LIST_ALL = 4           // keyword is ignored
};

// Display orders accepted by RecipeDB::setSortOption().
enum SortOption {
    SORT_BY_NAME = 1,       // alphabetical order (default)
    SORT_BY_COOK_TIME = 2   // shortest cooking time first
};

// RecipeDB : keeps the list of all recipes.
//            Inserts new recipes, searches the list and decides the order
//            in which search results are returned.
// Used by Greeter (insert, search, sort option).
// Uses Recipe (the data it stores) and FileManager (save, load).
class RecipeDB {
private:
    std::vector<Recipe> recipeList;  // every recipe stored in the database
    int sortOption = SORT_BY_NAME;   // current display order (a SortOption value)
    std::string fileName;            // file that keeps the recipes between runs

    // Sorts the given recipes by the current sort option.
    void sortRecipes(std::vector<Recipe>& recipes) const;

public:
    // Loads the recipes saved in the given file.
    // The list is empty if the file is empty or does not exist yet.
    RecipeDB(const std::string& fileName);

    // Adds a new recipe and saves the whole list to the file.
    // Returns false (and adds nothing) if the name is blank,
    // if a recipe with the same name already exists (case-insensitive),
    // or if saving fails.
    bool insertRecipe(const Recipe& recipe);

    // Removes the recipe with the given-name and saves the whole list to the file
    // Return false (And remove nothing) if no recipe has that name or if saving fails.
    bool deleteRecipe(const std::string& name);

    // Changes the display order of search results (a SortOption value).
    // An unknown option is ignored and the current order is kept.
    void setSortOption(int option);

    // Returns the recipes that match the keyword for the given search type
    // (a SearchType value), sorted by the current sort option.
    // Returns an empty list if nothing matches or the type is unknown.
    std::vector<Recipe> search(int type, const std::string& keyword) const;
};
