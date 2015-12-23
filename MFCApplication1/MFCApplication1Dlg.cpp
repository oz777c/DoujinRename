
// MFCApplication1Dlg.cpp : �����t�@�C��
//

#include <string.h>
#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1Dlg �_�C�A���O



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMFCApplication1Dlg::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMFCApplication1Dlg::OnLbnSelchangeList2)
END_MESSAGE_MAP()


// CMFCApplication1Dlg ���b�Z�[�W �n���h���[

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

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

		CFileFind finder;

		CString drive, path, file, ext;
		CDoujinFileRename::splitpath(fileName, drive, path, file, ext);
		file = "*";
		ext = ".*";
		CDoujinFileRename::joinpath(fileName, drive, path, file, ext);

		BOOL bWorking = finder.FindFile(fileName);
		while( bWorking ){
			bWorking = finder.FindNextFile();
			if( finder.IsDirectory() == TRUE ) continue;
			if( finder.IsDots() == TRUE ) continue;
			m_fileList.AddFileName(finder.GetFilePath());
		}
		finder.Close();

		m_fileList.SetResultName();

		{//����
			CStringList list;
			m_fileList.GetFileStringList(list, false);

			CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST1));
			POSITION pos = list.GetHeadPosition();
			while( pos ) pBox->AddString(list.GetNext(pos));
		}

		{//�ϊ�
			CStringList list;
			m_fileList.GetRenameFileStringList(list, false);

			CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST2));
			POSITION pos = list.GetHeadPosition();
			while( pos ) pBox->AddString(list.GetNext(pos));
		}

	} while( false );
}

void CMFCApplication1Dlg::OnBnClickedButton2()
{
	CStringList fileNameList, renameFileList;
	m_fileList.GetFileStringList(fileNameList, true);
	m_fileList.GetRenameFileStringList(renameFileList, true);

	if( fileNameList.GetCount() != renameFileList.GetCount() ) {
		AfxMessageBox("�w�肵���t�@�C�����ƕϊ���̃t�@�C��������������ŏ�����߂܂�");
		return;
	}

	for( int lpcnt = 0; lpcnt < fileNameList.GetCount(); lpcnt++ ){
		const CString sourceFile = fileNameList.GetAt(fileNameList.FindIndex(lpcnt));
		const CString renameFile = renameFileList.GetAt(renameFileList.FindIndex(lpcnt));

		MoveFile(sourceFile, renameFile);

		//CFileException e;
		//CFile file;
		//if( !file.Open(sourceFile, CFile::modeWrite, &e) ){
		//	TRACE(_T("File could not be opened %d\n"), e.m_cause);
		//	continue;
		//}

		//CString drive, path, filename, ext;
		//CDoujinFileRename::splitpath(sourceFile, drive, path, filename, ext);

		//CString drive_r, path_r, filename_r, ext_r;
		//CDoujinFileRename::splitpath(renameFile, drive_r, path_r, filename_r, ext_r);

		//file.Rename(filename + ext, filename_r + ext_r);
		//file.Close();
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
	((CStatic*)GetDlgItem(IDC_STATIC_AUTOR1))->SetWindowText(autor);
}

void CMFCApplication1Dlg::OnLbnSelchangeList2()
{
	CListBox *pBox = ((CListBox*)GetDlgItem(IDC_LIST2));
	const int sel = pBox->GetCurSel();

	const CString autor = m_fileList.GetAuthor(sel, false);
	((CStatic*)GetDlgItem(IDC_STATIC_AUTOR2))->SetWindowText(autor);
}
