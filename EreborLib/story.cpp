
#include <random>
#include <string>
#include <vector>

using namespace std;

// forward declarations
string build_success_text(const int);
string gen_intro_text();


// constant for splash screen
static const char* intro = "After weeks of digging, you finally break through, into the "
                    "catacombs. You have heard that the labyrinth beneathe Erebor goes all "
                    "the way to the center of the world. It is time to find out.";


/**
 *  Mostly static game intro.
 */
string gen_intro_text() {
    return string(intro);
}


/**
 *    Splash screen, after you finish a maze
 */
string build_success_text(const int count) {
    return string("You are now ") + to_string(count) + string(" levels under Erebor.");
}
