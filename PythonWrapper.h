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
typename std::enable_if<std::is_copy_constructible<T>::value, std::unique_ptr<T>>::type
maybeExtract(const bp::object & object)
{
    if(object.is_none())
        return std::unique_ptr<T>();
    return std::unique_ptr<T>(new T(bp::extract<T>(object)));
}

template <typename ... Args>
void unpackPythonTuple(const bp::tuple & pytuple, Args & ... args)
{
    unpack<0, Args ...>(pytuple, args ...);
}

template <int i, typename T, typename ... Args>
void unpack(const bp::tuple & pytuple, T & t, Args & ... args)
{
    t = bp::extract<T>(pytuple[i]);
    unpack<i + 1, Args ...>(pytuple, args ...);
}

template <int i>
void unpack(const bp::tuple &) { }

#endif // PYTHONWRAPPER_H