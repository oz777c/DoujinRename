#pragma once


// CDialog2 ダイアログ

class CDialog2 : public CDialog
{
	BOOL m_bWithThumbnai1;
	BOOL m_bWithThumbnai2;
	BOOL m_bWithThumbnai3;
	CStringList m_renameList;

	DECLARE_DYNAMIC(CDialog2)

public:
	CDialog2(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CDialog2();

// ダイアログ データ
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	void SetWithThumbnail1(BOOL sw){ m_bWithThumbnai1 = sw; };
	void SetWithThumbnail2(BOOL sw){ m_bWithThumbnai2 = sw; };
	void SetWithThumbnail3(BOOL sw){ m_bWithThumbnai3 = sw; };

	void SetRenameList(const CStringList &list){
		m_renameList.RemoveAll();
		POSITION pos = list.GetHeadPosition();
		while( pos ) m_renameList.AddTail(list.GetNext(pos));
	};
};
