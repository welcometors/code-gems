#include <Python.h>
#include "bellman\jacksCarRental.hpp"
#include "utils\pyconverter.hpp"

namespace bellman_py {

    namespace jacksCarRental {
        using namespace bellman::jacksCarRental;

        static int expIDcounter = 1;
        static unordered_map<int, TwoStationCompany> experiments;
        // Exposing a 'TwoStationCompany' object as an integer ID
        // which exposes the methods as functions to python
        // ToDo: Find other ways to expose class

        static PyObject* addExperiment(PyObject *self, PyObject *args)
        {
            int maxCars, maxTransfer, avgRequestA, avgReturnA, avgRequestB, avgReturnB, rent;
            double transferCost, gamma;
            if (PyArg_ParseTuple(args, "ii(ii)(ii)idd", &maxCars, &maxTransfer,
                &avgRequestA, &avgReturnA, &avgRequestB, &avgReturnB,
                &rent, &transferCost, &gamma)) {

                PyObject* currentId = toPyObject(expIDcounter);
                experiments[expIDcounter] = TwoStationCompany(
                    maxCars, maxTransfer, rent, transferCost,
                    avgRequestA, avgReturnA, avgRequestB, avgReturnB, gamma);
                ++expIDcounter;
                return currentId;
            }
            return 0;
        }

        static PyObject* evaluatePolicy(PyObject *self, PyObject *args)
        {
            int experimentID;
            if (PyArg_ParseTuple(args, "i", &experimentID)) {
                int iterations = experiments[experimentID].evaluatePolicy();
                return toPyTuple(std::make_pair(experiments[experimentID].getValueMap(), iterations));
            }
            return 0;
        }

        static PyObject* improvePolicy(PyObject *self, PyObject *args)
        {
            int experimentID;
            if (PyArg_ParseTuple(args, "i", &experimentID)) {
                int changes = experiments[experimentID].policyImprovement();
                return toPyTuple(std::make_pair(experiments[experimentID].getPolicy(), changes));
            }
            return 0;
        }

        static PyObject* endExperiment(PyObject *self, PyObject *args)
        {
            int experimentID;
            if (PyArg_ParseTuple(args, "i", &experimentID))
                return toPyObject((int)experiments.erase(experimentID));
            return 0;
        }
    }
}