#include <Python.h>
#include "nArmedBandit\experiments.hpp"
#include "utils\pyconverter.hpp"

namespace nArmedBandit_py {

	static PyObject* eGreedy(PyObject *self, PyObject *args) 
	{
		int n_runs, n_machines, n_steps;
		double exploration;
		if (PyArg_ParseTuple(args, "iiid", &n_runs, &n_machines, &n_steps, &exploration))
			return toPyTuple(nArmedBandit::experiments::eGreedy(n_runs, n_machines, n_steps, exploration));
		return 0;
	}

	static PyObject* softmax(PyObject *self, PyObject *args) 
	{
		int n_runs, n_machines, n_steps;
		double temperature;
		if (PyArg_ParseTuple(args, "iiid", &n_runs, &n_machines, &n_steps, &temperature))
			return toPyTuple(nArmedBandit::experiments::softmax(n_runs, n_machines, n_steps, temperature));
		return 0;
	}
}