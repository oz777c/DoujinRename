#pragma once

class CDoujinFileRename
{
	//	()()...()[サークル/作者名] 作品名 ()...()のフォーマット限定
	//パスと拡張子は取り除いてください。

	CStringList m_targetFileList;
	CStringList m_resultFileNameList;

	bool m_deleteHeadParenthesesInfo_Flg;
	bool m_deleteTailParenthesesInfo_DLEDITION_Flg;
	bool m_fixTailParenthesesInfo_ORIGINALTITLE_Flg;
	bool m_fixTailParenthesesInfo_UNCENSORED_Flg;
	bool m_deleteTailSpace_Flg;
	bool m_addTailParentheseInfo_ORIGINAL_Flg;

	BYTE m_fixTailParenthesesInfo_ORIGINAL_MODE;

	//著者関連
	static bool GetAuthorStartEndNum(const CString &file, int &start, int &end);
	static int GetAuthorStartNum(const CString &file);
	static int GetAuthorEndNum(const CString &file);
	static CString GetAuthor(const CString &file);

	//ファイル名変更関連
	static void DeleteHeadParenthesesInfo(CString &file);
	static void DeleteTailParenthesesInfo_DLEDITION(CString &file);
	static void DeleteTailSpace(CString &result);
	static void FixTailParenthesesInfo_ORIGINALTITLE(CString &file, bool bOriginalDelete);
	static void FixTailParenthesesInfo_UNCENSORED(CString &file);

	static void FixTailParenthesesInfo(CString &file, const CString tbl[], const CString &replace);
	static void DeleteTailParenthesesInfo(CString &file, const CString tbl[]);

	CString Rename(const CString &result) const;

public:
	CDoujinFileRename();
	~CDoujinFileRename();

	void Initialize();
	void AddFileName(const CString &targetFile);
	void SetResultName();

	void GetFileStringList(CStringList &resultList, bool bfullpath) const;
	void GetRenameFileStringList(CStringList &resultList, bool bfullpath) const;

	CString GetAuthor(int n, bool bResult) const;

	static void splitpath(const CString &fullPath, CString &drive, CString &path, CString &file, CString &ext);
	static void joinpath(CString &fullPath, const CString &drive, const CString &path, const CString &file, const CString &ext);

	//モード取得
	bool isDeleteHeadParenthesesInfo() const { return m_deleteHeadParenthesesInfo_Flg; };
	bool isDeleteTailParenthesesInfo_DLEDITION() const { return m_deleteTailParenthesesInfo_DLEDITION_Flg; };
	bool isFixTailParenthesesInfo_ORIGINALTITLE() const { return m_fixTailParenthesesInfo_ORIGINALTITLE_Flg; };
	bool isFixTailParenthesesInfo_UNCENSORED() const { return m_fixTailParenthesesInfo_UNCENSORED_Flg; };
	bool isDeleteTailSpace() const { return m_deleteTailSpace_Flg; };
	bool isAddTailParentheseInfo_ORIGINAL() const { return m_addTailParentheseInfo_ORIGINAL_Flg; };
	BYTE GetFixTailParenthesesInfo_ORIGINAL_MODE() const { return m_fixTailParenthesesInfo_ORIGINAL_MODE; };

	//モードセット
	void SetDeleteHeadParenthesesInfo(bool flg) { m_deleteHeadParenthesesInfo_Flg = flg; };
	void SetDeleteTailParenthesesInfo_DLEDITION(bool flg) { m_deleteTailParenthesesInfo_DLEDITION_Flg = flg; };
	void SetFixTailParenthesesInfo_ORIGINALTITLE(bool flg) { m_fixTailParenthesesInfo_ORIGINALTITLE_Flg = flg; };
	void SetFixTailParenthesesInfo_UNCENSORED(bool flg) { m_fixTailParenthesesInfo_UNCENSORED_Flg = flg; };
	void SetDeleteTailSpace(bool flg) { m_deleteTailSpace_Flg = flg; };
	void SetAddTailParentheseInfo_ORIGINAL(bool flg) { m_addTailParentheseInfo_ORIGINAL_Flg = flg; };
	void SetFixTailParenthesesInfo_ORIGINAL_MODE(BYTE mode) { m_fixTailParenthesesInfo_ORIGINAL_MODE = mode; };
};
