#include "StdAfx.h"
#include <cassert>
#include "grandcentraldispatch.h"

CGrandCentralDispatch::CGrandCentralDispatch(void)
{
    m_hMainWnd = NULL;
    m_nMsg = WM_NULL;
    m_pThreadPool = NULL;
}

CGrandCentralDispatch::~CGrandCentralDispatch(void)
{

}

CGrandCentralDispatch* CGrandCentralDispatch::Instance()
{
    static CGrandCentralDispatch sharedInstance;
    return &sharedInstance;
}

BOOL CGrandCentralDispatch::Init( HWND hWnd, int nDispatchMsg )
{
    BOOL bRet = FALSE;
    HRESULT hResult = S_FALSE;

    m_hMainWnd = hWnd;
    m_nMsg = nDispatchMsg;

    if (m_hMainWnd == NULL || m_nMsg == WM_NULL)
    {
        goto Exit0;
    }

    m_pThreadPool = new CThreadPool<CGloalDispatchWorker>;
    if (!m_pThreadPool)
    {
        goto Exit0;
    }

    hResult = m_pThreadPool->Initialize(static_cast<ICGloalDispatchWorkerDelegate*>(this), m_nMaxThreadCount);
    if (hResult != S_OK)
    {
        goto Exit0;
    }

    bRet = TRUE;

Exit0:
    return bRet;
}

LRESULT CGrandCentralDispatch::OnDispatch( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    std::auto_ptr<basic_lambda> ptrBlock(reinterpret_cast<basic_lambda*>(lParam));
    (*ptrBlock)();
    return 0;
}

void CGrandCentralDispatch::Dispatch( DispatchGroup group, std::auto_ptr<basic_lambda> ptrBlock )
{
    assert(Instance() != NULL);
    assert(ptrBlock.get() != NULL);

    if (Instance() == NULL || ptrBlock.get() == NULL)
    {
        goto Exit0;
    }

    if (group == DispatchGroupMain)
    {
        if (::IsWindow(Instance()->m_hMainWnd))
        {
            ::PostMessage(Instance()->m_hMainWnd, Instance()->m_nMsg, NULL, (LPARAM)ptrBlock.release());
        }
    }
    else if (group == DispatchGroupGlobal)
    {
        if (Instance()->m_pThreadPool)
        {
            Instance()->m_pThreadPool->QueueRequest(ptrBlock.release());
        }
    }

Exit0:
    return;
}

void CGrandCentralDispatch::Uinit()
{
    if (m_pThreadPool)
    {
        m_pThreadPool->Shutdown(0x80);
        delete m_pThreadPool;
        m_pThreadPool = NULL;
    }
}

BOOL CGloalDispatchWorker::Initialize( void *pvParam )
{
    m_pDelegate = reinterpret_cast<ICGloalDispatchWorkerDelegate*>(pvParam);
    return TRUE;
}

void CGloalDispatchWorker::Execute( CGloalDispatchWorker::RequestType pTask, void *pvParam, OVERLAPPED* pOverlapped )
{
    std::auto_ptr<basic_lambda> ptrBlock(pTask);
    (*ptrBlock)();
} 

void CGloalDispatchWorker::Terminate( void* pvParam )
{
    if (m_pDelegate)
    {
        m_pDelegate->CGloalDispatchWorkerTerminateThread(this);
    }
}

CGloalDispatchWorker::CGloalDispatchWorker( void )
{
    m_pDelegate = NULL;
}

CGloalDispatchWorker::~CGloalDispatchWorker( void )
{
}
