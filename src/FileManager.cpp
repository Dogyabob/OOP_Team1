#include <string>
#include <vector>
#include "FileManager.h"
#include "Recipe.h"
#include <sstream>
#include <fstream>
using namespace std;

static vector<string> split(const string& str, char delimiter) {
    vector<string> result;
    string token;
    stringstream sstream(str);
    while (getline(sstream, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}


    bool FileManager::save(const string& fileName, const vector<Recipe>& recipes) const {
        //Recipes to string.
        vector<string> strRecipes;
        for (int i = 0 ; i< recipes.size() ; i++) {
            string recipeLine = "";
            recipeLine.append(recipes[i].getName());
            recipeLine.append("|");
            for (const string& ingredient: recipes[i].getIngredients()) {
                recipeLine.append(ingredient);
                recipeLine.append(";");
            }
            recipeLine.append("|");
            for (const string& step : recipes[i].getSteps()) {
                recipeLine.append(step);
                recipeLine.append(";");
            }
            recipeLine.append("|");
            recipeLine.append(to_string(recipes[i].getCookTime()));
            strRecipes.push_back(recipeLine);
        }
        //write.
        ofstream myFile(fileName);
        if (myFile.is_open()) {
            for (int i = 0 ; i < strRecipes.size() ; i++) {
                myFile << strRecipes[i] << endl;
            }
            myFile.close();
            return true;
        }
        //save failed.
        return false;
    }

    vector<Recipe> FileManager::load(const string& fileName)const {
        vector<Recipe> recipes;
        ifstream myFile(fileName);
        if (myFile.is_open()) {
            string line;
            Recipe r;
            //parsing
            while (getline(myFile, line)) {
                vector<string> fields = split(line, '|');
                if (fields.size() < 4) continue;
                vector<string> ingredients = split(fields[1], ';');
                vector<string> steps = split(fields[2], ';');
                r.setName(fields[0]);
                r.setIngredients(ingredients);
                r.setSteps(steps);
                r.setCookTime(stoi(fields[3]));
                recipes.push_back(r);
            }
            myFile.close();
            return recipes;
        }else {
            ofstream createFile(fileName);
            createFile.close();
            return recipes;
        }
    }