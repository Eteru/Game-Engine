
#include "ScriptingModule.h"
#include <iostream>

ScriptingModule::ScriptingModule(const std::string & filename)
{
	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\"C:\\Python27\")");

	m_name = PyString_FromString(filename.c_str());
	m_module = PyImport_Import(m_name);
	if (nullptr == m_module) {
		PyErr_Print();
		return;
	}

	m_dictionary = PyModule_GetDict(m_module);
}

ScriptingModule::~ScriptingModule()
{
	Py_DECREF(m_dictionary);
	Py_DECREF(m_module);
	Py_DECREF(m_name);

	Py_Finalize();
}

std::string ScriptingModule::GetWelcomeMessage()
{
	std::string ret = "";
	PyObject *func = PyDict_GetItemString(m_dictionary, "welcome_message");

	if (PyCallable_Check(func)) {
		PyObject *args = PyTuple_New(1);
		PyObject *arg_name = PyString_FromString("<name>");
		PyTuple_SetItem(args, 0, arg_name);

		PyObject *value = PyObject_CallObject(func, args);
		ret = PyString_AsString(value);

		if (nullptr != args) {
			Py_DECREF(args);
		}
		if (nullptr != value) {
			Py_DECREF(value);
		}
	}
	else {
		PyErr_Print();
	}

	return ret;
}

std::string ScriptingModule::GetCollisionMessage(float x, float y, float z)
{
	std::string ret = "";
	PyObject *func = PyDict_GetItemString(m_dictionary, "collision_message");

	if (PyCallable_Check(func)) {
		PyObject *args = PyTuple_New(3);
		PyObject *arg1 = PyFloat_FromDouble(x);
		PyObject *arg2 = PyFloat_FromDouble(y);
		PyObject *arg3 = PyFloat_FromDouble(z);
		PyTuple_SetItem(args, 0, arg1);
		PyTuple_SetItem(args, 1, arg2);
		PyTuple_SetItem(args, 2, arg3);

		PyObject *value = PyObject_CallObject(func, args);
		ret = PyString_AsString(value);
		std::cout << ret << std::endl;

		if (nullptr != args) {
			Py_DECREF(args);
		}
		if (nullptr != value) {
			Py_DECREF(value);
		}
	}
	else {
		PyErr_Print();
	}

	return ret;
}
