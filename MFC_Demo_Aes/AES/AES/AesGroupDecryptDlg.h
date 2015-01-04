#pragma once
#include "resource.h"
#include "afxwin.h"
#include "Aes.h"

// CAesGroupDecryptDlg 对话框

class CAesGroupDecryptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAesGroupDecryptDlg)

public:
	CAesGroupDecryptDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAesGroupDecryptDlg();

// 对话框数据
	enum { IDD = IDD_AES_GROUP_DE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CEdit m_cipherEdit;
	CEdit m_keyEdit;
	CStatic m_plainText;
	CListBox m_listBox1;
	CListBox m_listBox2;
	CString plainToCString(Byte plain[]);
	afx_msg void OnBnClickedDecryptTest();
	afx_msg void OnBnClickedButton2();
	CString m_cipher;
	CString m_key;
};
