// Dialog2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "Dialog2.h"
#include "afxdialogex.h"


// CDialog2 ダイアログ

IMPLEMENT_DYNAMIC(CDialog2, CDialog)

CDialog2::CDialog2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog2::IDD, pParent)
{

}

CDialog2::~CDialog2()
{
}

void CDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog2, CDialog)
END_MESSAGE_MAP()


// CDialog2 メッセージ ハンドラー
BOOL CDialog2::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_THUMBNAIL1_STATUS)->SetWindowText(m_bWithThumbnai1 == TRUE ? "ON" : "OFF");
	GetDlgItem(IDC_THUMBNAIL2_STATUS)->SetWindowText(m_bWithThumbnai2 == TRUE ? "ON" : "OFF");
	GetDlgItem(IDC_THUMBNAIL3_STATUS)->SetWindowText(m_bWithThumbnai3 == TRUE ? "ON" : "OFF");

	((CListBox*)GetDlgItem(IDC_LIST1))->ResetContent();
	POSITION pos = m_renameList.GetHeadPosition();
	while( pos ) ((CListBox*)GetDlgItem(IDC_LIST1))->AddString(m_renameList.GetNext(pos));

	return TRUE;
}
