
#include <random>
#include <string>
#include "player.h"

using namespace std;

// forward declarations
const char* choice(const char* arr[], const int length);


// Tolkien & Norse Dwarf names, with slight variations
static const char* dwarf_names[] = {"Ai","Alf","Althin","Althror","Anar","Andor","Andwari","Andwarin","Aurwang",
                                    "Austri","Azagh","Azaghâl","Azaghnin","Azaghnir","Azaghnor","Azalin","Bafur",
                                    "Balin","Beli","Bifur","Bilin","Bláin","Bofur","Bombur","Borin","Bruni","Buri",
                                    "Burin","Dofur","Dolgen","Dori","Dorin","Draupnin","Draupnir","Duf","Dufin",
                                    "Durin","Dwalin","Dáin","Dís","Fair","Farin","Fid","Fidnir","Finn","Flói","Frerin",
                                    "Frár","Frór","Fundin","Fundir","Fíli","Gamil","Gamlin","Gimli","Ginar","Gingar",
                                    "Glóin","Grolin","Gróin","Grór","Hanar","Har","Harin","Hepti","Hornbori","Hornbur",
                                    "Hrasin","Hrasir","Hror","Hón","Ingi","Janir","Jari","Jarin","Khorin","Korin",
                                    "Kurin","Kíli","Kóna","Lit","Lófar","Lóni","Modnir","Modsognir","Modsudrin",
                                    "Modwitnir","Már","Narvi","Nidi","Niping","Nordri","Nori","Nyi","Nyir","Nyirin",
                                    "Nyr","Nyrad","Náin","Náli","Nár","Nói","Póri","Radswid","Regin","Rekk","Sin",
                                    "Skafid","Skirfir","Sognin","Sognir","Sudri","Swid","Telchar","Thek","Thorin",
                                    "Throin","Thráin","Thrór","Vestri","Vidar","Vili","Wali","Wig","Wigli","Wili",
                                    "Wirfir","Wit","Óin","Ónar","Óri","Órin"};
static const int n_dwarf_names = sizeof(dwarf_names) / sizeof(char *);
// Tolkien Human names
static const char* human_names[] = {"Ada","Adel","Aggy","Ash","Aud","Avi","Babs","Bell","Berni",  // begin female names
                                    "Berthoc","Berti","Bev","Cadi","Daisy","Darla","Dawn","Dee","Dera","Dernhild",
                                    "Dernwyn","Dicra","Dora","Dot","Déorhild","Déorwyn","Edi","Elfhild","Elfwyn",
                                    "Emmy","Fay","Fern","Flo","Gertie","Gilraen","Gléowyn","Guthwyn","Hattie","Hayley",
                                    "Hazel","Heled","Heruwyn","Hettie","Hild","Holdwyn","Holly","Ioreth","Ivorwen",
                                    "Ivy","Joy","Lily","Lindy","Lothíriel","Léofwyn","Marge","Milly","Modron","Morwen",
                                    "Robyn","Rosa","Rosalin","Rosalind","Rosamund","Rose","Sage","Sarf","Sath",
                                    "Théodwyn","Tilly","Violet","Wilhelmina","Winnie","Éohild","Éowyn",
                                    "Adrahil","Aldor","Alwis","Amlaith","Anardil","Anborn","Angbor",  // begin male names
                                    "Anárion","Arador","Araglas","Aragorn","Aragost","Arahad","Arahael","Aranarth",
                                    "Arannel","Aranuir","Araphant","Araphor","Arassuil","Arathorn","Araval","Aravir",
                                    "Aravorn","Argeleb","Argonui","Arvedui","Arvegil","Arveleg","Asabiarn","Baldor",
                                    "Baldred","Baranor","Bard","Belecthor","Beleg","Belegorn","Beregond","Beren",
                                    "Bergil","Biarn","Bondi","Booth","Boromir","Brand","Brander","Bregdan","Brego",
                                    "Brytta","Celepharn","Ceorl","Cirion","Dag","Damrod","Delling","Denethor","Dernfara",
                                    "Derngar","Dernhelm","Dernhild","Dernwine","Dernwyn","Derufin","Dervorin","Duilin",
                                    "Duinhir","Déor","Déorbrand","Déorgar","Déorhelm","Déorhild","Déorthain","Déorwine",
                                    "Déorwyn","Dírhael","Dúnhere","Ecthelion","Egalmoth","Einar","Eldacar","Elfhelm",
                                    "Elfhild","Elfwine","Elfwyn","Eradan","Eric","Erkenbrand","Erland","Faramir","Farman",
                                    "Fasthelm","Fastred","Fengel","Findegil","Finduilas","Folca","Folcred","Folcwine",
                                    "Forlong","Fram","Freca","Frumgar","Fréa","Fréaláf","Fréawine","Fulgar","Fulgrim",
                                    "Fulor","Fulthain","Gamling","Gléobeam","Gléomer","Gléothain","Gléowine","Gléowyn",
                                    "Golasgil","Gus","Gustaw","Guthwyn","Gálmód","Gárbald","Gárulf","Gárwine","Hakon",
                                    "Halbarad","Hallam","Hallas","Halstein","Halward","Hamar","Harald","Heruwyn","Hild",
                                    "Hirgon","Hirluin","Holdwyn","Holgeir","Húrin","Ingar","Ingemar","Ingold","Iorlas",
                                    "Iwar","Kell","Knut","Lif","Lunt","Léofwyn","Mablung","Malbeth","Malvegil","Odell",
                                    "Ohtar","Olaf","Orodreth","Ranulf","Ric","Roald","Rutland","Sigurd","Skip","Stig",
                                    "Storr","Sutherland","Swain","Tate","Thor","Thorald","Thorburn","Thorondir",
                                    "Thorongil","Théodwyn","Tor","Tore","Torwald","Turgon","Eadig","Eadmód","Eoblod",  // É
                                    "Eogar","Eoheort","Eohere","Eohild","Eoman","Eomer","Eomund","Eomód","Eorcanstan",
                                    "Eored","Eorl","Eothain","Eowine","Eowyn"};
static const int n_human_names = sizeof(human_names) / sizeof(char *);


/**
 *  Default initializer
 */
PlayerData::PlayerData() {
    loc[0] = 1;
    loc[1] = 1;
    name = choice(dwarf_names, n_dwarf_names);
    parent1 = choice(dwarf_names, n_dwarf_names);  // dwarf
    if (name == parent1) {
        name += string(" II");
    }
    parent2 = choice(human_names, n_human_names);  // human
    visited = new bool[1];
}


/**
 *  Helper method, to re-init the player name.
 */
void PlayerData::reset_names() {
    name = choice(dwarf_names, n_dwarf_names);
    parent1 = choice(dwarf_names, n_dwarf_names);  // dwarf
    if (name == parent1) {
        name += string(" II");
    }
    parent2 = choice(human_names, n_human_names);  // human
}


/**
 *    Randomly select an element from an array.
 */
const char* choice(const char* arr[], const int length) {
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return arr[(int)(length * r)];
}
