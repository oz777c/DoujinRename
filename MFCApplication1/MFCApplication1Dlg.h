
// MFCApplication1Dlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "DoujinRename.h"

// CMFCApplication1Dlg �_�C�A���O
class CMFCApplication1Dlg : public CDialogEx
{
	CDoujinFileRename m_fileList;

// �R���X�g���N�V����
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
