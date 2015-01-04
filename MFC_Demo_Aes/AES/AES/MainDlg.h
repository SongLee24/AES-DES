
// AESDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "AesGroupEncryptDlg.h"
#include "AesGroupDecryptDlg.h"
#include "AesFileEncryptDlg.h"
#include "AesFileDecryptDlg.h"
#include "AesAboutDlg.h"

// CMainDlg 对话框
class CMainDlg : public CDialogEx
{
// 构造
public:
	CMainDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	CAesGroupEncryptDlg groupEnDlg;  // 分组加密标签页
	CAesGroupDecryptDlg groupDeDlg;  // 分组解密标签页
	CAesFileEncryptDlg fileEnDlg;    // 文件加密标签页
	CAesFileDecryptDlg fileDeDlg;    // 文件解密标签页
	CAesAboutDlg aboutDlg;           // 关于标签页
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
