#pragma once


// CDialog1 ダイアログ

class CDialog1 : public CDialog
{
	DECLARE_DYNAMIC(CDialog1)

	CString m_fileName;
public:
	CDialog1(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CDialog1();

	virtual BOOL OnInitDialog();

// ダイアログ データ
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();


	CString GetNewFileName() const { return m_fileName; };
	void SetNewFileName(const CString &file) { m_fileName = file; };
};
