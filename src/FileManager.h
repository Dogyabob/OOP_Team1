#pragma once
#include <string>
#include "Recipe.h"
#include <vector>

// FileManager : save and load recipeList from RecipeDB on given file.
// File format : one recipe per line, fields are separated by '|' and list items by ';'.
// Do not use | or ; in recipe name, ingredients or steps.
class FileManager {
public:
    // Returns true if saving succeeds, and false if it fails.
    // If a file with the given fileName already exists, it is overwritten.
    // If no file with the given fileName exists, a new file is created and the given recipes are saved to it.
    bool save(const std::string& fileName, const std::vector<Recipe>& recipes) const;

    // Returns the list of recipes read from the given file. Returns an empty list if the file is empty.
    // If the file does not exist, a new file with the given fileName is created.
    std::vector<Recipe> load(const std::string& fileName) const;
};