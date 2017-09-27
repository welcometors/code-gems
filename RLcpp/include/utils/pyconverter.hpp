#ifndef   __PY_CONVERTER_H__
#define   __PY_CONVERTER_H__

#include <Python.h>

static PyObject* toPyObject(int x) {
	return PyLong_FromLong(x);
}

static PyObject* toPyObject(double x) {
	return PyFloat_FromDouble(x);
}

template<class T>
static PyObject* toPyObject(const std::vector<T>& ary) {
	PyObject* list = PyList_New(ary.size());
	for (size_t i = 0; i < ary.size(); i++)
		PyList_SetItem(list, i, toPyObject(ary[i]));
	return list;
}

template<class T1, class T2>
static PyObject* toPyTuple(const std::pair<T1,T2>& p) {
	PyObject* pyTuple = PyTuple_New(2);
	PyTuple_SET_ITEM(pyTuple, 0, toPyObject(p.first));
	PyTuple_SET_ITEM(pyTuple, 1, toPyObject(p.second));
	return pyTuple;
}

template<class Tuple, std::size_t... Is>
void setPyTuple(PyObject* obj, Tuple const& t, std::index_sequence<Is...>) {
	/*using swallow = int[];
	(void)swallow {
		0, (PyTuple_SET_ITEM(obj, Is, toPyObject(std::get<Is>(t))), 0)...
	};*/
	auto l = { (PyTuple_SET_ITEM(obj, Is, std::get<Is>(t)), 0)... };
}

template<class... Args>
static PyObject* toPyTuple(std::tuple<Args...> const& t) {
	PyObject* pyTuple = PyTuple_New(std::tuple_size<decltype(t)>::value);
	setPyTuple(pyTuple, t, std::make_index_sequence<sizeof...(Args)>());
	return pyTuple;
}

#endif