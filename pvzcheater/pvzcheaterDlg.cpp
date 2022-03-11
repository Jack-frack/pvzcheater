
// pvzcheaterDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "pvzcheater.h"
#include "pvzcheaterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CpvzcheaterDlg* g_dlg;
static HANDLE g_processHandle;
//将某个值写入植物大战僵尸内存（后面的可变参数是地址链，以-1结尾）
void WriteMemory(void* value, DWORD valueSize, ...) {
	if (value == NULL || valueSize == 0 || g_processHandle == NULL) return;
	DWORD tempValue = 0;
	va_list addresses;
	va_start(addresses, valueSize);
	DWORD offset = 0;
	DWORD lastAddress = 0;
	while ((offset = va_arg(addresses,DWORD))!=-1)
	{
		lastAddress = tempValue + offset;
		::ReadProcessMemory(g_processHandle, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), NULL);
	}
	
	if (lastAddress != 0) {
		va_end(addresses);
		::WriteProcessMemory(g_processHandle, (LPVOID)lastAddress, value, valueSize, NULL);
	}

}
void WriteMemory(void* value, DWORD valueSize, DWORD address) {
	WriteMemory(value, valueSize, address, -1);
}


//用来监视游戏的进程
DWORD monitorThreadFunc(LPVOID lpThreadParmeter){

	while (1)
	{
		//获取植物大战僵尸窗口的句柄
		HWND gameHANDLE = FindWindow(CString("MainWindow"), CString("植物大战僵尸中文版"));
		
		if (gameHANDLE == NULL) {
			g_dlg->m_bnkill.SetCheck(FALSE);
			g_dlg->m_bnsun.SetCheck(FALSE);
			g_dlg->bn_money.SetCheck(FALSE);
			g_dlg->m_bntimeno.SetCheck(FALSE);

			g_dlg->m_bnkill.EnableWindow(FALSE);
			g_dlg->m_bnsun.EnableWindow(FALSE);
			g_dlg->bn_money.EnableWindow(FALSE);
			g_dlg->m_bntimeno.EnableWindow(FALSE);

			g_processHandle = NULL;
		}
		else if(g_processHandle==NULL){
			//获得进程id
			DWORD pid;
			GetWindowThreadProcessId(gameHANDLE, &pid);
			//获得进程句柄
			g_processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			g_dlg->m_bnkill.EnableWindow(TRUE);
			g_dlg->m_bnsun.EnableWindow(TRUE);
			g_dlg->bn_money.EnableWindow(TRUE);
			g_dlg->m_bntimeno.EnableWindow(TRUE);
		}
		
		Sleep(1000);
	}
	return NULL;
}






// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpvzcheaterDlg 对话框



CpvzcheaterDlg::CpvzcheaterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PVZCHEATER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpvzcheaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_sun, m_bnsun);
	DDX_Control(pDX, IDC_kill, m_bnkill);
	DDX_Control(pDX, IDC_money, bn_money);
	DDX_Control(pDX, IDC_timeno, m_bntimeno);
}

BEGIN_MESSAGE_MAP(CpvzcheaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COURSE,CpvzcheaterDlg:: onbitclickedCourse)
	ON_BN_CLICKED(IDC_kill, &CpvzcheaterDlg::OnBnClickedkill)
	ON_BN_CLICKED(IDC_sun, &CpvzcheaterDlg::OnBnClickedsun)
	ON_BN_CLICKED(IDC_money, &CpvzcheaterDlg::OnBnClickedmoney)
	ON_BN_CLICKED(IDC_timeno, &CpvzcheaterDlg::OnBnClickedtimeno)
END_MESSAGE_MAP()


// CpvzcheaterDlg 消息处理程序

BOOL CpvzcheaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//创建一个子线程，监控游戏的启动
	m_monitorThread = CreateThread(NULL, NULL,monitorThreadFunc, NULL, NULL, NULL);
	//保存对话框
	g_dlg = this;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CpvzcheaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CpvzcheaterDlg::OnPaint()
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
HCURSOR CpvzcheaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//定义一个宏实现打印
#define log(fmt,...)\
CString str;\
str.Format(CString(fmt), __VA_ARGS__);\
AfxMessageBox(CString(str));
void CpvzcheaterDlg :: onbitclickedCourse() {
	//MFC几种打印方式
	// int age = 20
	//TRACE("age is %d\n",age);
	//AfxMessageBox(CString("hello"));
	//int age = 10;
	/*CString str;
	str.Format(CString("age is %d"), age);
	MessageBox(str, CString("警告"), MB_YESNOCANCEL | MB_ICONWARNING);*/
	//log("age id %d", age);

	//打开网页
	ShellExecute(NULL, CString("open"),
		CString("www.fracklee.top"),
		NULL, NULL, SW_SHOWNORMAL);
}


void CpvzcheaterDlg::OnBnClickedkill()
{
	// TODO: 在此添加控件通知处理程序代码
	//秒杀僵尸
	BOOL checked = IsDlgButtonChecked(IDC_kill);
	if (checked)
	{		//路障僵尸的防御去除
		BYTE data1[] = { 0x29,0xc9 };
		WriteMemory(data1, sizeof(data1), 0x00531044);
		//log("勾选");
		BYTE data[] = { 0xFF,0x90,0x90 };
		WriteMemory(data,sizeof(data), 0x00531310);
	

	}
	else {
		BYTE data1[] = { 0x29,0xc1 };
		WriteMemory(data1, sizeof(data1), 0x00531044);

		BYTE data[] = { 0x7c,0x24,0x20 };
		WriteMemory(data, sizeof(data), 0x00531310);
		
	
	}



}


void CpvzcheaterDlg::OnBnClickedsun()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (m_bnsun.GetCheck())
	//{
	//	//log("勾选");
	//}
	//else {

	//}
	if (g_dlg->m_bnsun.GetCheck())//需要无限阳光
	{
		DWORD value = 9990;
		WriteMemory(&value, sizeof(value), 0x6a9ec0, 0x320, 0x8, 0x0, 0x8, 0x144, 0x2c, 0x5560, -1);
	}

}


void CpvzcheaterDlg::OnBnClickedmoney()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_dlg->bn_money.GetCheck())//需要更多的money
	{
		DWORD lpBuffer;
		DWORD lpBaseAddress;
		::ReadProcessMemory(
			g_processHandle,
			(LPCVOID)0x094B41A0,
			&lpBuffer,
			sizeof(DWORD),
			NULL
		);
		DWORD value = lpBuffer + 5000;
		WriteMemory(&value, sizeof(value), 0x094B41A0);
	}
}
//




void CpvzcheaterDlg::OnBnClickedtimeno()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_dlg->m_bntimeno.GetCheck())//需要无限阳光
	{
		
		BYTE data[] = { 0x90,0x90 };
		WriteMemory(data, sizeof(data), 0x00487296);

	}
	else {
		BYTE data[] = {0x7e,0x14 };
		WriteMemory(data, sizeof(data), 0x00487296);
	}


}
