#pragma once

class DDSConverter : public CBCGPDialog
{
	DECLARE_DYNAMIC(DDSConverter)

public:
	DDSConverter(CWnd* pParent = NULL);
	virtual ~DDSConverter();

	enum { IDD = IDD_DDSCONVERTER };

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedLoadInput();
	afx_msg void OnBnClickedSaveOutput();
	afx_msg void OnBnClickedConvert();
	
	void DoDataExchange(CDataExchange* pDX);
	
	CString inputPath;
	CString outputPath;
};
