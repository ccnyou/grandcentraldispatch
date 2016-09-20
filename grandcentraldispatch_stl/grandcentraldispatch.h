#pragma once
#include <functional>
#include "ctpl\ctpl_stl.h"

enum DispatchGroup
{
    DispatchGroupMain,
    DispatchGroupGlobal
};

class CGrandCentralDispatch
{
public:
    CGrandCentralDispatch(void);
    virtual ~CGrandCentralDispatch(void);

    /*
    * @brief Ŀ�괰������Ӧ nDispatchMsg ��Ϣʱ��
    *   ���ñ������ OnDispatch ����
    * @param hWnd �����ھ��
    * @param nDispatchMsg ��������Ӧ����Ϣ
    */
    BOOL    Init(HWND hWnd, int nDispatchMsg);
    void    Uinit();

    LRESULT OnDispatch(WPARAM wParam, LPARAM lParam);

    static CGrandCentralDispatch*   Instance();
	static void                     Dispatch(DispatchGroup group, std::function<void(void)> lambda);

private:
    HWND                    m_hMainWnd;
    int                     m_nMsg;
    ctpl::thread_pool*		m_pThreadPool;
    static const int        m_nMaxThreadCount = 5;
};

void dispatch_async(DispatchGroup group, std::function<void(void)> lambda);