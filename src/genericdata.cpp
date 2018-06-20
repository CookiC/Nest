#include "genericdata.h"

//private:
void GenericData::insertColName(int index, const QString& name){
    int i;
    colName.prepend(QString());
    int n = colName.size();
    for(i=n-1;i>index;--i)
        colName[i]=colName[i-1];
    colName[index]=name;
}

void GenericData::insertRowName(int index, const QString& name){
    int i;
    rowName.prepend(QString());
    int n = rowName.size();
    for(i=n-1;i>index;--i)
        rowName[i]=rowName[i-1];
    rowName[index]=name;
}

void GenericData::deleteColName(int index){
    int n = colName.size();
    for(int i=index;i<n-1;++i)
        colName[i]=colName[i+1];
    colName.removeLast();
}

void GenericData::deleteRowName(int index){
    int n = rowName.size();
    for(int i=index;i<n-1;++i)
        rowName[i]=rowName[i+1];
    rowName.removeLast();
}

QStringVector GenericData::splitCsvLine(const QByteArray &line, QString& name){
    int i = 0;
    int n = line.size()-1;
    QString t;
    QStringVector v;
    if(rowNameFlag){
        while(line[i]!=','&&i<n){
            if(line[i]=='"')
                for(++i;line[i]!='"'&&i<n;++i)
                    t.append(line[i]);
            else
                t.append(line[i]);
            ++i;
        }
        name = t;
        t.clear();
    }
    while(i<n){
        if(line[i]==','){
            v.push_back(t);
            t.clear();
        }
        else if(line[i]=='"'){
            for(++i;line[i]!='"'&&i<n;++i)
                t.append(line[i]);
        }
        else
            t.append(line[i]);
        ++i;
    }
    return v;
}

//public:

GenericData::GenericData(){
}

GenericData::~GenericData(){
}

bool GenericData::appendCol(const QStringVector& col, const QString& name){
    if(!data.appendCol(col))
        return false;
    if(colNameFlag)
        colName.append(name);
    return true;
}

bool GenericData::appendRow(const QStringVector& col, const QString& name){
    if(!data.appendRow(col))
        return false;
    if(rowNameFlag)
        rowName.append(name);
    return true;
}

bool GenericData::insertCol(int index, const QStringVector& col, const QString& name){
    if(!data.insertCol(index,col))
        return false;
    if(colNameFlag)
        insertColName(index,name);
    return true;
}

bool GenericData::insertRow(int index, const QStringVector& row, const QString& name){
    if(!data.insertRow(index,row))
        return false;
    if(rowNameFlag)
        insertRowName(index,name);
    return true;
}

bool GenericData::deleteCol(int index){
    deleteCol(index);
    deleteColName(index);
}

bool GenericData::deleteRow(int index){
    deleteRow(index);
    deleteColName(index);
}

void GenericData::colStrSplit(int index, const QRegularExpression &regExp){
    int i,j=index;
    int n=data.getRowNum();
    int size=0;
    QString name = colName[index];
    QStringList *row = new QStringList[n];
    QStringVector col;

    for(i=0;i<n;++i){
        row[i] = data.at(i,j).split(regExp);
        if(row[i].size()>size)
            size = row[i].size();
    }
    deleteCol(index);

    for(i=0;i<size;++i){
        col.clear();
        for(j=0;j<n;++j){
            if(row[j].size()-size+i>=0)
                col.append(row[j][row[j].size()-size+i]);
            else
                col.append("");
        }
        insertCol(index+i,col,name);
    }
    delete[] row;
}

void GenericData::colStrSplit(int index, int rank, const QString& delimiter){
    QString name = colName[index];
    QStringVector list;
    int i;
    int j = index;
    int k = delimiter.size();
    int l;
    int n = data.getRowNum();

    for(i=0;i<n;++i){
        l=data.at(i,j).indexOf(delimiter);
        if(l==-1)
            list.append("");
        else{
            list.append(data.at(i,j).left(j));
            data.at(i,j).remove(0,j+k);
        }
    }
    insertCol(index,l,name);
}

int GenericData::getColIndex(const QString& name){
    int n = colName.size();
    for(int i=0;i<n;++i)
        if(colName[i]==name)
            return i;
    return -1;
}

bool GenericData::loadCsv(QString path, bool hasColName, bool hasRowName){
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!file.isOpen())
        return false;

    QByteArray line;
    QString name;
    QStringList row;
    int n,i;
    colNameFlag = hasColName;
    rowNameFlag = hasRowName;

    if(colNameFlag){
        line=file.readLine();
        row = splitCsvLine(line,name);
        n=row.size();
        for(i=0;i<n;++i)
            colName.append(row[i]);
    }
    cout<<"Colhead load Success!"<<endl;
    while(!file.atEnd()) {
        line = fileIn.readLine();
        row = splitCsvLine(line,name);
        if(rowNameFlag)
            rowName.append(name);
        n=row.size();
        if(!data.appendRow(row))
            return false;
    }
    file.close();
    cout<<"Load Success!"<<endl;
    return true;
}

void GenericData::saveCsv(QString filePath){
    QFile file(filePath);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    if(!file.isOpen())
        return;
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
