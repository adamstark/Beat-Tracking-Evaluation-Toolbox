#include <iostream>
#include <Python.h>
#include <numpy/arrayobject.h>
#include "../src/BeatTrackingEvaluationToolbox.h"

//=======================================================================
static PyObject * evaluate(PyObject *dummy, PyObject *args)
{
    PyObject *arg1=NULL;
    PyObject *arr1=NULL;
    PyObject *arg2=NULL;
    PyObject *arr2=NULL;
    
    if (!PyArg_ParseTuple(args, "OO", &arg1,&arg2))
    {
        return NULL;
    }
    
    arr1 = PyArray_FROM_OTF(arg1, NPY_DOUBLE, NPY_IN_ARRAY);
    if (arr1 == NULL)
    {
        return NULL;
    }
    
    arr2 = PyArray_FROM_OTF(arg2, NPY_DOUBLE, NPY_IN_ARRAY);
    if (arr2 == NULL)
    {
        return NULL;
    }
    
    ////////// GET INPUT DATA ///////////////////
    
    // get data as array
    double* beatData = (double*) PyArray_DATA(arr1);
    double* annotationData = (double*) PyArray_DATA(arr2);
    
    // get array size
    long numBeats = PyArray_Size((PyObject*)arr1);
    long numAnnotations = PyArray_Size((PyObject*)arr2);
    
    std::vector<double> beats;
    std::vector<double> annotations;
    
    for (int i = 0; i < numBeats;i++)
    {
        beats.push_back (beatData[i]);
    }
    
    for (int i = 0; i < numAnnotations;i++)
    {
        annotations.push_back (annotationData[i]);
    }
    
    std::vector<double> result = BeatTrackingEvaluationToolbox::evaluate (beats,annotations);
    
//    double results[result.size()];
//    
//    for (int i = 0;i < result.size();i++)
//    {
//        results[i] = result[i];
//    }
    
    
    ////////// CREATE ARRAY AND RETURN IT ///////////////////
    int nd=1;
    npy_intp m= result.size();

    
    PyObject* c=PyArray_SimpleNew(nd, &m, NPY_DOUBLE);
    
    void *arr_data = PyArray_DATA((PyArrayObject*)c);
    
    memcpy(arr_data, &result[0], PyArray_ITEMSIZE((PyArrayObject*) c) * m);
    
    
    Py_DECREF(arr1);
    Py_INCREF(Py_None);
    
    return (PyObject *)c;
}

//=======================================================================
static PyMethodDef beat_tracking_evaluation_toolbox_methods[] = {
    { "evaluate", evaluate, METH_VARARGS,"Evaluate a given beat sequence and annotation sequence"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

//=======================================================================
static struct PyModuleDef beat_tracking_evaluation_toolbox_definition = {
    PyModuleDef_HEAD_INIT,
    "beat_tracking_evaluation_toolbox",
    "A toolbox of beat tracking evaluation methods",
    -1,
    beat_tracking_evaluation_toolbox_methods
};

//=======================================================================
PyMODINIT_FUNC PyInit_beat_tracking_evaluation_toolbox(void)
{
    import_array();
    return PyModule_Create(&beat_tracking_evaluation_toolbox_definition);
}

//=======================================================================
int main(int argc, char *argv[])
{
    wchar_t* program = Py_DecodeLocale (argv[0], NULL);
    Py_SetProgramName (program);
    
    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();
    
    /* Add a static module */
    PyInit_beat_tracking_evaluation_toolbox();
}
