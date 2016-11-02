
#include <random>
#include <string>
#include <vector>
#include "player.h"

using namespace std;

// forward declarations
const char* skewed_choice(const char* arr[], const int length, const int exp=3);
string build_success_text(const int);


// constants for splash screen
static const char* splash_exclaim[] = {"", "Success! ", "Finally! ", "Whew! "};
static const int n_splash_exclaim = sizeof(splash_exclaim) / sizeof(char *);
static const char* splash_success[] = {"You did it!",
                                       "You are through!",
                                       "You found a way down!",
                                       "You found a way out!",
                                       "You found a way through!",
                                       "You are through the catacombs!",
                                       "You are through the labyrinth!",
                                       "You are through the maze!",
                                       "You found your way through the catacombs!",
                                       "You found your way through the labyrinth!",
                                       "You found your way through the maze!",
                                       "You found the end of the catacombs!",
                                       "You found the end of the labyrinth!",
                                       "You found the end of the maze!"};
static const int n_splash_success = sizeof(splash_success) / sizeof(char *);
static const char* splash_story[] = {"Engraved above the archway leading down further into the catacombs is a scene "
                                     "of Durin the Deathless waking up, first of all Dwarves, in his cave under "
                                     "Mount Dundabad",
                                     "Engraved above the stone doorway you find an engraved scene of a "
                                     "human archer killing a dragon.",
                                     "Engraved in the living rock above the staircase is a long series of "
                                     "scene of the great Dwarven smith Telchar crafting the Dragon-helm of Dor-lómin. "
                                     "Each panel details with extreme care the steps in blacksmith and magic that "
                                     "were used to craft the helm. Your knowledge of blacksmith and magic have "
                                     "improved.",
                                     "Engraved above the stairwell is an extremely detailed and polished "
                                     "depiction of King Azaghâl stabbing Glaurung, the first dragon, even "
                                     "as the Dragon kills him.",
                                     "Engraved along the walls of the spiral staircase are "
                                     "scenes of a King Thorin Oakenshield being buried with the Arkenstone."};
static const int n_splash_story = sizeof(splash_story) / sizeof(char *);
static const char* intro = ", and you can feel that the fourth "
                    "age of the world is drawing to a close. You hail from the the great "
                    "northern city of Annúminas, which your Dwarven ancestors helped rebuild."
                    "\n\nThere are very few Dwarves left in the world. Erebor, their last "
                    "great citadel, fell to a goblin army many centuries ago. There simply "
                    "weren't enough Dwarves left to defend the Lonely Mountain. Your father, "
                    "a great stone mason, was with the last human army to try and assault the "
                    "mountain. Their losses were terrible, and they never got passed the gate. "
                    "Instead, they sealed the gate with an avalanche, and fortified the "
                    "rubble into a mighty wall. Then they left the goblin army to starve in the "
                    "mines.\n\nIt has been 200 years since that battle and the humans have long "
                    "since forgot the threat. The young kings cannot believe that anyone could "
                    "long survive under ground. But they do not know the ways of Dwarven mining. "
                    "They do not know about black moth honey or grazing blind, pale livestock "
                    "on the deep moss that grows along underground streams. "
                    "And goblins have always preferred living underground. They "
                    "could tunnel out in a few short weeks, if they wanted.\n\nWord reached "
                    "your ear, though, that the last elven queen foretold huge armies "
                    "underneath the Lonely Mountain. Whose numbers are now nearly large enough "
                    "to assault Middle Earth. You have decided to defend the world by "
                    "venturing deep underneath Erebor. Far under the labyrinths and "
                    "catacombs there lies a weapon that the elven queen said could bring "
                    "down the mountain. It will collapse into the Earth, lava will consume "
                    "it, and every goblin in it will parish.\n\nDelving into the mountain would "
                    "mean facing the goblin hoards alone. Along with whatever fel beasts they "
                    "have bred. If you succeed, you will destroy the final home of Dwarves in "
                    "Middle Earth. And you will be deep under the mountain when it falls. But "
                    "you are half human, and cannot let the world fall without defending it.\n\n"
                    "You set off quietly months ago and started mining out the hidden entrance "
                    "your father left to the undermines. Sealing yourself in as you go. Today "
                    "you break through and enter the catacombs for the first and last time.";
static const char* you_win = "The corridor you follow down was carved carefully; it is polished to "
                    "a shine, even now. Straight as an arrow, the halways slopes "
                    "downward steeply. Finally, the corridor opens abruptly on a giant natural "
                    "cave. Your torch doesn't reach all the way across the room, but you can see "
                    "three huge natural pillars support the giant roof, far above in the dark. As "
                    "you walk further, you find a carved stone tomb.\n\nOn the enormous granite lid "
                    "of the tomb is the carved statue of Thorin Oakenshield; he is lying down with "
                    "a branch in his left arm and a mighty hammer in his right. You put down all "
                    "your gear and get to work moving the stone lid. By yourself, it takes a while "
                    "to rotate the lid off the tomb, but with proper leverage it slides back.\n\nAnd "
                    "just inside you see it.\n\nOn top of the suit of armor and mouldered clothes is "
                    "the Arkenstone, still shinning bright in the darkness. After coming all this way, "
                    "you are actually surprised to see it still sitting there. Joy and relief move "
                    "over you in waves as you stare at it, colored lights flowing across its surface. "
                    "After a while, you realize you are still staring at it, unmoving. Then the task "
                    "at hand comes back to you, and all joy leaves you.\n\nIt is said that the "
                    "Arkenstone is the heart of this mountain. And if you destroy it in this place, the "
                    "whole mountain will collapse in grief. Even the mountain cannot live without its "
                    "heart. The great roof above you will collapse, and all of the mountain above it will "
                    "come crashing down. Every orc and troll and foul creature in the mountain will be "
                    "killed, and the war will end before it starts. Or nothing will happen, and you will "
                    "have come all this way to prove yourself a fool.\n\nYou gently reach down and pick "
                    "up the Arkenstone, freeing it from the gauntlets of Thorin Oakenshield for the first "
                    "time in millenia. You walk around to the other side of the tomb, and sit down with "
                    "your back against the stone. You allow yourself an hour to watch the stone, and "
                    "remember all of the long time it had spent under the care of the King Under the "
                    "Mountain, and in the grip of dragons. Eventually, the gem becomes blurry and you "
                    "decide it is time to find out how the story ends.\n\nYou wipe the tears from your "
                    "eyes and place the Arkenstone on the floor. You walk back around the tomb and brace "
                    "all your weight against the lever you placed against the tomb lid. You think of your "
                    "parents, of your friends and family in Minas Tirith. You wonder if the Humans of the "
                    "town of Dale, just outside Erebor, will survive the collapsing mountain. The stone lid "
                    "slides off the tomb and lands heavily on the ground, crushing the Arkenstone.\n\nA "
                    "flash of light blinds you and you are thrown back across the cavern. For a moment you "
                    "wonder if you are still alive and the legend was wrong. But you hear the great pillars "
                    "in the room crack. And from high above you the deep rumbling of the mountain shaking. "
                    "The last you know is sound of the great roof collapsing and the deafing roar of the "
                    "mountain moving above you.\n\nErebor falls.";


/**
 *  Mostly static game text, for a winning game state.
 */
string gen_win_text() {
    return string(you_win);
}

/**
 *  Mostly static game intro.
 */
string gen_intro_text(PlayerData p) {
    string txt(string("You are ") + p.name + string(" son of ") + p.parent1 + string(" and ") +
               p.parent2 + string(intro));
    return txt;
}


/**
 *    Splash screen, after you finish a maze
 */
string build_success_text(const int count) {
    string txt(string(skewed_choice(splash_exclaim, n_splash_exclaim)) +
               string(skewed_choice(splash_success, n_splash_success)) + "\n\n" +
               string(skewed_choice(splash_story, n_splash_story, 1)) + "\n\n\n" +
               string("You take the staircase down.\n\nYou are now ") + to_string(count) + string(" levels under Erebor."));

    return txt;
}


/**
 *    Randomly select an element from a text array,
 *    skewing the results toward the first elements.
 *    Use the "exp" parameter to control how strong the skewing is.
 */
const char* skewed_choice(const char* arr[], const int length, const int exp) {
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return arr[(int)(length * pow(r, exp))];
}
