#include "genericdata.h"

class GenericData::Node{
public:
    Node* up;
    Node* down;
    Node* left;
    Node* right;
    QString str;
    Node():up(nullptr),down(nullptr),left(nullptr),right(nullptr),str(""){}
    Node(QString value):up(nullptr),down(nullptr),left(nullptr),right(nullptr),str(value){}
};

GenericData::GenericData(){
    Node* p = new Node();
    p->up = p;
    p->down = p;
    p->left = p;
    p->right = p;
    head = p;
    numCol = 0;
    numRow = 0;
}

GenericData::~GenericData(){
}

void GenericData::appendRowHead(const QString& rowName){
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

void GenericData::deleteRow(int index){
    Node *p,*q;
    for(p=rowHead[index]->left;p!=rowHead[index];p=q){
        q=p->left;
        p->up->down=p->down;
        p->down->up=p->up;
        delete p;
    }
    deleteRowHead(index);
}

bool GenericData::deleteRow(const QString& name){
    for(int i=0;i<numCol;++i)
        if(rowHead[i]->str==name){
            deleteRow(i);
            return true;
        }
    return true;
}

void GenericData::appendColHead(const QString& name){
    InsertColHead(numCol,name);
}

void GenericData::InsertColHead(int index, const QString &name){
    Node *q;
    if(index<numCol)
        q=colHead[index];
    else if(index==numCol)
        q=head;
    else
        return;
    Node *p = new Node(name);
    p->up = p;
    p->down = p;
    p->left = q->left;
    p->right = q;
    p->left->right = p;
    p->right->left = p;
    colHead.push_back(nullptr);
    for(int i=numCol;i>index;--i)
        colHead[i]=colHead[i-1];
    colHead[index]=p;
    numCol=(int)colHead.size();
}

void GenericData::deleteColHead(int index){
    Node* p = colHead[index];
    for(int i=index;i<numCol;++i)
        colHead[i]=colHead[i+1];
    colHead.pop_back();
    numCol=(int)colHead.size();
    p->left->right = p->right;
    p->right->left = p->left;
    delete p;
}

void GenericData::deleteCol(int index){
    Node *p,*q;
    for(p=colHead[index]->up;p!=colHead[index];p=q){
        q=p->up;
        p->left->right=p->right;
        p->right->left=p->left;
        delete p;
    }
    deleteColHead(index);
}

int GenericData::getColIndex(const QString& name){
    for(int i=0;i<numCol;++i)
        if(colHead[i]->str==name)
            return i;
    return -1;
}

bool GenericData::deleteCol(const QString& name){
    int i=getColIndex(name);
    if(i<0)
        return false;
    deleteCol(i);
    return true;
}

bool GenericData::appendRow(const QStringList &row, const QString& name){
    if(row.size()!=numCol)
        return false;
    appendRowHead(name);
    Node *q,*U,*R,*L,*D;
    Node *p=*rowHead.rbegin();
    for(auto s:row){
        q = new Node(s);
        L = p->left;
        R = p;
        U = L->up->right;
        D = L->down->right;
        q->right = R;
        q->left = L;
        q->up = U;
        q->down = D;
        L->right = q;
        R->left = q;
        U->down = q;
        D->up = q;
    }
    return true;
}

bool GenericData::insertCol(int index, const QStringList &col, const QString &name){
    if(col.size()!=numRow)
        return false;
    InsertColHead(index, name);
    Node *q,*U,*R,*L,*D;
    Node *p=colHead[index];
    for(auto s:col){
        q = new Node(s);
        U = p->up;
        D = p;
        L = U->left->down;
        R = U->right->down;
        q->right = R;
        q->left = L;
        q->up = U;
        q->down = D;
        L->right = q;
        R->left = q;
        U->down = q;
        D->up = q;
    }
    return true;
}

bool GenericData::loadCsv(QString filePath, bool hasRowName, bool hasColName){
    using std::ifstream;
    using std::getline;

    rowNameFlag = hasRowName;
    colNameFlag = hasColName;
    ifstream fileIn(filePath);
    QString line,val;
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

    QString rowName;
    QStringList row;
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

void GenericData::saveCsv(QString filePath){
    using std::ofstream;

    ofstream fileOut(filePath);
    if(rowNameFlag)
        fileOut<<',';
    if(colNameFlag){
        fileOut<<colHead[0]->str;
        for(int i=1;i<numCol;++i)
            fileOut<<','<<colHead[i]->str;
        fileOut<<endl;
    }
    for(auto p:rowHead){
        if(rowNameFlag)
            fileOut<<p->value<<',';
        Node* q=p->right;
        fileOut<<q->str;
        for(q=q->right;q!=p;q=q->right)
            if(q->str.find(',')==QString::npos)
                fileOut<<','<<q->str;
            else
                fileOut<<','<<'"'<<q->str<<'"';
        fileOut<<endl;
    }
}

void GenericData::colStrSplit(const QString& name, const QString& delimiter, bool repeat){
    int i=getColIndex(name);
    if(i<0)
        return;
    colStrSplit(i,delimiter);
}

void GenericData::colStrSplit(int index, const QString& delimiter, bool repeat){
    QString name=colHead[index]->str;
    bool canSplit;
    Node *q,*p;
    QStringList l;
    int i,j,k=(int)delimiter.size();

    i=0;
    do{
        canSplit=false;
        l.clear();
        p=colHead[i+index];
        for(q=p->down;q!=p;q=q->down){
           j=(int)q->str.find(delimiter);
           if(j==QString::npos)
               l.push_back("");
           else{
               canSplit=true;
               l.push_back(q->str.substr(0,j));
               q->str.erase(0,j+k);
           }
        }
        if(canSplit){
            insertCol(i+index,l,name+'#'+std::to_string(i));
            ++i;
        }
    }while(canSplit&&repeat);
    p->str=name+'#'+std::to_string(i);
}

StandardData& GenericData::toStandardData(){
    using StandardData::DataType;
    using StandardData::NUM;
    using StandardData::NOM;

    bool ok;
    int i,j;
    Node *p;
    StandardData *stdData=new StandardData(numRow, numCol);

    //主要完成以下数据的转换
    DataType *type=stdData->type;
    double **data=stdData->data;
    bool **missing=stdData->missing;
    QStringList *nomName=stdData->nomName;

    //检测数据类型，是否缺失，并完成连续值类型的转换
    for(i=0;i<numCol;++i){
        p=colHead[i];
        type[i]=NUM;
        for(j=0;j<numRow;++j){
            if(p->str.isEmpty())
                missing[i][j]=true;
            else{
                missing[i][j]=false;
                if(type[i]==NUM){
                    data[i][j]=p->str.toDouble(&ok);
                    if(!ok)
                        type[i]=NOM;
                }
            }
            p=p->down;
        }
    }

    //对每一类nom添加数字标签（ID）
    QMap<QString, int> m;
    for(i=0;i<numCol;++i){
        p=colHead[i];
        if(type[i]==NOM){
            m.clear();
            for(j=0;j<numRow;++j){
                if(m.contains(p->str))
                    data[i][j]=m.value(p->str);
                else{
                    data[i][j]=m.size();
                    nomName[j].push_back(p->str);
                    m.insert(p->str,m.size());
                }
                p=p->down;
            }
        }
    }
}

void GenericData::test(){
    GenericData genData;
    genData.loadCsv("E:/scientific research/experiment/data/Titanic Machine Learning from Disaster/train.csv");
    //gData.deleteCol("PassengerId");
    genData.colStrSplit("Name",", ");
    genData.colStrSplit("Name#1",". ");
    genData.colStrSplit("Ticket"," ");
    genData.saveCsv("E:/scientific research/experiment/data/Titanic Machine Learning from Disaster/train1.csv");
}
