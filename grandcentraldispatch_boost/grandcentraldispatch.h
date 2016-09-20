#pragma once
#include <boost/typeof/typeof.hpp>

enum DispatchGroup
{
    DispatchGroupMain,
    DispatchGroupGlobal
};

class basic_lambda
{
public:
    virtual void operator()() = 0;
    virtual ~basic_lambda(){}
};

class CGloalDispatchWorker;
class ICGloalDispatchWorkerDelegate
{
public:
    virtual void CGloalDispatchWorkerTerminateThread(CGloalDispatchWorker* pWorker) { };
};

class CGloalDispatchWorker
{
public:
    typedef basic_lambda*   RequestType;

public:
    CGloalDispatchWorker(void);
    virtual ~CGloalDispatchWorker(void);

    virtual BOOL Initialize(void *pvParam);
    virtual void Execute(RequestType pTask, void *pvParam, OVERLAPPED* pOverlapped);
    virtual void Terminate(void* pvParam);

private:
    ICGloalDispatchWorkerDelegate*   m_pDelegate;
};

class CGrandCentralDispatch
    : public ICGloalDispatchWorkerDelegate
{
public:
    CGrandCentralDispatch(void);
    virtual ~CGrandCentralDispatch(void);

    /*
    * @brief 目标窗口在响应 nDispatchMsg 消息时候，
    *   调用本对象的 OnDispatch 方法
    * @param hWnd 主窗口句柄
    * @param nDispatchMsg 主窗口响应的消息
    */
    BOOL    Init(HWND hWnd, int nDispatchMsg);
    void    Uinit();

    LRESULT OnDispatch(UINT uMsg, WPARAM wParam, LPARAM lParam);

    static CGrandCentralDispatch*   Instance();
    static void                     Dispatch(DispatchGroup group, std::auto_ptr<basic_lambda> ptrBlock);

private:
    HWND                    m_hMainWnd;
    int                     m_nMsg;
    CThreadPool<CGloalDispatchWorker>*
                            m_pThreadPool;
    static const int        m_nMaxThreadCount = 5;
};

//////////////////////////////////////////////////////////////////////////
#define BEGIN_LAMBDA_1(NAME, GROUP, VAR1)	\
struct _lambda_##NAME##_ARGS \
{\
    typedef _lambda_##NAME##_ARGS	MY_TYPE;\
    typedef BOOST_TYPEOF(VAR1)		VAR1_TYPE;\
    typedef BOOST_TYPEOF(GROUP)		GROUP_TYPE;\
    VAR1_TYPE var1;\
    GROUP_TYPE group;\
    \
    MY_TYPE(const GROUP_TYPE& g, const VAR1_TYPE& var1) : group(g), var1(var1) {}\
}_lambda_##NAME##_args(GROUP, VAR1);\
    \
class _lambda_##NAME : public basic_lambda\
{\
    typedef _lambda_##NAME	MY_TYPE;\
    typedef BOOST_TYPEOF(VAR1)	VAR1_TYPE;\
    \
private: \
    VAR1_TYPE VAR1;\
public: \
    MY_TYPE(const VAR1_TYPE& var) : VAR1(var) { }\

//////////////////////////////////////////////////////////////////////////
#define LAMBDA_FUN() \
public:\
    void operator()()

//////////////////////////////////////////////////////////////////////////
#define END_LAMBDA_1(NAME, VAR1)	};\
    _lambda_##NAME var_##LINE(VAR1);\
    var_##LINE();


//////////////////////////////////////////////////////////////////////////
#define BEGIN_LAMBDA_2(NAME, GROUP, VAR1, VAR2)	\
struct _lambda_##NAME##_ARGS \
{\
    typedef _lambda_##NAME##_ARGS	MY_TYPE;\
    typedef BOOST_TYPEOF(VAR1)		VAR1_TYPE;\
    typedef BOOST_TYPEOF(VAR2)		VAR2_TYPE;\
    typedef BOOST_TYPEOF(GROUP)		GROUP_TYPE;\
    VAR1_TYPE var1;\
    VAR2_TYPE var2;\
    GROUP_TYPE group;\
    \
    MY_TYPE(const GROUP_TYPE& g, const VAR1_TYPE& var1, const VAR2_TYPE& var2) : group(g), var1(var1), var2(var2) {}\
}_lambda_##NAME##_args(GROUP, VAR1, VAR2);\
    \
class _lambda_##NAME : public basic_lambda\
{\
    typedef _lambda_##NAME	MY_TYPE;\
    typedef BOOST_TYPEOF(VAR1) VAR1_TYPE;\
    typedef BOOST_TYPEOF(VAR2) VAR2_TYPE;\
private: \
    VAR1_TYPE VAR1;\
    VAR2_TYPE VAR2;\
public: \
    MY_TYPE(const VAR1_TYPE& var, const VAR2_TYPE& var2) : VAR1(var), VAR2(var2) { }

//////////////////////////////////////////////////////////////////////////
#define END_LAMBDA_2(NAME, VAR1, VAR2)	};\
    _lambda_##NAME var_##LINE(VAR1, VAR2);\
    var_##LINE();

/*****************************************
*参数：
*NAME 标识符
*VAR1 传进去的参数1
******************************************/
#define BEGIN_DISPATCH_ASYNC_1(NAME, GROUP, VAR1)	\
    {\
    BEGIN_LAMBDA_1(NAME, GROUP, VAR1)\

//////////////////////////////////////////////////////////////////////////
#define END_DISPATCH_ASYNC_1(NAME)	};\
    CGrandCentralDispatch::Dispatch(_lambda_##NAME##_args.group, std::auto_ptr<basic_lambda>(new _lambda_##NAME(_lambda_##NAME##_args.var1)));\
}

//////////////////////////////////////////////////////////////////////////
#define BEGIN_DISPATCH_ASYNC_2(NAME, GROUP, VAR1, VAR2)	\
    {\
    BEGIN_LAMBDA_2(NAME, GROUP, VAR1, VAR2)\

//////////////////////////////////////////////////////////////////////////
#define END_DISPATCH_ASYNC_2(NAME)	};\
    CGrandCentralDispatch::Dispatch(_lambda_##NAME##_args.group, std::auto_ptr<basic_lambda>(new _lambda_##NAME(_lambda_##NAME##_args.var1, _lambda_##NAME##_args.var2)));\
}