/*
IPCIDR Library Header
(c) 2022 Nathan "nwb99" Barnett
LGPLv3, see LICENSE.md
*/

#define VERSION 1.0.0

#ifndef IPCIDR_H
#define IPCIDR_H

#include <vector>
#include <stdexcept>
#include <string>

class ip {
public:
    ip(std::string ipaddr);
    void ip4Parse(std::string ipaddr);
    void ip6Parse(std::string ipaddr);
    void ipType(std::string ipaddr);
    const std::string getIP();

private:
    std::vector<int> v_ipaddr_;
    std::vector<std::string> v_ipaddr6_;
    std::vector<int> v_nmask_;
    int leadbits_;
    size_t ipaddrVSize_;
    bool ip6_;

    const char delim4_ = '.';
    const char delim6_ = ':';
};

class invalidIP : public std::runtime_error {
public:
    invalidIP(const std::string& what) : std::runtime_error(what) {};
};

#endif