// AesGroupEncryptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AesGroupEncryptDlg.h"
#include "afxdialogex.h"


// CAesGroupEncryptDlg 对话框

IMPLEMENT_DYNAMIC(CAesGroupEncryptDlg, CDialogEx)

CAesGroupEncryptDlg::CAesGroupEncryptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAesGroupEncryptDlg::IDD, pParent)
	, m_plain(_T("00112233445566778899aabbccddeeff"))
	, m_key(_T("000102030405060708090a0b0c0d0e0f"))
{

}

CAesGroupEncryptDlg::~CAesGroupEncryptDlg()
{
}

void CAesGroupEncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_plainEdit);
	DDX_Control(pDX, IDC_EDIT2, m_keyEdit);
	DDX_Control(pDX, IDC_LIST2, m_listBox2);
	DDX_Control(pDX, IDC_LIST1, m_listBox1);
	DDX_Control(pDX, IDC_STATIC_OUTPUT, m_cipherText);
	DDX_Text(pDX, IDC_EDIT1, m_plain);
	DDX_Text(pDX, IDC_EDIT2, m_key);
}


BEGIN_MESSAGE_MAP(CAesGroupEncryptDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CAesGroupEncryptDlg::OnBnClickedEncryptTest)
	ON_BN_CLICKED(IDC_BUTTON2, &CAesGroupEncryptDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CAesGroupEncryptDlg 消息处理程序


HBRUSH CAesGroupEncryptDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_DLG)
	{
		HBRUSH aBrush;
		aBrush = CreateSolidBrush(RGB(255, 255, 255));
		hbr = aBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_STATIC1)
	//{
	//	pDC->SelectObject(&font);
	//}
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	return hbr;
}


// 工具函数 - 将128位数据转换成十六进制的字符串
CString CAesGroupEncryptDlg::plainToCString( Byte plain[4*4] )
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

// 加密测试
void CAesGroupEncryptDlg::OnBnClickedEncryptTest()
{
	m_listBox1.ResetContent();  // 重置
	m_listBox2.ResetContent();

	CString CPlain, CKey;
	m_plainEdit.GetWindowText(CPlain);
	m_keyEdit.GetWindowText(CKey);

	if(CPlain == "")
		MessageBox(_T("请输入128-bits的十六进制明文（32个字符）！"), _T("提示"), MB_ICONINFORMATION);
	else if(CKey == "")
		MessageBox(_T("请输入128-bits的十六进制密钥（32个字符）！"), _T("提示"), MB_ICONINFORMATION);
	else if(CPlain.GetLength() != 32)
		MessageBox(_T("明文长度不对，请输入32个十六进制字符！"), _T("提示"), MB_ICONINFORMATION);
	else if(CKey.GetLength() != 32)
		MessageBox(_T("密钥长度不对，请输入32个十六进制字符！"), _T("提示"), MB_ICONINFORMATION);
	else
	{
		// CString转化为string
		CT2CA pszCAS_key(CKey);
		string keyStr(pszCAS_key);
		CT2CA pszCAS_plain(CPlain);
		string plainStr(pszCAS_plain);

		Aes aes;
		Byte plain[16]; // 明文
		Byte key[16];   // 密钥
		Word w[4*(Nr+1)];
		for(int i=0; i<4; ++i)
			for(int j=0; j<4; ++j)
			{
				string s1 = keyStr.substr(i*8+j*2, 2);
				key[i*4+j] = strtol(s1.c_str(), NULL, 16);
				string s2 = plainStr.substr(i*8+j*2, 2);
				plain[i+j*4] = strtol(s2.c_str(), NULL, 16);
			}
		// 密钥扩展
		aes.KeyExpansion(key, w);
		for(int i=0; i<4*(Nr+1); ++i)
		{
			CString temp;
			temp.Format(_T("w[%2d] = %08x"), i, w[i]);
			m_listBox2.AddString(temp);
		}
		// 16轮加密
		Word k[4];  
		for(int i=0; i<4; ++i)  
			k[i] = w[i];
		m_listBox1.AddString(_T("round[  0].input    ")+CPlain);
		m_listBox1.AddString(_T("round[  0].k_sch    ")+CKey);
		aes.AddRoundKey(plain, k);

		CString tmp;
		for(int round=1; round<Nr; ++round)
		{
			tmp.Format(_T("round[%3d].start    "), round);
			m_listBox1.AddString(tmp + plainToCString(plain));

			aes.SubBytes(plain);
			tmp.Format(_T("round[%3d].s_box    "), round);
			m_listBox1.AddString(tmp + plainToCString(plain));

			aes.ShiftRows(plain);
			tmp.Format(_T("round[%3d].s_row    "), round);
			m_listBox1.AddString(tmp + plainToCString(plain));

			aes.MixColumns(plain);
			tmp.Format(_T("round[%3d].m_col    "), round);
			m_listBox1.AddString(tmp + plainToCString(plain));

			CString k_sch("");
			for(int i=0; i<4; ++i)
			{
				k[i] = w[4*round+i];
				CString t;
				t.Format(_T("%08x"), k[i]);
				k_sch += t;
			}
			tmp.Format(_T("round[%3d].k_sch    "), round);
			m_listBox1.AddString(tmp + k_sch);
			aes.AddRoundKey(plain, k);
		}
		m_listBox1.AddString(_T("round[10].start    ") + plainToCString(plain));
		aes.SubBytes(plain);
		m_listBox1.AddString(_T("round[10].s_box    ") + plainToCString(plain));
		aes.ShiftRows(plain);
		m_listBox1.AddString(_T("round[10].s_row    ") + plainToCString(plain));
		CString k_sch("");
		for(int i=0; i<4; ++i)
		{
			k[i] = w[4*Nr+i];
			CString t;
			t.Format(_T("%08x"), k[i]);
			k_sch += t;
		}
		m_listBox1.AddString(_T("round[10].k_sch    ") + k_sch);
		aes.AddRoundKey(plain, k);
		CString output = plainToCString(plain);
		m_listBox1.AddString(_T("round[10].output    ") + output);
		m_cipherText.SetWindowText(output);
		m_cipherText.ShowWindow(SW_HIDE);
		m_cipherText.ShowWindow(SW_SHOW);
	}
}


// 重置按钮
void CAesGroupEncryptDlg::OnBnClickedButton2()
{
	m_plainEdit.SetWindowText(_T(""));
	m_keyEdit.SetWindowText(_T(""));
	m_cipherText.SetWindowText(_T(""));
	m_cipherText.ShowWindow(SW_HIDE);
	m_cipherText.ShowWindow(SW_SHOW);
	m_listBox1.ResetContent();
	m_listBox2.ResetContent();
}
