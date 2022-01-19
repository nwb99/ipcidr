/*
 * IPCIDR Library Implementation
 * (c) 2022 Nathan "nwb99" Barnett
 * LGPLv3, see LICENSE.md
 */

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include "ipcidr.h"

using std::string;
using std::vector;

ip::ip(string ipaddr)
{
    ip4Parse(ipaddr);
}

void ip::ipType(string ipaddr)
{
    string ip4Delim = ".";

    if (ipaddr.find(ip4Delim, 0) != string::npos) {
        ip4Parse(ipaddr);
    }
    else {
        ip6_ = true;
        ip6Parse(ipaddr);
    }
}

void ip::ip4Parse(string ipaddr)
{
    char delim = '.';
    string addrSubstr;
    size_t curpos = 0, epos, dlen = 1,
        iplen = ipaddr.length();

    // Find index of delimiter until end of string is reached.
    while ((epos = ipaddr.find(delim, curpos)) != string::npos) {
        addrSubstr = ipaddr.substr(curpos, (epos - curpos));
        ipaddr_.push_back(stoi(addrSubstr));
        epos += dlen;
        curpos = epos;
    }
    /*
     * Since IP addresses are typically not terminated with delimiter,
     * check and find the last part of address
     */
    if ((iplen - curpos) <= 3) {
        addrSubstr = ipaddr.substr(curpos, (iplen - curpos));

        char* posptr;
        ipaddr_.push_back(strtol(addrSubstr.c_str(), &posptr, 10));
        if (*posptr != '\0') {
            throw std::invalid_argument("IP not an int");
        }
    }

    ipaddrSize_ = ipaddr_.size();

    if (ipaddrSize_ > 4 || ipaddrSize_ < 4) {
        throw invalidIP("invalidformat");
    }

    for (size_t i = 0; i < ipaddrSize_; ++i) {
        if (ipaddr_[i] < 0 || ipaddr_[i] > 255) {
            throw invalidIP("outofrange4");
        }
    }

}

void ip::ip6Parse(string ipaddr)
{
    string delim = ":";
    string addrSubstr;
    string allowedChars = "0123456789abcdefABCDEF";
    size_t curpos = 0, epos, dlen = delim.length(),
        iplen = ipaddr.length();

    // Find index of delimiter until end of string is reached.
    while ((epos = ipaddr.find(delim, curpos)) != string::npos) {
        addrSubstr = ipaddr.substr(curpos, (epos - curpos));

        if (addrSubstr.find_first_not_of(allowedChars) == string::npos) {
            throw invalidIP("invalidformat6");
        }

        ipaddr6_.push_back(addrSubstr);
        if (ipaddr[epos++] == delim[0]) {
            epos += (dlen + 1);
            ipaddr6_.push_back("0");
        }
        else {
            epos += dlen;
        }
        curpos = epos;
    }

    ipaddrSize_ = ipaddr6_.size();
}


const string ip::getIP()
{
   std::stringstream ip;

    if (ip6_) {
        for (size_t i = 0; i < ipaddrSize_; ++i) {
            ip << ipaddr6_[i];
            if (i != (ipaddrSize_ - 1)) {
                ip << ":";
            }
        }
    }
    else {  // IPv4
        for (size_t i = 0; i < ipaddrSize_; ++i) {
           ip << ipaddr_[i];
           if (i != (ipaddrSize_ - 1)) {
               ip << ".";
           }
        } 
    }

   return ip.str();
}
