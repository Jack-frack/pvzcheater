
// pvzcheaterDlg.h: 头文件
//

#pragma once


// CpvzcheaterDlg 对话框
class CpvzcheaterDlg : public CDialogEx
{
// 构造
public:
	CpvzcheaterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PVZCHEATER_DIALOG };
#endif

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
	afx_msg void onbitclickedCourse();
	afx_msg void OnBnClickedkill();
	afx_msg void OnBnClickedsun();
private:
	friend DWORD monitorThreadFunc(LPVOID);
	// 无限阳光
	CButton m_bnkill;
	CButton m_bnsun;
	HANDLE m_monitorThread;
	// 一个银币变为5000
	CButton bn_money;
public:
	afx_msg void OnBnClickedmoney();


	afx_msg void OnBnClickedtimeno();
private:
	// 植物无冷却
	CButton m_bntimeno;
};
