
// CalculatorProjectDlg.h : header file
//

#pragma once
#include "QInt.h"
#include "QFloat.h"


// CCalculatorProjectDlg dialog
class CCalculatorProjectDlg : public CDialogEx
{
// Construction
public:
	CCalculatorProjectDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALCULATORPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMOutofmemoryHotkey1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangetxtnum1();
	afx_msg void OnCbnSelchangelistoperator();
	afx_msg void OnEnChangetxtnum2();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedbtnplus();
	afx_msg void OnBnClickedbtnsub();
	afx_msg void OnBnClickedbtnmul();
	afx_msg void OnBnClickedbtndiv();
	afx_msg void OnBnClickedbtnmod();
	afx_msg void OnBnClickedbtnlarger();
	afx_msg void OnBnClickedbtnsmaller();
	afx_msg void OnBnClickedbtnequal();
	afx_msg void OnBnClickedbtnand();
	afx_msg void OnBnClickedbtnor();
	afx_msg void OnBnClickedbtnxor();
	afx_msg void OnBnClickedbtnnot();
	afx_msg void OnBnClickedbtnsright();
	afx_msg void OnBnClickedbtnsleft();
	afx_msg void OnBnClickedbtnrright();
	afx_msg void OnBnClickedbtnrleft();
	afx_msg void OnCbnSelchangelisttype();
	afx_msg void OnEnChangeMfcmaskededit1();
};
