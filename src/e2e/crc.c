/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdbool.h>
#include <stdint.h>

#include "crc.h"

uint8_t Crc_CalculateCRC8(
    const uint8_t *Crc_DataPtr,
    uint32_t Crc_Length,
    uint8_t Crc_StartValue8,
    bool Crc_IsFirstCall)
{
    uint8_t crc;

    if (Crc_IsFirstCall)
    {
        crc = CRC8_INITIAL_VALUE;
    }
    else
    {
        crc = (CRC8_XOR_VALUE ^ Crc_StartValue8);
    }

    for (size_t i = 0; i < Crc_Length; ++i)
    {
        crc = CRC8_TABLE[crc ^ Crc_DataPtr[i]];
    }

    return (crc ^ CRC8_XOR_VALUE);
}

PyDoc_STRVAR(py_calculate_crc8_doc,
             "calculate_crc8(data: bytes, start_value: int = 0xFF, first_call: bool = True) -> int\n"
             "8-bit SAE J1850 CRC Calculation\n"
             "\n"
             ":param bytes data:\n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    which contains the data for CRC calculation\n"
             ":param int start_value:\n"
             "    First CRC of the algorithm (ignored when *first_call* is `True`).\n"
             "    In a sequence, this is expected to be the return value of the previous function call.\n"
             ":param bool first_call:\n"
             "    `True` if this is the first call of a sequence or an individual function call.\n"
             "    `False` if this is a subsequent call in a sequence.\n"
             ":return:\n"
             "    CRC value");
static PyObject *
py_calculate_crc8(PyObject *module,
                  PyObject *args,
                  PyObject *kwargs)
{
    Py_buffer data;
    unsigned char start_value = CRC8_INITIAL_VALUE;
    int first_call = true;
    static char *kwlist[] = {
        "data",
        "start_value",
        "first_call",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*|Bp:calculate_crc8",
                                     kwlist, &data, &start_value, &first_call))
    {
        return NULL;
    }

    uint8_t crc = Crc_CalculateCRC8((uint8_t*)data.buf, 
                                    (uint32_t)data.len, 
                                    (uint8_t)start_value, 
                                    (bool)first_call);
    PyBuffer_Release(&data);

    return (PyLong_FromUnsignedLong(crc));
}


static struct PyMethodDef methods[] = {
    {"calculate_crc8",
     (PyCFunction)py_calculate_crc8,
     METH_VARARGS | METH_KEYWORDS,
     py_calculate_crc8_doc},
    {NULL} // sentinel
};

static PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "e2e.crc",
    .m_doc = "",
    .m_size = -1,
    .m_methods = methods};

static int
_AddUnsignedIntConstant(PyObject *module, const char* name, uint64_t value)
{
    PyObject *obj = PyLong_FromUnsignedLongLong(value);
    if (PyModule_AddObject(module, name, obj) < 0) {
        Py_XDECREF(obj);
        return -1;
    }
    return 0;
}

#define _AddUnsignedIntMacro(m,c) _AddUnsignedIntConstant(m, #c, c)

PyMODINIT_FUNC PyInit_crc(void)
{

    PyObject *module_p;
    module_p = PyModule_Create(&module);

    if (module_p == NULL)
        return (NULL);
    
    _AddUnsignedIntMacro(module_p, CRC8_INITIAL_VALUE);
    _AddUnsignedIntMacro(module_p, CRC8_XOR_VALUE);
    _AddUnsignedIntMacro(module_p, CRC8_CHECK);
    _AddUnsignedIntMacro(module_p, CRC8_MAGIC_CHECK);

    return (module_p);
}