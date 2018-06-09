#ifndef CPYTHON_H
#define CPYTHON_H

#include <Python.h>
#include <arrayobject.h>
#include <cstring>
#include "header.h"
#include "cparray.h"

class CPython{
public:
    static void open();
    static void close();
    static bool call(const char*, const char*, const char*, ...);
    static void test();

private:
    static bool isRunning;
    static CPython* ptr;
    static PyObject* pLib;
    static PyObject* pDict;
    static void getArray(CPArray*, PyArrayObject*, NPY_TYPES);
    static bool npyType(char, NPY_TYPES&);
    static bool inConvert(PyObject*&, va_list&, const char*&);
    static bool outConvert(PyObject*, va_list&, const char*&);

    CPython();
    ~CPython();

    void* initNumpy();
};

#endif // CPYTHON_H
