/*
 * libvir.c: this modules implements the main part of the glue of the
 *           libvir library and the Python interpreter. It provides the
 *           entry points where an automatically generated stub is
 *           unpractical
 *
 * Copyright (C) 2012-2013 Red Hat, Inc.
 *
 * Daniel Veillard <veillard@redhat.com>
 */

#include <config.h>

/* Horrible kludge to work around even more horrible name-space pollution
   via Python.h.  That file includes /usr/include/python2.5/pyconfig*.h,
   which has over 180 autoconf-style HAVE_* definitions.  Shame on them.  */
#undef HAVE_PTHREAD_H

#include <Python.h>
#include <libvirt/libvirt-lxc.h>
#include <libvirt/virterror.h>
#include "typewrappers.h"
#include "libvirt-lxc.h"
#include "viralloc.h"
#include "virfile.h"

#define VIR_FROM_THIS VIR_FROM_NONE

#ifndef __CYGWIN__
extern void initlibvirtmod_lxc(void);
#else
extern void initcygvirtmod_lxc(void);
#endif

#if 0
# define DEBUG_ERROR 1
#endif

#if DEBUG_ERROR
# define DEBUG(fmt, ...)            \
   printf(fmt, __VA_ARGS__)
#else
# define DEBUG(fmt, ...)            \
   do {} while (0)
#endif

/* The two-statement sequence "Py_INCREF(Py_None); return Py_None;"
   is so common that we encapsulate it here.  Now, each use is simply
   return VIR_PY_NONE;  */
#define VIR_PY_NONE (Py_INCREF (Py_None), Py_None)
#define VIR_PY_INT_FAIL (libvirt_intWrap(-1))
#define VIR_PY_INT_SUCCESS (libvirt_intWrap(0))

/************************************************************************
 *									*
 *		Statistics						*
 *									*
 ************************************************************************/

static PyObject *
libvirt_lxc_virDomainLxcOpenNamespace(PyObject *self ATTRIBUTE_UNUSED,
                                      PyObject *args) {
    PyObject *py_retval;
    virDomainPtr domain;
    PyObject *pyobj_domain;
    unsigned int flags;
    int c_retval;
    int *fdlist = NULL;
    size_t i;

    if (!PyArg_ParseTuple(args, (char *)"Oi:virDomainLxcOpenNamespace",
                          &pyobj_domain, &flags))
        return NULL;
    domain = (virDomainPtr) PyvirDomain_Get(pyobj_domain);

    if (domain == NULL)
        return VIR_PY_NONE;
    LIBVIRT_BEGIN_ALLOW_THREADS;
    c_retval = virDomainLxcOpenNamespace(domain, &fdlist, flags);
    LIBVIRT_END_ALLOW_THREADS;

    if (c_retval < 0)
        return VIR_PY_NONE;

    py_retval = PyList_New(c_retval);
    for (i = 0; i < c_retval; i++) {
        PyObject *item = NULL;

        if ((item = PyInt_FromLong(fdlist[i])) == NULL)
            goto error;

        if (PyList_Append(py_retval, item) < 0) {
            Py_DECREF(item);
            goto error;
        }
    }
    return py_retval;

error:
    for (i = 0; i < c_retval; i++) {
        VIR_FORCE_CLOSE(fdlist[i]);
    }
    VIR_FREE(fdlist);
    return VIR_PY_NONE;
}

static PyObject *
libvirt_lxc_virDomainLxcEnterNamespace(PyObject *self ATTRIBUTE_UNUSED,
                                      PyObject *args) {
    PyObject *py_retval;
    virDomainPtr domain;
    PyObject *pyobj_domain;
    PyObject *pyobj_fd;
    PyObject *py_fdlist;
    int c_retval;
    unsigned int nfdlist;
    int *fdlist = NULL;
    unsigned int *noldfdlist = NULL;
    int **oldfdlist = NULL;
    unsigned int flags;
    size_t i;

    if (!PyArg_ParseTuple(args, (char *)"Oi:virDomainLxcEnterNamespace",
                          &pyobj_domain, &py_fdlist, &flags))
        return NULL;
    domain = (virDomainPtr) PyvirDomain_Get(pyobj_domain);

    if (domain == NULL)
        return VIR_PY_NONE;

    if (PyList_Check(py_fdlist))
        nfdlist = PyList_Size(py_fdlist);
    else
        return VIR_PY_NONE;

    if(VIR_ALLOC_N(fdlist, nfdlist) < 0)
        goto error;
 
    for(i = 0; i < nfdlist; i++) {
        pyobj_fd = PyList_GetItem(py_fdlist, i);
        fdlist[i] = PyInt_AsLong(pyobj_fd);
    }

    LIBVIRT_BEGIN_ALLOW_THREADS;
    c_retval = virDomainLxcEnterNamespace(domain, nfdlist, fdlist, noldfdlist, oldfdlist, flags);
    LIBVIRT_END_ALLOW_THREADS;

    py_retval = PyInt_FromLong( (long) c_retval);

    VIR_FREE(fdlist);
    return py_retval;

error:
    VIR_FREE(fdlist);
    return VIR_PY_NONE;
}
/************************************************************************
 *									*
 *			The registration stuff				*
 *									*
 ************************************************************************/
static PyMethodDef libvirtLxcMethods[] = {
#include "libvirt-lxc-export.c"
    {(char *) "virDomainLxcOpenNamespace", libvirt_lxc_virDomainLxcOpenNamespace, METH_VARARGS, NULL},
    {(char *) "virDomainLxcEnterNamespace", libvirt_lxc_virDomainLxcEnterNamespace, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

void
#ifndef __CYGWIN__
initlibvirtmod_lxc
#else
initcygvirtmod_lxc
#endif
  (void)
{
    static int initialized = 0;

    if (initialized != 0)
        return;

    if (virInitialize() < 0)
        return;

    /* initialize the python extension module */
    Py_InitModule((char *)
#ifndef __CYGWIN__
                  "libvirtmod_lxc"
#else
                  "cygvirtmod_lxc"
#endif
                  , libvirtLxcMethods);

    initialized = 1;
}
