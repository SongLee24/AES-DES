#pragma once
#include "resource.h"
#include "afxeditbrowsectrl.h"
#include "afxwin.h"
#include "Aes.h"

// CAesFileEncryptDlg 对话框

class CAesFileEncryptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAesFileEncryptDlg)

public:
	CAesFileEncryptDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAesFileEncryptDlg();

// 对话框数据
	enum { IDD = IDD_AES_FILE_EN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int count;  // 秒计时
	CFont font;
	CString arr[3];
	HANDLE hThread;
	DWORD threadID;
	CMFCEditBrowseCtrl m_editBrowse1;
	CMFCEditBrowseCtrl m_editBrowse2;
	CEdit m_editCtrl;
	afx_msg void OnBnClickedFileEncrypt();
	afx_msg void OnBnClickedClear();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
