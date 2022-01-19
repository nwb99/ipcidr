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
    if (ipaddr.find(delim4_, 0) != string::npos) {
        ip4Parse(ipaddr);
    }
    else {
        ip6_ = true;
        ip6Parse(ipaddr);
    }
}

void ip::ip4Parse(string ipaddr)
{
    string addrSubstr;
    size_t curpos = 0, epos, dlen = 1,
        iplen = ipaddr.length();

    // Find index of delimiter until end of string is reached.
    while ((epos = ipaddr.find(delim4_, curpos)) != string::npos) {
        addrSubstr = ipaddr.substr(curpos, (epos - curpos));
        v_ipaddr_.push_back(stoi(addrSubstr));
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
        v_ipaddr_.push_back(strtol(addrSubstr.c_str(), &posptr, 10));
        // if all characters were valid integers, last match should be null.
        if (*posptr != '\0') {
            throw std::invalid_argument("IP not an int");
        }
    }

    ipaddrVSize_ = v_ipaddr_.size();

    if (ipaddrVSize_ > 4 || ipaddrVSize_ < 4) {
        throw invalidIP("invalidformat");
    }

    for (size_t i = 0; i < ipaddrVSize_; ++i) {
        if (v_ipaddr_[i] < 0 || v_ipaddr_[i] > 255) {
            throw invalidIP("outofrange4");
        }
    }

}

// not working and unfinished
void ip::ip6Parse(string ipaddr)
{
    string addrSubstr;
    const string allowedChars = "0123456789abcdefABCDEF";
    size_t curpos = 0, epos,
        iplen = ipaddr.length();

    // Find index of delimiter until end of string is reached.
    while ((epos = ipaddr.find(delim6_, curpos)) != string::npos) {
        addrSubstr = ipaddr.substr(curpos, (epos - curpos));

        if (addrSubstr.find_first_not_of(allowedChars) == string::npos) {
            throw invalidIP("invalidformat6");
        }

        v_ipaddr6_.push_back(addrSubstr);
        if (ipaddr[epos++] == delim6_) {
            epos += 2;
            v_ipaddr6_.push_back("0");
        }
        else {
            epos++;
        }
        curpos = epos;
    }

    ipaddrVSize_ = v_ipaddr6_.size();
}


const string ip::getIP()
{
   std::stringstream ip;

    // ipv6 not done
    if (ip6_) {
        for (size_t i = 0; i < ipaddrVSize_; ++i) {
            ip << v_ipaddr6_[i];
            if (i != (ipaddrVSize_ - 1)) {
                ip << ":";
            }
        }
    }
    else {  // IPv4
        for (size_t i = 0; i < ipaddrVSize_; ++i) {
           ip << v_ipaddr_[i];

           // don't append delimiter to end of string
           if (i != (ipaddrVSize_ - 1)) {
               ip << ".";
           }
        } 
    }

   return ip.str();
}
