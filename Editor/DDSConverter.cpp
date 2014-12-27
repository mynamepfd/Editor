#include "stdafx.h"
#include "Editor.h"
#include "DDSConverter.h"
#include "StringUtils.h"

#include "d3dx9.h"

IMPLEMENT_DYNAMIC(DDSConverter, CBCGPDialog)

DDSConverter::DDSConverter(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(DDSConverter::IDD, pParent)
{

}

DDSConverter::~DDSConverter()
{
}

void DDSConverter::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, inputPath);
	DDX_Text(pDX, IDC_EDIT2, outputPath);
}

BEGIN_MESSAGE_MAP(DDSConverter, CBCGPDialog)
	ON_BN_CLICKED(IDC_LOAD_INPUT, &DDSConverter::OnBnClickedLoadInput)
	ON_BN_CLICKED(IDC_SAVE_OUTPUT, &DDSConverter::OnBnClickedSaveOutput)
	ON_BN_CLICKED(IDC_CONVERT, &DDSConverter::OnBnClickedConvert)
END_MESSAGE_MAP()

void DDSConverter::OnBnClickedLoadInput()
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		inputPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void DDSConverter::OnBnClickedSaveOutput()
{
	CFileDialog dlg(FALSE);
	if(dlg.DoModal() == IDOK)
	{
		outputPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void DDSConverter::OnBnClickedConvert()
{
	if(inputPath.IsEmpty() || outputPath.IsEmpty())
	{
		AfxMessageBox("需选择输入与输出文件");
		return;
	}

	IDirect3DDevice9 *device = theApp.getDevice(); 
	IDirect3DTexture9 *texture = NULL;

	D3DXCreateTextureFromFile(device, inputPath, &texture);
	D3DXSaveTextureToFile((StringUtils::filename(std::string(inputPath))+".dds").c_str(), D3DXIFF_DDS, texture, NULL);
	texture->Release();
	
	AfxMessageBox("生成成功");
}
