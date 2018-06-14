#include "nstring.h"

bool NString::isDigit(){
    int numPoint=0;
    for(auto c:*this)
        if(c=='.'){
            ++numPoint;
            if(numPoint>1)
                return false;
        }
        else if(!isdigit(c))
            return false;
    return true;
}

double NString::toDouble(){
    double res=0;
    double div=1;
    for(auto c:*this)
        if(c=='.')
            div=10;
        else{
            res = res*10+c-'0';
            if(div>1)
                div*=10;
        }
    return res/div;
}
