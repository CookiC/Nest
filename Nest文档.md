# Nest文档

[TOC]

## 数据类

### GenericData

​	使用十字循环链表作为基础数据存储结构，插入行列的复杂度为O(N+M)，N为行数，M为列数，每个节点存储的是字符串，将支持csv、arff的读入与保存。需开发arff文件读入。
#### bool loadCsv(QString, bool=0, bool=1);
载入csv文件（文件名及路径，是否有行名，是否有列名）
#### void saveCsv(QString)

保存csv文件（文件名及路径）

#### void saveCsv(QString)

#### bool appendRow(const QStringList &, const QString& = "")

向尾部添加列（行数据，行名）

#### void colStrSplit(int, const QString &, bool=false)

列字符串分割（列号，分割符，是否多次分割）

#### void colStrSplit(int, const QRegularExpression &)

列字符串分割（列号，正则表达式）

#### void colStrSplit(const QString &, const QString&, bool=false)

列字符串分割（列名，分割符，是否多次分割）

#### void colStrSplit(const QString &, const QRegularExpression &)

列字符串分割（列名，正则表达式）

#### void deleteRow(int)

#### void deleteCol(int)

#### bool deleteRow(const QString&)

#### bool deleteCol(const QString&)

#### bool insertCol(int, const QStringList&, const QString& = "")

#### int getColIndex(const QString&)

#### StandardData* toStandardData()

转化成标准数据格式

### StandardData

​	使用double二维数组存储数据，所以插入和删除行列的速度会很慢，就干脆不支持行列插入删除了，可支持csv、arff文件的读入保存（可选择性开发，因为已经有通用数据能读写了）。支持缺失值和离散值，离散值都用数字索引表示，索引对应的离散值名都存储在nomName中。需开发处理缺失值离散值的函数以及转化函数。

#### bool \*\*missing

#### DataType \*type

#### QStringList rowName

#### QStringList colName

#### QStringList \*nomName


### StrictData

​	使用double二维数组存储数据，不支持行列插入删除、缺失值、离散值、arff以及csv文件读入保存，需要开发数据归一化函数，详情请见Min-max normalization，z-score方法。需开发转换函数。



