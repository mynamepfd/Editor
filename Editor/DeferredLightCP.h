// Deferred shading composition pass(DeferredLightCP)
#ifndef _DEFERRED_LIGHT_CP_H
#define _DEFERRED_LIGHT_CP_H

#include "MaterialGenerator.h"
#include "OgreCompositorInstance.h"
#include "OgreCustomCompositionPass.h"

class DLight;
class AmbientLight;

//The render operation that will be called each frame in the custom composition pass
//This is the class that will send the actual render calls of the spheres (point lights),
//cones (spotlights) and quads (directional lights) after the GBuffer has been constructed
class DeferredLightRenderOperation : public Ogre::CompositorInstance::RenderSystemOperation
{
public:
	DeferredLightRenderOperation(Ogre::CompositorInstance* instance, const Ogre::CompositionPass* pass);
	
	/** @copydoc CompositorInstance::RenderSystemOperation::execute */
	virtual void execute(Ogre::SceneManager *sm, Ogre::RenderSystem *rs);

	virtual ~DeferredLightRenderOperation();
private:

	/** Create a new deferred light
	 */
	DLight *createDLight(Ogre::Light* light);
	
	//The texture names of the GBuffer components
	Ogre::String mTexName0;
	Ogre::String mTexName1;

	//The material generator for the light geometry
	MaterialGenerator* mLightMaterialGenerator;

	//The map of deferred light geometries already constructed
	typedef std::map<Ogre::Light*, DLight*> LightsMap;
	LightsMap mLights;

	//The ambient light used to render the scene
	AmbientLight* mAmbientLight;

	//The viewport that we are rendering to
	Ogre::Viewport* mViewport;
};

//The custom composition pass that is used for rendering the light geometry
//This class needs to be registered with the CompositorManager
class DeferredLightCompositionPass : public Ogre::CustomCompositionPass
{
public:

	/** @copydoc CustomCompositionPass::createOperation */
	virtual Ogre::CompositorInstance::RenderSystemOperation* createOperation(
		Ogre::CompositorInstance* instance, const Ogre::CompositionPass* pass)
	{
		return OGRE_NEW DeferredLightRenderOperation(instance, pass);
	}

protected:
	virtual ~DeferredLightCompositionPass() {}
};

#endif
