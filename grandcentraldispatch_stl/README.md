# grand central dispatch
grand central dispatch(GCD) for Windows with C++

## grandcentraldispatch_stl
支持visual studio 2013以上(需要C++11)

依赖：
* [ctpl_stl](https://github.com/vit-vit/CTPL)   threadpool

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
LRESULT CMainDlg::OnGrandCentralDispatch(WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = 0;
    lResult = CGrandCentralDispatch::Instance()->OnDispatch(wParam, lParam);
    return lResult;
}
```

#### Dispatch Main Group
```c++
dispatch_async(DispatchGroupMain, [=]() 
{
	// execute in main thread
});
```

#### Dispatch Global Group
```c++
dispatch_async(DispatchGroupMain, [=]() {
    // execute in global thread
});
```
