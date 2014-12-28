#include "stdafx.h"
#include "Editor.h"

#include "OgreXMLConverter.h"
#include "OgreXMLMeshSerializer.h"
#include "OgreXMLSkeletonSerializer.h"
#include "StringUtils.h"

#include "OgreMeshManager.h"
#include "OgreMeshSerializer.h"
#include "OgreSkeletonManager.h"
#include "OgreSkeletonSerializer.h"
#include "OgreXMLSkeletonSerializer.h"

IMPLEMENT_DYNAMIC(OgreXMLConverter, CBCGPDialog)

OgreXMLConverter::OgreXMLConverter(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(OgreXMLConverter::IDD, pParent)
{

}

OgreXMLConverter::~OgreXMLConverter()
{
}

void OgreXMLConverter::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT_PATH, inputPath);
	DDX_Text(pDX, IDC_ARGS, args);
}

BOOL OgreXMLConverter::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	EnableVisualManagerStyle();
	return TRUE;
}

BEGIN_MESSAGE_MAP(OgreXMLConverter, CBCGPDialog)
	ON_BN_CLICKED(IDC_GET_INPUT_PATH, &OgreXMLConverter::OnBnClickedGetInputPath)
	ON_BN_CLICKED(IDC_GET_OUTPUT_PATH, &OgreXMLConverter::OnBnClickedGetOutputPath)
	ON_BN_CLICKED(IDC_XML_TO_BINARY, &OgreXMLConverter::OnBnClickedXmlToBinary)
	ON_BN_CLICKED(IDC_BINARY_TO_XML, &OgreXMLConverter::OnBnClickedBinaryToXml)
END_MESSAGE_MAP()

void OgreXMLConverter::OnBnClickedGetInputPath()  
{  
    CFileDialog dlg(TRUE);  
    if(dlg.DoModal() == IDOK)
	{
        inputPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void OgreXMLConverter::OnBnClickedGetOutputPath()
{
	/*CFileDialog dlg(FALSE);
	if(dlg.DoModal() == IDOK)
	{
		outputPath = dlg.GetPathName();
		UpdateData(FALSE);
	}*/
}

void OgreXMLConverter::OnBnClickedBinaryToXml()  
{  
    if(inputPath.IsEmpty())  
        return;  
	std::string extension = StringUtils::extension(std::string(inputPath));  
    if(extension == "xml")  
    {  
        AfxMessageBox("输入文件并非Mesh或Skeleton！");  
        return;  
    }  
  
	outputPath = (inputPath + ".xml");  
    std::ifstream ifs; ifs.open(inputPath, std::ios_base::in | std::ios_base::binary);  
    Ogre::DataStreamPtr stream(new Ogre::FileStreamDataStream(Ogre::String(inputPath), &ifs, false));  
  
    if(extension == "mesh")  
    {  
        Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().create("conversion",   
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);  
        Ogre::MeshSerializer MeshSerializer;  
        MeshSerializer.importMesh(stream, mesh.getPointer());  
        Ogre::XMLMeshSerializer XMLMeshSerializer;  
        XMLMeshSerializer.exportMesh(mesh.getPointer(), Ogre::String(outputPath));

		UpdateData(FALSE);
        AfxMessageBox("Mesh -> XML 成功！");  
    } else  
    if(extension == "skeleton")  
    {  
        Ogre::SkeletonPtr skel = Ogre::SkeletonManager::getSingleton().create("conversion",   
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);  
        Ogre::SkeletonSerializer SkeletonSerializer;  
        SkeletonSerializer.importSkeleton(stream, skel.getPointer());  
        Ogre::XMLSkeletonSerializer XMLSkeletonSerializer;  
        XMLSkeletonSerializer.exportSkeleton(skel.getPointer(), Ogre::String(outputPath));

		UpdateData(FALSE);
        AfxMessageBox("Skeleton -> XML 成功！");  
    }  
    else  
    {  
        AfxMessageBox("输入文件并非Mesh或Skeleton！");  
    }  
}  
  
void OgreXMLConverter::OnBnClickedXmlToBinary()  
{  
	if(inputPath.IsEmpty())  
        return;  
	std::string extension = StringUtils::extension(std::string(inputPath));  
    if(extension != "xml")  
    {  
        AfxMessageBox("输入文件并非Mesh或Skeleton的XML！");  
        return;  
    }  
  
	outputPath = StringUtils::filename(std::string(inputPath)).c_str();  
    TiXmlDocument* doc = new TiXmlDocument(inputPath); doc->LoadFile();  
    TiXmlElement* root = doc->RootElement();  
  
    if (!stricmp(root->Value(), "mesh"))  
    {  
        delete doc;  
        Ogre::MeshPtr newMesh = Ogre::MeshManager::getSingleton().createManual("conversion",   
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);  
        Ogre::VertexElementType colourElementType = Ogre::VET_COLOUR_ARGB;  
        Ogre::XMLMeshSerializer XMLMeshSerializer;  
        XMLMeshSerializer.importMesh(Ogre::String(inputPath), colourElementType, newMesh.getPointer());  
        newMesh->_determineAnimationTypes();  
        Ogre::MeshSerializer MeshSerializer;  
        MeshSerializer.exportMesh(newMesh.getPointer(), Ogre::String(outputPath));  
        Ogre::MeshManager::getSingleton().remove("conversion");  

		UpdateData(FALSE);
        AfxMessageBox("Mesh XML -> Mesh 成功！");  
    }  
    else if (!stricmp(root->Value(), "skeleton"))  
    {  
        delete doc;  
        Ogre::SkeletonPtr newSkel = Ogre::SkeletonManager::getSingleton().create("conversion",   
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);  
        Ogre::XMLSkeletonSerializer XMLSkeletonSerializer;  
        XMLSkeletonSerializer.importSkeleton(Ogre::String(inputPath), newSkel.getPointer());  
        Ogre::SkeletonSerializer SkeletonSerializer;  
        SkeletonSerializer.exportSkeleton(newSkel.getPointer(), Ogre::String(outputPath), Ogre::SKELETON_VERSION_LATEST);  
        Ogre::SkeletonManager::getSingleton().remove("conversion");

		UpdateData(FALSE);
        AfxMessageBox("Skeleton XML -> Skeleton 成功！");  
    }  
    else  
    {  
        delete doc;  
        AfxMessageBox("输入文件并非Mesh或Skeleton的XML！");  
    }  
}
