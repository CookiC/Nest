#include "cpython.h"

bool CPython::isRunning = 0;

CPython* CPython::ptr = nullptr;

PyObject* CPython::pLib = nullptr;

PyObject* CPython::pDict = nullptr;

CPython::CPython(){
    isRunning = true;
    Py_Initialize();
    if(!Py_IsInitialized())
        cout<<"Initialize Fail!"<<endl;
    initNumpy();
    PyRun_SimpleString("import os");
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("os.chdir('../py')");
    PyRun_SimpleString("sys.path.append('.')");
    pLib = PyImport_ImportModule("module");
    if(!pLib)
        cout<<"Import module wrong!"<<endl;
    pDict = PyModule_GetDict(pLib);
    if(!pDict)
        cout<<"Get dictionary wrong!"<<endl;
}

CPython::~CPython(){
    Py_DECREF(pDict);
    Py_DECREF(pLib);
    Py_Finalize();
    isRunning = false;
}

void CPython::open(){
    if(!isRunning)
        ptr = new CPython();
}

bool CPython::npyType(char c,NPY_TYPES& t){
    switch(c){
    case 'b':
        t = NPY_BOOL;
        break;
    case 'L':
        t = NPY_LONGLONG;
        break;
    case 'l':
        t = NPY_LONG;
        break;
    case 'i':
        t = NPY_INT;
        break;
    case 'd':
        t = NPY_DOUBLE;
        break;
    case 'f':
        t = NPY_FLOAT;
        break;
    default:
        return false;
    }
    return true;
}

bool CPython::inConvert(PyObject*& o, va_list& arg, const char*& p){
    switch(*p){
    case 'b':
        o = PyBool_FromLong((long)va_arg(arg,bool));
        break;
    case 'L':
        o = PyLong_FromLongLong(va_arg(arg,long long));
        break;
    case 'l':
        o = PyLong_FromLong(va_arg(arg,long));
        break;
    case 'i':
        o = PyLong_FromLong((long)va_arg(arg,int));
        break;
    case 'd':
        o = PyFloat_FromDouble(va_arg(arg,double));
        break;
    case 'f':
        o = PyFloat_FromDouble((double)va_arg(arg,float));
        break;
    case '[':{
        //cout<<(LL)arg<<' '<<sizeof(int)+sizeof(LL*)*2<<' '<<(unsigned)*arg<<endl;
        const CPArray &a = va_arg(arg,CPArray);
        //cout<<(LL)&a<<' '<<(LL)arg<<endl;
        NPY_TYPES type;
        ++p;
        if(!npyType(*p,type))
            return false;
        o = PyArray_SimpleNewFromData(a.n, a.dim, type, a.data);
        ++p;
        break;
    }
    default:
        return false;
    }
    return true;
}

bool CPython::outConvert(PyObject* o, va_list& arg, const char*& p){
    switch(*p){
    case 'b':
        *va_arg(arg,bool*) = PyLong_AsLong(o);
        break;
    case 'L':
        *va_arg(arg,long long*) = PyLong_AsLongLong(o);
        break;
    case 'l':
        *va_arg(arg,long*) = PyLong_AsLong(o);
        break;
    case 'i':
        *va_arg(arg,int*) = PyLong_AsLong(o);
        break;
    case 'd':
        *va_arg(arg,double*) = PyFloat_AsDouble(o);
        break;
    case 'f':
        *va_arg(arg,float*) = (float)PyFloat_AsDouble(o);
        break;
    case '[':{
        CPArray* a = va_arg(arg,CPArray*);
        NPY_TYPES type;
        ++p;
        if(!npyType(*p,type))
            return false;
        PyArrayObject* pArray = (PyArrayObject*)o;
        getArray(a,pArray,type);
        Py_DECREF(pArray);
        ++p;
        break;
    }
    default:
        cout<<"Output format wrong! Has invalid character '"<<*p<<"'"<<endl;
        return false;
    }
    return true;
}

void CPython::getArray(CPArray* a, PyArrayObject* p, NPY_TYPES t){
    a->n = p->nd;
    a->dim = new ll[a->n];
    memcpy(a->dim, p->dimensions, sizeof(void*)*a->n);
    int i;
    long long sum=1;
    for(i=0;i<a->n;++i)
        sum*=a->dim[i];
    cout<<"numpy type "<<p->descr->type_num<<"\n";
    switch(t){
    case NPY_BOOL:
        a->data = new bool[sum];
        memcpy(a->data, p->data, sizeof(bool)*sum);
        break;
    case NPY_LONGLONG:
        a->data = new long long[sum];
        memcpy(a->data, p->data, sizeof(long long)*sum);
        break;
    case NPY_LONG:
        a->data = new long[sum];
        memcpy(a->data, p->data, sizeof(long)*sum);
        break;
    case NPY_INT:
        a->data = new int[sum];
        memcpy(a->data, p->data, sizeof(int)*sum);
        break;
    case NPY_DOUBLE:
        a->data = new double[sum];
        memcpy(a->data, p->data, sizeof(double)*sum);
        break;
    case NPY_FLOAT:
        a->data = new float[sum];
        memcpy(a->data, p->data, sizeof(float)*sum);
        break;
    default:
        return;
    }
}

bool CPython::call(const char* func,const char* para, const char* retu, ...){
 /* warning:当参数多于格式化字符串所表示时，函数将会发生未知错误
  * 参数需传入值，返回需传入地址
  * 数组请使用CPArray规整CPArray(维数，长度，数组指针)
  * funcName:在python文件中的函数名
  * format:格式化参数，参照:
  *     b       -bool
  *     L       -long long
  *     l       -long
  *     i       -int
  *     d       -double
  *     f       -float
  *     [i]      -int[]
  * python返回值一律使用tuple类型
  */
    const char* pStr;
    va_list arg;
    PyObject* pPara = nullptr;
    va_start(arg,retu);
    if(para[0]){
        int n=0,i=0;
        PyObject* pTmp = nullptr;
        for(pStr=para;*pStr;++pStr)
            if(*pStr!='['&&*pStr!=']')
                ++n;
        pPara = PyTuple_New(n);
        for(pStr=para;*pStr;++pStr){
            if(!inConvert(pTmp, arg, pStr)||!pTmp)
                cout<<"InConvert wrong!"<<endl;
            PyTuple_SetItem(pPara, i, pTmp);
            ++i;
        }
    }

    PyObject *pFunc = PyDict_GetItemString(pDict, func);
    if(!pFunc)  return false;
    PyObject *pRetu = PyObject_CallObject(pFunc, pPara);
    if(pPara)   Py_DECREF(pPara);
    if(pFunc)   Py_DECREF(pFunc);

    if(!pRetu)  cout<<"Call object wrong!"<<endl;
    if(pRetu==Py_None&&retu[0]||pRetu!=Py_None&&!retu[0]){
        cout<<"The reverted parameter is inconsistent!";
        return false;
    }
    if(pRetu!=Py_None){
        if(retu[0])
            if(PyTuple_Check(pRetu)){
                //返回多个值，使用tuple存储
                int i=0;
                for(pStr=retu;*pStr;++pStr){
                    if(!outConvert(PyTuple_GetItem(pRetu,i),arg,pStr))
                        return false;
                    ++i;
                }
            }
            else{
                //返回单个值
                pStr=retu;
                if(!outConvert(pRetu,arg,pStr))
                    return false;
            }
        cout<<"Release pRetu!"<<endl;
    }
    va_end(arg);
    Py_DECREF(pRetu);
    return true;
}

void CPython::close(){
    if(isRunning)
        delete ptr;
}

void* CPython::initNumpy(){
    import_array();
    return 0;
}

void CPython::test(){
    open();
    //call("HelloWorld","","");
    int n=2;
    ll dim[2]={3,3};
    int data[3][3]={{1,1,2},{1,1,1},{1,1,1}};
    CPArray out;
    call("Sum1","[i]i","[i]",CPArray(n,dim,data),n,&out);
    int sum=1;
    int i;
    cout<<out.n<<endl;
    for(i=0;i<out.n;++i)
        sum*=out.dim[i];
    cout<<sum<<endl;
    for(i=0;i<sum;++i)
        cout<<((int*)out.data)[i]<<' ';
    close();
}
