#pragma once
#include "featurebaselist.h"
#include "..\..\Inc\IPropertyImage.h"
#include "..\..\Inc\IPropertyImageBase.h"
#include "..\..\Inc\IUiFeatureKernel.h"
#include "..\..\Inc\IPropertySkinManager.h"

class CImageBasePropEditDlg;
class CLocalImageList;
class CImageBaseNameList : public CFeatureBaseList
{
public:
	CImageBaseNameList(void);
	~CImageBaseNameList(void);

	void OnDeleteImageBaseProp();
	bool OnModifyImageBaseProp(CString strName);
	bool OnCreateImageBaseProp(CString strName);
	IPropertyImageBase * GetSelectImageBase();
	void InitImageBaseShow(IUiFeatureKernel* pUiKernel, IPropertyImage* pParentImgProp);
	void Init(CImageBasePropEditDlg *pImgBaseDlg, CLocalImageList* pLocalImg);

public:
	IPropertyImageBase* m_pSelectImgBaseProp;

protected:
	virtual void OnSelectItem();
	void LoadImageBaseProp();

private:
	IUiFeatureKernel* m_pUiKernel;
	CImageBasePropEditDlg *m_pImgBaseDlg;
	CLocalImageList* m_pLocalImgList;
	IPropertyImage* m_pParentImgProp;
};
