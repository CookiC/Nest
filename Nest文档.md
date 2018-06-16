# Nest文档

[TOC]

## Updates

### Ash v1.12

1. 取消了StrictData类
2. 取消了GenericData类与StandardData类中的行名
3. 重写了Standard类的构造和析构函数
4. 完善了Nest文档
5. 测试了GenericData::load_csv与GenericData::save_csv函数，应该没有问题了

## Data Classes

### GenericData

使用十字循环链表作为基础数据存储结构，插入行列的复杂度为O(N+M)，N为行数，M为列数，每个节点存储的是字符串，将支持csv、arff的读入与保存。需开发arff文件读入。

#### public

* **bool appendRow(const QStringList &)**

  向尾部添加行（行数据），返回是否添加成功。

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

* **bool loadCsv(QString filePath, bool hasColName)**

  载入csv文件（文件名及路径，是否有列名）

* **void saveCsv(QString)**

  保存csv文件（文件名及路径）

#### private

* **QList<Node\*> rowHead**

  列头数组，指向列名节点，例，rowHead[0]->str为列名，rowHead[0]->down->str为第0行第0个字符串元素。

* **QList<Node\*> colHead**

  行头指针，指向每行第0个元素，例colHead[0]->str为第0行第0个字符串元素。

* **void appendColHead(const QString &colName = "")**

  向列头数组尾部添加列头（列名）

* **void appendRowHead(const QString &str = "")**

  向行头数组尾部添加行头（第0列字符串元素）

* **void deleteColHead(int index)**

  删除列头，并释放内存

* **void deleteRowHead(int index)**

  删除行头，并释放内存

* **void InsertColHead(int index, const QString &name = "")**

  在*index*位置插入列头

### StandardData

​	使用double二维数组存储数据，所以插入和删除行列的速度会很慢，就干脆不支持行列插入删除了，可支持csv、arff文件的读入保存（可选择性开发，因为已经有通用数据能读写了）。支持缺失值和离散值，离散值都用数字索引表示，索引对应的离散值名都存储在nomName中。需开发处理缺失值离散值的函数以及转化函数。需要开发数据归一化函数，详情请见Min-max normalization，z-score方法。
#### private
* **bool \*\*missing**

* **DataType \*type**

* **QStringList colName**

* **QStringList \*nomName**



