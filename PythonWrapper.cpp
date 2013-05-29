#include "stdafx.h"
#include "PythonWrapper.h"

PythonWrapper::PythonWrapper()
{
    Py_Initialize();
    PyEval_InitThreads();

    globals_ = bp::import(MAIN_MODULE).attr(DICT_ATTR);
    mainThreadState_ = PyEval_SaveThread();

    execFile(":/Python/Twitch");

    qAddPostRoutine(clean);
}

PythonWrapper & PythonWrapper::instance()
{
    static PythonWrapper pythonWrapper;
    return pythonWrapper;
}

void PythonWrapper::clean()
{
    auto & self = instance();

    PyEval_RestoreThread(self.mainThreadState_);
    self.globals_ = bp::object();

    for(auto & thread : self.threads_) 
        if(thread.joinable()) thread.join();

    Py_Finalize();
}

void PythonWrapper::exec(const PyFunctor & pyFunctor, bool concurrent)
{
    auto & self = instance();

    BindedPyFunctor bindedFunctor = std::bind(pyFunctor, self.globals_);
    self.threads_.push_back(
        std::thread(
            std::bind(&PythonWrapper::execImpl, &self, bindedFunctor)));
    
    if(concurrent) self.threads_.rbegin()->join();
}

void PythonWrapper::exec(const QString & code, bool concurrent)
{
    exec([&](const bp::object & globals)
    {
        bp::exec(code.toLocal8Bit().constData(), globals);
    }, concurrent);
}

void PythonWrapper::execFile(const QString & fileName, bool concurrent)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
        exec(QString(file.readAll()), concurrent);
}

PythonWrapper::GILLocker::GILLocker()
{
    state_ = PyGILState_Ensure();
}

PythonWrapper::GILLocker::~GILLocker()
{
    PyGILState_Release(state_);
}

void PythonWrapper::execImpl(const BindedPyFunctor & functor)
{
    GILLocker lock;

    try
    {
        functor();
    }
    catch(const bp::error_already_set &)
    {
        PyErr_Print();
    }
}