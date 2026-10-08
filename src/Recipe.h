#pragma once
#include <string>
#include <vector>

// Recipe : stores the information of a single recipe
//          (name, ingredients, cooking steps, cooking time).
// Used by RecipeDB (insert, search, sort), Greeter (input, output)
// and FileManager (save, load).
class Recipe {
private:
    std::string name;                     // recipe name(used as a unique key in RecipeDB)
    std::vector<std::string> ingredients; // list of ingredient names
    std::vector<std::string> steps;       // cooking steps, in order
    int cookTime=0;                       // cooking time (minutes)

public:
    // Creates an empty recipe. name is "", lists are empty and cookTime is 0.
    Recipe();

    // Creates a recipe with all fields.
    Recipe(const std::string& name, const std::vector<std::string>& ingredients, const std::vector<std::string>& steps, int cookTime);

    // Setters : replace the value of each field.
    void setName(const std::string& name);
    void setIngredients(const std::vector<std::string>& ingredients);
    void setSteps(const std::vector<std::string>& steps);
    void setCookTime(int cookTime);

    // Getters : return the value of each field without changing the recipe.
    std::string getName() const;
    std::vector<std::string> getIngredients() const;
    std::vector<std::string> getSteps() const;
    int getCookTime() const;

    // Prints all information of the recipe.
    // (name, cooking time, ingredients, numbered steps).
    void print() const;

    // Returns true if the ingredient list contains ingredientName, and false otherwise.
    //(exact match, case-insensitive)
    bool hasIngredient(const std::string& ingredientName) const;
};