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

    void insertDown(Node *M){
        Node* const& U=this;
        Node*& D=down;
        Node*& L=down->left;
        Node*& R=down->right;
        M->up = U;
        M->down = D;
        M->left = L;
        M->right = R;
        U->down = M;
        D->up = M;
        L->right = M;
        R->left = M;
    }

    void insertUp(Node *M){
        Node*& U=up;
        Node* const& D=this;
        Node*& L=up->left;
        Node*& R=up->right;
        M->up = U;
        M->down = D;
        M->left = L;
        M->right = R;
        U->down = M;
        D->up = M;
        L->right = M;
        R->left = M;
    }

    void insertRight(Node *M){
        Node* const& L=this;
        Node*& R=right;
        Node*& U=right->up;
        Node*& D=right->down;
        M->up = U;
        M->down = D;
        M->left = L;
        M->right = R;
        U->down = M;
        D->up = M;
        L->right = M;
        R->left = M;
    }

    void insertLeft(Node *M){
        Node*& L=left;
        Node* const& R=this;
        Node*& U=left->up;
        Node*& D=left->down;
        M->up = U;
        M->down = D;
        M->left = L;
        M->right = R;
        U->down = M;
        D->up = M;
        L->right = M;
        R->left = M;
    }

    void deleteDown(){
        Node* M=this->down;
        Node* U=M->up;
        Node* D=M->down;
        Node* L=M->left;
        Node* R=M->right;
        U->down = D;
        L->right = D;
        R->left = D;
        D->up = U;
        D->left = L;
        D->right = R;
        delete M;
    }

    void deleteUp(){
        Node* M=this->up;
        Node* U=M->up;
        Node* D=M->down;
        Node* L=M->left;
        Node* R=M->right;
        U->down = D;
        L->right = D;
        R->left = D;
        D->up = U;
        D->left = L;
        D->right = R;
        delete M;
    }

    void deleteRight(){
        Node* M=this->right;
        Node* U=M->up;
        Node* D=M->down;
        Node* L=M->left;
        Node* R=M->right;
        L->right = R;
        U->down = R;
        D->up = R;
        R->left = L;
        R->up = U;
        R->down = D;
        delete M;
    }

    void deleteLeft(){
        Node* M=this->left;
        Node* U=M->up;
        Node* D=M->down;
        Node* L=M->left;
        Node* R=M->right;
        L->right = R;
        U->down = R;
        D->up = R;
        R->left = L;
        R->up = U;
        R->down = D;
        delete M;
    }
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

void GenericData::rowAppend(const string rowName){
    Node* p = new Node(rowName);
    p->left = p;
    p->right = p;
    head->insertUp(p);
    rowHead.push_back(p);
    numRow=(int)rowHead.size();
}

void GenericData::colAppend(const string colName){
    Node* p = new Node(colName);
    p->up = p;
    p->right = p;
    head->insertLeft(p);
    colHead.push_back(p);
    numCol=(int)colHead.size();
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
                colAppend(val);
                val.clear();
            }
            else if(line[i]=='"'){
                for(++i;line[i]!='"'&&i<line.size();++i)
                    val.push_back(line[i]);
            }
            else
                val.push_back(line[i]);
        }
    }

    while(fileIn) {
        getline(fileIn,line);
        val.clear();
        if(rowNameFlag)
            return false;//可添加处理代码
        else
            rowAppend();
        for(i=0;i<line.size();++i){
            if(line[i]==','){
                rowHead[numRow-1]->insertLeft(new Node(val));
                val.clear();
            }
            else if(line[i]=='"'){
                for(++i;line[i]!='"'&&i<line.size();++i)
                    val.push_back(line[i]);
            }
            else
                val.push_back(line[i]);
        }
    }
    cout<<"Load end!"<<endl;
    return true;
}

void GenericData::saveCsv(string filePath){
    using std::ofstream;

    //ofstream cout(filePath);
    if(rowNameFlag)
        cout<<',';
    if(colNameFlag){
        cout<<colHead[0]->value;
        for(int i=1;i<numCol;++i)
            cout<<','<<colHead[i]->value;
    }
    system("pause");
    for(auto p:rowHead){
        if(rowNameFlag)
            cout<<p->value<<',';
        Node* q=p->right;
        cout<<q->value;
        for(q=q->right;q!=p;q=q->right){
            cout<<','<<q->value;
            system("pause");
        }
    }
    cout<<"Save end!"<<endl;
}

void GenericData::test(){
    GenericData gData;
    gData.loadCsv("E:/scientific research/experiment/data/Titanic Machine Learning from Disaster/train.csv");
    gData.saveCsv("E:/scientific research/experiment/data/Titanic Machine Learning from Disaster/train1.csv");
}
