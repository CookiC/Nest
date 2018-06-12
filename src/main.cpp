#include "cpython.h"
#include "genericdata.h"
#include <ctime>
#include <QByteArray>
#include <QString>
#include <QtGlobal>

int main(int argc, char *argv[]){
    int i;
    clock_t t1,t2,t3;

    t1 = clock();
    string A("1234567890"),B("0987654321"),C;
    for(i=0;i<1e8;++i){
        A += std::to_string(qrand()%10);
        A = A.substr(1);
    }

    t2 = clock();
    QByteArray a("1234567890"),b("0987654321"),c;
    for(i=0;i<1e8;++i){
        a += QByteArray::number(qrand()%10);
        a = a.mid(1);
        //cout<<a.data();
        //system("pause");
    }
    t3 = clock();
    printf("%d %d\n",t2-t1,t3-t2);
    return 0;
}
