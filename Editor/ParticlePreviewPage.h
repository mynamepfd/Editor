#pragma once

class RenderView;

namespace Ogre
{
	class SceneManager; class Camera;
	class ParticleSystem;
};

class ParticlePreviewPage : public CBCGPDialog
{
	DECLARE_DYNAMIC(ParticlePreviewPage)
public:
	ParticlePreviewPage(CWnd* pParent = NULL);
	virtual ~ParticlePreviewPage();

	void setCurrentFile(CString currentFile) { this->currentFile = currentFile; }

	CListBox *getParticleList() { return &particleList; }
	void clearParticleList() { particleList.ResetContent(); }

	void update(float Elapsed);

	enum { IDD = IDD_PARTICLE_PREVIEW };

	static ParticlePreviewPage *current;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedEditParticle();
	afx_msg void OnBnClickedApplyParticle();
	afx_msg void OnBnClickedClearParticle();

	void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Modified from MaterialPreviewPage
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void reloadParticle(const std::string& particleName, const std::string &groupName, const std::string& filename, bool parseParticleScript);
	void unloadParticles(const std::string& filename);

	Ogre::SceneManager *sceneManager;
	Ogre::Camera *camera;

	Ogre::ParticleSystem *particleSystem;

	RenderView *view;
	CStatic particleFrame;

	CString currentFile;
	CListBox particleList;
};
