
// MFCApplication1Dlg.cpp : �����t�@�C��
//

#include <string.h>
#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "Dialog1.h"
#include "Dialog2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1Dlg �_�C�A���O



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
	, m_bWithThumbnai1(TRUE)
	, m_bWithThumbnai2(TRUE)
	, m_bWithThumbnai3(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_withThumbnailFolder1 = "C:\\Apache24\\htdocs\\www\\Secret_ex\\cache\\pdf";
	m_withThumbnailFolder2 = "T:\\cache\\original\\H_doujin";
	m_withThumbnailFolder3 = "T:\\cache\\resize\\H_doujin";
	m_filterMode = 0;
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
	ON_LBN_DBLCLK(IDC_LIST1, &CMFCApplication1Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication1Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication1Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication1Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCApplication1Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_CHECK10, &CMFCApplication1Dlg::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_BUTTON10, &CMFCApplication1Dlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_RADIO4, &CMFCApplication1Dlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CMFCApplication1Dlg::OnBnClickedRadio5)
END_MESSAGE_MAP()


// CMFCApplication1Dlg ���b�Z�[�W �n���h���[

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(FALSE);

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(m_fileList.isDeleteHeadParenthesesInfo() ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(m_fileList.isDeleteTailParenthesesInfo_DLEDITION() ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(m_fileList.isFixTailParenthesesInfo_UNCENSORED() ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(m_fileList.isFixTailParenthesesInfo_ORIGINALTITLE() ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(m_fileList.isDeleteTailSpace() ? BST_CHECKED : BST_UNCHECKED);

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(m_fileList.GetFixTailParenthesesInfo_ORIGINAL_MODE() == 0 ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(m_fileList.GetFixTailParenthesesInfo_ORIGINAL_MODE() == 1 ? BST_CHECKED : BST_UNCHECKED);

	((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(BST_CHECKED);
	InitFilterModeItem();

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
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

			m_fileList.AddFileName(finder.GetFilePath());
		}
		finder.Close();

		//m_fileList.SetResultName();
		m_fileList.CopyToResultName();
		ResetList();

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

	{//�ϊ�
		m_fileList.SetResultName();

		CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST1));
		pBox->ResetContent();
		POSITION pos = m_fileList.GetList().GetHeadPosition();
		while( pos ){
			const CDoujinFileRename::CFileName &fileInfo = m_fileList.GetList().GetNext(pos);
			pBox->AddString(fileInfo.GetFileName(true));
		}
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
	if( sel < 0 || pBox->GetCount() <= sel ) return;
	CString selectStr;
	pBox->GetText(sel, selectStr);
	if( selectStr.IsEmpty() == TRUE ) return;

	const CString autor = m_fileList.GetAuthor(selectStr, false);
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(autor);

	const CString title = m_fileList.GetOriginalTitle(selectStr, false);
	((CEdit*)GetDlgItem(IDC_EDIT3))->SetWindowText(autor == title ? "" : title);
}

void CMFCApplication1Dlg::OnBnClickedButton4()
{
	UpdateData();

	CDialog2 dlg;
	dlg.SetWithThumbnail1(m_bWithThumbnai1);
	dlg.SetWithThumbnail2(m_bWithThumbnai2);
	dlg.SetWithThumbnail3(m_bWithThumbnai3);

	CStringList list;
	POSITION pos = m_fileList.GetList().GetHeadPosition();
	while( pos ){
		const CDoujinFileRename::CFileName &fileInfo = m_fileList.GetList().GetNext(pos);
		if( !fileInfo.isRename() ) continue;

		const CString sourceFileName = fileInfo.GetFileName(false);
		const CString renameFileName = fileInfo.GetFileName(true);

		list.AddTail(sourceFileName + " ---> " + renameFileName);
	}

	dlg.SetRenameList(list);
	if( dlg.DoModal() != IDOK ) return;

	bool noMsgFlg = ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck() == BST_UNCHECKED;

#define NUMBER 3
	pos = m_fileList.GetList().GetHeadPosition();
	while( pos ){
		CDoujinFileRename::CFileName &fileInfo = m_fileList.GetList().GetNext(pos);
		if( !fileInfo.isRename() ) continue;

		const CString source = fileInfo.GetFileName(false);
		const CString rename = fileInfo.GetFileName(true);

		if( !noMsgFlg ){
			CString message;
			message.Format("�t�@�C�����O��ύX���܂����H\n%s -> %s", source, rename);
			UINT result = AfxMessageBox(message, MB_YESNOCANCEL);
			if( result == IDNO ) continue;
			if( result == IDCANCEL ) break;
		}

		CString sourceFile, renameFile;
		CDoujinFileRename::joinpath(sourceFile, m_fileList.GetDrive(), m_fileList.GetPath(), source, fileInfo.GetExt());
		CDoujinFileRename::joinpath(renameFile, m_fileList.GetDrive(), m_fileList.GetPath(), rename, fileInfo.GetExt());
		if( MoveFile(sourceFile, renameFile) == FALSE && !noMsgFlg ){
			CString message;
			message.Format("�t�@�C�����̕ύX�Ɏ��s���܂����B\n%s -> %s", source, rename);
			AfxMessageBox(message);
			continue;
		}

		//�T���l�C��
		CDoujinFileRename::CThumbnailCtrl temp[NUMBER] = {
			CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai1 == TRUE, m_withThumbnailFolder1 + "\\" + source + ".*")
			, CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai2 == TRUE, m_withThumbnailFolder2 + "\\" + source + ".*")
			, CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai3 == TRUE, m_withThumbnailFolder3 + "\\" + source + ".*")
		};

		for( int lpcnt = 0; lpcnt < NUMBER; lpcnt++ ){
			temp[lpcnt].FixFileName(rename);
		}
		fileInfo.Rename();
	}
}


void CMFCApplication1Dlg::OnBnClickedButton5()
{
	{
		m_fileList.CopyToResultName();

		ResetList();
	}
}

//�t�B���^�[�N���A
void CMFCApplication1Dlg::OnBnClickedButton7()
{
	m_filterStr.Empty();
	UpdateData(FALSE);
}

//�t�@�C�����̕ύX
void CMFCApplication1Dlg::OnBnClickedButton6()
{
	do {
		CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST1));
		const int sel = pBox->GetCurSel();
		if( sel < 0 || pBox->GetCount() <= sel ) break;
		CString selectStr;
		pBox->GetText(sel, selectStr);
		if( selectStr.IsEmpty() == TRUE ) break;


		CDialog1 dlg;
		dlg.SetNewFileName(m_fileList.GetFileName(selectStr, true, false));

		if( dlg.DoModal() != IDOK ) break;

		m_fileList.SetResultFileName(selectStr, true, dlg.GetNewFileName());
		pBox->DeleteString(sel);
		pBox->InsertString(sel, dlg.GetNewFileName());
	} while( false );
}

//�t�@�C���폜
void CMFCApplication1Dlg::OnBnClickedButton8()
{
	do {
		CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST1));
		const int sel = pBox->GetCurSel();
		if( sel < 0 || pBox->GetCount() <= sel ) break;
		CString selectStr;
		pBox->GetText(sel, selectStr);
		if( selectStr.IsEmpty() == TRUE ) break;

		const CString fullFileName = m_fileList.GetFileName(selectStr, true, true);
		::DeleteFile(fullFileName);

		const CString fileName = m_fileList.GetFileName(selectStr, true, false);

		CString message;
		message.Format("�u%s�v���폜���܂�", fileName);
		if( AfxMessageBox(message, MB_OKCANCEL) != IDOK ) break;

#define NUMBER 3
		//�T���l�C��
		CString drive, path, tfileName, ext;
		CDoujinFileRename::splitpath(fileName, drive, path, tfileName, ext);
		CDoujinFileRename::CThumbnailCtrl temp[NUMBER] = {
			CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai1 == TRUE, m_withThumbnailFolder1 + "\\" + fileName + ".*")
			, CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai2 == TRUE, m_withThumbnailFolder2 + "\\" + fileName + ".*")
			, CDoujinFileRename::CThumbnailCtrl(m_bWithThumbnai3 == TRUE, m_withThumbnailFolder3 + "\\" + fileName + ".*")
		};

		for( int lpcnt = 0; lpcnt < NUMBER; lpcnt++ ) temp[lpcnt].DeleteFile();

		m_fileList.RemoveFile(selectStr, true);
		ResetList();
	} while( false );
}

//���X�g�X�V
void CMFCApplication1Dlg::ResetList()
{
	UpdateData();
	const bool bEnableFilter = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck() == BST_CHECKED;

	CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST1));
	pBox->ResetContent();

	if( bEnableFilter ){
		if( m_filterMode == 0 ){
			POSITION pos = m_fileList.GetList().GetHeadPosition();
			while( pos ){
				const CDoujinFileRename::CFileName &fileName = m_fileList.GetList().GetNext(pos);
				const CString fileNameStr = fileName.GetFileName(true);
				if( fileNameStr.Find(m_filterStr) == -1 && m_filterStr.IsEmpty() == FALSE ) continue;

				pBox->AddString(fileNameStr);
			}
		}
		else if( m_filterMode == 1 ){
			CStringList fileList;
			m_fileList.GetSimilarAutherList(fileList);

			POSITION pos = fileList.GetHeadPosition();
			while( pos ) 	pBox->AddString(fileList.GetNext(pos));
		}
	}
	else {
		POSITION pos = m_fileList.GetList().GetHeadPosition();
		while( pos ){
			const CDoujinFileRename::CFileName &fileName = m_fileList.GetList().GetNext(pos);
			const CString fileNameStr = fileName.GetFileName(true);
			pBox->AddString(fileNameStr);
		}
	}
}


//�t�B���^�[ON
void CMFCApplication1Dlg::OnBnClickedCheck10()
{
	InitFilterModeItem();
	ResetList();
}

//���X�g�̍X�V(�蓮)
void CMFCApplication1Dlg::OnBnClickedButton10()
{
	InitFilterModeItem();
	ResetList();
}

void CMFCApplication1Dlg::OnBnClickedRadio4()
{
	m_filterMode = 0;
	InitFilterModeItem();
	ResetList();
}


void CMFCApplication1Dlg::OnBnClickedRadio5()
{
	m_filterMode = 1;
	InitFilterModeItem();
	ResetList();
}

void CMFCApplication1Dlg::InitFilterModeItem()
{
	bool filterFlg = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck() == BST_CHECKED;
	((CEdit*)GetDlgItem(IDC_EDIT1))->EnableWindow(filterFlg && m_filterMode == 0);
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(m_filterMode == 0 ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(m_filterMode == 1 ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(m_filterMode == 2 ? BST_CHECKED : BST_UNCHECKED);

	((CButton*)GetDlgItem(IDC_RADIO4))->EnableWindow(filterFlg ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_RADIO5))->EnableWindow(filterFlg ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_RADIO6))->EnableWindow(filterFlg ? TRUE : FALSE);
}
