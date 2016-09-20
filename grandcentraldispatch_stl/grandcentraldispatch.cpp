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

BOOL CGrandCentralDispatch::Init(HWND hWnd, int nDispatchMsg)
{
    BOOL bRet = FALSE;
    HRESULT hResult = S_FALSE;

    m_hMainWnd = hWnd;
    m_nMsg = nDispatchMsg;

    if (m_hMainWnd == NULL || m_nMsg == WM_NULL)
    {
        goto Exit0;
    }

	m_pThreadPool = new ctpl::thread_pool(m_nMaxThreadCount);
    if (!m_pThreadPool)
    {
        goto Exit0;
    }

    bRet = TRUE;

Exit0:
    return bRet;
}

LRESULT CGrandCentralDispatch::OnDispatch(WPARAM wParam, LPARAM lParam)
{
	std::function<void(void)>* block(reinterpret_cast<std::function<void(void)>*>(lParam));
	if (block)
	{
		(*block)();
		delete block;
	}

    return 0;
}

void CGrandCentralDispatch::Dispatch(DispatchGroup group, std::function<void(void)> lambda)
{
    assert(Instance());
	assert(lambda);
	if (Instance() == NULL || !lambda)
    {
        goto Exit0;
    }

    if (group == DispatchGroupMain)
    {
        if (::IsWindow(Instance()->m_hMainWnd))
        {
			std::function<void(void)>* p = new std::function<void(void)>(lambda);
            ::PostMessage(Instance()->m_hMainWnd, Instance()->m_nMsg, NULL, (LPARAM)p);
        }
    }
    else if (group == DispatchGroupGlobal)
    {
        if (Instance()->m_pThreadPool)
        {
			Instance()->m_pThreadPool->push([=](int id) { lambda(); });
        }
    }

Exit0:
    return;
}

void CGrandCentralDispatch::Uinit()
{
    if (m_pThreadPool)
    {
        m_pThreadPool->stop();
        delete m_pThreadPool;
        m_pThreadPool = NULL;
    }
}

void dispatch_async(DispatchGroup group, std::function<void(void)> lambda)
{
	CGrandCentralDispatch* inst = CGrandCentralDispatch::Instance();
	inst->Dispatch(group, lambda);
}
