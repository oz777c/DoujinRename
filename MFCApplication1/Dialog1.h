#pragma once


// CDialog1 �_�C�A���O

class CDialog1 : public CDialog
{
	DECLARE_DYNAMIC(CDialog1)

	CString m_fileName;
public:
	CDialog1(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CDialog1();

	virtual BOOL OnInitDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();


	CString GetNewFileName() const { return m_fileName; };
	void SetNewFileName(const CString &file) { m_fileName = file; };
};
