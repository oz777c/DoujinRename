#include "stdafx.h"
#include "DoujinRename.h"


//CDoujinFileRename


//static関連
//ファイル名にあるサークル名/作者の開始/終了番号を取得
bool CDoujinFileRename::GetAuthorStartEndNum(const CString &file, int &start, int &end)
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
int CDoujinFileRename::GetAuthorStartNum(const CString &file)
{
	int start = -1, end = -1;
	CDoujinFileRename::GetAuthorStartEndNum(file, start, end);
	return start;
}

//ファイル名にあるサークル名/作者の終了番号を取得
int CDoujinFileRename::GetAuthorEndNum(const CString &file)
{
	int start = -1, end = -1;
	CDoujinFileRename::GetAuthorStartEndNum(file, start, end);
	return end;
}

//ファイル名にあるサークル名/作者を取得
CString CDoujinFileRename::GetAuthor(const CString &file)
{
	CString result = file;
	do{

		int start = -1, end = -1;
		if( !CDoujinFileRename::GetAuthorStartEndNum(file, start, end) ){
			result.Empty();
			break;
		}

		result.Delete(end, result.GetLength() - end + 1);
		result.Delete(0, start + 1);
	} while( false );

	return result;
}

//ファイル名にある元ネタタイトルを取得
CString CDoujinFileRename::GetOriginalTitle(const CString &file)
{
	CString result = file;
	do{

		int start = -1, end = -1;
		if( !CDoujinFileRename::GetOriginalTitleStartEndNum(file, start, end) ){
			result.Empty();
			break;
		}

		result.Delete(end, result.GetLength() - end + 1);
		result.Delete(0, start + 1);
	} while( false );

	return result;
}


//static関連
//ファイル名にある元ネタタイトルの開始/終了番号を取得
bool CDoujinFileRename::GetOriginalTitleStartEndNum(const CString &file, int &start, int &end)
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
void CDoujinFileRename::DeleteHeadParenthesesInfo(CString &file)
{
	file.TrimLeft();
	while( CDoujinFileRename::GetAuthorStartNum(file) > 1 ){
		const int start = file.Find('(');
		if( start == -1 ) break;
		const int end = file.Find(')', start);
		if( end == -1 ) break;

		file.Delete(start, end - start + 1);
		file.TrimLeft();
	}
}

//	末尾の(DL版)を削除する
void CDoujinFileRename::DeleteTailParenthesesInfo_DLEDITION(CString &file)
{
	//DL版
	const CString dl_edition[] = {
		"(DL版)"
		, "[DL版]"
		, ""
	};
	CDoujinFileRename::DeleteTailParenthesesInfo(file, dl_edition);
}


//	末尾の(無修正)、(無修正版)を[無修正]に変更する
void CDoujinFileRename::FixTailParenthesesInfo_UNCENSORED(CString &file)
{
	//無修正
	const CString unsensord[] = {
		"(無修正)"
		, "(無修正版)"
		, ""
	};
	const CString new_unsensord = "[無修正]";
	CDoujinFileRename::FixTailParenthesesInfo(file, unsensord, new_unsensord);
}

//	末尾の()内で囲われた原作タイトルを[]囲いに変更する
//	(よろず)は削除、(オリジナル)はフラグ参照
void CDoujinFileRename::FixTailParenthesesInfo_ORIGINALTITLE(CString &file, bool bOriginalDelete)
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
		CDoujinFileRename::DeleteTailParenthesesInfo(file, title_original);
	}
	else {
		const CString new_title_original = "[オリジナル]";
		CDoujinFileRename::FixTailParenthesesInfo(file, title_original, new_title_original);
	}


	//よろず
	const CString title_yorozu[] = {
		"(よろず)"
		, ""
	};
	CDoujinFileRename::DeleteTailParenthesesInfo(file, title_yorozu);

	const int start = CDoujinFileRename::GetAuthorEndNum(file);
	if( start != -1 ) {
		while( file.Find('(', start) != -1 ){
			const int search_s = file.Find('(', start);
			const int search_e = file.Find(')', search_s);
			if( search_e == -1 ) break;
			if( search_e <= search_s ) break;

			file.Delete(search_s, 1);
			file.Insert(search_s, '[');
			file.Delete(search_e, 1);
			file.Insert(search_e, ']');
		}
	}
}

//ファイル名からtblで指定された文字列をreplaceに置換
void CDoujinFileRename::FixTailParenthesesInfo(CString &file, const CString tbl[], const CString &replace)
{
	for( int lpcnt = 0; tbl[lpcnt].IsEmpty() == FALSE; lpcnt++ ){
		const int start = CDoujinFileRename::GetAuthorEndNum(file) + 1;
		const int search = file.Find(tbl[lpcnt], start);
		if( search == -1 ) continue;

		file.Delete(search, tbl[lpcnt].GetLength());
		file.Insert(search, replace);
	}
}

//ファイル名からtblで指定された文字列を削除
void CDoujinFileRename::DeleteTailParenthesesInfo(CString &file, const CString tbl[])
{
	for( int lpcnt = 0; tbl[lpcnt].IsEmpty() == FALSE; lpcnt++ ){
		const int start = CDoujinFileRename::GetAuthorEndNum(file) + 1;
		const int search = file.Find(tbl[lpcnt], start);
		if( search == -1 ) continue;
		file.Delete(search, tbl[lpcnt].GetLength());
	}
}

//ファイル名から末尾のスペース文字を消す
void CDoujinFileRename::DeleteTailSpace(CString &result)
{
	result.Trim();
}


//ファイル名を変更する
CString CDoujinFileRename::Rename(const CString &file) const
{
	CString result = file;

	if( m_deleteHeadParenthesesInfo_Flg ) DeleteHeadParenthesesInfo(result);
	if( m_deleteTailParenthesesInfo_DLEDITION_Flg ) DeleteTailParenthesesInfo_DLEDITION(result);
	if( m_deleteTailSpace_Flg ) DeleteTailSpace(result);
	if( m_fixTailParenthesesInfo_UNCENSORED_Flg ) FixTailParenthesesInfo_UNCENSORED(result);
	if( m_fixTailParenthesesInfo_ORIGINALTITLE_Flg ) FixTailParenthesesInfo_ORIGINALTITLE(result, m_fixTailParenthesesInfo_ORIGINAL_MODE == 0);

	return result;
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

//public関連
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
	m_targetFileList.RemoveAll();
	m_resultFileNameList.RemoveAll();

	m_deleteHeadParenthesesInfo_Flg = true;
	m_deleteTailParenthesesInfo_DLEDITION_Flg = true;
	m_fixTailParenthesesInfo_ORIGINALTITLE_Flg = true;
	m_fixTailParenthesesInfo_UNCENSORED_Flg = true;
	m_deleteTailSpace_Flg = true;
	m_addTailParentheseInfo_ORIGINAL_Flg = true;

	m_fixTailParenthesesInfo_ORIGINAL_MODE = 1;
}

//該当ファイルをリストに追加
void CDoujinFileRename::AddFileName(const CString &targetFile)
{
	m_targetFileList.AddTail(targetFile);
}

//対象ファイルリストを取得する
//	bfullpath	:	フルパス or ファイル名称のみ
void CDoujinFileRename::GetFileStringList(CStringList &resultList, bool bfullpath) const
{
	resultList.RemoveAll();
	POSITION pos = m_targetFileList.GetHeadPosition();
	while( pos ){
		const CString str = m_targetFileList.GetNext(pos);

		if( bfullpath ) resultList.AddTail(str);
		else {
			CString drive, path, file, ext;
			CDoujinFileRename::splitpath(str, drive, path, file, ext);
			resultList.AddTail(file);
		}
	}
}

//変更後のファイルリストを取得する
//	bfullpath	:	フルパス or ファイル名称のみ
void CDoujinFileRename::GetRenameFileStringList(CStringList &resultList, bool bfullpath) const
{
	resultList.RemoveAll();
	POSITION pos = m_resultFileNameList.GetHeadPosition();
	while( pos ){
		const CString str = m_resultFileNameList.GetNext(pos);

		if( bfullpath ) resultList.AddTail(str);
		else {
			CString drive, path, file, ext;
			CDoujinFileRename::splitpath(str, drive, path, file, ext);
			resultList.AddTail(file);
		}
	}
}

//ファイル名
//	n	:	リストの番号
//	bResult	:	変換後	or 対象ファイル
CString CDoujinFileRename::GetFileName(int n, bool bResult, bool bFullPath) const
{
	CString result = "";
	const CStringList &list = bResult ? m_targetFileList : m_resultFileNameList;

	do {
		const int count = list.GetCount();
		if( n < 0 || (count - 1) < n ) break;

		POSITION pos = list.FindIndex(n);
		if( pos == NULL ) break;

		const CString full = list.GetAt(pos);
		result = full;
		if( !bFullPath ){
			CString drive, path, file, ext;
			splitpath(full, drive, path, result, ext);
		}
	} while( false );

	return result;
}

//	n	:	リストの番号
//	bResult	:	変換後	or 対象ファイル
void CDoujinFileRename::SetFileName(int n, const CString &file)
{
	CStringList &list = m_resultFileNameList;
	do {
		const int count = list.GetCount();
		if( n < 0 || (count - 1) < n ) break;

		const POSITION pos = list.FindIndex(n);
		if( pos == NULL ) break;
		const CString fullPath = list.GetAt(pos);

		CString drive, path, dumy, ext;
		splitpath(fullPath, drive, path, dumy, ext);
		CString newFullPath;
		joinpath(newFullPath, drive, path, file, ext);

		if( newFullPath.IsEmpty() == TRUE ) break;

		list.SetAt(pos, newFullPath);
	} while( false );
}

//ファイルからサークル名/作者を取得する
//	n	:	リストの番号
//	bResult	:	変換後	or 対象ファイル
CString CDoujinFileRename::GetAuthor(int n, bool bResult) const
{
	CString result = "";
	const CStringList &list = bResult ? m_targetFileList : m_resultFileNameList;

	do {
		const int count = list.GetCount();
		if( n < 0 || (count - 1) < n ) break;

		POSITION pos = list.FindIndex(n);
		if( pos == NULL ) break;

		result = CDoujinFileRename::GetAuthor(list.GetAt(pos));
	} while( false );

	return result;
}

//ファイルから元ネタタイトルを取得する
//	n	:	リストの番号
//	bResult	:	変換後	or 対象ファイル
CString CDoujinFileRename::GetOriginalTitle(int n, bool bResult) const
{
	CString result = "";
	const CStringList &list = bResult ? m_targetFileList : m_resultFileNameList;

	do {
		const int count = list.GetCount();
		if( n < 0 || (count - 1) < n ) break;

		POSITION pos = list.FindIndex(n);
		if( pos == NULL ) break;

		result = CDoujinFileRename::GetOriginalTitle(list.GetAt(pos));
	} while( false );

	return result;
}

//	対象ファイルを変換し、結果をm_resultFileNameListに格納する
void CDoujinFileRename::SetResultName()
{
	m_resultFileNameList.RemoveAll();

	CStringList checkStringList;

	POSITION pos = m_targetFileList.GetHeadPosition();
	while( pos ){
		const CString str = m_targetFileList.GetNext(pos);

		CString drive, path, file, ext;
		CDoujinFileRename::splitpath(str, drive, path, file, ext);

		CString rename = Rename(file);

		int count = 0;
		POSITION search = checkStringList.GetHeadPosition();
		while( search ){
			if( checkStringList.GetNext(search).Find(rename) == -1 ) continue;
			count++;
		}
		if( count != 0 ) rename.AppendFormat("(%d)", count);
		checkStringList.AddTail(rename);

		CString result;
		joinpath(result, drive, path, rename, ext);

		m_resultFileNameList.AddTail(result);
	}
}

void CDoujinFileRename::CopyToResultName()
{
	m_resultFileNameList.RemoveAll();
	POSITION pos = m_targetFileList.GetHeadPosition();
	while( pos ) m_resultFileNameList.AddTail(m_targetFileList.GetNext(pos));
}

void CDoujinFileRename::RemoveFile(const int sel)
{
	POSITION pos1 = m_targetFileList.FindIndex(sel);
	POSITION pos2 = m_resultFileNameList.FindIndex(sel);
	if( pos1 != NULL && pos2 != NULL ){
		m_targetFileList.RemoveAt(pos1);
		m_resultFileNameList.RemoveAt(pos2);
	}
}
