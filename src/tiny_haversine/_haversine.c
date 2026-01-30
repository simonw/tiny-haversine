#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>

// Minimal haversine distance in kilometers.
// Arguments: lat1, lon1, lat2, lon2 (degrees)
static double haversine_km(double lat1, double lon1, double lat2, double lon2) {
    const double earth_radius_km = 6371.0;
    const double deg_to_rad = M_PI / 180.0;

    const double lat1_rad = lat1 * deg_to_rad;
    const double lon1_rad = lon1 * deg_to_rad;
    const double lat2_rad = lat2 * deg_to_rad;
    const double lon2_rad = lon2 * deg_to_rad;

    const double dlat = lat2_rad - lat1_rad;
    const double dlon = lon2_rad - lon1_rad;

    const double sin_dlat = sin(dlat * 0.5);
    const double sin_dlon = sin(dlon * 0.5);

    const double h = sin_dlat * sin_dlat +
                     cos(lat1_rad) * cos(lat2_rad) * sin_dlon * sin_dlon;

    const double central_angle = 2.0 * asin(sqrt(h));
    return earth_radius_km * central_angle;
}

static PyObject *py_haversine(PyObject *self, PyObject *args) {
    double lat1;
    double lon1;
    double lat2;
    double lon2;

    if (!PyArg_ParseTuple(args, "dddd", &lat1, &lon1, &lat2, &lon2)) {
        return NULL;
    }

    const double result = haversine_km(lat1, lon1, lat2, lon2);
    return PyFloat_FromDouble(result);
}

static PyMethodDef module_methods[] = {
    {
        "haversine",
        py_haversine,
        METH_VARARGS,
        PyDoc_STR("haversine(lat1, lon1, lat2, lon2) -> kilometers"),
    },
    {NULL, NULL, 0, NULL},
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "_haversine",
    "Tiny haversine C extension.",
    -1,
    module_methods,
};

PyMODINIT_FUNC PyInit__haversine(void) {
    return PyModule_Create(&moduledef);
}
