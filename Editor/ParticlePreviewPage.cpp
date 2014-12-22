#include "stdafx.h"
#include "Editor.h"
#include "ParticlePreviewPage.h"

#include "RenderView.h"
#include "StringUtils.h"

#include "OgreCamera.h"
#include "OgreParticleSystem.h"
#include "OgreParticleSystemManager.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"

IMPLEMENT_DYNAMIC(ParticlePreviewPage, CBCGPDialog)

ParticlePreviewPage *ParticlePreviewPage::current = NULL;
ParticlePreviewPage::ParticlePreviewPage(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(ParticlePreviewPage::IDD, pParent)
{
	current = this;
	particleSystem = NULL;
}

ParticlePreviewPage::~ParticlePreviewPage()
{
}

void ParticlePreviewPage::update(float Elapsed)
{
	view->Invalidate(FALSE);
}

BEGIN_MESSAGE_MAP(ParticlePreviewPage, CBCGPDialog)
	ON_BN_CLICKED(IDC_EDIT_PARTICLE, &ParticlePreviewPage::OnBnClickedEditParticle)
	ON_BN_CLICKED(IDC_APPLY_PARTICLE, &ParticlePreviewPage::OnBnClickedApplyParticle)
	ON_BN_CLICKED(IDC_CLEAR_PARTICLE, &ParticlePreviewPage::OnBnClickedClearParticle)
END_MESSAGE_MAP()

void ParticlePreviewPage::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARTICLE_FRAME, particleFrame);
	DDX_Control(pDX, IDC_PARTICLE_LIST, particleList);
}

BOOL ParticlePreviewPage::OnInitDialog()
{
	CBCGPDialog::OnInitDialog(); // µ÷DoDataExchange

	sceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);
	camera = sceneManager->createCamera(
		Ogre::StringConverter::toString(Ogre::Math::RangeRandom(0, 1000)) + ".Camera");

	//////////////////////////////////////////////////
	// View
	//////////////////////////////////////////////////

	CRect rectParticle;
	particleFrame.GetClientRect(rectParticle);
	particleFrame.MapWindowPoints(this, &rectParticle);
	camera->setAspectRatio((float)rectParticle.Width() / rectParticle.Height());

	view = (RenderView*)RUNTIME_CLASS(RenderView)->CreateObject();
  	view->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, rectParticle, this, IDC_MATERIAL_FRAME);
	view->_setCamera(camera);

	return TRUE;
}

void ParticlePreviewPage::OnBnClickedEditParticle()
{
	CFileDialog Dlg(TRUE);
	if(Dlg.DoModal() == IDOK)
		theApp.getDocTemplate("Particle")->OpenDocumentFile(Dlg.GetPathName());
}


void ParticlePreviewPage::OnBnClickedApplyParticle()
{
	int currSel = particleList.GetCurSel();
	if(currSel != LB_ERR)
	{
		if(particleSystem)
			OnBnClickedClearParticle();

		CString particle;
		particleList.GetText(currSel, particle);

		std::string internalPath = StringUtils::internalPath(theApp.getPath("Particle"), std::string(currentFile));
		reloadParticle(std::string(particle), "General", internalPath, true);

		particleSystem = sceneManager->createParticleSystem(Ogre::String(particle), Ogre::String(particle));
		sceneManager->getRootSceneNode()->attachObject(particleSystem);
	}
}


void ParticlePreviewPage::OnBnClickedClearParticle()
{
	if(particleSystem)
	{
		sceneManager->destroyParticleSystem(particleSystem);
		particleSystem = NULL;
	}
}

void ParticlePreviewPage::reloadParticle(const std::string& particleName, const std::string &groupName, const std::string& filename, bool parseCompositorScript)
{
    unloadParticles(filename);

	if (parseCompositorScript)
    {
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
		Ogre::ParticleSystemManager::getSingleton().parseScript(stream, groupName);
	}
}

void ParticlePreviewPage::unloadParticles(const std::string& filename)
{
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
    if(!stream.isNull())
    {
        while(!stream->eof())
        {
            std::string line = stream->getLine();
            Ogre::StringUtil::trim(line);
 
            ///
            /// UNLOAD PARTICLES
            ///
            if (Ogre::StringUtil::startsWith(line, "particle_system "))
            {
				Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line, "\t\r\n: "); // ":"±í¼Ì³Ð
				bool skipFirst = true, canPrint = true;
				std::string match, _match;
				for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it)
				{
					if (skipFirst)
					{
						skipFirst = false;
						continue;
					}

					std::string match = (*it);

					if(match.find_first_of('\"') != std::string::npos)
					{
						if(canPrint)
						{
							canPrint = false;
						}
						else
						{
							canPrint = true;
							match = _match + match;
						}
					}
				
					if(canPrint)
					{
						Ogre::StringUtil::trim(match);
						match.erase(0, match.find_first_not_of('\"'));  
						match.erase(match.find_last_not_of('\"')+1);

						if(!match.empty())
						{
							Ogre::ParticleSystemManager::getSingleton().removeTemplate(match);
							break;
						}
					}
					else
					{
						_match += (match + " ");
					}
				}
            }
        }
    }
    stream->close();
}
