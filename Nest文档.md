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
2. 更改了编译选项，可以使用debug模式了，不过不会链接到python了，如果需要链接，要更改编译选项。

## Attention

1. C++中内联函数和模板类的定义都必须放在头文件中，否则编译失败，声明请与定义分开。
2. 函数中在堆区申请的变量作为返回值请返回指针或值，不要返回引用。
3. 函数中不要返回局部变量的指针或引用，只能返回值。

## Base Classes

### NTable\<T>

​	二维表，支持时间复杂度*O(N)*的行删除、*O(N\*M)*的列删除、*O(N)*的行插入，空间复杂度为*O(N\*M)*，可以说是逼近下限了。

#### public

* **void deleteCol(int i)**

* **void deleteRow(int i)**

* **bool insertCol(int i, const QVector\<T>& col)**

* **bool insertRow(int i, const QVector\<T>& row)**

* **bool appendCol(const QVector\<T>& col)**

  在尾部添加列。

* **bool appendRow(const QVector\<T>& row)**

  在尾部添加行。

* **T& at(int i,int j)**

  返回对应索引的引用。

* **const T& get(int i,int j) const**

  返回对应索引的常量值，建议日常使用该函数，与at区分开来。

* **int getColNum()**
* **const T* getCRow(int i)**

* **int getRowNum()**

* **void set(int i,int j,const T& value)**

  设置对应索引值。

* **int getColNum()**

* **int getRowNum()**

### NString

在遥远的计划中的类，用以替代内存消耗巨大的QString类以及使用不怎么方便的std:tring类。

## Data Classes

### AbstractData\<T> : NTable\<T>

数据类的虚基类，主要负责处理列名，头名。

#### public
* **AbstractData()**

* **bool appendCol(const QVector\<T>& col, const QString& name)**

* **bool appendRow(const QVector\<T>& col, const QString& name)**

* **void deleteCol(int index)**

* **void deleteRow(int index)**

* **int getColIndex(const QString& name)**

  返回与*name*相等列名的最小的索引，如不存在返回-1。

* **bool insertCol(int index, const QVector\<T>& col, const QString& name)**

* **bool insertRow(int index, const QVector\<T>& row, const QString& name)**

* **bool loadCsv(const QString &path, bool hasColName, bool hasRowName)**
  载入csv文件（文件名及路径，是否有列名，是否有行名）

* **void saveCsv(const QString &)**

  保存csv文件（文件名及路径）

* **void setY(int i)**

  设置目标属性。

* **int getY()**

  返回目标属性。

#### protected
* **bool colNameFlag**

* **bool rowNameFlag**

* **void deleteColName(int index)**

* **void deleteRowName(int index)**

* **void insertColName(int index, const QString& name)**

* **void insertRowName(int index, const QString& name)**

* **QStringVector splitCsvLine(const QByteArray &line, QString& name)**

  分割csv文件的一行，返回值已移除‘，’分隔符。

* **void appendColName(const QString& name)**

* **void appendRowName(const QString& name)**

* **const QString& getColName(int i)**

* **const QString& getRowName(int i)**

* **virtual bool loadRow(int i, const QStringVector &row) = 0**

* **virtual void saveRow(int i, QStringVector& row) = 0**
### GenericData : AbstractData\<QString>

将支持csv、arff的读入与保存。需开发arff文件读入。

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

* **int getColIndex(const QString& name)**

  根据列名查找索引，*O(numCol)* 复杂度。

* **bool insertCol(int index, const QStringList &col, const QString &name)** 

  插入列（列索引，列数据，列名）。

* **StandardData* toStandardData()**

  将字符串转化为数字，能成功转化的判定为连续值，失败的判定为离散值，添加索引并记录；统计是否有缺失值，字符串为空即判定为缺失。


### StandardData : AbstractData\<double>

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

* **virtual void fit(StandardData *data) = 0**

  拟合数据。

* **virtual NTable\<int> predictHard(StandardData *data) = 0**
  输出预测类，为所属标签或相应01向量。

* **virtual NTable\<double> predictSoft(StandardData *data) = 0**

  输出预测各类概率。

* **virtual void predict(StandardData *data, NTable\<int> *hard, NTable\<double> *soft) = 0**

  输出预测类与各类概率。

### CARTClassifier

*CART*决策树分类器

*uMap*存储当前节点各类别的数量，第一个*return*处理样本属于同一类的情况，第二个*return*处理剩余属性集为空或样本取值相同的情况