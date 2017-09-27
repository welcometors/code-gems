#include "rlcpp.hpp"

PyMODINIT_FUNC
PyInit_RLcpp(void)
{
	PyObject *m;

	m = PyModule_Create(&RLcppModule);
	if (m == NULL)
		return NULL;
	return m;
}