#include <windows.h>
#include "AgtSvr.h"
#include "AgtSvr_i.c"

static const LPWSTR kpwzCharacterOld = L"\\program files\\microsoft agent\\characters\\genie.acs";

LPSTR _declspec (dllexport)
Speak(LPSTR lpszSpeak)
{
	HRESULT				hRes;
	VARIANT				vAgentPath;
	BSTR				bstrSpeak;
	long				lCharacterID;
	long				lRequestID;
	IAgentEx		   *pAgentEx;
	IAgentCharacterEx  *pCharacterEx = NULL;
	int					mblen;
	
    if (FAILED(CoInitialize(NULL)))
        return NULL;

	hRes = CoCreateInstance(
		&CLSID_AgentServer,
		NULL,
		CLSCTX_SERVER,
		&IID_IAgentEx,
		(LPVOID *)&pAgentEx);
	if (FAILED(hRes))
	{
		CoUninitialize();
		return NULL;
	}

	VariantInit(&vAgentPath);
	vAgentPath.vt = VT_EMPTY;

	hRes = pAgentEx->lpVtbl->Load(pAgentEx, vAgentPath, &lCharacterID, &lRequestID);
	if (FAILED(hRes))
	{
		vAgentPath.vt = VT_BSTR;
		vAgentPath.bstrVal = SysAllocString(kpwzCharacterOld);
		if (vAgentPath.bstrVal == NULL)
			goto Fail;

		hRes = pAgentEx->lpVtbl->Load(pAgentEx, vAgentPath, &lCharacterID, &lRequestID);
		if (FAILED(hRes))
			goto Fail;
	}

	hRes = pAgentEx->lpVtbl->GetCharacterEx(pAgentEx, lCharacterID, &pCharacterEx);
	if (FAILED(hRes))
		goto Fail;

#if 0
	hRes = pCharacterEx->lpVtbl->SetLanguageID(
		pCharacterEx,
		MAKELCID(MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), SORT_DEFAULT));
	if (FAILED(hRes))
	{
		hRes = pCharacterEx->lpVtbl->SetLanguageID(
			pCharacterEx,
			MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT));

		if (FAILED(hRes))
			goto Fail;
	}
#endif

	hRes = pCharacterEx->lpVtbl->Show(pCharacterEx, FALSE, &lRequestID);
	if (FAILED(hRes))
		goto Fail;

	if (lpszSpeak && (mblen = strlen(lpszSpeak)) > 0)
	{
		wchar_t* wcr = (wchar_t*)malloc(sizeof(wchar_t)*mblen+2);
		MultiByteToWideChar(CP_ACP, 0,
			lpszSpeak, mblen + 1, wcr, mblen + 1);

		bstrSpeak = SysAllocString(wcr);
		free(wcr);
	}
	else
		bstrSpeak = SysAllocString(L"...");

	hRes = pCharacterEx->lpVtbl->Speak(pCharacterEx, bstrSpeak, NULL, &lRequestID);
	SysFreeString(bstrSpeak);
	if (FAILED(hRes))
		goto Fail;

	Sleep(2000);

Fail:
	if (pCharacterEx)
	{
		pCharacterEx->lpVtbl->Release(pCharacterEx);
		pAgentEx->lpVtbl->Unload(pAgentEx, lCharacterID);
	}
	
	pAgentEx->lpVtbl->Release(pAgentEx);
	VariantClear(&vAgentPath);
	CoUninitialize();
	return NULL;
}
