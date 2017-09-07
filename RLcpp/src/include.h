#include <Python.h>
#include "nArmedBandit\eGreedy.h"
#include "nArmedBandit\softmax.h"

static PyObject*
RLcpp_e_greedy(PyObject *self, PyObject *args)
{
	int n_runs, n_machines, n_steps;
	double exploration;
	if (PyArg_ParseTuple(args, "iiid", &n_runs, &n_machines, &n_steps, &exploration)) {
		auto results = eGreedy::conductExperiments(n_runs, n_machines, n_steps, exploration);
		
		PyObject* rewards = PyList_New(n_steps);
		for (size_t i = 0; i < n_steps; i++)
			PyList_SetItem(rewards, i, PyFloat_FromDouble(results.first[i]));

		PyObject* optimals = PyList_New(n_steps);
		for (size_t i = 0; i < n_steps; i++)
			PyList_SetItem(optimals, i, PyFloat_FromDouble(results.second[i]));
		
		PyObject* answers = PyTuple_New(2);
		PyTuple_SET_ITEM(answers, 0, rewards);
		PyTuple_SET_ITEM(answers, 1, optimals);
		return answers;
	}
	return 0;
}

static PyObject*
RLcpp_softmax(PyObject *self, PyObject *args)
{
	int n_runs, n_machines, n_steps;
	double temperature;
	if (PyArg_ParseTuple(args, "iiid", &n_runs, &n_machines, &n_steps, &temperature)) {
		auto results = softmax::conductExperiments(n_runs, n_machines, n_steps, temperature);

		PyObject* rewards = PyList_New(n_steps);
		for (size_t i = 0; i < n_steps; i++)
			PyList_SetItem(rewards, i, PyFloat_FromDouble(results.first[i]));

		PyObject* optimals = PyList_New(n_steps);
		for (size_t i = 0; i < n_steps; i++)
			PyList_SetItem(optimals, i, PyFloat_FromDouble(results.second[i]));

		PyObject* answers = PyTuple_New(2);
		PyTuple_SET_ITEM(answers, 0, rewards);
		PyTuple_SET_ITEM(answers, 1, optimals);
		return answers;
	}
	return 0;
}

static PyMethodDef RLcppMethods[] = {
	{ "e_greedy", RLcpp_e_greedy, METH_VARARGS, "e-Greedy n-Armed-Bandit" },
	{ "softmax", RLcpp_softmax, METH_VARARGS, "softmax n-Armed-Bandit" },
	{ NULL,NULL,0,NULL }
};

static struct PyModuleDef RLcppModule = {
	PyModuleDef_HEAD_INIT,
	"RLcpp",   /* name of module */
	NULL, /* module documentation, may be NULL */
	-1,       /* size of per-interpreter state of the module,
			  or -1 if the module keeps state in global variables. */
	RLcppMethods
};