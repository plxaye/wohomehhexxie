#include "StdAfx.h"
#include "LocalImageList.h"
#include "ImageBasePropEditDlg.h"
#include "MainFrm.h"
#include "..\..\Inc\UiFeatureDefs.h"

CLocalImageList::CLocalImageList(void)
{
	m_pImgBaseDlg = NULL;
	m_pUiKernel = NULL;
	m_pImgBaseLise = NULL;
	m_ZipFileVec = NULL;
	m_pSelImgBase = NULL;
}

CLocalImageList::~CLocalImageList(void)
{
}

void CLocalImageList::OnSelectItem()
{
	if (m_pImgBaseDlg == NULL)
		return;

	if (m_nSelectItem <= 0)
	{
		// 没有选择
	}
	else
	{
	}
}

void CLocalImageList::Init(IUiFeatureKernel* pUiKernel, CImageBasePropEditDlg *pImgBaseDlg, CImageBaseNameList* pImgBaseLise, IPropertyImageBase* pSelImgBase)
{
	if (pUiKernel == NULL || pImgBaseDlg == NULL || pImgBaseLise == NULL)
		return;

	m_pImgBaseDlg = pImgBaseDlg;
	m_pUiKernel = pUiKernel;
	m_pImgBaseLise = pImgBaseLise;
	m_pSelImgBase = pSelImgBase;

	m_ZipFileVec = NULL;
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if (pMain != NULL)
		m_ZipFileVec = pMain->GetZipFileVec();

	if (m_pSelImgBase == NULL)
		RefreshList(NULL);
	else
		RefreshList(m_pSelImgBase->GetZipFile());
}

void CLocalImageList::RefreshList(ZIP_FILE* pSelZipFile)
{
	USES_CONVERSION;
	this->DeleteAllItems();

	this->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	this->ModifyStyle(0, LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	this->InsertColumn(0, _T("#"), LVCFMT_LEFT, 50);
	this->InsertColumn(1, _T("本地图片名称"), LVCFMT_LEFT, 160);

	if (m_ZipFileVec != NULL)
	{
		int nNo = 0;
		for (ZIP_FILE_VEC::iterator pZipItem = m_ZipFileVec->begin(); pZipItem != m_ZipFileVec->end(); pZipItem++, nNo++)
		{
			ZIP_FILE *pZipFile = *pZipItem;
			if (pZipFile == NULL)
				continue;

			CString strNo(_T(""));
			strNo.Format(_T("%d"), nNo+1);
			this->InsertItem(nNo, strNo);
			this->SetItemText(nNo, 1, A2W(pZipFile->strFileName.c_str()));
			this->SetItemData(nNo, (DWORD_PTR)pZipFile);

			if (pSelZipFile == pZipFile)
				this->SetItemState(nNo, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

bool CLocalImageList::OnLoadLocalImage(CString strFilePath, CString strFileName)
{
	USES_CONVERSION;
	if (m_pUiKernel == NULL || strFileName.GetLength() <= 0 || strFilePath.GetLength() <= 0 || m_ZipFileVec == NULL)
		return false;

	WIN32_FILE_ATTRIBUTE_DATA FileAttr;
	if (!::GetFileAttributesExA(W2A(strFilePath), GetFileExInfoStandard, &FileAttr))
		return false;

	FILE *pFile = NULL;
	fopen_s(&pFile, W2A(strFilePath), "rb");
	if (pFile == NULL)
		return false;

	BYTE *pReadBuf = new BYTE[FileAttr.nFileSizeLow];
	if (pReadBuf == NULL)
	{
		fclose(pFile);
		return false;
	}

	ZIP_FILE *pFileItem = new ZIP_FILE;
	pFileItem->byFileType = ZFT_IMAGE;
	pFileItem->strFileName = W2A(strFileName);
	pFileItem->dwSrcFileLen = FileAttr.nFileSizeLow;
	pFileItem->dwZipDatalen = 0;
	pFileItem->pFileData = new BYTE[FileAttr.nFileSizeLow];
	if (pFileItem->pFileData == NULL)
	{
		SAFE_DELETE(pFileItem);
		fclose(pFile);
		return false;
	}

	int nReadLen = 0;
	while (nReadLen < (int)FileAttr.nFileSizeLow)
	{
		BYTE *pRead = pFileItem->pFileData + nReadLen;
		int nNeedRead = FileAttr.nFileSizeLow - nReadLen;

		int nRead = fread_s(pRead, nNeedRead, 1, nNeedRead, pFile);
		nReadLen += nRead;

		if (errno != 0)
		{
			SAFE_DELETE(pFileItem);
			SAFE_DELETE_LIST(pFileItem->pFileData);
			fclose(pFile);
			return false;
		}
	}

	if (nReadLen != (int)FileAttr.nFileSizeLow)
	{
		SAFE_DELETE(pFileItem);
		SAFE_DELETE_LIST(pFileItem->pFileData);
		fclose(pFile);
		return false;
	}

	m_ZipFileVec->push_back(pFileItem);
	RefreshList(pFileItem);
	return true;
}
