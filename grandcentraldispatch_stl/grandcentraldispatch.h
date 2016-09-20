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
    * @brief 目标窗口在响应 nDispatchMsg 消息时候，
    *   调用本对象的 OnDispatch 方法
    * @param hWnd 主窗口句柄
    * @param nDispatchMsg 主窗口响应的消息
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