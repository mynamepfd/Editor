#include "stdafx.h"
#include "Editor.h"
#include "SceneDoc.h"

#include "Liquid.h"
#include "LiquidControls.h"
#include "DynamicModel.h"
#include "PropertyWnd.h"
#include "SceneResourceTree.h"
#include "StaticMesh.h"
#include "Light.h"

#include "OgreSceneManager.h"

IMPLEMENT_DYNAMIC(SceneResourceTree, CTreeCtrl)

SceneResourceTree *SceneResourceTree::current = NULL;
SceneResourceTree::SceneResourceTree()
{
	current = this;
}

SceneResourceTree::~SceneResourceTree()
{
}

void SceneResourceTree::resetImageList()
{
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_SCENE_RESOURCE_TREE);

	BITMAP _bitmap;
	bitmap.GetBitmap(&_bitmap);

	imageList.Create(16, _bitmap.bmHeight, ILC_MASK | ILC_COLOR24, 0, 0);
	imageList.Add(&bitmap, RGB(255, 0, 255));

	SetImageList(&imageList, TVSIL_NORMAL);
}

void SceneResourceTree::rebuildTree()
{
	treeItems[SCENE] = InsertItem("Scene", SCENE, SCENE);
	treeItems[SKY] = InsertItem("Sky", SKY, SKY, treeItems[SCENE]);
	treeItems[TERRAIN] = InsertItem("Terrain", TERRAIN, TERRAIN, treeItems[SCENE]);
	treeItems[LIGHT_SET] = InsertItem("Lights", LIGHT_SET, LIGHT_SET, treeItems[SCENE]);
	treeItems[MODEL_SET] = InsertItem("Models",	MODEL_SET, MODEL_SET, treeItems[SCENE]);
	treeItems[MESH_SET] = InsertItem("Meshs", MESH_SET, MESH_SET, treeItems[SCENE]);
	treeItems[LIQUID_SET] = InsertItem("Liquids", LIQUID_SET, LIQUID_SET, treeItems[SCENE]);
	treeItems[PARTICLE_SET]	= InsertItem("Particles", PARTICLE_SET,	PARTICLE_SET, treeItems[SCENE]);
}

void SceneResourceTree::firePropertyChanged()
{
	HTREEITEM selectedItem = GetSelectedItem();
	afterSelectTreeItem(selectedItem);
}

void SceneResourceTree::onPropertyChanged(CBCGPProp *prop)
{
	/** 在SceneDoc销毁后应清空？
	*/
	if(!SceneDoc::current || !SceneDoc::current->isInitialized())
		return;

	HTREEITEM selectedItem = GetSelectedItem();
	SceneObject *sceneObject = (SceneObject*)GetItemData(selectedItem);

	int image, selectedImage;
	GetItemImage(selectedItem, (int&)image, selectedImage);

	switch(prop->GetData())
	{
	case SCENE:
		{
			onUpdateScene(prop);
		}
		break;

	case SKY:
		{
			onUpdateSky(prop);
		}
		break;

	case TERRAIN_SET:
		{
			onUpdateTerrainSet(prop);
		}
		break;

	case TERRAIN:
		{
			onUpdateTerrain(prop, sceneObject);
		}
		break;

	case LIGHT_SET:
		{
			onUpdateLightSet(prop);
		}
		break;

	case LIGHT:
		{
			onUpdateLight(prop, sceneObject);	
		}
		break;

	case MODEL_SET:
		{
			onUpdateModelSet(prop);
		}
		break;

	case MODEL:
		{
			onUpdateModel(prop, sceneObject);
		}
		break;

	case MESH_SET:
		{
			onUpdateMeshSet(prop);
		}
		break;

	case MESH:
		{
			onUpdateMesh(prop, sceneObject);	
		}
		break;

	case LIQUID_SET:
		{
			onUpdateLiquidSet(prop);
		}
		break;

	case LIQUID:
		{
			onUpdateLiquid(prop, sceneObject);
		}
		break;

	case PARTICLE_SET:
		{
			onUpdateParticleSet(prop);
		}
		break;

	case PARTICLE:
		{
			onUpdateParticle(prop, sceneObject);
		}
	}
}

BEGIN_MESSAGE_MAP(SceneResourceTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
END_MESSAGE_MAP()

void SceneResourceTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	 
	SceneObject *sceneObject = NULL;

	HTREEITEM treeItem = GetSelectedItem();
	if(treeItem != NULL)
	{
		sceneObject = (SceneObject*)GetItemData(treeItem);

		SetItemState(treeItem, TVIS_SELECTED, TVIS_SELECTED);
		afterSelectTreeItem(treeItem);
	}

	SceneDoc::current->selectObject(sceneObject);
}

void SceneResourceTree::afterSelectTreeItem(HTREEITEM treeItem)
{
	if(!SceneDoc::current || !SceneDoc::current->isInitialized())
		return;

	PropertyWnd::current->setListener(this);
	PropertyWnd::current->getPropList()->RemoveAll();
	PropertyWnd::current->getPropList()->RedrawWindow();

	SceneObject *sceneObject = (SceneObject*)GetItemData(treeItem);

	int image, selectedImage;
	GetItemImage(treeItem, (int&)image, selectedImage);

	switch(image)
	{
	case SCENE:
		{
			onSelectScene();
		}
		break;

	case SKY:
		{
			onSelectSky();
		}
		break;

	case TERRAIN_SET:
		{
			onSelectTerrainSet();
		}
		break;

	case TERRAIN:
		{
			onSelectTerrain(sceneObject);
		}
		break;

	case LIGHT_SET:
		{
			onSelectLightSet();
		}
		break;

	case LIGHT:
		{
			onSelectLight(sceneObject);	
		}
		break;

	case MODEL_SET:
		{
			onSelectModelSet();
		}
		break;

	case MODEL:
		{
			onSelectModel(sceneObject);
		}
		break;

	case MESH_SET:
		{
			onSelectMeshSet();
		}
		break;

	case MESH:
		{
			onSelectMesh(sceneObject);	
		}
		break;

	case LIQUID_SET:
		{
			onSelectLiquidSet();
		}
		break;

	case LIQUID:
		{
			onSelectLiquid(sceneObject);
		}
		break;

	case PARTICLE_SET:
		{
			onSelectParticleSet();
		}
		break;

	case PARTICLE:
		{
			onSelectParticle(sceneObject);
		}
	}
}

void SceneResourceTree::onSelectScene()
{
	CBCGPPropList *propList = PropertyWnd::current->getPropList(); CBCGPProp *prop = NULL, *subProp = NULL;

	//////////////////////////////////////////////////
	// Scene name
	//////////////////////////////////////////////////

	CString sceneName = SceneDoc::current->getSceneName();
	prop = new CBCGPProp("Scene name", (_variant_t)sceneName, "", SCENE);
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Ambient light
	//////////////////////////////////////////////////

	Ogre::ColourValue ambientLight = 
		SceneDoc::current->getSceneManager()->getAmbientLight();
	prop = new CBCGPColorProp("Ambient light", RGB(ambientLight.r*255, ambientLight.g*255, ambientLight.b*255), NULL, NULL, SCENE);
	((CBCGPColorProp*)prop)->EnableOtherButton("Other");
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Shadow technique
	//////////////////////////////////////////////////

	CString shadowTechnique = SceneDoc::current->getShadowTechnique();
	if(shadowTechnique.IsEmpty())
		shadowTechnique = "None";
	prop = new CBCGPProp("Shadow technique", (_variant_t)shadowTechnique, "", SCENE);
	prop->AllowEdit(FALSE);
	prop->AddOption("None");
	prop->AddOption("Colour Shadows");
	prop->AddOption("Depth Shadows");
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Fog
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Fog");

	Ogre::FogMode fogMode = SceneDoc::current->getSceneManager()->getFogMode();
	switch(fogMode)
	{
	case Ogre::FOG_NONE:
		subProp = new CBCGPProp("Mode", (_variant_t)"None", NULL, SCENE);
		break;

	case Ogre::FOG_LINEAR:
		subProp = new CBCGPProp("Mode", (_variant_t)"Linear", NULL, SCENE);
		break;

	case Ogre::FOG_EXP:
		subProp = new CBCGPProp("Mode", (_variant_t)"Exp", NULL, SCENE);
		break;

	case Ogre::FOG_EXP2:
		subProp = new CBCGPProp("Mode", (_variant_t)"Exp2", NULL, SCENE);
	}
	subProp->AllowEdit(FALSE);
	subProp->AddOption("None");
	subProp->AddOption("Linear");
	subProp->AddOption("Exp");
	subProp->AddOption("Exp2");
	prop->AddSubItem(subProp);

	Ogre::ColourValue fogColor = 
		SceneDoc::current->getSceneManager()->getFogColour();
	subProp = new CBCGPColorProp("Color", RGB(fogColor.r*255, fogColor.g*255, fogColor.b*255), NULL, NULL, SCENE);
	((CBCGPColorProp*)subProp)->EnableOtherButton("Other");
	prop->AddSubItem(subProp);

	float fogDensity = SceneDoc::current->getSceneManager()->getFogDensity();
	subProp = new CBCGPProp("Density", fogDensity, NULL, SCENE);
	prop->AddSubItem(subProp);

	float fogStart = SceneDoc::current->getSceneManager()->getFogStart();
	subProp = new CBCGPProp("Start", fogStart, NULL, SCENE);
	prop->AddSubItem(subProp);

	float fogEnd = SceneDoc::current->getSceneManager()->getFogEnd();
	subProp =  new CBCGPProp("End", fogEnd, NULL, SCENE);
	prop->AddSubItem(subProp);

	propList->AddProperty(prop);
}

void SceneResourceTree::onSelectSky()
{
}

void SceneResourceTree::onSelectTerrainSet()
{
}

void SceneResourceTree::onSelectTerrain(SceneObject *sceneObject)
{
}

void SceneResourceTree::onSelectLightSet()
{
}

void SceneResourceTree::onSelectLight(SceneObject *sceneObject)
{
	CBCGPPropList *propList = PropertyWnd::current->getPropList(); CBCGPProp *prop = NULL, *subProp = NULL;

	Light *light = (Light*)sceneObject;
	Ogre::Light *_light = light->getLight();

	//////////////////////////////////////////////////
	// Name
	//////////////////////////////////////////////////

	CString lightName = _light->getName().c_str();
	prop = new CBCGPProp("Name", (_variant_t)lightName, "");
	prop->AllowEdit(FALSE);
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////

	Ogre::Light::LightTypes lightType = _light->getType();
	switch(lightType)
	{
	case Ogre::Light::LT_POINT:
		{
			prop = new CBCGPProp("Type", (_variant_t)"Point", "", LIGHT);
		}
		break;

	case Ogre::Light::LT_DIRECTIONAL:
		{
			prop = new CBCGPProp("Type", (_variant_t)"Directional", "", LIGHT);
		}
		break;

	case Ogre::Light::LT_SPOTLIGHT:
		{
			prop = new CBCGPProp("Type", (_variant_t)"Spotlight", "", LIGHT);
		}
	}
	prop->AddOption("Point");
	prop->AddOption("Directional");
	prop->AddOption("Spotlight");
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Diffuse colour
	//////////////////////////////////////////////////

	Ogre::ColourValue diffuseColour = _light->getDiffuseColour();
	prop = new CBCGPColorProp("Diffuse colour", RGB(diffuseColour.r*255, diffuseColour.g*255, diffuseColour.b*255), NULL, "", LIGHT);
	((CBCGPColorProp*)prop)->EnableOtherButton("Other");
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Specular colour
	//////////////////////////////////////////////////

	Ogre::ColourValue specularColour = _light->getSpecularColour();
	prop = new CBCGPColorProp("Specular colour", RGB(specularColour.r*255, specularColour.g*255, specularColour.b*255), NULL, "", LIGHT);
	((CBCGPColorProp*)prop)->EnableOtherButton("Other");
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Position
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Position");
	Ogre::Vector3 position = light->getSceneNode()->getPosition();
	subProp = new CBCGPProp("X", (_variant_t)position.x, "", LIGHT);
	prop->AddSubItem(subProp);
	subProp = new CBCGPProp("Y", (_variant_t)position.y, "", LIGHT);
	prop->AddSubItem(subProp);
	subProp = new CBCGPProp("Z", (_variant_t)position.z, "", LIGHT);
	prop->AddSubItem(subProp);
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Direction
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Direction");
	Ogre::Vector3 direction = light->getLight()->getDirection();
	subProp = new CBCGPProp("X", (_variant_t)direction.x, "", LIGHT);
	prop->AddSubItem(subProp);
	subProp = new CBCGPProp("Y", (_variant_t)direction.y, "", LIGHT);
	prop->AddSubItem(subProp);
	subProp = new CBCGPProp("Z", (_variant_t)direction.z, "", LIGHT);
	prop->AddSubItem(subProp);
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Attenuation
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Attenuation");
	subProp = new CBCGPProp("Range", (_variant_t)_light->getAttenuationRange(), "", LIGHT);
	prop->AddSubItem(subProp);
	subProp = new CBCGPProp("Constant", (_variant_t)_light->getAttenuationConstant(), "", LIGHT);
	prop->AddSubItem(subProp);
	subProp = new CBCGPProp("Linear", (_variant_t)_light->getAttenuationLinear(), "", LIGHT);
	prop->AddSubItem(subProp);
	subProp = new CBCGPProp("Quadratic", (_variant_t)_light->getAttenuationQuadric(), "", LIGHT);
	prop->AddSubItem(subProp);
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Spotlight range
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Spotlight range");
	subProp = new CBCGPProp("Inner angle", (_variant_t)_light->getSpotlightInnerAngle().valueRadians(), "", LIGHT);
	prop->AddSubItem(subProp); // 弧度
	subProp = new CBCGPProp("Outer angle", (_variant_t)_light->getSpotlightOuterAngle().valueRadians(), "", LIGHT);
	prop->AddSubItem(subProp);
	subProp = new CBCGPProp("Falloff", (_variant_t)_light->getSpotlightFalloff(), "", LIGHT);
	prop->AddSubItem(subProp);
	propList->AddProperty(prop);
}

void SceneResourceTree::onSelectModelSet()	
{
}

void SceneResourceTree::onSelectModel(SceneObject *sceneObject)
{
}

void SceneResourceTree::onSelectMeshSet()
{
}

void SceneResourceTree::onSelectMesh(SceneObject *sceneObject)
{
	CBCGPPropList *propList = PropertyWnd::current->getPropList(); CBCGPProp *prop = NULL, *subProp = NULL;
	
	CString objectName = sceneObject->getSceneNode()->getName().c_str();
	prop = new CBCGPProp("Name", (_variant_t)objectName, "", MESH);
	prop->AllowEdit(FALSE);
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Position
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Position");

	const Ogre::Vector3 &position = sceneObject->getSceneNode()->getPosition();
			
	subProp = new CBCGPProp("X", (_variant_t)position.x, "", MESH);
	prop->AddSubItem(subProp);

	subProp = new CBCGPProp("Y", (_variant_t)position.y, "", MESH);
	prop->AddSubItem(subProp);

	subProp = new CBCGPProp("Z", (_variant_t)position.z, "", MESH);
	prop->AddSubItem(subProp);

	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Scale
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Scale");

	const Ogre::Vector3 &scale = sceneObject->getSceneNode()->getScale();
			
	subProp = new CBCGPProp("X", (_variant_t)scale.x, "", MESH);
	prop->AddSubItem(subProp);

	subProp = new CBCGPProp("Y", (_variant_t)scale.y, "", MESH);
	prop->AddSubItem(subProp);

	subProp = new CBCGPProp("Z", (_variant_t)scale.z, "", MESH);
	prop->AddSubItem(subProp);

	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Direction
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Direction");

	const Ogre::Quaternion &orientation = sceneObject->getSceneNode()->getOrientation();
			
	subProp = new CBCGPProp("X", (_variant_t)orientation.x, "", MESH);
	prop->AddSubItem(subProp);

	subProp = new CBCGPProp("Y", (_variant_t)orientation.y, "", MESH);
	prop->AddSubItem(subProp);

	subProp = new CBCGPProp("Z", (_variant_t)orientation.z, "", MESH);
	prop->AddSubItem(subProp);

	propList->AddProperty(prop);
}

void SceneResourceTree::onSelectLiquidSet()
{
}

void SceneResourceTree::onSelectLiquid(SceneObject *sceneObject)	
{
	CBCGPPropList *propList = PropertyWnd::current->getPropList(); CBCGPProp *prop = NULL, *subProp = NULL;
	
	Liquid *liquid = (Liquid*)sceneObject;
			
	std::string materialName = liquid->getMaterial();
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName, "General");
			
	Ogre::Technique *technique = material->getSupportedTechnique(0);
	Ogre::Pass *pass = technique->getPass(0);
	Ogre::GpuProgramParametersSharedPtr vertexParams = pass->getVertexProgramParameters();
	Ogre::GpuProgramParametersSharedPtr fragmentParams = pass->getFragmentProgramParameters();

	CString liquidName = liquid->getSceneNode()->getName().c_str();
	prop = new CBCGPProp("Name", (_variant_t)liquidName, LIQUID);
	prop->AllowEdit(FALSE);
	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Position
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Position");

	const Ogre::Vector3 &position = liquid->getSceneNode()->getPosition();
			
	subProp = new CBCGPProp("X", (_variant_t)position.x, "", LIQUID);
	prop->AddSubItem(subProp);

	subProp = new CBCGPProp("Y", (_variant_t)position.y, "", LIQUID);
	prop->AddSubItem(subProp);

	subProp = new CBCGPProp("Z", (_variant_t)position.z, "", LIQUID);
	prop->AddSubItem(subProp);

	propList->AddProperty(prop);

	//////////////////////////////////////////////////
	// Controls
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Controls");

	std::vector<ShaderControl> *controls = LiquidControls::shared()->getControls(materialName);
	for(int i=0; i<controls->size(); i++)
	{
		ShaderControl *shaderControl = &(*controls)[i];
		switch(shaderControl->valueType)
		{
		case GPU_VERTEX:
		case GPU_FRAGMENT:
			{
				Ogre::GpuProgramParametersSharedPtr activeParams = 
					(shaderControl->valueType == GPU_VERTEX) ? vertexParams : fragmentParams;
				const Ogre::GpuConstantDefinition def = activeParams->getConstantDefinition(shaderControl->paramName);
				shaderControl->physicalIndex = def.physicalIndex;
				float *values = activeParams->getFloatPointer(def.physicalIndex);
				subProp = new CBCGPProp(shaderControl->name.c_str(), i, (_variant_t)values[shaderControl->elementIndex], "", LIQUID);
				prop->AddSubItem(subProp);
			}
		}
	}

	propList->AddProperty(prop);
}

void SceneResourceTree::onSelectParticleSet()
{
}

void SceneResourceTree::onSelectParticle(SceneObject *sceneObject)
{
}

void SceneResourceTree::onUpdateScene(CBCGPProp *prop)
{
	CString propName = prop->GetName();

	//////////////////////////////////////////////////
	// Scene name
	//////////////////////////////////////////////////
	if(propName == "Scene name")
	{
		CString sceneName = prop->GetValue();
		SceneDoc::current->setSceneName(sceneName);
	} else
	//////////////////////////////////////////////////
	// Ambient light
	//////////////////////////////////////////////////
	if(propName == "Ambient light")
	{
		COLORREF ambientLight = prop->GetValue();
		SceneDoc::current->getSceneManager()->setAmbientLight(
			Ogre::ColourValue(GetRValue(ambientLight)/255.0f, GetGValue(ambientLight)/255.0f, GetBValue(ambientLight)/255.0f));
	} else
	if(propName == "Shadow technique")
	{
		CString shadowTechnique = prop->GetValue();
		SceneDoc::current->configureShadows(shadowTechnique);
	} else
	//////////////////////////////////////////////////
	// Fog
	//////////////////////////////////////////////////
	{
		Ogre::FogMode fogMode = SceneDoc::current->getSceneManager()->getFogMode();
		Ogre::ColourValue fogColour = SceneDoc::current->getSceneManager()->getFogColour();
		float fogDensity = SceneDoc::current->getSceneManager()->getFogDensity();
		float fogStart = SceneDoc::current->getSceneManager()->getFogStart();
		float fogEnd = SceneDoc::current->getSceneManager()->getFogEnd();

		if(propName == "Mode")
		{
			CString strFogMode = prop->GetValue();
			if(strFogMode == "None")
			{
				fogMode = Ogre::FOG_NONE;
			} else
			if(strFogMode == "Linear")
			{
				fogMode = Ogre::FOG_LINEAR;
			} else
			if(strFogMode == "Exp")
			{
				fogMode = Ogre::FOG_EXP;
			} else
			if(strFogMode == "Exp2")
			{
				fogMode = Ogre::FOG_EXP2;
			}
		} else
		if(propName == "Colour")
		{
			COLORREF refFogColour = prop->GetValue();
			fogColour = Ogre::ColourValue(GetRValue(refFogColour)/255.0f, GetGValue(refFogColour)/255.0f, GetBValue(refFogColour)/255.0f);
		} else
		if(propName == "Density")
		{
			fogDensity = prop->GetValue();
		} else
		if(propName == "Start")
		{
			fogStart = prop->GetValue();
		} else
		if(propName == "End")
		{
			fogEnd = prop->GetValue();
		}

		SceneDoc::current->getSceneManager()->setFog(fogMode, fogColour, fogDensity, fogStart, fogEnd);
	}
}

void SceneResourceTree::onUpdateSky(CBCGPProp *prop)
{
}

void SceneResourceTree::onUpdateTerrainSet(CBCGPProp *prop)
{
}

void SceneResourceTree::onUpdateTerrain(CBCGPProp *prop, SceneObject *sceneObject)
{
}

void SceneResourceTree::onUpdateLightSet(CBCGPProp *prop)
{
}

void SceneResourceTree::onUpdateLight(CBCGPProp *prop, SceneObject *sceneObject)
{
}

void SceneResourceTree::onUpdateModelSet(CBCGPProp *prop)
{
}

void SceneResourceTree::onUpdateModel(CBCGPProp *prop, SceneObject *sceneObject)
{
}

void SceneResourceTree::onUpdateMeshSet(CBCGPProp *prop)
{
}

void SceneResourceTree::onUpdateMesh(CBCGPProp *prop, SceneObject *sceneObject)
{
}

void SceneResourceTree::onUpdateLiquidSet(CBCGPProp *prop)
{
}

void SceneResourceTree::onUpdateLiquid(CBCGPProp *prop, SceneObject *sceneObject)
{
	Liquid *liquid = (Liquid*)sceneObject;
			
	std::string materialName = liquid->getMaterial();
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName, "General");
			
	Ogre::Technique *technique = material->getSupportedTechnique(0);
	Ogre::Pass *pass = technique->getPass(0);
	Ogre::GpuProgramParametersSharedPtr vertexParams = pass->getVertexProgramParameters();
	Ogre::GpuProgramParametersSharedPtr fragmentParams = pass->getFragmentProgramParameters();

	std::vector<ShaderControl> *controls = LiquidControls::shared()->getControls(materialName);
	ShaderControl *shaderControl = &(*controls)[prop->GetID()];
	switch(shaderControl->valueType)
	{
	case GPU_VERTEX:
	case GPU_FRAGMENT:
		{
			Ogre::GpuProgramParametersSharedPtr activeParams = 
				(shaderControl->valueType == GPU_VERTEX) ? vertexParams : fragmentParams;
			float value = prop->GetValue();
			activeParams->_writeRawConstant(
				shaderControl->physicalIndex + shaderControl->elementIndex, value);
		}
	}
}

void SceneResourceTree::onUpdateParticleSet(CBCGPProp *prop)
{
}

void SceneResourceTree::onUpdateParticle(CBCGPProp *prop, SceneObject *sceneObject)
{
}
