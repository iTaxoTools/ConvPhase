#ifndef PYTHON_WRAPPER_H
#define PYTHON_WRAPPER_H

#include <Python.h>

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
} IteratorObject;

static PyObject* py_seqPhaseStep1(PyObject* self, PyObject* args);
static PyObject* py_phase(PyObject* self, PyObject* args);
static PyObject* py_seqPhaseStep2(PyObject* self, PyObject* args);
static PyObject* py_convPhase(PyObject* self, PyObject* args);
static PyObject* py_iterator_test(PyObject* self, PyObject* args);

static PyObject* Iterator_iter(PyObject* self);
static PyObject* Iterator_next(PyObject* self);

#endif
