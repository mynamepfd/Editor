#pragma once
#include "afxwin.h"


// DDSConverter 对话框

class DDSConverter : public CDialogEx
{
	DECLARE_DYNAMIC(DDSConverter)

public:
	DDSConverter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DDSConverter();

// 对话框数据
	enum { IDD = IDD_DDSCONVERTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString inputPath;
	CString outputPath;
	afx_msg void OnBnClickedLoadInput();
	afx_msg void OnBnClickedSaveOutput();
	afx_msg void OnBnClickedConvert();
};
