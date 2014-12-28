#pragma once

class OgreXMLConverter : public CBCGPDialog
{
	DECLARE_DYNAMIC(OgreXMLConverter)

public:
	OgreXMLConverter(CWnd* pParent = NULL);
	virtual ~OgreXMLConverter();

	enum { IDD = IDD_OGRE_XML_CONVERTER };

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedGetInputPath();
	afx_msg void OnBnClickedGetOutputPath();
	afx_msg void OnBnClickedXmlToBinary();
	afx_msg void OnBnClickedBinaryToXml();

	void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();

	CString inputPath;
	CString outputPath;
	CString args;
};
