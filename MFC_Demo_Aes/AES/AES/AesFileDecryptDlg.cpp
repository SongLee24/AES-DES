// AesFileDecryptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AesFileDecryptDlg.h"
#include "afxdialogex.h"
#define TIMER_ID 1

// CAesFileDecryptDlg 对话框

IMPLEMENT_DYNAMIC(CAesFileDecryptDlg, CDialogEx)

CAesFileDecryptDlg::CAesFileDecryptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAesFileDecryptDlg::IDD, pParent),
	count(0)
{
	font.CreateFont(16, 0, 0, 0, FW_BOLD, false, false, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, _T("黑体"));
}

CAesFileDecryptDlg::~CAesFileDecryptDlg()
{
}

void CAesFileDecryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_editBrowse1);
	DDX_Control(pDX, IDC_MFCEDITBROWSE2, m_editBrowse2);
	DDX_Control(pDX, IDC_EDIT1, m_editCtrl);
}


BEGIN_MESSAGE_MAP(CAesFileDecryptDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CAesFileDecryptDlg::OnBnClickedDecrypt)
	ON_BN_CLICKED(IDC_BUTTON2, &CAesFileDecryptDlg::OnBnClickedClear)
END_MESSAGE_MAP()


// CAesFileDecryptDlg 消息处理程序


HBRUSH CAesFileDecryptDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_DLG)
	{
		HBRUSH aBrush;
		aBrush = CreateSolidBrush(RGB(255, 255, 255));
		hbr = aBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{
		pDC->SelectObject(&font);
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC7)
	{
		pDC->SetTextColor(RGB(200,0,0));
	}
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	
	return hbr;
}


void CAesFileDecryptDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_ID)
	{
		CString str;
		int Second, Minute, Hour;
		++count;
		if (count >= 3600)
		{
			Hour = count/3600;  // 时
			Minute = (count-3600*Hour)/60;  // 分
			Second = (count-3600*Hour-60*Minute);  // 秒
		}
		else
		{
			Hour = 0;
			Minute = count/60;
			Second = count%60;
		}
		str.Format(_T("%02d:%02d:%02d"),Hour,Minute,Second);
		GetDlgItem(IDC_STATIC6)->SetWindowText(str);
		GetDlgItem(IDC_STATIC6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC6)->ShowWindow(SW_SHOW);

		DWORD exitcode = 0;
		GetExitCodeThread(hThread, &exitcode);
		if (exitcode != STILL_ACTIVE)
		{
			KillTimer(TIMER_ID);
			count = 0;
			GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


// 线程函数
UINT threadDecryptFunc(LPVOID lpParam)
{
	CString* arr = (CString*)lpParam;
	CString cs_fileName = arr[0];
	CString cs_savePath = arr[1];
	CString cs_key = arr[2];
	CT2CA pszCAS_key(cs_key);  // 将 TCHAR 转换为 LPCSTR
	string keyStr(pszCAS_key); // 从 LPCSTR 构造 string
	CT2CA pszCAS_filename(cs_fileName);
	string fileName(pszCAS_filename);
	CT2CA pszCAS_savepath(cs_savePath);
	string savePath(pszCAS_savepath);

	Aes aes;
	ifstream in_(fileName.c_str(), ios::binary);
	ofstream out_(savePath.c_str(), ios::binary);
	Byte key[16]; 
	aes.charToByte(key, keyStr.c_str());
	// 密钥扩展
	Word w[4*(Nr+1)];
	aes.KeyExpansion(key, w);

	bitset<128> data;  // 临时存放读取的数据
	Byte plain[16];
	while(in_.read((char*)&data, sizeof(data)))
	{
		aes.divideByte(plain, data);
		aes.decrypt(plain, w);
		data = aes.mergeByte(plain);
		out_.write((char*)&data, sizeof(data));
		data.reset();  // 置0
	}
	in_.close();
	out_.close();
	return 0;
}

// 解密按钮
void CAesFileDecryptDlg::OnBnClickedDecrypt()
{
	CString m_fileName, m_savePath, m_key;
	m_editBrowse1.GetWindowTextW(m_fileName);
	m_editBrowse2.GetWindowTextW(m_savePath);
	m_editCtrl.GetWindowTextW(m_key);
	if (m_fileName == "")
		MessageBox(_T("请选择待解密的文件！"), _T("提示"), MB_ICONINFORMATION);
	else if (m_savePath == "")
		MessageBox(_T("请选择解密文件的保存路径！"), _T("提示"), MB_ICONINFORMATION);
	else if (m_key == "")
		MessageBox(_T("请输入密钥！"), _T("提示"), MB_ICONINFORMATION);
	else   // 解密
	{
		if(m_key.GetLength() != 16)
		{
			MessageBox(_T("密钥需要16位！"), _T("提示"), MB_ICONINFORMATION);
			return;
		}
		CT2CA pszCAS_filename(m_fileName);
		string fileName(pszCAS_filename);
		ifstream _in(fileName.c_str(), ios::binary);
		if(!_in)  // 如果不存在
			MessageBox(_T("待加密的文件不存在！"), _T("提示"), MB_ICONINFORMATION);
		else
		{
			SetTimer(TIMER_ID, 1000, NULL);  // 开始计时
			GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
			// 开辟子线程对文件进行解密
			arr[0] = m_fileName;
			arr[1] = m_savePath;
			arr[2] = m_key;
			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadDecryptFunc, (void*)arr, 0, &threadID);
		}
		_in.close();
	}
}

// 重置按钮
void CAesFileDecryptDlg::OnBnClickedClear()
{
	m_editBrowse1.SetWindowTextW(_T(""));
	m_editBrowse2.SetWindowTextW(_T(""));
	m_editCtrl.SetWindowTextW(_T(""));
	GetDlgItem(IDC_STATIC6)->SetWindowText(_T("00:00:00"));
	GetDlgItem(IDC_STATIC6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC6)->ShowWindow(SW_SHOW);
}
