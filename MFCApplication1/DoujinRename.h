#pragma once

class CDoujinFileRename
{
	//	()()...()[サークル/作者名] 作品名 ()...()のフォーマット限定
	//パスと拡張子は取り除いてください。

	CStringList m_targetFileList;
	CStringList m_resultFileNameList;

	//著者関連
	static bool GetAuthorStartEndNum(const CString &file, int &start, int &end);
	static int GetAuthorStartNum(const CString &file);
	static int GetAuthorEndNum(const CString &file);
	static CString GetAuthor(const CString &file);

	//ファイル名変更関連
	static void DeleteHeadParenthesesInfo(CString &file);
	static void DeleteTailParenthesesInfo_DLEDITION(CString &file);
	static void FixTailParenthesesInfo_ORIGINALTITLE(CString &file, bool bOriginalDelete);
	static void FixTailParenthesesInfo_UNCENSORED(CString &file);

	static void FixTailParenthesesInfo(CString &file, const CString tbl[], const CString &replace);
	static void DeleteTailParenthesesInfo(CString &file, const CString tbl[]);

	static CString Rename(const CString &result);

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
};
