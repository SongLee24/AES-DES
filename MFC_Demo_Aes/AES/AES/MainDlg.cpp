
// AESDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "MainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RED     RGB(255,0,0)
#define YELLOW  RGB(255,255,0)
#define MAGENTA RGB(255,0,255)
#define WHITE   RGB(255,255,255)
#define BLUE    RGB(0,0,255)


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMainDlg 对话框



CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMainDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_tab.InsertItem(0, _T("分组加密"));
	m_tab.InsertItem(1, _T("分组解密"));
	m_tab.InsertItem(2, _T("文件加密"));
	m_tab.InsertItem(3, _T("文件解密"));
	m_tab.InsertItem(4, _T("关于"));
	// 关联对话框与标签页
	groupEnDlg.Create(IDD_AES_GROUP_EN, &m_tab);
	groupDeDlg.Create(IDD_AES_GROUP_DE, &m_tab);
	fileEnDlg.Create(IDD_AES_FILE_EN, &m_tab);
	fileDeDlg.Create(IDD_AES_FILE_DE, &m_tab);
	aboutDlg.Create(IDD_AES_ABOUT, &m_tab);
	// 设定在Tab内显示的范围
	CRect rc;
	m_tab.GetClientRect(&rc);
	rc.top += 22;
	rc.bottom -= 3;
	rc.left += 1;
	rc.right -= 3;
	groupEnDlg.MoveWindow(&rc);
	groupDeDlg.MoveWindow(&rc);
	fileEnDlg.MoveWindow(&rc);
	fileDeDlg.MoveWindow(&rc);
	aboutDlg.MoveWindow(&rc);
	// 显示初始页面
	groupEnDlg.ShowWindow(SW_SHOW);
	groupDeDlg.ShowWindow(SW_HIDE);
	fileEnDlg.ShowWindow(SW_HIDE);
	fileDeDlg.ShowWindow(SW_HIDE);
	aboutDlg.ShowWindow(SW_HIDE);
	// 设置默认选项卡
	m_tab.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// Tab标签页点击响应函数
void CMainDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int curSel = m_tab.GetCurSel();
	switch (curSel)
	{
	case 0:
		groupEnDlg.ShowWindow(SW_SHOW);
		groupDeDlg.ShowWindow(SW_HIDE);
		fileEnDlg.ShowWindow(SW_HIDE);
		fileDeDlg.ShowWindow(SW_HIDE);
		aboutDlg.ShowWindow(SW_HIDE);
		break;
	case 1:
		groupEnDlg.ShowWindow(SW_HIDE);
		groupDeDlg.ShowWindow(SW_SHOW);
		fileEnDlg.ShowWindow(SW_HIDE);
		fileDeDlg.ShowWindow(SW_HIDE);
		aboutDlg.ShowWindow(SW_HIDE);
		break;
	case 2:
		groupEnDlg.ShowWindow(SW_HIDE);
		groupDeDlg.ShowWindow(SW_HIDE);
		fileEnDlg.ShowWindow(SW_SHOW);
		fileDeDlg.ShowWindow(SW_HIDE);
		aboutDlg.ShowWindow(SW_HIDE);
		break;
	case 3:
		groupEnDlg.ShowWindow(SW_HIDE);
		groupDeDlg.ShowWindow(SW_HIDE);
		fileEnDlg.ShowWindow(SW_HIDE);
		fileDeDlg.ShowWindow(SW_SHOW);
		aboutDlg.ShowWindow(SW_HIDE);
		break;
	case 4:
		groupEnDlg.ShowWindow(SW_HIDE);
		groupDeDlg.ShowWindow(SW_HIDE);
		fileEnDlg.ShowWindow(SW_HIDE);
		fileDeDlg.ShowWindow(SW_HIDE);
		aboutDlg.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
	*pResult = 0;
}