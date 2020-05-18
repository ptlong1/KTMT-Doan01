
// CalculatorProjectDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CalculatorProject.h"
#include "CalculatorProjectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CCalculatorProjectDlg dialog



CCalculatorProjectDlg::CCalculatorProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CALCULATORPROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalculatorProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCalculatorProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CCalculatorProjectDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalculatorProjectDlg::OnBnClickedButton1)
	ON_EN_CHANGE(txtNum1, &CCalculatorProjectDlg::OnEnChangetxtnum1)
	ON_CBN_SELCHANGE(listOperator, &CCalculatorProjectDlg::OnCbnSelchangelistoperator)
	ON_EN_CHANGE(txtNum2, &CCalculatorProjectDlg::OnEnChangetxtnum2)
	ON_BN_CLICKED(btnplus, &CCalculatorProjectDlg::OnBnClickedbtnplus)
	ON_BN_CLICKED(btnSub, &CCalculatorProjectDlg::OnBnClickedbtnsub)
	ON_BN_CLICKED(btnMul, &CCalculatorProjectDlg::OnBnClickedbtnmul)
	ON_BN_CLICKED(btnDiv, &CCalculatorProjectDlg::OnBnClickedbtndiv)
	ON_BN_CLICKED(btnMod, &CCalculatorProjectDlg::OnBnClickedbtnmod)
	ON_BN_CLICKED(btnLarger, &CCalculatorProjectDlg::OnBnClickedbtnlarger)
	ON_BN_CLICKED(btnSmaller, &CCalculatorProjectDlg::OnBnClickedbtnsmaller)
	ON_BN_CLICKED(btnEqual, &CCalculatorProjectDlg::OnBnClickedbtnequal)
	ON_BN_CLICKED(btnAnd, &CCalculatorProjectDlg::OnBnClickedbtnand)
	ON_BN_CLICKED(btnOr, &CCalculatorProjectDlg::OnBnClickedbtnor)
	ON_BN_CLICKED(btnXor, &CCalculatorProjectDlg::OnBnClickedbtnxor)
	ON_BN_CLICKED(btnNot, &CCalculatorProjectDlg::OnBnClickedbtnnot)
	ON_BN_CLICKED(btnSright, &CCalculatorProjectDlg::OnBnClickedbtnsright)
	ON_BN_CLICKED(btnSleft, &CCalculatorProjectDlg::OnBnClickedbtnsleft)
	ON_BN_CLICKED(btnRright, &CCalculatorProjectDlg::OnBnClickedbtnrright)
	ON_BN_CLICKED(btnRleft, &CCalculatorProjectDlg::OnBnClickedbtnrleft)
	ON_CBN_SELCHANGE(listType, &CCalculatorProjectDlg::OnCbnSelchangelisttype)
	ON_EN_CHANGE(IDC_MFCMASKEDEDIT1, &CCalculatorProjectDlg::OnEnChangeMfcmaskededit1)
END_MESSAGE_MAP()


// CCalculatorProjectDlg message handlers

BOOL CCalculatorProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCalculatorProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCalculatorProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCalculatorProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCalculatorProjectDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CCalculatorProjectDlg::OnNMOutofmemoryHotkey1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CCalculatorProjectDlg::OnBnClickedButton1()
{
	CString ctype;
	GetDlgItemText(listType, ctype);
	string type(CW2A(ctype.GetString()));

	CString cmode;
	GetDlgItemText(listMode, cmode);
	string mode(CW2A(cmode.GetString()));

	CString cnum1;
	GetDlgItemText(txtNum1, cnum1);
	string num1(CW2A(cnum1.GetString()));

	CString cnum2;
	GetDlgItemText(txtNum2, cnum2);
	string num2(CW2A(cnum2.GetString()));

	CString coper;
	GetDlgItemText(txtOper, coper);
	string oper(CW2A(coper.GetString()));
	//cout << num1 << ' ' << num2 << '\n';
	if (type == "QINT")
	{
		int base = 0;
		if (mode == "BIN") base = 2;
		if (mode == "DEC") base = 10;
		if (mode == "HEX") base = 16;
		QInt a(num1, base), b(num2, base), ans;
		if (a.over || b.over)
		{
			string tmp = "Xin loi, ban that la tham lam, so qua lon.";
			CString t(tmp.c_str());
			SetDlgItemText(txtBin, t);
			SetDlgItemText(txtDec, t);
			SetDlgItemText(txtHex, t);
			return;
		}
		QInt h("128", 10);
		int c, d;
		if (b >= h) c = 128;
		else c = stoi(b.toDec());
		d = stoi((b / h).second.toDec());
		if (oper == "+") ans = a + b;
		if (oper == "-") ans = a - b;
		if (oper == "*") ans = a * b;
		if (oper == "/")
		{
			QInt zero;
			if (b == zero)
			{
				string tmp = "Xin loi, khong the chia cho 0.";
				CString t(tmp.c_str());
				SetDlgItemText(txtBin, t);
				SetDlgItemText(txtDec, t);
				SetDlgItemText(txtHex, t);
				return;
			}
			ans = (a / b).first;
		}
		if (oper == "%") ans = (a / b).second;
		if (oper == "&") ans = a & b;
		if (oper == "|") ans = a | b;
		if (oper == "^") ans = a ^ b;
		if (oper == "~") ans = ~a;
		if (oper == ">>") ans = a >> c;
		if (oper == "<<") ans = a << c;
		if (oper == "ror") ans = a.ror(d);
		if (oper == "rol") ans = a.rol(d);

		if (oper == ">")
			if (a > b) ans = "1";
			else ans = "0";
		if (oper == "<")
			if (a < b) ans = "1";
			else ans = "0";
		if (oper == "=")
			if (a == b) ans = "1";
			else ans = "0";

		CString cbin, cdec, chex;
	
		cbin = ans.toBin().c_str();
		//cbin = num1.c_str();
		cdec = ans.toDec().c_str();
		chex = ans.toHex().c_str();
		SetDlgItemText(txtBin, cbin);
		SetDlgItemText(txtDec, cdec);
		SetDlgItemText(txtHex, chex);
	}
	if (type == "QFLOAT")
	{
		string tmp = "Xin loi, chung toi khong duoc yeu cau lam tinh nang nay.";
		CString t(tmp.c_str());
		if (mode == "HEX" || oper == "%")
		{
			SetDlgItemText(txtBin, t);
			SetDlgItemText(txtDec, t);
			SetDlgItemText(txtHex, t);
			return;
		}
		int base = 0;
		if (mode == "BIN") base = 2;
		if (mode == "DEC") base = 10;
		QFloat a(num1, base), b(num2, base), ans;
		if (oper == "+") ans = a + b;
		if (oper == "-") ans = a - b;
		if (oper == "*") ans = a * b;
		if (oper == "/")
		{
			ans = (a / b);
		}
		CString cbin, cdec, chex;
		cbin = ans.toBin().c_str();
		cdec = ans.toDec().c_str();
		//chex = ans.toHex().c_str();
		SetDlgItemText(txtBin, cbin);
		SetDlgItemText(txtDec, cdec);
		SetDlgItemText(txtHex, t);

	}
}


void CCalculatorProjectDlg::OnEnChangetxtnum1()
{
	CString ctype;
	GetDlgItemText(listType, ctype);
	string type(CW2A(ctype.GetString()));

	CString cmode;
	GetDlgItemText(listMode, cmode);
	string mode(CW2A(cmode.GetString()));


	CString ct;
	GetDlgItemText(txtNum1, ct);
	string t(CW2A(ct.GetString()));

	char c[20] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	char d[15] = { 'A', 'B', 'C', 'D', 'E', 'F', 'a', 'b', 'c', 'd', 'e', 'f' };
	int n = t.size();
	if (n == 0) return;
	int kt = 0;
	int point = 0;
	CEdit* e = (CEdit*)GetDlgItem(txtNum1);
	int vt = e->CharFromPos(e->GetCaretPos());
	if (vt == 0) return;

	if (vt == 1 && t[vt - 1] == '-')
	{
		if (n > 1 && t[vt] != '-') return;
		if (n == 1) return;
	}

	for (int i = 0; i < n; ++i)
		if (t[i] == '.') point++;

	if (n > 1)
	{
		if (t[vt - 1] == '.' && type == "QFLOAT" && point == 1)
			kt = 1;
	}


	if (mode == "BIN")
	{
		if (t[vt - 1] == '0' || t[vt - 1] == '1')
			kt = 1;
	}
	if (mode == "DEC")
	{
		for (int i = 0; i < 10; ++i)
			if (t[vt - 1] == c[i])
			{
				kt = 1;
				break;
			}
	}
	if (mode == "HEX")
	{
		for (int i = 0; i < 10; ++i)
			if (t[vt - 1] == c[i])
			{
				kt = 1;
				break;
			}
		for (int i = 0; i < 12; ++i)
			if (t[vt - 1] == d[i])
			{
				kt = 1;
				break;
			}
	}
	if (kt == 0)
	{
		t.erase(vt - 1, 1);
		CString t1(t.c_str());
		SetDlgItemText(txtNum1, t1);
		e->SetFocus();
		e->SetSel(vt - 1, vt - 1);
		//e->SetSel(- 1);
	}


}


void CCalculatorProjectDlg::OnCbnSelchangelistoperator()
{
	CString coper;
	GetDlgItemText(listOperator, coper);
	string oper(CW2A(coper.GetString()));
	if (oper == "+")
	{
		string tmp = "Xin loi, chung toi khong duoc yeu cau lam tinh nang nay.";
		CString t(tmp.c_str());
		SetDlgItemText(txtBin, t);
		SetDlgItemText(txtDec, t);
		SetDlgItemText(txtHex, t);
	}
}


void CCalculatorProjectDlg::OnEnChangetxtnum2()
{
	CString ctype;
	GetDlgItemText(listType, ctype);
	string type(CW2A(ctype.GetString()));

	CString cmode;
	GetDlgItemText(listMode, cmode);
	string mode(CW2A(cmode.GetString()));


	CString ct;
	GetDlgItemText(txtNum2, ct);
	string t(CW2A(ct.GetString()));

	char c[20] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	char d[15] = { 'A', 'B', 'C', 'D', 'E', 'F', 'a', 'b', 'c', 'd', 'e', 'f' };
	int n = t.size();
	if (n == 0) return;
	int kt = 0;
	int point = 0;
	CEdit* e = (CEdit*)GetDlgItem(txtNum2);
	int vt = e->CharFromPos(e->GetCaretPos());
	if (vt == 0) return;

	if (vt == 1 && t[vt - 1] == '-')
	{
		if (n > 1 && t[vt] != '-') return;
		if (n == 1) return;
	}

	for (int i = 0; i < n; ++i)
		if (t[i] == '.') point++;

	if (n > 1)
	{
		if (t[vt - 1] == '.' && type == "QFLOAT" && point == 1)
			kt = 1;
	}


	if (mode == "BIN")
	{
		if (t[vt - 1] == '0' || t[vt - 1] == '1')
			kt = 1;
	}
	if (mode == "DEC")
	{
		for (int i = 0; i < 10; ++i)
			if (t[vt - 1] == c[i])
			{
				kt = 1;
				break;
			}
	}
	if (mode == "HEX")
	{
		for (int i = 0; i < 10; ++i)
			if (t[vt - 1] == c[i])
			{
				kt = 1;
				break;
			}
		for (int i = 0; i < 12; ++i)
			if (t[vt - 1] == d[i])
			{
				kt = 1;
				break;
			}
	}
	if (kt == 0)
	{
		t.erase(vt - 1, 1);
		CString t1(t.c_str());
		SetDlgItemText(txtNum2, t1);
		e->SetFocus();
		e->SetSel(vt- 1, vt - 1);
		//e->SetSel(- 1);
	}

}


void CCalculatorProjectDlg::OnBnClickedButton15()
{
	// TODO: Add your control notification handler code here
}


void CCalculatorProjectDlg::OnBnClickedbtnplus()
{
	string tmp = "+";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnsub() 
{
	string tmp = "-";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}



void CCalculatorProjectDlg::OnBnClickedbtnmul()
{
	string tmp = "*";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtndiv()
{
	string tmp = "/";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnmod()
{
	string tmp = "%";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnlarger()
{
	string tmp = ">";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnsmaller()
{
	string tmp = "<";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnequal()
{
	string tmp = "=";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnand()
{
	string tmp = "&";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnor()
{
	string tmp = "|";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnxor()
{
	string tmp = "^";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnnot()
{
	string tmp = "~";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
	// TODO: Add your control notification handler code here
}


void CCalculatorProjectDlg::OnBnClickedbtnsright()
{
	string tmp = ">>";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnsleft()
{
	string tmp = "<<";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnrright()
{
	string tmp = "ror";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnBnClickedbtnrleft()
{
	string tmp = "rol";
	CString t(tmp.c_str());
	SetDlgItemText(txtOper, t);
}


void CCalculatorProjectDlg::OnCbnSelchangelisttype()
{
	CString ctype;
	GetDlgItemText(listType, ctype);
	string type(CW2A(ctype.GetString()));
	bool kt = (type == "QINT");
	CButton* pButton;
	pButton = (CButton*)GetDlgItem(btnplus);
	pButton->EnableWindow(TRUE);
	pButton = (CButton*)GetDlgItem(btnSub);
	pButton->EnableWindow(TRUE);
	pButton = (CButton*)GetDlgItem(btnMul);
	pButton->EnableWindow(TRUE);
	pButton = (CButton*)GetDlgItem(btnDiv);
	pButton->EnableWindow(TRUE);
	pButton = (CButton*)GetDlgItem(btnMod);
	pButton->EnableWindow(kt);
	pButton = (CButton*)GetDlgItem(btnLarger);
	pButton->EnableWindow(kt);
	pButton = (CButton*)GetDlgItem(btnSmaller);
	pButton->EnableWindow(kt);
	pButton = (CButton*)GetDlgItem(btnEqual);
	pButton->EnableWindow(kt);
	pButton = (CButton*)GetDlgItem(btnAnd);
	pButton->EnableWindow(kt);
	pButton = (CButton*)GetDlgItem(btnOr);
	pButton->EnableWindow(kt);
	pButton = (CButton*)GetDlgItem(btnXor);
	pButton->EnableWindow(kt);
	pButton = (CButton*)GetDlgItem(btnNot);
	pButton->EnableWindow(kt);
	pButton = (CButton*)GetDlgItem(btnSleft);
	pButton->EnableWindow(kt);
	pButton = (CButton*)GetDlgItem(btnSright);
	pButton->EnableWindow(kt);
	pButton = (CButton*)GetDlgItem(btnRleft);
	pButton->EnableWindow(kt);
	pButton = (CButton*)GetDlgItem(btnRright);
	pButton->EnableWindow(kt);
}


void CCalculatorProjectDlg::OnEnChangeMfcmaskededit1()
{
}
