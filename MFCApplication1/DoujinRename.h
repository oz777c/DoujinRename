#pragma once

class CDoujinFileRename
{
public:
	//サムネイルの変換用///////////////////////////////////////////////////////////////////////////////////////////////
	class CThumbnailCtrl{
		bool m_enableRenameFlg;
		CString m_targetfile;//対象のドライブ、パス、ファイル名

		CThumbnailCtrl(){};
	public:
		CThumbnailCtrl(bool f, CString &s){ m_enableRenameFlg = f; m_targetfile = s; };
		void FixFileName(const CString &newfile);
		void DeleteFile();
	};

	//ファイル管理/変換用///////////////////////////////////////////////////////////////////////////////////////////////
	class CFileName {
		CString m_targetFileName;	//	元ファイル名称
		CString m_resultFileName;	//	変換後ファイル名称
		CString m_ext;				//	拡張子
		bool m_renameTargetFlg;

		//	()()...()[サークル/作者名] 作品名 ()...()のフォーマット限定
		//パスと拡張子は取り除いてください。
	public:
		//著者関連
		static bool GetAuthorStartEndNum(const CString &file, int &start, int &end);
		static int GetAuthorStartNum(const CString &file);
		static int GetAuthorEndNum(const CString &file);
		static CString GetAuthor(const CString &file);
		static bool GetOriginalTitleStartEndNum(const CString &file, int &start, int &end);
		static CString GetOriginalTitle(const CString &file);

		//ファイル名変更関連
		static void DeleteHeadParenthesesInfo(CString &file);
		static void DeleteTailParenthesesInfo_DLEDITION(CString &file);
		static void DeleteTailSpace(CString &result);
		static void FixTailParenthesesInfo_ORIGINALTITLE(CString &file, bool bOriginalDelete);
		static void FixTailParenthesesInfo_UNCENSORED(CString &file);

		static void FixTailParenthesesInfo(CString &file, const CString tbl[], const CString &replace);
		static void DeleteTailParenthesesInfo(CString &file, const CString tbl[]);


	public:
		CFileName(){};
		CFileName(const CString &file, const CString &ext){ m_targetFileName = m_resultFileName = file;	m_renameTargetFlg = true; m_ext = ext; };

		bool isChangeFilename() const { return m_targetFileName != m_resultFileName; };
		bool isRenameTarget() const { return m_renameTargetFlg; };
		bool isRename() const { return isChangeFilename() && isRenameTarget(); };
		CString GetExt() const {return m_ext;};
		CString GetFileName(bool bResult) const { return bResult ? m_resultFileName : m_targetFileName; };
		CString GetRename(const CDoujinFileRename &info) const;
		bool isSelectFile(const CString& selectString, bool bResult) const;
		CString GetAuthor() const { return GetAuthor(m_targetFileName); };
		CString GetOriginalTitle() const { return GetOriginalTitle(m_targetFileName); };

		void SetRenameTarget(bool sw){m_renameTargetFlg = sw;};
		void ResetResultFileName() {m_resultFileName = m_targetFileName;};
		void Rename() { m_targetFileName = m_resultFileName; };
		void SetResultFileName(const CString &filename){ m_resultFileName = filename; };

		CFileName & operator= (const CFileName&a){
			m_targetFileName = a.m_targetFileName;
			m_resultFileName = a.m_resultFileName;
			m_renameTargetFlg = a.m_renameTargetFlg;
			m_ext = a.m_ext;
			return *this;
		};
	};
private:
	//CStringList m_targetFileList;
	//CStringList m_resultFileNameList;
	CList<CFileName> m_fileList;
	CString m_drive;
	CString m_path;

	bool m_deleteHeadParenthesesInfo_Flg;
	bool m_deleteTailParenthesesInfo_DLEDITION_Flg;
	bool m_fixTailParenthesesInfo_ORIGINALTITLE_Flg;
	bool m_fixTailParenthesesInfo_UNCENSORED_Flg;
	bool m_deleteTailSpace_Flg;
	bool m_addTailParentheseInfo_ORIGINAL_Flg;

	BYTE m_fixTailParenthesesInfo_ORIGINAL_MODE;

	CString Rename(const CString &result) const;

public:
	static void splitpath(const CString &fullPath, CString &drive, CString &path, CString &file, CString &ext);
	static void joinpath(CString &fullPath, const CString &drive, const CString &path, const CString &file, const CString &ext);

public:
	CDoujinFileRename();
	~CDoujinFileRename();

	CString GetDrive() const { return m_drive; };
	CString GetPath() const { return m_path; };
	void Initialize();
	void AddFileName(const CString &targetFile);
	void SetResultName();
	void CopyToResultName();
	void RemoveFile(const CString &selectString, bool bResult);

	CString GetFileName(const CString &selectString, bool bResult, bool bFullPath) const;
	void SetResultFileName(const CString &selectString, bool bResult, const CString &file);
	CString GetAuthor(const CString &selectString, bool bResult) const;
	CString GetOriginalTitle(const CString &selectString, bool bResult) const;
	const CList<CFileName> &GetList() const { return m_fileList; };
	CList<CFileName> &GetList() { return m_fileList; };

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
