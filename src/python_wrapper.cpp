#include "python_wrapper.h"
#include "convphase.h"

static PyMethodDef convPhaseFuncs[]{
	{"seqPhaseStep1", py_seqPhaseStep1, METH_VARARGS, "Convert from fasta format to phase format"},
	{"phase",         py_phase,         METH_VARARGS, "Execute phase algorithm"},
	{"seqPhaseStep2", py_seqPhaseStep2, METH_VARARGS, "Convert from phase format to fasta format"},
	{"convPhase",     py_convPhase,     METH_VARARGS, "Execute convphase algorithm"},
	{"_iterator_test",     py_iterator_test,     METH_VARARGS, "Read and return iterators"},
	{NULL,            NULL,             0,            NULL}
};
static struct PyModuleDef convPhaseModule{
	PyModuleDef_HEAD_INIT,
	"convphase",
	NULL,
	-1,
	convPhaseFuncs,
	NULL,
	NULL,
	NULL,
	NULL
};

PyMODINIT_FUNC PyInit_convphase(){
	initHxcpp();
	return PyModule_Create(&convPhaseModule);
}

PyObject* py_seqPhaseStep1(PyObject* self, PyObject* args){
	std::string str1;
	std::string str2 = "";
	std::string str3 = "";

	char* cstr1 = NULL;
	char* cstr2 = NULL;
	char* cstr3 = NULL;

	if(!PyArg_ParseTuple(args, "s|ss", &cstr1, &cstr2, &cstr3))
		return NULL;
	if(cstr1)
		str1 = cstr1;
	if(cstr2)
		str2 = cstr2;
	if(cstr3)
		str3 = cstr3;

	SeqPhaseStep1Result output = seqPhaseStep1(str1, str2, str3);
	PyObject* tuple = PyTuple_New(3);
	PyTuple_SetItem(tuple, 0, PyUnicode_DecodeUTF8(output.inpData.c_str(), output.inpData.size(), NULL));
	PyTuple_SetItem(tuple, 1, PyUnicode_DecodeUTF8(output.knownData.c_str(), output.knownData.size(), NULL));
	PyTuple_SetItem(tuple, 2, PyUnicode_DecodeUTF8(output.constData.c_str(), output.constData.size(), NULL));

	return tuple;
}
PyObject* py_phase(PyObject* self, PyObject* args){
	std::string inpData;
	std::string knownData;
	std::vector<char const*> options;

	int argc = PyTuple_Size(args);
	if(!argc){
		//TODO throw exception
		return NULL;
	}
	char const* str;

	str = PyUnicode_AsUTF8(PyTuple_GetItem(args, 0));
	if(!str)
		return NULL;
	inpData = str;

	if(argc >= 2){
		str = PyUnicode_AsUTF8(PyTuple_GetItem(args, 1));
		if(!str)
			return NULL;
		knownData = str;
	}

	for(int i = 2; i < argc; ++i){
		str = PyUnicode_AsUTF8(PyTuple_GetItem(args, i));
		if(!str)
			return NULL;
		options.push_back(str);
	}

	PhaseOutput output = phase(PhaseInput{inpData, knownData}, options);

	return PyUnicode_DecodeUTF8(output.output.c_str(), output.output.size(), NULL);
}
PyObject* py_seqPhaseStep2(PyObject* self, PyObject* args){
	std::string phaseFile;
	std::string constFile;
	bool reduce = false;
	bool sort = false;

	char* phaseFileStr;
	char* constFileStr;
	if(!PyArg_ParseTuple(args, "ss|bb", &phaseFileStr, &constFileStr, &reduce, &sort))
		return NULL;
	phaseFile = phaseFileStr;
	constFile = constFileStr;

	std::string output = seqPhaseStep2(phaseFile, constFile, reduce, sort);

	return PyUnicode_DecodeUTF8(output.c_str(), output.size(), NULL);
}
PyObject* py_convPhase(PyObject* self, PyObject* args){
	std::string input;
	std::vector<char const*> options;
	bool reduce = false;
	bool sort = false;

	int argc = PyTuple_Size(args);
	if(!argc){
		//TODO throw exception
		return NULL;
	}
	char const* str = PyUnicode_AsUTF8(PyTuple_GetItem(args, 0));
	if(!str)
		return NULL;
	input = str;

	for(int i = 1; i < argc; ++i){
		str = PyUnicode_AsUTF8(PyTuple_GetItem(args, i));
		if(!str)
			return NULL;
		options.push_back(str);
	}

	std::string output = convPhase(input, options, reduce, sort);

	return PyUnicode_DecodeUTF8(output.c_str(), output.size(), NULL);
}

PyObject* py_iterator_test(PyObject* self, PyObject* args) {
  PyObject* iterable;

  if (!PyArg_ParseTuple(args, "O", &iterable)) {
    return NULL;
  }

  PyObject* iterator = PyObject_GetIter(iterable);

  if (iterator == NULL) {
    PyErr_SetString(PyExc_TypeError, "Not an iterable");
    return NULL;
  }

  PyObject* item;

  while ((item = PyIter_Next(iterator)) != NULL) {
		if (PyUnicode_Check(item)) {
			const char* str = PyUnicode_AsUTF8(item);
			printf("+%s+\n", str);
		} else {
			printf("Not a string\n");
		}
    Py_DECREF(item);
  }

  Py_DECREF(iterator);
  Py_RETURN_NONE;
}
