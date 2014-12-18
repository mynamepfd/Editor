#include "stdafx.h"
#include "Editor.h"
#include "GameDoc.h"
#include "GameView.h"
#include "RenderPump.h"

#include "OgreCamera.h"
#include "OgreEntity.h"
#include "OgreLight.h"
#include "OgreMeshManager.h"
#include "OgreOverlaySystem.h"
#include "OgrePlane.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreResourceGroupManager.h"

IMPLEMENT_DYNCREATE(GameDoc, CDocument)

GameDoc *GameDoc::current = NULL;
GameDoc::GameDoc()
{
	current = this;
}

GameDoc::~GameDoc()
{
}

//Utility function to help set scene up
void setEntityHeight(Ogre::Entity* ent, Ogre::Real newHeight)
{
    Ogre::Real curHeight = ent->getMesh()->getBounds().getSize().y;
    Ogre::Real scaleFactor = newHeight / curHeight;
        
    Ogre::SceneNode* parentNode = ent->getParentSceneNode();
    parentNode->setScale(scaleFactor, scaleFactor, scaleFactor);
}

void GameDoc::Initialize()
{
	sceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC); 
	sceneManager->addRenderQueueListener(RenderPump::current->getOverlaySystem());

	sceneManager->setAmbientLight(Ogre::ColourValue(0.15, 0.00, 0.00));

	// Create main, static light
	Ogre::Light* l1 = sceneManager->createLight();
    l1->setType(Ogre::Light::LT_DIRECTIONAL);
    l1->setDiffuseColour(0.5f, 0.45f, 0.1f);
	l1->setDirection(1, -0.5, -0.2);
	l1->setShadowFarClipDistance(3000);
	l1->setShadowFarDistance(75);
	//Turn this on to have the directional light cast shadows
	l1->setCastShadows(false);

	camera = sceneManager->createCamera(
		Ogre::StringConverter::toString(Ogre::Math::RangeRandom(0, 1000)) + ".Camera");
	cameraManager.setCamera(camera);
	cameraManager.setDragLook(TRUE);

	camera->setPosition(25, 5, 0);
    camera->lookAt(0,0,0);
	camera->setFarClipDistance(3000.0);
    camera->setNearClipDistance(0.5);

	// µØÃæ
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);   
	Ogre::MeshManager::getSingleton().createPlane("Myplane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 10, 10, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);  
	Ogre::Entity* pPlaneEnt = sceneManager->createEntity( "plane", "Myplane" );  
	pPlaneEnt->setMaterialName("DeferredDemo/Ground");  
	pPlaneEnt->setCastShadows(false);  
	sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(pPlaneEnt);

	Ogre::Entity* knotEnt = sceneManager->createEntity("Knot", "knot.mesh");
	knotEnt->setMaterialName("DeferredDemo/RockWall");
	Ogre::SceneNode *knotNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	knotNode->setPosition(0, 2, 0);
	knotNode->attachObject(knotEnt);
	setEntityHeight(knotEnt, 3);

	Ogre::Light* knotLight = sceneManager->createLight("KnotLight1");
    knotLight->setType(Ogre::Light::LT_SPOTLIGHT);
    knotLight->setDiffuseColour(Ogre::ColourValue::Red);
    knotLight->setSpecularColour(Ogre::ColourValue::White);
    knotLight->setPosition(knotNode->getPosition() + Ogre::Vector3(0,3,0));
    knotLight->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
    knotLight->setSpotlightRange(Ogre::Degree(25), Ogre::Degree(45), 1);
    knotLight->setAttenuation(6, 1, 0.2, 0);
}

void GameDoc::Destroy()
{
}

void GameDoc::update(float Elapsed)
{
	POSITION ViewPos = GetFirstViewPosition();
	while(ViewPos != NULL)
	{
		((GameView*)GetNextView(ViewPos))->Invalidate(FALSE);
	}
}

BEGIN_MESSAGE_MAP(GameDoc, CDocument)
END_MESSAGE_MAP()
