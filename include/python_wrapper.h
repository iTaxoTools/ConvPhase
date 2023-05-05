#ifndef PYTHON_WRAPPER_H
#define PYTHON_WRAPPER_H

#include <Python.h>
#include <vector>
#include <string>

struct InputLine{
	std::string id;
	std::string data;
};

struct OutputLine{
	std::string id;
  std::string data_a;
	std::string data_b;
};

struct IteratorObject{
    PyObject_HEAD
    std::vector<OutputLine> lines;
    int current;
} ;

static PyObject* py_seqPhaseStep1(PyObject* self, PyObject* args);
static PyObject* py_phase(PyObject* self, PyObject* args);
static PyObject* py_seqPhaseStep2(PyObject* self, PyObject* args);
static PyObject* py_convPhase(PyObject* self, PyObject* args);
static PyObject* py_iterator_test(PyObject* self, PyObject* args);

static PyObject* Iterator_new(PyTypeObject* type, PyObject* args, PyObject* kwargs);
static PyObject* Iterator_iter(PyObject* self);
static PyObject* Iterator_next(PyObject* self);

#endif
