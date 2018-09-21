
// MFCApplication1Dlg.h : ヘッダー ファイル
//

#pragma once
#include "DoujinRename.h"

// CMFCApplication1Dlg ダイアログ
class CMFCApplication1Dlg : public CDialogEx
{
	CDoujinFileRename m_fileList;
	CString m_filterStr;
	BOOL m_bWithThumbnai1;
	BOOL m_bWithThumbnai2;
	BOOL m_bWithThumbnai3;
	CString m_withThumbnailFolder1;
	CString m_withThumbnailFolder2;
	CString m_withThumbnailFolder3;

	void ResetList();

// コンストラクション
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedCheck10();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
};
