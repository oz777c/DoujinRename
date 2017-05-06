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
//static�֘A
//�t�@�C�����ɂ���T�[�N����/��҂̊J�n/�I���ԍ����擾
bool CDoujinFileRename::CFileName::GetAuthorStartEndNum(const CString &file, int &start, int &end)
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
int CDoujinFileRename::CFileName::GetAuthorStartNum(const CString &file)
{
	int start = -1, end = -1;
	CDoujinFileRename::CFileName::GetAuthorStartEndNum(file, start, end);
	return start;
}

//�t�@�C�����ɂ���T�[�N����/��҂̏I���ԍ����擾
int CDoujinFileRename::CFileName::GetAuthorEndNum(const CString &file)
{
	int start = -1, end = -1;
	CDoujinFileRename::CFileName::GetAuthorStartEndNum(file, start, end);
	return end;
}

//�t�@�C�����ɂ���T�[�N����/��҂��擾
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

//�t�@�C�����ɂ��錳�l�^�^�C�g�����擾
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

//�t�@�C�����ɂ��錳�l�^�^�C�g���̊J�n/�I���ԍ����擾
bool CDoujinFileRename::CFileName::GetOriginalTitleStartEndNum(const CString &file, int &start, int &end)
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

//	������(DL��)���폜����
void CDoujinFileRename::CFileName::DeleteTailParenthesesInfo_DLEDITION(CString &file)
{
	//DL��
	const CString dl_edition[] = {
		"(DL��)"
		, "[DL��]"
		, ""
	};
	CDoujinFileRename::CFileName::DeleteTailParenthesesInfo(file, dl_edition);
}


//	������(���C��)�A(���C����)��[���C��]�ɕύX����
void CDoujinFileRename::CFileName::FixTailParenthesesInfo_UNCENSORED(CString &file)
{
	//���C��
	const CString unsensord[] = {
		"(���C��)"
		, "(���C����)"
		, ""
	};
	const CString new_unsensord = "[���C��]";
	CDoujinFileRename::CFileName::FixTailParenthesesInfo(file, unsensord, new_unsensord);
}

//	������()���ň͂�ꂽ����^�C�g����[]�͂��ɕύX����
//	(��낸)�͍폜�A(�I���W�i��)�̓t���O�Q��
void CDoujinFileRename::CFileName::FixTailParenthesesInfo_ORIGINALTITLE(CString &file, bool bOriginalDelete)
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
		CDoujinFileRename::CFileName::DeleteTailParenthesesInfo(file, title_original);
	}
	else {
		const CString new_title_original = "[�I���W�i��]";
		CDoujinFileRename::CFileName::FixTailParenthesesInfo(file, title_original, new_title_original);
	}


	//��낸
	const CString title_yorozu[] = {
		"(��낸)"
		, ""
	};
	CDoujinFileRename::CFileName::DeleteTailParenthesesInfo(file, title_yorozu);

	const int start = CDoujinFileRename::CFileName::GetAuthorEndNum(file);
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

//�t�@�C��������tbl�Ŏw�肳�ꂽ��������폜
void CDoujinFileRename::CFileName::DeleteTailParenthesesInfo(CString &file, const CString tbl[])
{
	for( int lpcnt = 0; tbl[lpcnt].IsEmpty() == FALSE; lpcnt++ ){
		const int start = CDoujinFileRename::CFileName::GetAuthorEndNum(file) + 1;
		const int search = file.Find(tbl[lpcnt], start);
		if( search == -1 ) continue;
		file.Delete(search, tbl[lpcnt].GetLength());
	}
}

//�t�@�C�������疖���̃X�y�[�X����������
void CDoujinFileRename::CFileName::DeleteTailSpace(CString &result)
{
	result.Trim();
}


//�t�@�C������ύX����
CString CDoujinFileRename::CFileName::GetRename(const CDoujinFileRename &info) const
{
	CString result = m_targetFileName;

	if( info.isDeleteHeadParenthesesInfo() ) DeleteHeadParenthesesInfo(result);
	if( info.isDeleteTailParenthesesInfo_DLEDITION() ) DeleteTailParenthesesInfo_DLEDITION(result);
	if( info.isDeleteTailSpace() ) DeleteTailSpace(result);
	if( info.isFixTailParenthesesInfo_UNCENSORED() ) FixTailParenthesesInfo_UNCENSORED(result);
	if( info.isFixTailParenthesesInfo_ORIGINALTITLE() ) FixTailParenthesesInfo_ORIGINALTITLE(result, info.GetFixTailParenthesesInfo_ORIGINAL_MODE() == 0);

	return result;
}

//�O���C���^�[�t�F�[�X���őI�����ꂽ�����񂪁A�Y���f�[�^�̑Ώۂ��ǂ���
bool CDoujinFileRename::CFileName::isSelectFile(const CString &selectString, bool bResult) const
{
	const CString &string = bResult ? m_resultFileName : m_targetFileName;
	return string == selectString;
}


//
// class CDoujinFileRename //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	m_fileList.RemoveAll();

	m_deleteHeadParenthesesInfo_Flg = true;
	m_deleteTailParenthesesInfo_DLEDITION_Flg = true;
	m_fixTailParenthesesInfo_ORIGINALTITLE_Flg = true;
	m_fixTailParenthesesInfo_UNCENSORED_Flg = true;
	m_deleteTailSpace_Flg = true;
	m_addTailParentheseInfo_ORIGINAL_Flg = true;

	m_fixTailParenthesesInfo_ORIGINAL_MODE = 1;
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
//�Y���t�@�C�������X�g�ɒǉ�
void CDoujinFileRename::AddFileName(const CString &targetFile)
{
	CString file, ext;
	splitpath(targetFile, m_drive, m_path, file, ext);
	m_fileList.AddTail(CFileName(file, ext));
}

//�t�@�C����
//	n	:	���X�g�̔ԍ�
//	bResult	:	�ϊ���	or �Ώۃt�@�C��
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

//�t�@�C������T�[�N����/��҂��擾����
//	n	:	���X�g�̔ԍ�
//	bResult	:	�ϊ���	or �Ώۃt�@�C��
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

//�t�@�C�����猳�l�^�^�C�g�����擾����
//	n	:	���X�g�̔ԍ�
//	bResult	:	�ϊ���	or �Ώۃt�@�C��
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

//	�Ώۃt�@�C����ϊ����A���ʂ�m_resultFileNameList�Ɋi�[����
void CDoujinFileRename::SetResultName()
{
	CStringList checkStringList;

	POSITION pos = m_fileList.GetHeadPosition();
	while( pos ){
		CFileName &fileName = m_fileList.GetNext(pos);
		if( !fileName.isRename() ) continue;
		CString rename = fileName.GetRename(*this);

		if( fileName.GetFileName(false) != rename ){
			int count = 0;

			CString tempRename = rename;
			while(count < 500 ){
				if( checkStringList.Find(tempRename) == NULL ) break;
				count++;
				tempRename.Format("%s(%d)", rename, count);
			}
			rename = tempRename;
		}

		checkStringList.AddTail(rename);
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
