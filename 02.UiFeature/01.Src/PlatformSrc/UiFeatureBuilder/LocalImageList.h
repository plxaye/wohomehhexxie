#pragma once
#include "featurebaselist.h"
#include "..\..\Inc\IUiFeatureKernel.h"

typedef vector<ZIP_FILE*>		ZIP_FILE_VEC;

class CImageBasePropEditDlg;
class CImageBaseNameList;
class CLocalImageList : public CFeatureBaseList
{
public:
	CLocalImageList(void);
	~CLocalImageList(void);

	void Init(IUiFeatureKernel* pUiKernel, CImageBasePropEditDlg *pImgBaseDlg, CImageBaseNameList* pImgBaseLise);
	bool OnLoadLocalImage(CString strFilePath, CString strFileName);

protected:
	virtual void OnSelectItem();

private:
	CImageBasePropEditDlg *m_pImgBaseDlg;
	IUiFeatureKernel* m_pUiKernel;
	CImageBaseNameList* m_pImgBaseLise;
	ZIP_FILE_VEC* m_ZipFileVec;
};
