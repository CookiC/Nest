#include "genericdata.h"

class GenericData::Node{
public:
    Node* up;
    Node* down;
    Node* left;
    Node* right;
    string value;
    Node():up(nullptr),down(nullptr),left(nullptr),right(nullptr),value(""){}
    Node(string value):up(nullptr),down(nullptr),left(nullptr),right(nullptr),value(value){}
};

GenericData::GenericData(){
    Node* p = new Node();
    p->up = p;
    p->down = p;
    p->left = p;
    p->right = p;
    head = p;
}

GenericData::~GenericData(){
    for(auto p:colHead){
        while(p->up!=p)
            p->deleteUp();
    }
    while(head->up!=head)
        head->deleteUp();
    while(head->left!=head)
        head->deleteLeft();
}

void GenericData::appendRowHead(const string rowName){
    Node* p = new Node(rowName);
    p->left = p;
    p->right = p;
    p->up = head->up;
    p->down = head;
    p->up->down = p;
    head->up = p;
    rowHead.push_back(p);
    numRow=(int)rowHead.size();
}

void GenericData::appendColHead(const string colName){
    Node* p = new Node(colName);
    p->up = p;
    p->down = p;
    p->left = head->left;
    p->right = head->right;
    p->left->right = p;
    head->left = p;
    colHead.push_back(p);
    numCol=(int)colHead.size();
}

void GenericData::deleteRowHead(int index){
    Node* p = rowHead[index];
    for(int i=index;i<numRow;++i)
        rowHead[i]=rowHead[i+1];
    rowHead.pop_back();
    numRow=(int)rowHead.size();
    p->up->down = p->down;
    p->down->up = p->up;
    delete p;
}

bool GenericData::appendRow(const GenericList &row, const string name){
    appendRowHead(name);
    if(row.size()!=numCol)
        return false;
    Node* q;
    Node* p=*rowHead.rbegin();
    for(auto s:row){
        q = new Node(s);
        Node*& L = p->left;
        Node*& R = p;
        Node*& U = L->up->right;
        Node*& D = L->down->right;
        q->right = R;
        q->left = L;
        q->up = U;
        q->down = D;
        L->right = q;
        R->left = q;
        U->down = q;
        D->up = q;
    }
}

void GenericData::deleteRow(int i){

}

bool GenericData::loadCsv(string filePath, bool hasRowName, bool hasColName){
    using std::ifstream;
    using std::getline;

    rowNameFlag = hasRowName;
    colNameFlag = hasColName;
    ifstream fileIn(filePath);
    string line,val;
    int i;
    if(fileIn&&colNameFlag){
        getline(fileIn,line);
        val.clear();
        for(i=0;i<line.size();++i){
            if(line[i]==','){
                appendColHead(val);
                val.clear();
            }
            else if(line[i]=='"'){
                for(++i;line[i]!='"'&&i<line.size();++i)
                    val.push_back(line[i]);
            }
            else
                val.push_back(line[i]);
        }
        appendColHead(val);
    }

    string rowName;
    GenericList row;
    while(fileIn) {
        getline(fileIn,line);
        val.clear();
        row.clear();
        if(rowNameFlag)
            rowName="";//可添加处理代码
        else
            rowName="";
        for(i=0;i<line.size();++i){
            if(line[i]==','){
                row.push_back(val);
                val.clear();
            }
            else if(line[i]=='"'){
                for(++i;line[i]!='"'&&i<line.size();++i)
                    val.push_back(line[i]);
            }
            else
                val.push_back(line[i]);
        }
        row.push_back(val);
        appendRow(row,rowName);
    }
    cout<<"Load end!"<<endl;
    return true;
}

void GenericData::saveCsv(string filePath){
    using std::ofstream;

    ofstream fileOut(filePath);
    if(rowNameFlag)
        fileOut<<',';
    if(colNameFlag){
        fileOut<<colHead[0]->value;
        for(int i=1;i<numCol;++i)
            fileOut<<','<<colHead[i]->value;
        fileOut<<endl;
    }
    for(auto p:rowHead){
        if(rowNameFlag)
            fileOut<<p->value<<',';
        Node* q=p->right;
        fileOut<<q->value;
        for(q=q->right;q!=p;q=q->right)
            if(q->value.find(',')==string::npos)
                fileOut<<','<<q->value;
            else
                fileOut<<','<<'"'<<q->value<<'"';
        fileOut<<endl;
    }
}

void GenericData::test(){
    GenericData gData;
    gData.loadCsv("E:/scientific research/experiment/data/Titanic Machine Learning from Disaster/train.csv");
    gData.saveCsv("E:/scientific research/experiment/data/Titanic Machine Learning from Disaster/train1.csv");
}
