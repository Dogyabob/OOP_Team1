#include <string>
#include "Greeter.h"

//Usage: OOP_Team1.exe <data file> (e.g. OOP_Team1.exe recipes.txt)
int main(int argc, char* argv[]) {
    std::sting fileName = "recipes.txt";
    if (argc >= 2) {
        fileName = argv[1];
    }
    RecipeDB db(fileName);
    Greeter greeter(db);
    greeter.run();
}
