
// MFCApplication1Dlg.cpp : 実装ファイル
//

#include <string.h>
#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "Dialog1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1Dlg ダイアログ



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
	, m_bWithThumbnai1(FALSE)
	, m_bWithThumbnai2(FALSE)
	, m_bWithThumbnai3(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_withThumbnailFolder1 = "C:\\Apache24\\htdocs\\www\\Secret_ex\\cache\\pdf";
	m_withThumbnailFolder2 = "T:\\cache\\original\\H_doujin";
	m_withThumbnailFolder3 = "T:\\cache\\resize\\H_doujin";
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_filterStr);

	DDX_Text(pDX, IDC_EDIT4, m_withThumbnailFolder1);
	DDX_Text(pDX, IDC_EDIT5, m_withThumbnailFolder2);
	DDX_Text(pDX, IDC_EDIT6, m_withThumbnailFolder3);
	DDX_Check(pDX, IDC_CHECK6, m_bWithThumbnai1);
	DDX_Check(pDX, IDC_CHECK7, m_bWithThumbnai2);
	DDX_Check(pDX, IDC_CHECK8, m_bWithThumbnai3);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMFCApplication1Dlg::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMFCApplication1Dlg::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication1Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication1Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication1Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCApplication1Dlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CMFCApplication1Dlg メッセージ ハンドラー

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(FALSE);

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(m_fileList.isDeleteHeadParenthesesInfo() ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(m_fileList.isDeleteTailParenthesesInfo_DLEDITION() ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(m_fileList.isFixTailParenthesesInfo_UNCENSORED() ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(m_fileList.isFixTailParenthesesInfo_ORIGINALTITLE() ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(m_fileList.isDeleteTailSpace() ? BST_CHECKED : BST_UNCHECKED);

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(m_fileList.GetFixTailParenthesesInfo_ORIGINAL_MODE() == 0 ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(m_fileList.GetFixTailParenthesesInfo_ORIGINAL_MODE() == 1 ? BST_CHECKED : BST_UNCHECKED);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#define FILE_LIST_BUFFER_SIZE 256
void CMFCApplication1Dlg::OnBnClickedButton1()
{
	UpdateData();

	do {

		CString fileName;
		char * p = fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);

		CFileDialog dlgFile(TRUE);
		OPENFILENAME& ofn = dlgFile.GetOFN();
		ofn.Flags |= OFN_ALLOWMULTISELECT;
		ofn.lpstrFile = p;
		ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

		if( dlgFile.DoModal() != IDOK ) break;

		fileName.ReleaseBuffer();
		CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST1));
		if( !pBox ) break;
		pBox->ResetContent();

		CFileFind finder;

		CString drive, path, file, ext;
		CDoujinFileRename::splitpath(fileName, drive, path, file, ext);
		file = "*";
		ext = ".*";
		CDoujinFileRename::joinpath(fileName, drive, path, file, ext);

		BOOL bWorking = finder.FindFile(fileName);
		if( bWorking ) m_fileList.Initialize();
		while( bWorking ){
			bWorking = finder.FindNextFile();
			if( finder.IsDirectory() == TRUE ) continue;
			if( finder.IsDots() == TRUE ) continue;
			if( finder.GetFileName().Find(m_filterStr) == CB_ERR && m_filterStr.IsEmpty() == FALSE ) continue;

			m_fileList.AddFileName(finder.GetFilePath());
		}
		finder.Close();

		//m_fileList.SetResultName();
		m_fileList.CopyToResultName();

		{//入力
			CStringList list;
			m_fileList.GetRenameFileStringList(list, false);

			POSITION pos = list.GetHeadPosition();
			while( pos ) pBox->AddString(list.GetNext(pos));
		}
	} while( false );
}//2017/04/14 okamoto

void CMFCApplication1Dlg::OnBnClickedButton2()
{
	m_fileList.SetDeleteHeadParenthesesInfo(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() == BST_CHECKED);
	m_fileList.SetDeleteTailParenthesesInfo_DLEDITION(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() == BST_CHECKED);
	m_fileList.SetFixTailParenthesesInfo_UNCENSORED(((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck() == BST_CHECKED);
	m_fileList.SetFixTailParenthesesInfo_ORIGINALTITLE(((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck() == BST_CHECKED);
	m_fileList.SetDeleteTailSpace(((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck() == BST_CHECKED);
	m_fileList.SetFixTailParenthesesInfo_ORIGINAL_MODE(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() == BST_CHECKED ? 0 : 1);

	{//変換
		m_fileList.SetResultName();

		CStringList list;
		m_fileList.GetRenameFileStringList(list, false);

		CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST1));
		pBox->ResetContent();
		POSITION pos = list.GetHeadPosition();
		while( pos ) pBox->AddString(list.GetNext(pos));
	}
}


void CMFCApplication1Dlg::OnBnClickedButton3()
{
	OnOK();
}

void CMFCApplication1Dlg::OnLbnSelchangeList1()
{
	CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST1));
	const int sel = pBox->GetCurSel();

	const CString autor = m_fileList.GetAuthor(sel, false);
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(autor);

	const CString title = m_fileList.GetOriginalTitle(sel, false);
	((CEdit*)GetDlgItem(IDC_EDIT3))->SetWindowText(autor == title ? "" : title);
}

void CMFCApplication1Dlg::OnLbnSelchangeList2()
{
	CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST2));
	const int sel = pBox->GetCurSel();

	const CString autor = m_fileList.GetAuthor(sel, false);
	((CStatic*)GetDlgItem(IDC_STATIC_AUTOR2))->SetWindowText(autor);
}

void CMFCApplication1Dlg::OnBnClickedButton4()
{
	UpdateData();
	CStringList fileNameList, renameFileList;
	m_fileList.GetFileStringList(fileNameList, true);
	m_fileList.GetRenameFileStringList(renameFileList, true);

	if( fileNameList.GetCount() != renameFileList.GetCount() ) {
		AfxMessageBox("指定したファイル数と変換後のファイル数がちがうんで処理やめます");
		return;
	}

#define NUMBER 3
	for( int lpcnt = 0; lpcnt < fileNameList.GetCount(); lpcnt++ ){
		const CString sourceFile = fileNameList.GetAt(fileNameList.FindIndex(lpcnt));
		const CString renameFile = renameFileList.GetAt(renameFileList.FindIndex(lpcnt));

		MoveFile(sourceFile, renameFile);

		//サムネイル
		CString drive, path, fileName, ext;
		CDoujinFileRename::splitpath(sourceFile, drive, path, fileName, ext);
		CDoujinFileRename::CThumbnailCtrl temp[NUMBER] = {
			CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai1 == TRUE, m_withThumbnailFolder1 + "\\" + fileName + ".*")
			, CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai2 == TRUE, m_withThumbnailFolder2 + "\\" + fileName + ".*")
			, CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai3 == TRUE, m_withThumbnailFolder3 + "\\" + fileName + ".*")
		};

		CDoujinFileRename::splitpath(renameFile, drive, path, fileName, ext);

		for( int lpcnt = 0; lpcnt < NUMBER; lpcnt++ ){
			temp[lpcnt].FixFileName(fileName);
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedButton5()
{
	{
		m_fileList.CopyToResultName();
		CStringList list;
		m_fileList.GetRenameFileStringList(list, false);

		CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST1));
		pBox->ResetContent();
		POSITION pos = list.GetHeadPosition();
		while( pos ) pBox->AddString(list.GetNext(pos));
	}

}

//フィルタークリア
void CMFCApplication1Dlg::OnBnClickedButton7()
{
	m_filterStr.Empty();
	UpdateData(FALSE);
}

//ファイル名称変更
void CMFCApplication1Dlg::OnBnClickedButton6()
{
	do {
		CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST1));
		const int sel = pBox->GetCurSel();
		if( sel == CB_ERR ) break;

		CDialog1 dlg;
		dlg.SetNewFileName(m_fileList.GetFileName(sel, false, false));

		if( dlg.DoModal() != IDOK ) break;

		m_fileList.SetFileName(sel, dlg.GetNewFileName());
		pBox->DeleteString(sel);
		pBox->InsertString(sel, dlg.GetNewFileName());
	} while( false );
}

//ファイル削除
void CMFCApplication1Dlg::OnBnClickedButton8()
{
	do {
		CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST1));
		const int sel = pBox->GetCurSel();
		if( sel == CB_ERR ) break;

		const CString fullFileName = m_fileList.GetFileName(sel, true, true);
		::DeleteFile(fullFileName);

		const CString fileName = m_fileList.GetFileName(sel, true, false);

		CString message;
		message.Format("「%s」を削除します", fileName);
		if( AfxMessageBox(message, MB_OKCANCEL) != IDOK ) break;

#define NUMBER 3
		//サムネイル
		CString drive, path, tfileName, ext;
		CDoujinFileRename::splitpath(fileName, drive, path, tfileName, ext);
		CDoujinFileRename::CThumbnailCtrl temp[NUMBER] = {
			CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai1 == TRUE, m_withThumbnailFolder1 + "\\" + fileName + ".*")
			, CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai2 == TRUE, m_withThumbnailFolder2 + "\\" + fileName + ".*")
			, CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai3 == TRUE, m_withThumbnailFolder3 + "\\" + fileName + ".*")
		};

		for( int lpcnt = 0; lpcnt < NUMBER; lpcnt++ ) temp[lpcnt].DeleteFile();

		AfxMessageBox("消しました");


		m_fileList.RemoveFile(sel);
		pBox->ResetContent();
		CStringList list;
		m_fileList.GetRenameFileStringList(list, false);
		POSITION pos = list.GetHeadPosition();
		while( pos ) pBox->AddString(list.GetNext(pos));
	} while( false );
}
