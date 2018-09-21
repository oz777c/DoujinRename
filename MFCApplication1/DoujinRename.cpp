#include "stdafx.h"
#include "DoujinRename.h"


//
// class CThumbnailCtrl //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDoujinFileRename::CThumbnailCtrl::FixFileName(const CString &newfile)
{
	if( !m_enableRenameFlg ) return;

	CString drive, path, file, ext;
	CDoujinFileRename::splitpath(m_targetfile, drive, path, file, ext);

	CString target;
	CDoujinFileRename::joinpath(target, drive, path, file, ".*");

	CFileFind finder;
	BOOL bWorking = finder.FindFile(target);
	while( bWorking ){
		bWorking = finder.FindNextFile();
		if( finder.IsDirectory() == TRUE ) continue;
		if( finder.IsDots() == TRUE ) continue;

		const CString sourceFile = finder.GetFilePath();
		CDoujinFileRename::splitpath(sourceFile, drive, path, file, ext);
		CDoujinFileRename::joinpath(target, drive, path, newfile, ext);

		MoveFile(sourceFile, target);
	}
	finder.Close();
};

void CDoujinFileRename::CThumbnailCtrl::DeleteFile()
{
	if( !m_enableRenameFlg ) return;

	CString drive, path, file, ext;
	CDoujinFileRename::splitpath(m_targetfile, drive, path, file, ext);

	CString target;
	CDoujinFileRename::joinpath(target, drive, path, file, ".*");

	CFileFind finder;
	BOOL bWorking = finder.FindFile(target);
	while( bWorking ){
		bWorking = finder.FindNextFile();
		if( finder.IsDirectory() == TRUE ) continue;
		if( finder.IsDots() == TRUE ) continue;

		::DeleteFile(finder.GetFilePath());
	}
	finder.Close();
};

//
// class CFileName //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static関連
//ファイル名にあるサークル名/作者の開始/終了番号を取得
bool CDoujinFileRename::CFileName::GetAuthorStartEndNum(const CString &file, int &start, int &end)
{
	//start : 最初に見つかった'['
	//end : '['が見つかった後の']'
	bool result = false;
	do {
		start = file.Find('[');
		if( start == -1 ) break;

		end = file.Find(']', start);
		if( end == -1 ){
			start = -1;
			break;
		}

		if( end <= start ){
			start = -1;
			end = -1;
			break;
		}

		result = true;
	} while( false );

	return true;
}

//ファイル名にあるサークル名/作者の開始番号を取得
int CDoujinFileRename::CFileName::GetAuthorStartNum(const CString &file)
{
	int start = -1, end = -1;
	CDoujinFileRename::CFileName::GetAuthorStartEndNum(file, start, end);
	return start;
}

//ファイル名にあるサークル名/作者の終了番号を取得
int CDoujinFileRename::CFileName::GetAuthorEndNum(const CString &file)
{
	int start = -1, end = -1;
	CDoujinFileRename::CFileName::GetAuthorStartEndNum(file, start, end);
	return end;
}

//ファイル名にあるサークル名/作者を取得
CString CDoujinFileRename::CFileName::GetAuthor(const CString &file)
{
	CString result = file;
	do{

		int start = -1, end = -1;
		if( !CDoujinFileRename::CFileName::GetAuthorStartEndNum(file, start, end) ){
			result.Empty();
			break;
		}

		result.Delete(end, result.GetLength() - end + 1);
		result.Delete(0, start + 1);
	} while( false );

	return result;
}

void CDoujinFileRename::CFileName::GetAuthor(const CString &file, CString &author1, CString & author2, bool spaceTrimFlg)
{
	const CString author = GetAuthor(file);

	const CString startStr = "(";
	const CString endStr = ")";

	const int start = author.Find(startStr);
	const int end = author.Find(endStr);

	if( start == -1 || end == -1 || start > end ) {
		author1 = author;
		author2.Empty();
		return;
	}

	author1 = author;
	author1.Delete(start, author.GetLength() - start);
	if( spaceTrimFlg ) author1.Trim();

	author2 = author;
	author2.Delete(0, start);
	if( spaceTrimFlg ){
		author2.Trim();
		author2.TrimLeft("(");
		author2.TrimRight(")");
	}
}

//ファイル名にある元ネタタイトルを取得
CString CDoujinFileRename::CFileName::GetOriginalTitle(const CString &file)
{
	CString result = file;
	do{

		int start = -1, end = -1;
		if( !CDoujinFileRename::CFileName::GetOriginalTitleStartEndNum(file, start, end) ){
			result.Empty();
			break;
		}

		result.Delete(end, result.GetLength() - end + 1);
		result.Delete(0, start + 1);
	} while( false );

	return result;
}

//ファイル名にある元ネタタイトルの開始/終了番号を取得
bool CDoujinFileRename::CFileName::GetOriginalTitleStartEndNum(const CString &file, int &start, int &end)
{
	//文字列末尾から検索して
	//start : 最初に見つかった']'
	//end : ']'が見つかった後の'['
	bool result = false;
	do {
		start = file.ReverseFind('[');
		if( start == -1 ) break;

		end = file.ReverseFind(']');
		if( end == -1 ){
			start = -1;
			break;
		}

		if( end <= start ){
			start = -1;
			end = -1;
			break;
		}

		result = true;
	} while( false );

	return true;
}

//	先頭の()に囲まれた項目を削除する
void CDoujinFileRename::CFileName::DeleteHeadParenthesesInfo(CString &file)
{
	file.TrimLeft();
	while( CDoujinFileRename::CFileName::GetAuthorStartNum(file) > 1 ){
		const int start = file.Find('(');
		if( start == -1 ) break;
		const int end = file.Find(')', start);
		if( end == -1 ) break;

		file.Delete(start, end - start + 1);
		file.TrimLeft();
	}
}

//	【】を[]に変換する
void CDoujinFileRename::CFileName::FixParentheses(CString &file)
{
	file.TrimLeft();
	int count = 0;
	while( count < 1000 ){
		const int start = file.Find("【");
		if( start == -1 ) break;
		const int end = file.Find("】", start);
		if( end == -1 ) break;
		if( start > end ) break;

		file.Replace("【", "[");
		file.Replace("】", "]");
		count++;
	}
}

//サークル名と作者名の間にスペースを追加する
void CDoujinFileRename::CFileName::AddSpaceBwCircle2Author(CString &file)
{
}

//	末尾の(DL版)を削除する
void CDoujinFileRename::CFileName::DeleteTailParenthesesInfo_DLEDITION(CString &file)
{
	//DL版
	const CString dl_edition[] = {
		"(DL版)"
		, "[DL版]"
		, ""
	};
	CDoujinFileRename::CFileName::DeleteTailParenthesesInfo(file, dl_edition);
}


//	末尾の(無修正)、(無修正版)を[無修正]に変更する
void CDoujinFileRename::CFileName::FixTailParenthesesInfo_UNCENSORED(CString &file)
{
	//無修正
	const CString unsensord[] = {
		"(無修正)"
		, "(無修正版)"
		, ""
	};
	const CString new_unsensord = "[無修正]";
	CDoujinFileRename::CFileName::FixTailParenthesesInfo(file, unsensord, new_unsensord);
}

//	末尾の()内で囲われた原作タイトルを[]囲いに変更する
//	(よろず)は削除、(オリジナル)はフラグ参照
void CDoujinFileRename::CFileName::FixTailParenthesesInfo_ORIGINALTITLE(CString &file, bool bOriginalDelete)
{
	//オリジナル
	const CString title_original[] = {
		"(オリジナル)"
		, "(original)"
		, "[オリジナル]"
		, "[original]"
		, ""
	};
	if( bOriginalDelete ){
		CDoujinFileRename::CFileName::DeleteTailParenthesesInfo(file, title_original);
	}
	else {
		const CString new_title_original = "[オリジナル]";
		CDoujinFileRename::CFileName::FixTailParenthesesInfo(file, title_original, new_title_original);
	}


	//よろず
	const CString title_yorozu[] = {
		"(よろず)"
		, ""
	};
	CDoujinFileRename::CFileName::DeleteTailParenthesesInfo(file, title_yorozu);

	int start = CDoujinFileRename::CFileName::GetAuthorEndNum(file);
	if( start != -1 ) {
		while( file.Find('(', start) != -1 ){
			const int search_s = file.Find('(', start);
			start++;
			const int search_s2 = file.Find('(', start + 1);
			const int search_e = file.Find(')', search_s);
			if( search_e == -1 ) break;
			if( search_e <= search_s ) break;
			if( search_e > search_s2 ) continue;

			//()内が数値なら変更しない。
			CString check = file.Mid(search_s + 1, search_e - search_s-1);
			CString res = check.SpanIncluding("0123456789");
			if( res != check ){
				file.Delete(search_s, 1);
				file.Insert(search_s, '[');
				file.Delete(search_e, 1);
				file.Insert(search_e, ']');
			}
		}
	}

	//お尻の_1とかを消す
	start = file.ReverseFind('_');
	if( start != -1 ) {
		CString check = file;
		check.Delete(0, start+1);
		CString res = check.SpanIncluding("0123456789");
		if( res == check ){
			file.Delete(start, file.GetLength() - start);
		}
	}
}

//ファイル名からtblで指定された文字列をreplaceに置換
void CDoujinFileRename::CFileName::FixTailParenthesesInfo(CString &file, const CString tbl[], const CString &replace)
{
	for( int lpcnt = 0; tbl[lpcnt].IsEmpty() == FALSE; lpcnt++ ){
		const int start = CDoujinFileRename::CFileName::GetAuthorEndNum(file) + 1;
		const int search = file.Find(tbl[lpcnt], start);
		if( search == -1 ) continue;

		file.Delete(search, tbl[lpcnt].GetLength());
		file.Insert(search, replace);
	}
}

//ファイル名からtblで指定された文字列を削除
void CDoujinFileRename::CFileName::DeleteTailParenthesesInfo(CString &file, const CString tbl[])
{
	for( int lpcnt = 0; tbl[lpcnt].IsEmpty() == FALSE; lpcnt++ ){
		const int start = CDoujinFileRename::CFileName::GetAuthorEndNum(file) + 1;
		const int search = file.Find(tbl[lpcnt], start);
		if( search == -1 ) continue;
		file.Delete(search, tbl[lpcnt].GetLength());
	}
}

//ファイル名から末尾のスペース文字を消す
void CDoujinFileRename::CFileName::DeleteTailSpace(CString &result)
{
	result.Trim();
}


//ファイル名を変更する
CString CDoujinFileRename::CFileName::GetRename(const CDoujinFileRename &info) const
{
	CString result = m_targetFileName;

	if( info.isFixParenthesis() ) FixParentheses(result);

	if( info.isDeleteHeadParenthesesInfo() ) DeleteHeadParenthesesInfo(result);
	if( info.isDeleteTailParenthesesInfo_DLEDITION() ) DeleteTailParenthesesInfo_DLEDITION(result);
	if( info.isDeleteTailSpace() ) DeleteTailSpace(result);
	if( info.isFixTailParenthesesInfo_UNCENSORED() ) FixTailParenthesesInfo_UNCENSORED(result);
	if( info.isFixTailParenthesesInfo_ORIGINALTITLE() ) FixTailParenthesesInfo_ORIGINALTITLE(result, info.GetFixTailParenthesesInfo_ORIGINAL_MODE() == 0);
	if( info.isAddSpaceBwCircle2Author() ) AddSpaceBwCircle2Author(result);

	return result;
}

//外部インターフェース等で選択された文字列が、該当データの対象かどうか
bool CDoujinFileRename::CFileName::isSelectFile(const CString &selectString, bool bResult) const
{
	const CString &string = bResult ? m_resultFileName : m_targetFileName;
	return string == selectString;
}

//新しい著者名をセットしたファイル名を取得する
CString CDoujinFileRename::CFileName::GetRename_NewAuthor(const CString &newAuthor) const
{
	CString result = GetFileName(false);
	const CString author = GetAuthor();

	const int sel = result.Find(author);
	result.Delete(sel, author.GetLength());
	result.Insert(sel, newAuthor);

	return result;
}

//
// class CDoujinFileRename //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//コンストラクタ
CDoujinFileRename::CDoujinFileRename()
{
	Initialize();
}

//デストラクタ
CDoujinFileRename::~CDoujinFileRename()
{
	Initialize();
}

//初期化
void CDoujinFileRename::Initialize()
{
	m_fileList.RemoveAll();

	m_deleteHeadParenthesesInfo_Flg = true;
	m_deleteTailParenthesesInfo_DLEDITION_Flg = true;
	m_fixTailParenthesesInfo_ORIGINALTITLE_Flg = true;
	m_fixTailParenthesesInfo_UNCENSORED_Flg = true;
	m_deleteTailSpace_Flg = true;
	m_addTailParentheseInfo_ORIGINAL_Flg = true;

	m_fixTailParenthesesInfo_ORIGINAL_MODE = 1;

	m_fixParenthesis_Flg = true;
	m_addSpaceBwCircle2Author_Flg = true;
}


//フルパスからドライブ、パス、ファイル名、拡張子を取得する
void CDoujinFileRename::splitpath(const CString &fullpath, CString &drive, CString &path, CString &file, CString &ext)
{
	CString tempfullpath = fullpath;
	char _drive[256], _path[256], _file[256], _ext[256];

	const char *_fullpath = tempfullpath.GetBuffer(256);

	//LPCTSTR
	_splitpath_s(_fullpath, _drive, _path, _file, _ext);
	tempfullpath.ReleaseBuffer();

	drive = _drive;
	path = _path;
	file = _file;
	ext = _ext;
}

//ドライブ、パス、ファイル名、拡張子からフルパスを作成する
void CDoujinFileRename::joinpath(CString &fullPath, const CString &drive, const CString &path, const CString &file, const CString &ext)
{
	CString renameFileStr;
	fullPath = drive + path + file + ext;
}
//該当ファイルをリストに追加
void CDoujinFileRename::AddFileName(const CString &targetFile)
{
	CString file, ext;
	splitpath(targetFile, m_drive, m_path, file, ext);
	m_fileList.AddTail(CFileName(file, ext));
}

//ファイル名
//	n	:	リストの番号
//	bResult	:	変換後	or 対象ファイル
CString CDoujinFileRename::GetFileName(const CString &selectString, bool bResult, bool bFullPath) const
{
	CString result;

	do {
		POSITION pos = m_fileList.GetHeadPosition();
		while( pos ){
			const CFileName &fileName = m_fileList.GetNext(pos);
			if( !fileName.isSelectFile(selectString, bResult) ) continue;

			result = fileName.GetFileName(bResult);
			if( bFullPath ){
				const CString fileNameStr = result;
				CDoujinFileRename::joinpath(result, m_drive, m_path, fileNameStr, fileName.GetExt());
			}
			break;
		}
	} while( false );

	return result;
}

void CDoujinFileRename::SetResultFileName(const CString &selectString, bool bResult, const CString &file)
{
	do {
		POSITION pos = m_fileList.GetHeadPosition();
		while( pos ){
			CFileName &fileName = m_fileList.GetNext(pos);
			if( !fileName.isSelectFile(selectString, bResult) ) continue;
			fileName.SetResultFileName(file);
			break;
		}
	} while( false );
}

//ファイルからサークル名/作者を取得する
//	n	:	リストの番号
//	bResult	:	変換後	or 対象ファイル
CString CDoujinFileRename::GetAuthor(const CString &selectString, bool bResult) const
{
	CString result;
	do {
		POSITION pos = m_fileList.GetHeadPosition();
		while( pos ){
			const CFileName &fileName = m_fileList.GetNext(pos);
			if( !fileName.isSelectFile(selectString, bResult) ) continue;
			result = fileName.GetAuthor();
			break;
		}
	} while( false );

	return result;
}

//ファイルから元ネタタイトルを取得する
//	n	:	リストの番号
//	bResult	:	変換後	or 対象ファイル
CString CDoujinFileRename::GetOriginalTitle(const CString &selectString, bool bResult) const
{
	CString result;
	do {
		POSITION pos = m_fileList.GetHeadPosition();
		while( pos ){
			const CFileName &fileName = m_fileList.GetNext(pos);
			if( !fileName.isSelectFile(selectString, bResult) ) continue;
			result = fileName.GetOriginalTitle();
			break;
		}
	} while( false );

	return result;
}

//	対象ファイルを変換し、結果をm_resultFileNameListに格納する
void CDoujinFileRename::SetResultName()
{
	POSITION pos = m_fileList.GetHeadPosition();
	while( pos ){
		CFileName &fileName = m_fileList.GetNext(pos);
		CString rename = fileName.GetRename(*this);

		if( fileName.GetFileName(false) != rename ){
			int count = 0;

			CString tempRename = rename;
			while(count < 500 ){
				if( Find(tempRename) == NULL ) break;
				count++;
				tempRename.Format("%s(%d)", rename, count);
			}
			rename = tempRename;
		}

		fileName.SetResultFileName(rename);
	}
}

void CDoujinFileRename::CopyToResultName()
{
	POSITION pos = m_fileList.GetHeadPosition();
	while( pos ) m_fileList.GetNext(pos).ResetResultFileName();
}

void CDoujinFileRename::RemoveFile(const CString &selectString, bool bResult)
{
	do {
		POSITION pos = m_fileList.GetHeadPosition();
		while( pos ){
			POSITION delPos = pos;
			const CFileName &fileName = m_fileList.GetNext(pos);
			if( !fileName.isSelectFile(selectString, bResult) ) continue;

			m_fileList.RemoveAt(delPos);
			break;
		}
	} while( false );
}

POSITION CDoujinFileRename::Find(const CString &selectString) const
{
	POSITION result = NULL;
	do {
		POSITION pos = m_fileList.GetHeadPosition();
		while( pos ){
			POSITION backup = pos;
			const CFileName &fileName = m_fileList.GetNext(pos);
			const CString name1 = fileName.GetFileName(false);
			const CString name2 = fileName.GetFileName(true);
			if( name1 == selectString || name2 == selectString ){
				result = backup;
				break;
			}
		}
	} while( false );

	return result;
}

void CDoujinFileRename::SetNewAuthor(const CString &oldAuthor, const CString &newAuthor)
{
	do {
		if( oldAuthor.IsEmpty() == TRUE ) break;
		if( oldAuthor == newAuthor ) break;

		POSITION pos = m_fileList.GetHeadPosition();
		while( pos ){
			CFileName &fileName = m_fileList.GetNext(pos);
			fileName.ResetResultFileName();
			const CString author = fileName.GetAuthor();

			if( oldAuthor == author ){
				CString rename = fileName.GetRename_NewAuthor(newAuthor);
				int count = 0;

				CString tempRename = rename;
				while( count < 500 ){
					if( Find(tempRename) == NULL ) break;
					count++;
					tempRename.Format("%s(%d)", rename, count);
				}
				rename = tempRename;
				fileName.SetResultFileName(rename);
			}
		}
	} while( false );
}
