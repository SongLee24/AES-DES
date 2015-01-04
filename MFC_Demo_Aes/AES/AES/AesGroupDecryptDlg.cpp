// AesGroupDecryptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AesGroupDecryptDlg.h"
#include "afxdialogex.h"


// CAesGroupDecryptDlg 对话框

IMPLEMENT_DYNAMIC(CAesGroupDecryptDlg, CDialogEx)

CAesGroupDecryptDlg::CAesGroupDecryptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAesGroupDecryptDlg::IDD, pParent)
	, m_cipher(_T("69c4e0d86a7b0430d8cdb78070b4c55a"))
	, m_key(_T("000102030405060708090a0b0c0d0e0f"))
{

}

CAesGroupDecryptDlg::~CAesGroupDecryptDlg()
{
}

void CAesGroupDecryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cipherEdit);
	DDX_Control(pDX, IDC_EDIT2, m_keyEdit);
	DDX_Control(pDX, IDC_STATIC_OUTPUT, m_plainText);
	DDX_Control(pDX, IDC_LIST1, m_listBox1);
	DDX_Control(pDX, IDC_LIST2, m_listBox2);
	DDX_Text(pDX, IDC_EDIT1, m_cipher);
	DDX_Text(pDX, IDC_EDIT2, m_key);
}


BEGIN_MESSAGE_MAP(CAesGroupDecryptDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CAesGroupDecryptDlg::OnBnClickedDecryptTest)
	ON_BN_CLICKED(IDC_BUTTON2, &CAesGroupDecryptDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CAesGroupDecryptDlg 消息处理程序


HBRUSH CAesGroupDecryptDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

// 工具函数 - 将128位数据转换成十六进制的字符串
CString CAesGroupDecryptDlg::plainToCString( Byte plain[4*4] )
{
	CString output("");
	for(int i=0; i<4; ++i)
		for(int j=0; j<4; ++j)
		{
			CString tmp;
			tmp.Format(_T("%02x"), plain[i+j*4]);
			output += tmp;
		}
	return output;
}


// 解密按钮
void CAesGroupDecryptDlg::OnBnClickedDecryptTest()
{
	m_listBox1.ResetContent();  // 重置
	m_listBox2.ResetContent();

	CString CCipher,CKey;
	m_cipherEdit.GetWindowTextW(CCipher);
	m_keyEdit.GetWindowTextW(CKey);

	if(CCipher == "")
		MessageBox(_T("请输入128-bits的十六进制密文（32个字符）！"), _T("提示"), MB_ICONINFORMATION);
	else if(CKey == "")
		MessageBox(_T("请输入128-bits的十六进制密钥（32个字符）！"), _T("提示"), MB_ICONINFORMATION);
	else if(CCipher.GetLength() != 32)
		MessageBox(_T("密文长度不对，请输入32个十六进制字符！"), _T("提示"), MB_ICONINFORMATION);
	else if(CKey.GetLength() != 32)
		MessageBox(_T("密钥长度不对，请输入32个十六进制字符！"), _T("提示"), MB_ICONINFORMATION);
	else
	{
		// CString转化为string
		CT2CA pszCAS_key(CKey);
		string keyStr(pszCAS_key);
		CT2CA pszCAS_cipher(CCipher);
		string cipherStr(pszCAS_cipher);

		Aes aes;
		Byte cipher[16]; // 密文
		Byte key[16];   // 密钥
		Word w[4*(Nr+1)];
		for(int i=0; i<4; ++i)
			for(int j=0; j<4; ++j)
			{
				string s1 = keyStr.substr(i*8+j*2, 2);
				key[i*4+j] = strtol(s1.c_str(), NULL, 16);
				string s2 = cipherStr.substr(i*8+j*2, 2);
				cipher[i+j*4] = strtol(s2.c_str(), NULL, 16);
			}
		// 密钥扩展
		aes.KeyExpansion(key, w);
		for(int i=0; i<4*(Nr+1); ++i)
		{
			CString temp;
			temp.Format(_T("w[%2d] = %08x"), i, w[i]);
			m_listBox2.AddString(temp);
		}
		// 16轮解密
		Word k[4];  
		CString ik_sch("");
		for(int i=0; i<4; ++i)
		{
			k[i] = w[4*Nr+i];
			CString t;
			t.Format(_T("%08x"), k[i]);
			ik_sch += t;
		}
		m_listBox1.AddString(_T("round[  0].iinput    ")+CCipher);
		m_listBox1.AddString(_T("round[  0].ik_sch    ")+ik_sch);
		aes.AddRoundKey(cipher, k);

		CString tmp;
		for(int round=Nr-1; round>0; --round)
		{
			tmp.Format(_T("round[%3d].istart    "), 10-round);
			m_listBox1.AddString(tmp + plainToCString(cipher));

			aes.InvShiftRows(cipher);
			tmp.Format(_T("round[%3d].is_row    "), 10-round);
			m_listBox1.AddString(tmp + plainToCString(cipher));

			aes.InvSubBytes(cipher);
			tmp.Format(_T("round[%3d].is_box    "), 10-round);
			m_listBox1.AddString(tmp + plainToCString(cipher));

			ik_sch = "";
			for(int i=0; i<4; ++i)
			{
				k[i] = w[4*round+i];
				CString t;
				t.Format(_T("%08x"), k[i]);
				ik_sch += t;
			}
			tmp.Format(_T("round[%3d].ik_sch    "), 10-round);
			m_listBox1.AddString(tmp + ik_sch);

			aes.AddRoundKey(cipher, k);
			tmp.Format(_T("round[%3d].ik_add    "), 10-round);
			m_listBox1.AddString(tmp + plainToCString(cipher));

			aes.InvMixColumns(cipher);
		}
		m_listBox1.AddString(_T("round[10].istart    ") + plainToCString(cipher));
		aes.InvSubBytes(cipher);
		m_listBox1.AddString(_T("round[10].is_box    ") + plainToCString(cipher));
		aes.InvShiftRows(cipher);
		m_listBox1.AddString(_T("round[10].is_row    ") + plainToCString(cipher));
		ik_sch = "";
		for(int i=0; i<4; ++i)
		{
			k[i] = w[i];
			CString t;
			t.Format(_T("%08x"), k[i]);
			ik_sch += t;
		}
		m_listBox1.AddString(_T("round[10].ik_sch    ") + ik_sch);
		aes.AddRoundKey(cipher, k);
		CString ioutput = plainToCString(cipher);
		m_listBox1.AddString(_T("round[10].ioutput    ") + ioutput);
		m_plainText.SetWindowText(ioutput);
		m_plainText.ShowWindow(SW_HIDE);
		m_plainText.ShowWindow(SW_SHOW);
	}
}

// 重置按钮
void CAesGroupDecryptDlg::OnBnClickedButton2()
{
	m_cipherEdit.SetWindowText(_T(""));
	m_keyEdit.SetWindowText(_T(""));
	m_plainText.SetWindowText(_T(""));
	m_plainText.ShowWindow(SW_HIDE);
	m_plainText.ShowWindow(SW_SHOW);
	m_listBox1.ResetContent();
	m_listBox2.ResetContent();
}