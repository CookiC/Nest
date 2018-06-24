# Nest文档

[TOC]

## Purpose

1. 整合重写补全机器学习库
2. 通过实现机器学习算法加深对机器学习的认识

## Updates

### Ash v1.13

1. 添加了StandardData::saveCsv()函数。
2. 更新了一系列StandardData类的内联函数以供外部类访问成员变量。
3. 为了减少GenericData的内存占用，新添DynamicTable\<T>类。
### Ash v1.14

1. 实现了NTable模板类

## Attention

1. 在C++中内联函数和模板类的定义都必须放在头文件中，否则编译失败，声明请与定义分开。

## Base Classes

### NTable\<T>

​	二维表，支持时间复杂度*O(N)*的行列删除和*O(N+M)*的行列插入，空间复杂度为*O(N\*M)*，可以说是逼近下限了。采用了内存回收机制，删除的列或行并不会直接将内存返还给系统，而是在析构时才将内存返还，以达到在删除后再次插入无需再向系统申请内存。

#### public

* **void deleteCol(int i)**

* **void deleteRow(int i)**

* **bool insertCol(int i, const QVector\<T>& col)**

* **bool insertRow(int i, const QVector\<T>& row)**

* **bool appendCol(const QVector\<T>& col)**

  在尾部添加列。

* **bool appendRow(const QVector\<T>& row)**

  在尾部添加行。

* **T& at(int i,int j);**

  返回对应索引的引用。

* **const T& get(int i,int j) const;**

  返回对应索引的常量值，建议日常使用该函数，与at区分开来。

* **int getColNum();**

* **int getRowNum();**

* **void set(int i,int j,const T& value);**

  设置对应索引值。

* **int getColNum()**

* **int getRowNum()**

### NString

在遥远的计划中的类，用以替代内存消耗巨大的QString类以及使用不怎么方便的std:tring类。

## Data Classes

数据存储类型

### GenericData

使用十字循环链表作为基础数据存储结构，插入行列的复杂度为O(N+M)，N为行数，M为列数，每个节点存储的是字符串，将支持csv、arff的读入与保存。需开发arff文件读入。

#### private

* **bool colNameFlag**

  是否有列名的标志

* **bool rowNameFlag**

  是否有行名的标志

* **Node \*head**

  *rowHead*与*colHead*的头指针,可存储行名的名字。

* **QList<Node\*> colHead**

  列头数组，指向列头节点，colHead[0]->str为列名，colHead[0]->down->str为第0行第0个字符串元素。
  
* **QList<Node\*> rowHead**

  行头数组，指向行头节点，rowHead[0]->str为行名，rowHead[0]->right->str为第0行第0个字符串元素。

* **void appendColHead(const QString &colName = "")**

  向列头数组尾部添加列头（列名）

* **void appendRowHead(const QString &colName = "")**

  向行头数组尾部添加行头（行名）

* **void deleteColHead(int index)**

  删除列头，并释放内存

* **void deleteRowHead(int index)**

  删除行头，并释放内存

* **void InsertColHead(int index, const QString &name = "")**

  在*index*位置插入列头

#### public

* **bool appendRow(const QStringList &row, const QString &name = "")**

  向尾部添加行（行数据，行名），返回是否添加成功。

* **void colStrSplit(int index, const QRegularExpression &regExp)**

  列字符串分割，根据正则表达式*regExp*匹配分隔符，分隔符将被抛弃，对*index*列的字符串进行分割，假如分割成*n*列，则按原字符串顺序，自*index*位置向后插入列。

* **void colStrSplit(int index, const QString& delimiter, bool repeat)**

  列字符串分割，根据分隔符*delimiter*进行分割，分隔符将被抛弃，*repeat*表示是否分割多次。

* **void colStrSplit(const QString& name, const QRegularExpression &regExp)**

  列字符串分割，根据列名查询所需分割列索引。

* **void colStrSplit(const QString& name, const QString& delimiter, bool repeat**

  列字符串分割，根据列名查询所需分割列索引。

* **void deleteRow(int index)**

* **bool deleteRow(const QString& name)**

* **void deleteCol(int index)**

* **bool deleteCol(const QString& name)**

* **int getColIndex(const QString& name)**

  根据列名查找索引，*O(numCol)* 复杂度。

* **bool insertCol(int index, const QStringList &col, const QString &name)** 

  插入列（列索引，列数据，列名）。

* **bool loadCsv(QString filePath, bool hasColName， bool hasRowName)**

  载入csv文件（文件名及路径，是否有列名）

* **void saveCsv(QString)**

  保存csv文件（文件名及路径）

* **StandardData* toStandardData()**

  将字符串转化为数字，能成功转化的判定为连续值，失败的判定为离散值，添加索引并记录；统计是否有缺失值，字符串为空即判定为缺失。


### StandardData

​	使用double二维数组存储数据，所以插入和删除行列的速度会很慢，就干脆不支持行列插入删除了，可支持csv、arff文件的读入保存（可选择性开发，因为已经有通用数据能读写了）。支持缺失值和离散值，离散值都用数字索引表示，索引对应的离散值名都存储在nomName中。需开发处理缺失值离散值的函数以及转化函数。需要开发数据归一化函数，详情请见Min-max normalization，z-score方法。
#### private

* **bool \*\*missing**

  缺失判定数组

* **bool isMissing**

  表示是否包含缺失值

* **DataType \*type**

  列类型判定数组

* **QStringList colName**

  列名

* **QStringList colName**

  行名

* **QStringList \*nomName**

  离散值索引数组

#### public

* **StandardData(int numRow, int numCol, bool isMissing)**

  构造函数，可指定是否包含缺失值，若包含，将申请二维数组*missing*记录每个位置的元素是否缺失，否则，*missing*数组为空指针。

* **void saveCsv(const QString& path)**

  保存离散值索引化后的数据。

* **inline...**

  *inline*函数为内联函数，管理成员变量并提升速度，需定义在头文件中。

## Classifier Classes

分类器

###  AbstractClassifier

#### public

* **virtual fit() = 0**

  拟合数据。

* **virtual predictHard() = 0**
  输出预测类，为所属标签或相应01向量。

* **virtual predictSoft() = 0**

  输出预测各类概率。

* **virtual predict() = 0**

  输出预测类与各类概率。

