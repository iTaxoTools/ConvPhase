#include "python_wrapper.h"
#include "convphase.h"

static PyTypeObject OutputLinesType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "convphase.OutputLines",        /* tp_name */
  sizeof(OutputLinesObject),      /* tp_basicsize */
  0,                              /* tp_itemsize */
  0,                              /* tp_dealloc */
  0,                              /* tp_vectorcall_offset */
  0,                              /* tp_getattr */
  0,                              /* tp_setattr */
  0,                              /* tp_as_async */
  0,                              /* tp_repr */
  0,                              /* tp_as_number */
  0,                              /* tp_as_sequence */
  0,                              /* tp_as_mapping */
  0,                              /* tp_hash */
  0,                              /* tp_call */
  0,                              /* tp_str */
  0,                              /* tp_getattro */
  0,                              /* tp_setattro */
  0,                              /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,             /* tp_flags */
  PyDoc_STR(										  /* tp_doc */
		"Iterator over tuples of (seqid, allelle_a, allele_b)"
	),
  0,                              /* tp_traverse */
  0,                              /* tp_clear */
  0,                              /* tp_richcompare */
  0,                              /* tp_weaklistoffset */
  OutputLines_iter,                  /* tp_iter */
  OutputLines_next,                  /* tp_iternext */
  0,                              /* tp_methods */
  0,                              /* tp_members */
  0,                              /* tp_getset */
  0,                              /* tp_base */
  0,                              /* tp_dict */
  0,                              /* tp_descr_get */
  0,                              /* tp_descr_set */
  0,                              /* tp_dictoffset */
  0,                              /* tp_init */
  0,                              /* tp_alloc */
  OutputLines_new,                   /* tp_new */
};

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

	PyObject *m;
	if (PyType_Ready(&OutputLinesType) < 0)
		return NULL;

	m = PyModule_Create(&convPhaseModule);
	if (m == NULL)
		return NULL;

	Py_INCREF(&OutputLinesType);
	if (PyModule_AddObject(m, "Iterator", (PyObject *) &OutputLinesType) < 0) {
		Py_DECREF(&OutputLinesType);
		Py_DECREF(m);
		return NULL;
	}

	return m;
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

static PyObject* py_iterator_test(PyObject* self, PyObject* args) {

  PyObject* argument;
  if (!PyArg_ParseTuple(args, "O", &argument)) {
		PyErr_SetString(PyExc_TypeError, "Argument must be a single iterator");
    return NULL;
  }

  PyObject* input_iterator = PyObject_GetIter(argument);

  if (input_iterator == NULL) {
    PyErr_SetString(PyExc_TypeError, "Argument must be iterable");
    return NULL;
  }

	std::vector<InputLine> input_vector;

  PyObject* item;
  while ((item = PyIter_Next(input_iterator)) != NULL) {

		const char *c_id, *c_data;

    if (!PyArg_ParseTuple(item, "ss", &c_id, &c_data)) {
        return NULL;
    }

		std::string str_id(c_id);
		std::string str_data(c_data);
		InputLine line = {str_id, str_data};
		input_vector.push_back(line);

    Py_DECREF(item);
  }

  Py_DECREF(input_iterator);

	std::vector<OutputLine> output_vector;

	// Replace this segment with actual calculations
	for (InputLine elem : input_vector) {
		std::string str_a = elem.data + "-";
		std::string str_b = elem.data + "+";
		OutputLine line = {elem.id, str_a, str_b};
		output_vector.push_back(line);
	}

	OutputLinesObject *result = (OutputLinesObject *)OutputLinesType.tp_new(&OutputLinesType, NULL, NULL);
	if (result == NULL) {
		return NULL;
	}

	result->lines = output_vector;
	return (PyObject*)result;
}

static PyObject* OutputLines_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  OutputLinesObject *self;
  self = (OutputLinesObject *) type->tp_alloc(type, 0);
  if (self != NULL) {
		self->current = 0;
  }
  return (PyObject *) self;
}

static PyObject* OutputLines_iter(PyObject* self) {
  Py_INCREF(self);
  return self;
}

static PyObject* OutputLines_next(PyObject* self) {
	OutputLinesObject *obj = (OutputLinesObject *) self;
	if ( obj->current < obj->lines.size() ) {
		OutputLine line = obj->lines[obj->current];
		obj->current ++;

		PyObject* id = PyUnicode_FromString(line.id.c_str());
		PyObject* data_a = PyUnicode_FromString(line.data_a.c_str());
		PyObject* data_b = PyUnicode_FromString(line.data_b.c_str());

		PyObject* tuple = PyTuple_New(3);
		PyTuple_SetItem(tuple, 0, id);
		PyTuple_SetItem(tuple, 1, data_a);
		PyTuple_SetItem(tuple, 2, data_b);
		return tuple;
	}
	PyErr_SetNone(PyExc_StopIteration);
  return NULL;
}
