#include "stdafx.h"
#include "DoujinRename.h"


//CDoujinFileRename


//static�֘A
//�t�@�C�����ɂ���T�[�N����/��҂̊J�n/�I���ԍ����擾
bool CDoujinFileRename::GetAuthorStartEndNum(const CString &file, int &start, int &end)
{
	//start : �ŏ��Ɍ�������'['
	//end : '['�������������']'
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

//�t�@�C�����ɂ���T�[�N����/��҂̊J�n�ԍ����擾
int CDoujinFileRename::GetAuthorStartNum(const CString &file)
{
	int start = -1, end = -1;
	CDoujinFileRename::GetAuthorStartEndNum(file, start, end);
	return start;
}

//�t�@�C�����ɂ���T�[�N����/��҂̏I���ԍ����擾
int CDoujinFileRename::GetAuthorEndNum(const CString &file)
{
	int start = -1, end = -1;
	CDoujinFileRename::GetAuthorStartEndNum(file, start, end);
	return end;
}

//�t�@�C�����ɂ���T�[�N����/��҂��擾
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

//�t�@�C�����ɂ��錳�l�^�^�C�g�����擾
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


//static�֘A
//�t�@�C�����ɂ��錳�l�^�^�C�g���̊J�n/�I���ԍ����擾
bool CDoujinFileRename::GetOriginalTitleStartEndNum(const CString &file, int &start, int &end)
{
	//�����񖖔����猟������
	//start : �ŏ��Ɍ�������']'
	//end : ']'�������������'['
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

//	�擪��()�Ɉ͂܂ꂽ���ڂ��폜����
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

//	������(DL��)���폜����
void CDoujinFileRename::DeleteTailParenthesesInfo_DLEDITION(CString &file)
{
	//DL��
	const CString dl_edition[] = {
		"(DL��)"
		, "[DL��]"
		, ""
	};
	CDoujinFileRename::DeleteTailParenthesesInfo(file, dl_edition);
}


//	������(���C��)�A(���C����)��[���C��]�ɕύX����
void CDoujinFileRename::FixTailParenthesesInfo_UNCENSORED(CString &file)
{
	//���C��
	const CString unsensord[] = {
		"(���C��)"
		, "(���C����)"
		, ""
	};
	const CString new_unsensord = "[���C��]";
	CDoujinFileRename::FixTailParenthesesInfo(file, unsensord, new_unsensord);
}

//	������()���ň͂�ꂽ����^�C�g����[]�͂��ɕύX����
//	(��낸)�͍폜�A(�I���W�i��)�̓t���O�Q��
void CDoujinFileRename::FixTailParenthesesInfo_ORIGINALTITLE(CString &file, bool bOriginalDelete)
{
	//�I���W�i��
	const CString title_original[] = {
		"(�I���W�i��)"
		, "(original)"
		, "[�I���W�i��]"
		, "[original]"
		, ""
	};
	if( bOriginalDelete ){
		CDoujinFileRename::DeleteTailParenthesesInfo(file, title_original);
	}
	else {
		const CString new_title_original = "[�I���W�i��]";
		CDoujinFileRename::FixTailParenthesesInfo(file, title_original, new_title_original);
	}


	//��낸
	const CString title_yorozu[] = {
		"(��낸)"
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

//�t�@�C��������tbl�Ŏw�肳�ꂽ�������replace�ɒu��
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

//�t�@�C��������tbl�Ŏw�肳�ꂽ��������폜
void CDoujinFileRename::DeleteTailParenthesesInfo(CString &file, const CString tbl[])
{
	for( int lpcnt = 0; tbl[lpcnt].IsEmpty() == FALSE; lpcnt++ ){
		const int start = CDoujinFileRename::GetAuthorEndNum(file) + 1;
		const int search = file.Find(tbl[lpcnt], start);
		if( search == -1 ) continue;
		file.Delete(search, tbl[lpcnt].GetLength());
	}
}

//�t�@�C�������疖���̃X�y�[�X����������
void CDoujinFileRename::DeleteTailSpace(CString &result)
{
	result.Trim();
}


//�t�@�C������ύX����
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

//�t���p�X����h���C�u�A�p�X�A�t�@�C�����A�g���q���擾����
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

//�h���C�u�A�p�X�A�t�@�C�����A�g���q����t���p�X���쐬����
void CDoujinFileRename::joinpath(CString &fullPath, const CString &drive, const CString &path, const CString &file, const CString &ext)
{
	CString renameFileStr;
	fullPath = drive + path + file + ext;
}

//public�֘A
//�R���X�g���N�^
CDoujinFileRename::CDoujinFileRename()
{
	Initialize();
}

//�f�X�g���N�^
CDoujinFileRename::~CDoujinFileRename()
{
	Initialize();
}

//������
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

//�Y���t�@�C�������X�g�ɒǉ�
void CDoujinFileRename::AddFileName(const CString &targetFile)
{
	m_targetFileList.AddTail(targetFile);
}

//�Ώۃt�@�C�����X�g���擾����
//	bfullpath	:	�t���p�X or �t�@�C�����̂̂�
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

//�ύX��̃t�@�C�����X�g���擾����
//	bfullpath	:	�t���p�X or �t�@�C�����̂̂�
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

//�t�@�C����
//	n	:	���X�g�̔ԍ�
//	bResult	:	�ϊ���	or �Ώۃt�@�C��
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

//	n	:	���X�g�̔ԍ�
//	bResult	:	�ϊ���	or �Ώۃt�@�C��
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

//�t�@�C������T�[�N����/��҂��擾����
//	n	:	���X�g�̔ԍ�
//	bResult	:	�ϊ���	or �Ώۃt�@�C��
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

//�t�@�C�����猳�l�^�^�C�g�����擾����
//	n	:	���X�g�̔ԍ�
//	bResult	:	�ϊ���	or �Ώۃt�@�C��
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

//	�Ώۃt�@�C����ϊ����A���ʂ�m_resultFileNameList�Ɋi�[����
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
