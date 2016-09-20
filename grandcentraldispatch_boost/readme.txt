支持visual studio 2005以上

依赖：
+boost typeof
+wtl   threadpool

usage:
    BEGIN_DISPATCH_ASYNC_2(main, DispatchGroupMain, pThis, nSuccessCount)
        LAMBDA_FUN()
    {
            // execute in main thread
            CString strMsg;
            strMsg.Format(_T("转换 %d 个文件，成功 %d 个"), pThis->m_krcFilePaths.size(), nSuccessCount);
            pThis->MessageBox(strMsg);

            pThis->_FinishedConvert();
    }
    END_DISPATCH_ASYNC_2(main);
    
    BEGIN_DISPATCH_ASYNC_1(global, DispatchGroupGlobal, pThis)
        LAMBDA_FUN()
    {
            // execute in global thread
            pThis->_ConvertProc();
    }
    END_DISPATCH_ASYNC_1(global);