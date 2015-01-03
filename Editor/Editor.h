#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"
#include <map>
#include <string>

struct IDirect3DDevice9;

class CEditorApp : public CBCGPWinApp
{
public:
	CEditorApp();

	BOOL InitInstance();
	int ExitInstance();

	CMultiDocTemplate *getDocTemplate(std::string name);
	void addDocTemplate(std::string name, CMultiDocTemplate *docTemplate);

	std::string getPath(std::string name);
	void addPath(std::string name, std::string path);

	IDirect3DDevice9 *getDevice();
	void setDevice(IDirect3DDevice9 *device);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnNewScene();
	afx_msg void OnOpenScene();
	afx_msg void OnNewGame();
	afx_msg void OnOgreXmlConverter();
	afx_msg void OnScriptEditor();
	afx_msg void OnClearLog();
	afx_msg void OnAppAbout();
	afx_msg void OnUpdateTextureMenu(CCmdUI* pCmdUI);

	std::map<std::string, CMultiDocTemplate*> docTemplates;
	std::map<std::string, std::string> paths;
	IDirect3DDevice9 *device;

	afx_msg void OnImageCombinator();
	afx_msg void OnDDSConverter();
	afx_msg void OnDetailLow();
	afx_msg void OnDetailNormal();
	afx_msg void OnDetailBoreme();
	afx_msg void OnUpdateDetailLow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDetailNormal(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDetailBoreme(CCmdUI *pCmdUI);
};

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	DECLARE_MESSAGE_MAP()
	void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();
};

extern CEditorApp theApp;
