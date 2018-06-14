#ifndef NSTRING_H
#define NSTRING_H

#include <string>

class NString:public std::string{
public:
    bool isDigit();
    double toDouble();
};

#endif // NSTRING_H
