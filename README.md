# grand central dispatch
grand central dispatch(GCD) for Windows with C++

## grandcentraldispatch_boost
支持visual studio 2005以上(不需要C++11)

依赖：
* boost typeof
* wtl   threadpool

## Usage

#### Init GCD
```c++
BOOL CMainDlg::_InitGCD()
{
    BOOL bRet = FALSE;
    bRet = CGrandCentralDispatch::Instance()->Init(m_hWnd, UM_GCD);
    return bRet;
}

// MSG Proc
LRESULT CMainDlg::OnGrandCentralDispatch(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = 0;

    SetMsgHandled(TRUE);
    lResult = CGrandCentralDispatch::Instance()->OnDispatch(uMsg, wParam, lParam);

    return lResult;
}
```

#### Dispatch Main Group
```c++
CMainDlg* pThis = this; // catch this
BEGIN_DISPATCH_ASYNC_2(main, DispatchGroupMain, pThis, nSuccessCount)
    LAMBDA_FUN()
{
        // execute in main thread
        CString strMsg(_T("Success"));
        pThis->MessageBox(strMsg);
}
END_DISPATCH_ASYNC_2(main);
```

#### Dispatch Global Group
```c++
BEGIN_DISPATCH_ASYNC_1(global, DispatchGroupGlobal, pThis)
    LAMBDA_FUN()
{
        // execute in global thread
        pThis->_ConvertProc();
}
END_DISPATCH_ASYNC_1(global);
```
