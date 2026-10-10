#include "Greeter.h"
int main() {
    RecipeDB db("recipes.txt");
    Greeter greeter(db);
    greeter.run();
}
