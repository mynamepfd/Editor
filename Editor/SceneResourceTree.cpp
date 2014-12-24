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

	int image, selectedImage;
	SceneResourceTree::current->GetItemImage(selectedItem, image, selectedImage);

	CBCGPPropList *propList = PropertyWnd::current->getPropList(); CBCGPProp *prop = NULL;
	switch(image)
	{
	//////////////////////////////////////////////////
	// Light
	//////////////////////////////////////////////////
	case LIGHT:
		{
			Light *light = (Light*)SceneResourceTree::current->GetItemData(selectedItem);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			const Ogre::Vector3 &position = light->getSceneNode()->getPosition();
			
			prop = propList->FindItemByData(LIGHT_POSITION_X);
			prop->SetValue((_variant_t)position.x);

			prop = propList->FindItemByData(LIGHT_POSITION_Y);
			prop->SetValue((_variant_t)position.y);

			prop = propList->FindItemByData(LIGHT_POSITION_Z);
			prop->SetValue((_variant_t)position.z);

			//////////////////////////////////////////////////
			// Direction
			//////////////////////////////////////////////////

			const Ogre::Vector3 &direction = light->getLight()->getDirection();
			
			prop = propList->FindItemByData(LIGHT_DIRECTION_X);
			prop->SetValue((_variant_t)direction.x);

			prop = propList->FindItemByData(LIGHT_DIRECTION_Y);
			prop->SetValue((_variant_t)direction.y);

			prop = propList->FindItemByData(LIGHT_DIRECTION_Z);
			prop->SetValue((_variant_t)direction.z);
		}
		break;

	//////////////////////////////////////////////////
	// Model & Mesh
	//////////////////////////////////////////////////
	case MODEL:
	case MESH:
		{
			SceneObject *sceneObject = (SceneObject*)SceneResourceTree::current->GetItemData(selectedItem);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			const Ogre::Vector3 &position = sceneObject->getSceneNode()->getPosition();
			
			prop = propList->FindItemByData(MESH_POSITION_X);
			prop->SetValue((_variant_t)position.x);

			prop = propList->FindItemByData(MESH_POSITION_Y);
			prop->SetValue((_variant_t)position.y);

			prop = propList->FindItemByData(MESH_POSITION_Z);
			prop->SetValue((_variant_t)position.z);

			//////////////////////////////////////////////////
			// Scale
			//////////////////////////////////////////////////

			const Ogre::Vector3 &scale = sceneObject->getSceneNode()->getScale();
			
			prop = propList->FindItemByData(MESH_SCALE_X);
			prop->SetValue((_variant_t)scale.x);

			prop = propList->FindItemByData(MESH_SCALE_Y);
			prop->SetValue((_variant_t)scale.y);

			prop = propList->FindItemByData(MESH_SCALE_Z);
			prop->SetValue((_variant_t)scale.z);

			//////////////////////////////////////////////////
			// Direction
			//////////////////////////////////////////////////

			const Ogre::Quaternion &Orientation = sceneObject->getSceneNode()->getOrientation();
			
			prop = propList->FindItemByData(MESH_DIRECTION_X);
			prop->SetValue((_variant_t)Orientation.x);

			prop = propList->FindItemByData(MESH_DIRECTION_Y);
			prop->SetValue((_variant_t)Orientation.y);

			prop = propList->FindItemByData(MESH_DIRECTION_Z);
			prop->SetValue((_variant_t)Orientation.z);
		}
		break;
	
	//////////////////////////////////////////////////
	// Liquid
	//////////////////////////////////////////////////
	case LIQUID:
		{
			SceneObject *sceneObject = (SceneObject*)SceneResourceTree::current->GetItemData(selectedItem);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			const Ogre::Vector3 &position = sceneObject->getSceneNode()->getPosition();
			
			prop = propList->FindItemByData(LIQUID_POSITION_X);
			prop->SetValue((_variant_t)position.x);

			prop = propList->FindItemByData(LIQUID_POSITION_Y);
			prop->SetValue((_variant_t)position.y);

			prop = propList->FindItemByData(LIQUID_POSITION_Z);
			prop->SetValue((_variant_t)position.z);
		}
	}
}

void SceneResourceTree::onPropertyChanged(CBCGPProp *prop)
{
	if(!SceneDoc::current)
		return;

	CBCGPPropList *propList = PropertyWnd::current->getPropList();
	switch(prop->GetData())
	{
	case SCENE_NAME:
		{
			CString SceneName = prop->GetValue();
			SceneDoc::current->setSceneName(SceneName);
		}
		break;

	case SCENE_AMBIENT_LIGHT:
		{
			COLORREF AmbientLight = prop->GetValue();
			SceneDoc::current->getSceneManager()->setAmbientLight(
				Ogre::ColourValue(GetRValue(AmbientLight)/255.0f, GetGValue(AmbientLight)/255.0f, GetBValue(AmbientLight)/255.0f));
		}
		break;

	case SCENE_SHADOW_TECHNIQUE:
		{
			CString ShadowTechnique = prop->GetValue();
			SceneDoc::current->configureShadows(ShadowTechnique != "None", ShadowTechnique == "Depth Shadows");
		}
		break;

	case SCENE_FOG_MODE:
	case SCENE_FOG_COLOR:
	case SCENE_FOG_DENSITY:
	case SCENE_FOG_START:
	case SCENE_FOG_END:
		{
			COLORREF FogColor =propList->FindItemByData(SCENE_FOG_COLOR)->GetValue();
			float FogDensity = propList->FindItemByData(SCENE_FOG_DENSITY)->GetValue();
			float FogStart = propList->FindItemByData(SCENE_FOG_START)->GetValue();
			float FogEnd = propList->FindItemByData(SCENE_FOG_END)->GetValue();

			CString FogMode = propList->FindItemByData(SCENE_FOG_MODE)->GetValue();
			if(FogMode == "None")
			{
				SceneDoc::current->getSceneManager()->setFog(
					Ogre::FOG_NONE, Ogre::ColourValue(GetRValue(FogColor)/255.0f, GetGValue(FogColor)/255.0f, GetBValue(FogColor)/255.0f), FogDensity, FogStart, FogEnd);
			} else 
			if(FogMode == "Linear")
			{
				SceneDoc::current->getSceneManager()->setFog(
					Ogre::FOG_LINEAR, Ogre::ColourValue(GetRValue(FogColor)/255.0f, GetGValue(FogColor)/255.0f, GetBValue(FogColor)/255.0f), FogDensity, FogStart, FogEnd);
			} else 
			if(FogMode == "Exp")
			{
				SceneDoc::current->getSceneManager()->setFog(
					Ogre::FOG_EXP, Ogre::ColourValue(GetRValue(FogColor)/255.0f, GetGValue(FogColor)/255.0f, GetBValue(FogColor)/255.0f), FogDensity, FogStart, FogEnd);
			} else 
			if(FogMode == "Exp2")
			{
				SceneDoc::current->getSceneManager()->setFog(
					Ogre::FOG_EXP2, Ogre::ColourValue(GetRValue(FogColor)/255.0f, GetGValue(FogColor)/255.0f, GetBValue(FogColor)/255.0f), FogDensity, FogStart, FogEnd);
			}
		}
		break;

	case LIGHT_TYPE:
	case LIGHT_DIFFUSE_COLOUR:
	case LIGHT_SPECULAR_COLOUR:
	case LIGHT_POSITION_X:
	case LIGHT_POSITION_Y:
	case LIGHT_POSITION_Z:
	case LIGHT_DIRECTION_X:
	case LIGHT_DIRECTION_Y:
	case LIGHT_DIRECTION_Z:
	case LIGHT_ATTENUATION_RANGE:
	case LIGHT_ATTENUATION_CONSTANT:
	case LIGHT_ATTENUATION_LINEAR:
	case LIGHT_ATTENUATION_QUADRATIC:
	case LIGHT_SPOTLIGHT_RANGE_INNER_ANGLE:
	case LIGHT_SPOTLIGHT_RANGE_OUTER_ANGLE:
	case LIGHT_SPOTLIGHT_RANGE_FALLOFF:
		{
			Light *light = (Light*)SceneResourceTree::current->GetItemData(GetSelectedItem());
			
			CString Type = propList->FindItemByData(LIGHT_TYPE)->GetValue();
			if(Type == "Point")
			{
				light->getLight()->setType(Ogre::Light::LT_POINT);
			} else
			if(Type == "Directional")
			{
				light->getLight()->setType(Ogre::Light::LT_DIRECTIONAL);
			} else
			if(Type == "Spotlight")
			{
				light->getLight()->setType(Ogre::Light::LT_SPOTLIGHT);
			}

			COLORREF DiffuseColour = propList->FindItemByData(LIGHT_DIFFUSE_COLOUR)->GetValue();
			light->getLight()->setDiffuseColour(
				Ogre::ColourValue(GetRValue(DiffuseColour)/255.0f, GetGValue(DiffuseColour)/255.0f, GetBValue(DiffuseColour)/255.0f));

			COLORREF SpecularColour = propList->FindItemByData(LIGHT_SPECULAR_COLOUR)->GetValue();
			light->getLight()->setSpecularColour(
				Ogre::ColourValue(GetRValue(SpecularColour)/255.0f, GetGValue(SpecularColour)/255.0f, GetBValue(SpecularColour)/255.0f));

			float PositionX = propList->FindItemByData(LIGHT_POSITION_X)->GetValue();
			float PositionY = propList->FindItemByData(LIGHT_POSITION_Y)->GetValue();
			float PositionZ = propList->FindItemByData(LIGHT_POSITION_Z)->GetValue();
			light->getSceneNode()->setPosition(Ogre::Vector3(PositionX, PositionY, PositionZ));

			float DirectionX = propList->FindItemByData(LIGHT_DIRECTION_X)->GetValue();
			float DirectionY = propList->FindItemByData(LIGHT_DIRECTION_Y)->GetValue();
			float DirectionZ = propList->FindItemByData(LIGHT_DIRECTION_Z)->GetValue();
			light->getLight()->setDirection(Ogre::Vector3(DirectionX, DirectionY, DirectionZ));

			float AttenuationRange = propList->FindItemByData(LIGHT_ATTENUATION_RANGE)->GetValue();
			float AttenuationConstant = propList->FindItemByData(LIGHT_ATTENUATION_CONSTANT)->GetValue();
			float AttenuationLinear = propList->FindItemByData(LIGHT_ATTENUATION_LINEAR)->GetValue();
			float AttenuationQuadratic = propList->FindItemByData(LIGHT_ATTENUATION_QUADRATIC)->GetValue();
			light->getLight()->setAttenuation(AttenuationRange, AttenuationConstant, AttenuationLinear, AttenuationQuadratic);

			float SpotlightInnerAngle = propList->FindItemByData(LIGHT_SPOTLIGHT_RANGE_INNER_ANGLE)->GetValue();
			float SpotlightOuterAngle = propList->FindItemByData(LIGHT_SPOTLIGHT_RANGE_OUTER_ANGLE)->GetValue();
			float SpotlightFalloff = propList->FindItemByData(LIGHT_SPOTLIGHT_RANGE_FALLOFF)->GetValue();
			light->getLight()->setSpotlightRange(Ogre::Radian(SpotlightInnerAngle),  Ogre::Radian(SpotlightOuterAngle), SpotlightFalloff);
		}
	}
}

BEGIN_MESSAGE_MAP(SceneResourceTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
END_MESSAGE_MAP()

void SceneResourceTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	if(!SceneDoc::current)
		return;

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	SceneObject *Object = NULL;
	if(hItem != NULL)
	{
		Object = (SceneObject*)GetItemData(hItem);

		SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
		afterSelectTreeItem(hItem);
	}
	SceneDoc::current->selectObject(Object);
}

void SceneResourceTree::afterSelectTreeItem(HTREEITEM treeItem)
{
	CBCGPPropList *propList = PropertyWnd::current->getPropList();
	propList->RemoveAll();

	if(!SceneDoc::current || !SceneDoc::current->isInitialized())
		return;

	PropertyWnd::current->setListener(this);

	int image, selectedImage;
	GetItemImage(treeItem, (int&)image, selectedImage);

	CBCGPProp *prop = NULL, *subProp = NULL;
	switch(image)
	{
	//////////////////////////////////////////////////
	// Scene
	//////////////////////////////////////////////////
	case SCENE:
		{
			//////////////////////////////////////////////////
			// Scene name
			//////////////////////////////////////////////////

			CString sceneName = SceneDoc::current->getSceneName();
			prop = new CBCGPProp("Scene name", (_variant_t)sceneName, "", SCENE_NAME);
			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Ambient light
			//////////////////////////////////////////////////

			Ogre::ColourValue ambientLight = 
				SceneDoc::current->getSceneManager()->getAmbientLight();
			prop = new CBCGPColorProp("Ambient Light", RGB(ambientLight.r*255, ambientLight.g*255, ambientLight.b*255), NULL, NULL, SCENE_AMBIENT_LIGHT);
			((CBCGPColorProp*)prop)->EnableOtherButton("Other");
			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Shadow technique
			//////////////////////////////////////////////////

			//prop = new CBCGPProp("Shadow technique", (_variant_t)"None", "", SCENE_SHADOW_TECHNIQUE);
			//prop->AllowEdit(FALSE);
			//prop->AddOption("None");
			//prop->AddOption("Colour Shadows");
			//prop->AddOption("Depth Shadows");
			//propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Fog
			//////////////////////////////////////////////////

			prop = new CBCGPProp("Fog");

			Ogre::FogMode fogMode = SceneDoc::current->getSceneManager()->getFogMode();
			switch(fogMode)
			{
			case Ogre::FOG_NONE:
				subProp = new CBCGPProp("Mode", (_variant_t)"None", NULL, SCENE_FOG_MODE);
				break;

			case Ogre::FOG_LINEAR:
				subProp = new CBCGPProp("Mode", (_variant_t)"Linear", NULL, SCENE_FOG_MODE);
				break;

			case Ogre::FOG_EXP:
				subProp = new CBCGPProp("Mode", (_variant_t)"Exp", NULL, SCENE_FOG_MODE);
				break;

			case Ogre::FOG_EXP2:
				subProp = new CBCGPProp("Mode", (_variant_t)"Exp2", NULL, SCENE_FOG_MODE);
			}
			subProp->AllowEdit(FALSE);
			subProp->AddOption("None");
			subProp->AddOption("Linear");
			subProp->AddOption("Exp");
			subProp->AddOption("Exp2");
			prop->AddSubItem(subProp);

			Ogre::ColourValue fogColor = 
				SceneDoc::current->getSceneManager()->getFogColour();
			subProp = new CBCGPColorProp("Color", RGB(fogColor.r*255, fogColor.g*255, fogColor.b*255), NULL, NULL, SCENE_FOG_COLOR);
			((CBCGPColorProp*)subProp)->EnableOtherButton("Other");
			prop->AddSubItem(subProp);

			float fogDensity = SceneDoc::current->getSceneManager()->getFogDensity();
			subProp = new CBCGPProp("Density", fogDensity, NULL, SCENE_FOG_DENSITY);
			prop->AddSubItem(subProp);

			float fogStart = SceneDoc::current->getSceneManager()->getFogStart();
			subProp = new CBCGPProp("Start", fogStart, NULL, SCENE_FOG_START);
			prop->AddSubItem(subProp);

			float fogEnd = SceneDoc::current->getSceneManager()->getFogEnd();
			subProp =  new CBCGPProp("End", fogEnd, NULL, SCENE_FOG_END);
			prop->AddSubItem(subProp);

			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Sky
			//////////////////////////////////////////////////

			//prop = new CBCGPProp("Sky");

			//subProp = new CBCGPProp("Type", (_variant_t)SceneDoc::current->getSkyType(), "", SCENE_SKY_TYPE);
			//subProp->AllowEdit(FALSE);
			//subProp->AddOption("None");
			//subProp->AddOption("Box");
			//subProp->AddOption("Dome");
			//subProp->AddOption("Plane");
			//prop->AddSubItem(subProp);

			//propList->AddProperty(prop);
			// subProp = new CBCGPProp("Material", (_variant_t)

		}
		break;

	//////////////////////////////////////////////////
	// Sky
	//////////////////////////////////////////////////
	case SKY:
		{
		}
		break;

	//////////////////////////////////////////////////
	// Terrain
	//////////////////////////////////////////////////
	case TERRAIN:
		{
		}
		break;

	//////////////////////////////////////////////////
	// Light
	//////////////////////////////////////////////////
	case LIGHT:
		{
			Light *light = (Light*)GetItemData(treeItem);
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
					prop = new CBCGPProp("Type", (_variant_t)"Point", "", LIGHT_TYPE);
				}
				break;

			case Ogre::Light::LT_DIRECTIONAL:
				{
					prop = new CBCGPProp("Type", (_variant_t)"Directional", "", LIGHT_TYPE);
				}
				break;

			case Ogre::Light::LT_SPOTLIGHT:
				{
					prop = new CBCGPProp("Type", (_variant_t)"Spotlight", "", LIGHT_TYPE);
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
			prop = new CBCGPColorProp("Diffuse colour", RGB(diffuseColour.r*255, diffuseColour.g*255, diffuseColour.b*255), NULL, "", LIGHT_DIFFUSE_COLOUR);
			((CBCGPColorProp*)prop)->EnableOtherButton("Other");
			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Specular colour
			//////////////////////////////////////////////////

			Ogre::ColourValue specularColour = _light->getSpecularColour();
			prop = new CBCGPColorProp("Specular colour", RGB(specularColour.r*255, specularColour.g*255, specularColour.b*255), NULL, "", LIGHT_SPECULAR_COLOUR);
			((CBCGPColorProp*)prop)->EnableOtherButton("Other");
			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			prop = new CBCGPProp("Position");
			Ogre::Vector3 position = light->getSceneNode()->getPosition();
			subProp = new CBCGPProp("X", (_variant_t)position.x, "", LIGHT_POSITION_X);
			prop->AddSubItem(subProp);
			subProp = new CBCGPProp("Y", (_variant_t)position.y, "", LIGHT_POSITION_Y);
			prop->AddSubItem(subProp);
			subProp = new CBCGPProp("Z", (_variant_t)position.z, "", LIGHT_POSITION_Z);
			prop->AddSubItem(subProp);
			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Direction
			//////////////////////////////////////////////////

			prop = new CBCGPProp("Direction");
			Ogre::Vector3 direction = light->getLight()->getDirection();
			subProp = new CBCGPProp("X", (_variant_t)direction.x, "", LIGHT_DIRECTION_X);
			prop->AddSubItem(subProp);
			subProp = new CBCGPProp("Y", (_variant_t)direction.y, "", LIGHT_DIRECTION_Y);
			prop->AddSubItem(subProp);
			subProp = new CBCGPProp("Z", (_variant_t)direction.z, "", LIGHT_DIRECTION_Z);
			prop->AddSubItem(subProp);
			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Attenuation
			//////////////////////////////////////////////////

			prop = new CBCGPProp("Attenuation");
			subProp = new CBCGPProp("Range", (_variant_t)_light->getAttenuationRange(), "", LIGHT_ATTENUATION_RANGE);
			prop->AddSubItem(subProp);
			subProp = new CBCGPProp("Constant", (_variant_t)_light->getAttenuationConstant(), "", LIGHT_ATTENUATION_CONSTANT);
			prop->AddSubItem(subProp);
			subProp = new CBCGPProp("Linear", (_variant_t)_light->getAttenuationLinear(), "", LIGHT_ATTENUATION_LINEAR);
			prop->AddSubItem(subProp);
			subProp = new CBCGPProp("Quadratic", (_variant_t)_light->getAttenuationQuadric(), "", LIGHT_ATTENUATION_QUADRATIC);
			prop->AddSubItem(subProp);
			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Spotlight range
			//////////////////////////////////////////////////

			prop = new CBCGPProp("Spotlight range");
			subProp = new CBCGPProp("Inner angle", (_variant_t)_light->getSpotlightInnerAngle().valueRadians(), "", LIGHT_SPOTLIGHT_RANGE_INNER_ANGLE);
			prop->AddSubItem(subProp); // »¡¶È
			subProp = new CBCGPProp("Outer angle", (_variant_t)_light->getSpotlightOuterAngle().valueRadians(), "", LIGHT_SPOTLIGHT_RANGE_OUTER_ANGLE);
			prop->AddSubItem(subProp);
			subProp = new CBCGPProp("Falloff", (_variant_t)_light->getSpotlightFalloff(), "", LIGHT_SPOTLIGHT_RANGE_FALLOFF);
			prop->AddSubItem(subProp);
			propList->AddProperty(prop);
		}
		break;

	//////////////////////////////////////////////////
	// Model & Mesh
	//////////////////////////////////////////////////
	case MODEL:
	case MESH:
		{
			SceneObject *sceneObject = (SceneObject*)GetItemData(treeItem);

			CString objectName = sceneObject->getSceneNode()->getName().c_str();
			prop = new CBCGPProp("Name", (_variant_t)sceneObject);
			prop->AllowEdit(FALSE);
			propList->AddProperty(prop);

			//prop = new CBCGPProp("Shadow", (_variant_t)"FALSE", "", MESH_SHADOW);
			//prop->AllowEdit(FALSE);
			//prop->AddOption("TRUE");
			//prop->AddOption("FALSE");
			//propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			prop = new CBCGPProp("Position");

			const Ogre::Vector3 &position = sceneObject->getSceneNode()->getPosition();
			
			subProp = new CBCGPProp("X", (_variant_t)position.x, "", MESH_POSITION_X);
			prop->AddSubItem(subProp);

			subProp = new CBCGPProp("Y", (_variant_t)position.y, "", MESH_POSITION_Y);
			prop->AddSubItem(subProp);

			subProp = new CBCGPProp("Z", (_variant_t)position.z, "", MESH_POSITION_Z);
			prop->AddSubItem(subProp);

			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Scale
			//////////////////////////////////////////////////

			prop = new CBCGPProp("Scale");

			const Ogre::Vector3 &scale = sceneObject->getSceneNode()->getScale();
			
			subProp = new CBCGPProp("X", (_variant_t)scale.x, "", MESH_SCALE_X);
			prop->AddSubItem(subProp);

			subProp = new CBCGPProp("Y", (_variant_t)scale.y, "", MESH_SCALE_Y);
			prop->AddSubItem(subProp);

			subProp = new CBCGPProp("Z", (_variant_t)scale.z, "", MESH_SCALE_Z);
			prop->AddSubItem(subProp);

			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Direction
			//////////////////////////////////////////////////

			prop = new CBCGPProp("Direction");

			const Ogre::Quaternion &orientation = sceneObject->getSceneNode()->getOrientation();
			
			subProp = new CBCGPProp("X", (_variant_t)orientation.x, "", MESH_DIRECTION_X);
			prop->AddSubItem(subProp);

			subProp = new CBCGPProp("Y", (_variant_t)orientation.y, "", MESH_DIRECTION_Y);
			prop->AddSubItem(subProp);

			subProp = new CBCGPProp("Z", (_variant_t)orientation.z, "", MESH_DIRECTION_Z);
			prop->AddSubItem(subProp);

			propList->AddProperty(prop);
		}
		break;

	//////////////////////////////////////////////////
	// Liquid
	//////////////////////////////////////////////////
	case LIQUID:
		{
			Liquid *liquid = (Liquid*)GetItemData(treeItem);
			
			std::string materialName = liquid->getMaterial();
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName, "General");
			
			Ogre::Technique *technique = material->getSupportedTechnique(0);
			Ogre::Pass *pass = technique->getPass(0);
			
			Ogre::GpuProgramPtr vertexProgram = pass->getVertexProgram();
			Ogre::GpuProgramParametersSharedPtr vertexParams = pass->getVertexProgramParameters();

			Ogre::GpuProgramPtr fragmentProgram = pass->getFragmentProgram();
			Ogre::GpuProgramParametersSharedPtr fragmentParams = pass->getFragmentProgramParameters();

			CString liquidName = liquid->getSceneNode()->getName().c_str();
			prop = new CBCGPProp("Name", (_variant_t)liquidName);
			prop->AllowEdit(FALSE);
			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Position
			//////////////////////////////////////////////////

			prop = new CBCGPProp("Position");

			const Ogre::Vector3 &position = liquid->getSceneNode()->getPosition();
			
			subProp = new CBCGPProp("X", (_variant_t)position.x, "", LIQUID_POSITION_X);
			prop->AddSubItem(subProp);

			subProp = new CBCGPProp("Y", (_variant_t)position.y, "", LIQUID_POSITION_Y);
			prop->AddSubItem(subProp);

			subProp = new CBCGPProp("Z", (_variant_t)position.z, "", LIQUID_POSITION_Z);
			prop->AddSubItem(subProp);

			propList->AddProperty(prop);

			//////////////////////////////////////////////////
			// Controls
			//////////////////////////////////////////////////

			prop = new CBCGPProp("Controls");

			std::vector<ShaderControl> controls = LiquidControls::shared()->getControls(materialName);
			for(int i=0; i<controls.size(); i++)
			{
				ShaderControl shaderControl = controls[i];
				switch(shaderControl.valueType)
				{
				case GPU_VERTEX:
				case GPU_FRAGMENT:
					{
						Ogre::GpuProgramParametersSharedPtr activeParams = 
							(shaderControl.valueType == GPU_VERTEX) ? vertexParams : fragmentParams;
						const Ogre::GpuConstantDefinition def = activeParams->getConstantDefinition(shaderControl.paramName);
						float *values = activeParams->getFloatPointer(def.physicalIndex);
						subProp = new CBCGPProp(shaderControl.name.c_str(), (_variant_t)values[shaderControl.elementIndex], "");
						prop->AddSubItem(subProp);
					}
				}
			}

			propList->AddProperty(prop);
		}
		break;

	case PARTICLE:
		{
		}
	}
}
