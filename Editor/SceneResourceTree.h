#pragma once

#include "PropertyWnd.h"

class SceneObject;

namespace Ogre
{
	class SceneManager;
}

class SceneResourceTree : public CTreeCtrl, public PropertyListener
{
	DECLARE_DYNAMIC(SceneResourceTree)
public:
	SceneResourceTree();
	virtual ~SceneResourceTree();

	HTREEITEM getTreeItem(int type) { return treeItems[type]; }

	void firePropertyChanged();
	void onPropertyChanged(CBCGPProp *prop);

	void resetImageList();
	void rebuildTree();

	enum
	{
		SCENE,
		SKY,
		TERRAIN_SET,
		TERRAIN,
		LIGHT_SET,
		LIGHT,
		MODEL_SET,
		MODEL,
		MESH_SET,
		MESH,
		LIQUID_SET,
		LIQUID,
		PARTICLE_SET,
		PARTICLE,
		_COUNT
	};

	static SceneResourceTree *current;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);

	void afterSelectTreeItem(HTREEITEM treeItem);
	void onSelectScene();
	void onSelectSky();
	void onSelectTerrainSet();
	void onSelectTerrain(SceneObject *sceneObject);
	void onSelectLightSet();
	void onSelectLight(SceneObject *sceneObject);
	void onSelectModelSet();
	void onSelectModel(SceneObject *sceneObject);
	void onSelectMeshSet();
	void onSelectMesh(SceneObject *sceneObject);
	void onSelectLiquidSet();
	void onSelectLiquid(SceneObject *sceneObject);
	void onSelectParticleSet();
	void onSelectParticle(SceneObject *sceneObject);

	void onUpdateScene(CBCGPProp *prop);
	void onUpdateSky(CBCGPProp *prop);
	void onUpdateTerrainSet(CBCGPProp *prop);
	void onUpdateTerrain(CBCGPProp *prop, SceneObject *sceneObject);
	void onUpdateLightSet(CBCGPProp *prop);
	void onUpdateLight(CBCGPProp *prop, SceneObject *sceneObject);
	void onUpdateModelSet(CBCGPProp *prop);
	void onUpdateModel(CBCGPProp *prop, SceneObject *sceneObject);
	void onUpdateMeshSet(CBCGPProp *prop);
	void onUpdateMesh(CBCGPProp *prop, SceneObject *sceneObject);
	void onUpdateLiquidSet(CBCGPProp *prop);
	void onUpdateLiquid(CBCGPProp *prop, SceneObject *sceneObject);
	void onUpdateParticleSet(CBCGPProp *prop);
	void onUpdateParticle(CBCGPProp *prop, SceneObject *sceneObject);

	CImageList imageList;
	HTREEITEM treeItems[_COUNT];
};
