/*
 * IPCIDR example library use
 * (c) 2022 Nathan "nwb99" Barnett
 * MIT License
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cstring>
#include "ipcidr.h"

using std::cout;
using std::cin;
using std::string;
using std::cerr;
using std::endl;


int main(int argc, char *argv[])
{

// error strings
string ip4_oor = "error: IPv4 addresses must have values between 0 and 255.";
string ip4_invalidfmt = "error: IPv4 addresses are quad-dotted.";

try {
    ip ipIn(argv[1]);
    cout << "IP Address: " << ipIn.getIP() << endl;
}
catch (std::invalid_argument&) {
    cerr << "error: invalid input" << endl;
    return EXIT_FAILURE;
}
catch (std::out_of_range&){
    cerr << "error: out of range exception" << endl;
    return EXIT_FAILURE;
}
catch (invalidIP& errwhat) {
    const char oor[12] = "outofrange4";
    const char badformat[14] = "invalidformat";
    if ((strcmp(errwhat.what(), oor) == 0)) {
        cerr << ip4_oor << endl;
        return EXIT_FAILURE;
    }
    else if ((strcmp(errwhat.what(), badformat)) == 0) {
        cerr << ip4_invalidfmt << endl;
        return EXIT_FAILURE;
    }
}


return EXIT_SUCCESS;

}
