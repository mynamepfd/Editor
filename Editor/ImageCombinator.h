#pragma once

class ImageCombinator : public CDialogEx
{
	DECLARE_DYNAMIC(ImageCombinator)

public:
	ImageCombinator(CWnd* pParent = NULL);
	virtual ~ImageCombinator();

	enum { IDD = IDD_IMAGE_COMBINATOR };

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedLoadDiffuse();
	afx_msg void OnBnClickedLoadSpecular();
	afx_msg void OnBnClickedSaveDiffuseSpecular();
	afx_msg void OnBnClickedLoadNormal();
	afx_msg void OnBnClickedLoadHeight();
	afx_msg void OnBnClickedSaveNormalHeight();
	afx_msg void OnBnClickedGenerate();

	void DoDataExchange(CDataExchange* pDX);

	CString diffusePath;
	CString specularPath;
	CString diffuseSpecularPath;
	CString normalPath;
	CString heightPath;
	CString normalHeightPath;
};
