#include <Python.h>
#include "nArmedBandit.hpp"
#include "bellman.hpp"

static PyMethodDef RLcppMethods[] = {

	{ "nArmedBandit_eGreedy", nArmedBandit_py::eGreedy, METH_VARARGS, "n-Armed-Bandit e-Greedy" },
	{ "nArmedBandit_softmax", nArmedBandit_py::softmax, METH_VARARGS, "n-Armed-Bandit softmax" },

	{ "bellman_jcr_addExperiment", bellman_py::jacksCarRental::addExperiment, METH_VARARGS, "Jacks car rental" },
	{ "bellman_jcr_evaluatePolicy", bellman_py::jacksCarRental::evaluatePolicy, METH_VARARGS, "Jacks car rental" },
	{ "bellman_jcr_improvePolicy", bellman_py::jacksCarRental::improvePolicy, METH_VARARGS, "Jacks car rental" },
	{ "bellman_jcr_endExperiment", bellman_py::jacksCarRental::endExperiment, METH_VARARGS, "Jacks car rental" },

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