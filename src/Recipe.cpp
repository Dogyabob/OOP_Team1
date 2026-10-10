#include "Recipe.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

//make a lowercase copy for case-insensitive compare.
static string toLower(const string& str) {
    string result = str;
    for (int i = 0; i < result.size(); i++) {
        result[i] = tolower(result[i]);
    }
    return result;
}

Recipe::Recipe() {}

Recipe::Recipe(const string& name, const vector<string>& ingredients, const vector<string>& steps, int cookTime) {
    this->name = name;
    this->ingredients = ingredients;
    this->steps = steps;
    this->cookTime = cookTime;
}

void Recipe::setName(const string& name) {
    this->name = name;
}

void Recipe::setIngredients(const vector<string>& ingredients) {
    this->ingredients = ingredients;
}

void Recipe::setSteps(const vector<string>& steps) {
    this->steps = steps;
}

void Recipe::setCookTime(int cookTime) {
    this->cookTime = cookTime;
}

string Recipe::getName() const {
    return name;
}

vector<string> Recipe::getIngredients() const {
    return ingredients;
}

vector<string> Recipe::getSteps() const {
    return steps;
}

int Recipe::getCookTime() const {
    return cookTime;
}

void Recipe::print() const {
    cout << "Recipe: " << name << endl;
    cout << "Cooking time: " << cookTime << " min" << endl;

    //ingredients on one line, comma separated.
    cout << "Ingredients: ";
    for (int i = 0; i < ingredients.size(); i++) {
        cout << ingredients[i];
        if (i < ingredients.size() - 1) cout << ", ";
    }
    cout << endl;

    //steps with numbers.
    cout << "Steps:" << endl;
    for (int i = 0; i < steps.size(); i++) {
        cout << i + 1 << ". " << steps[i] << endl;
    }
}

bool Recipe::hasIngredient(const string& ingredientName) const {
    //exact match, ignore case.
    string target = toLower(ingredientName);
    for (int i = 0; i < ingredients.size(); i++) {
        if (toLower(ingredients[i]) == target) return true;
    }
    return false;
}