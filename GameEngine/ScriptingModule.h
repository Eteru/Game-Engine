#pragma once

#ifdef _DEBUG
	#undef _DEBUG
		#include <Python.h>
	#define _DEBUG
	#else
		#include <Python.h>
#endif

#include <string>

class ScriptingModule
{
public:
	ScriptingModule(const std::string & filename);
	virtual ~ScriptingModule();

	std::string GetWelcomeMessage();
	std::string GetCollisionMessage(float x, float y, float z);

private:
	PyObject *m_name;
	PyObject *m_module;
	PyObject *m_dictionary;
};

