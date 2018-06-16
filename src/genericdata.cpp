#include "genericdata.h"

//private:

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

void GenericData::appendRowHead(const QString& name){
    Node* p = new Node(name);
    p->left = p;
    p->right = p;
    p->up = head->up;
    p->down = head;
    p->up->down = p;
    head->up = p;
    rowHead.append(p);
    numRow=(int)rowHead.size();
}

void GenericData::appendColHead(const QString& name){
    InsertColHead(numCol,name);
}

void GenericData::deleteColHead(int index){
    Node* p = colHead[index];
    for(int i=index;i<numCol;++i)
        colHead[i]=colHead[i+1];
    colHead.removeLast();
    numCol=(int)colHead.size();
    p->left->right = p->right;
    p->right->left = p->left;
    delete p;
}

void GenericData::deleteRowHead(int index){
    Node* p = rowHead[index];
    for(int i=index;i<numRow;++i)
        rowHead[i]=rowHead[i+1];
    rowHead.removeLast();
    numRow=(int)rowHead.size();
    p->up->down = p->down;
    p->down->up = p->up;
    delete p;
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

//public:

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

bool GenericData::appendRow(const QStringList &row, const QString &name){
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

void GenericData::colStrSplit(int index, const QRegularExpression &regExp){
    QString name = colHead[index]->str;
    Node *h,*p;
    QStringList *row = new QStringList[numRow];
    QStringList col;
    int i,j,size=0;

    h=colHead[index];
    i=0;
    for(p=h->down;p!=h;p=p->down){
        row[i] = p->str.split(regExp);
        if(row[i].size()>size)
            size = row[i].size();
        ++i;
    }
    deleteCol(index);

    for(i=0;i<size;++i){
        col.clear();
        for(j=0;j<numRow;++j){
            if(row[j].size()-size+i>=0)
                col.append(row[j][row[j].size()-size+i]);
            else
                col.append("");
        }
        insertCol(index+i,col,name+"#"+QString::number(i));
    }
}

void GenericData::colStrSplit(int index, const QString& delimiter, bool repeat){
    QString name=colHead[index]->str;
    bool canSplit;
    Node *q,*p;
    QStringList l;
    int i,j,k=delimiter.size();

    i=0;
    do{
        canSplit=false;
        l.clear();
        p=colHead[i+index];
        for(q=p->down;q!=p;q=q->down){
           j=q->str.indexOf(delimiter);
           if(j==-1)
               l.push_back("");
           else{
               canSplit=true;
               l.push_back(q->str.left(j));
               q->str.remove(0,j+k);
           }
        }
        if(canSplit){
            insertCol(i+index,l,name+'#'+QString::number(i));
            ++i;
        }
    }while(canSplit&&repeat);
    p->str=name+'#'+QString::number(i);
}


void GenericData::colStrSplit(const QString& name, const QRegularExpression &regExp){
    int i=getColIndex(name);
    if(i<0)
        return;
    colStrSplit(i, regExp);
}

void GenericData::colStrSplit(const QString& name, const QString& delimiter, bool repeat){
    int i=getColIndex(name);
    if(i<0)
        return;
    colStrSplit(i,delimiter,repeat);
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

bool GenericData::deleteCol(const QString& name){
    int i=getColIndex(name);
    if(i<0)
        return false;
    deleteCol(i);
    return true;
}

int GenericData::getColIndex(const QString& name){
    for(int i=0;i<numCol;++i)
        if(colHead[i]->str==name)
            return i;
    return -1;
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

bool GenericData::loadCsv(QString filePath, bool hasColName, bool hasRowName){
    colNameFlag = hasColName;
    rowNameFlag = hasRowName;
    QFile fileIn(filePath);
    QByteArray line,val,rowName="";
    fileIn.open(QIODevice::ReadOnly | QIODevice::Text);
    int i;
    if(fileIn.isOpen()&&colNameFlag){
        line=fileIn.readLine();
        i=0;
        if(rowNameFlag){
            rowName.clear();
            while(line[i]!=','&&i<line.size()-1){
                rowName.append(line[i]);
                ++i;
            }
            head->str = rowName;
            ++i;
        }
        val.clear();
        while(i<line.size()-1){
            if(line[i]==','){
                appendColHead(val);
                val.clear();
            }
            else if(line[i]=='"'){
                for(++i;line[i]!='"'&&i<line.size();++i)
                    val.append(line[i]);
            }
            else
                val.append(line[i]);
            ++i;
        }
        appendColHead(val);
    }
    cout<<"Colhead load Success!"<<endl;
    QStringList row;
    while(!fileIn.atEnd()) {
        line=fileIn.readLine();
        i=0;
        if(rowNameFlag){
            rowName.clear();
            while(line[i]!=','&&i<line.size()-1){
                rowName.append(line[i]);
                ++i;
            }
            head->str = rowName;
            ++i;
        }
        val.clear();
        row.clear();
        while(i<line.size()-1){
            if(line[i]==','){
                row.push_back(val);
                val.clear();
            }
            else if(line[i]=='"'){
                for(++i;line[i]!='"'&&i<line.size();++i)
                    val.append(line[i]);
            }
            else
                val.append(line[i]);
            ++i;
        }
        row.append(val);
        appendRow(row,rowName);
    }
    fileIn.close();
    cout<<"Load Success!"<<endl;
    return true;
}

void GenericData::saveCsv(QString filePath){
    QFile file(filePath);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&file);
    if(colNameFlag){
        if(rowNameFlag)
            out<<',';
        out<<colHead[0]->str;
        for(int i=1;i<numCol;++i)
            out<<','<<colHead[i]->str;
        out<<'\n';
    }
    for(auto p:rowHead){
        if(rowNameFlag)
            out<<p->str<<',';
        Node* q=p->right;
        out<<q->str;
        for(q=q->right;q!=p;q=q->right)
            if(q->str.contains(','))
                out<<','<<'"'<<q->str<<'"';
            else
                out<<','<<q->str;
        out<<'\n';
    }
    file.close();
}

StandardData* GenericData::toStandardData(){
    bool ok;
    int i,j;
    Node *p;
    StandardData *stdData=new StandardData(numRow, numCol);

    if(colNameFlag)
        for(i=0;i<numCol;++i)
            stdData->colName.append(colHead[i]->str);
    if(rowNameFlag)
        for(i=0;i<numRow;++i)
            stdData->rowName.append(rowHead[i]->str);

    StandardData::DataType *type=stdData->type;
    double **data=stdData->data;
    bool **missing=stdData->missing;
    QStringList *nomName=stdData->nomName;

    for(j=0;j<numCol;++j){
        p=colHead[j]->down;
        type[j]=StandardData::NUM;
        for(i=0;i<numRow;++i){
            if(p->str.isEmpty())
                missing[i][j]=true;
            else{
                missing[i][j]=false;
                if(type[j]==StandardData::NUM){
                    data[i][j]=p->str.toDouble(&ok);
                    if(!ok)
                        type[j]=StandardData::NOM;
                }
            }
            p=p->down;
        }
    }
    QMap<QString, int> m;
    for(j=0;j<numCol;++j){
        p=colHead[j];
        if(type[j]==StandardData::NOM){
            m.clear();
            for(i=0;i<numRow;++i){
                if(m.contains(p->str))
                    data[i][j]=m.value(p->str);
                else{
                    data[i][j]=m.size();
                    nomName[j].append(p->str);
                    m.insert(p->str,m.size());
                }
                p=p->down;
            }
        }
    }
    deb<<"Transform success！";
    return stdData;
}

void GenericData::test(){
    GenericData genData;
    StandardData *stdData;
    genData.loadCsv("E:/scientific research/experiment/data/Digit Recognizer/test.csv",1);
    stdData=genData.toStandardData();
    stdData->saveCsv("E:/scientific research/experiment/data/Digit Recognizer/test1.csv");
    delete stdData;
}
