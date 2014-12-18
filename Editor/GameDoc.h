#pragma once

#include "RenderScene.h"

class DeferredShadingSystem;

class GameDoc : public RenderScene
{
	DECLARE_DYNCREATE(GameDoc)
public:
	GameDoc();
	virtual ~GameDoc();

	DeferredShadingSystem *getDeferredShadingSystem() { return deferredShadingSystem; }
	void setDeferredShadingSystem(DeferredShadingSystem *deferredShadingSystem) { this->deferredShadingSystem = deferredShadingSystem; }

	void Initialize();
	void Destroy();

	void update(float Elapsed);

	static GameDoc *current;

	DECLARE_MESSAGE_MAP()
	DeferredShadingSystem *deferredShadingSystem;
};
