// AesAboutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AesAboutDlg.h"
#include "afxdialogex.h"


// CAesAboutDlg 对话框

IMPLEMENT_DYNAMIC(CAesAboutDlg, CDialogEx)

CAesAboutDlg::CAesAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAesAboutDlg::IDD, pParent)
{
	
}

CAesAboutDlg::~CAesAboutDlg()
{
}

void CAesAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAesAboutDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK3, &CAesAboutDlg::OnNMClickSyslink3)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK4, &CAesAboutDlg::OnNMClickSyslink4)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK5, &CAesAboutDlg::OnNMClickSyslink5)
END_MESSAGE_MAP()


// CAesAboutDlg 消息处理程序


HBRUSH CAesAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_DLG)
	{
		HBRUSH aBrush;
		aBrush = CreateSolidBrush(RGB(255, 255, 255));
		hbr = aBrush;
	}
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	return hbr;
}

void CAesAboutDlg::OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult)
{
	PNMLINK pNMLink = (PNMLINK) pNMHDR;   
	if (wcscmp(pNMLink->item.szUrl, _T("http:\/\/weibo.com\/lisonglisong")) == 0)  
	{  
		ShellExecuteW(NULL, L"open", pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);  //主要执行语句  
	}  
	*pResult = 0;
}


void CAesAboutDlg::OnNMClickSyslink4(NMHDR *pNMHDR, LRESULT *pResult)
{
	PNMLINK pNMLink = (PNMLINK) pNMHDR;   
	if (wcscmp(pNMLink->item.szUrl, _T("http:\/\/songlee24.github.io")) == 0)  
	{  
		ShellExecuteW(NULL, L"open", pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);  //主要执行语句  
	}  
	*pResult = 0;
}


void CAesAboutDlg::OnNMClickSyslink5(NMHDR *pNMHDR, LRESULT *pResult)
{
	PNMLINK pNMLink = (PNMLINK) pNMHDR;   
	if (wcscmp(pNMLink->item.szUrl, _T("http:\/\/blog.csdn.net\/lisonglisonglisong")) == 0)  
	{  
		ShellExecuteW(NULL, L"open", pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);  //主要执行语句  
	}  
	*pResult = 0;
}
