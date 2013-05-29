#ifndef PYTHONWRAPPER_H
#define PYTHONWRAPPER_H

Constant MAIN_MODULE = "__main__";
Constant DICT_ATTR = "__dict__";

class PythonWrapper : boost::noncopyable
{
    typedef std::function<void (const bp::object &)> PyFunctor;
    typedef std::function<void ()> BindedPyFunctor;

    public :
        static PythonWrapper & instance();

        static void exec(const PyFunctor &, bool = true);
        static void exec(const QString &, bool = true);
        static void execFile(const QString &, bool = true);

        class GILLocker
        {
            public :
                GILLocker();
                ~GILLocker();
            private :
                PyGILState_STATE state_;
        };

    private :
        PythonWrapper();

        static void clean();
        void execImpl(const BindedPyFunctor &);

        bp::object globals_;
        PyThreadState * mainThreadState_;

        std::vector<std::thread> threads_;
};

template <typename T>
std::unique_ptr<T> maybeExtract(const bp::object & object)
{
    if(object.is_none())
        return std::unique_ptr<T>();
    return std::unique_ptr<T>(new T(bp::extract<T>(object)));
}

#endif // PYTHONWRAPPER_H