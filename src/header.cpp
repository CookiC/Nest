#include "header.h"

bool std::is_digit(const string& str){
    int numPoint=0;
    for(auto c:str)
        if(c=='.'){
            ++numPoint;
            if(numPoint>1)
                return false;
        }
        else if(!isdigit(c))
            return false;
    return true;
}

double std::to_double(const string& str){
    double res=0;
    double div=1;
    for(auto c:str)
        if(c=='.')
            div=10;
        else{
            res = res*10+c-'0';
            if(div>1)
                div*=10;
        }
    return res/div;
}
