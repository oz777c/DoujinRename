// Dialog1.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "Dialog1.h"
#include "afxdialogex.h"


// CDialog1 ダイアログ

IMPLEMENT_DYNAMIC(CDialog1, CDialog)

CDialog1::CDialog1(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog1::IDD, pParent)
{

}

CDialog1::~CDialog1()
{
}

void CDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fileName);
}


BEGIN_MESSAGE_MAP(CDialog1, CDialog)
	ON_BN_CLICKED(IDOK, &CDialog1::OnBnClickedOk)
END_MESSAGE_MAP()


// CDialog1 メッセージ ハンドラー
BOOL CDialog1::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;
}


void CDialog1::OnBnClickedOk()
{
	UpdateData();

	CDialog::OnOK();
}
