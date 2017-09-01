
#ifndef AD_DRAGDROP_H
#define AD_DRAGDROP_H

#include <string>

typedef BOOL (WINAPI *PtrChangeWindowMessageFilterEx)(
  _In_         HWND hWnd,
  _In_         UINT message,
  _In_         DWORD action,
  _Inout_opt_  PCHANGEFILTERSTRUCT pChangeFilterStruct
);

BOOL EnableDragDrop(CWnd* pWnd);

BOOL GetExeFileNameFromDropFiles(std::string szDroppedPath, std::string& szExeFileName);

#endif