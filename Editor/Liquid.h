#pragma once

#include "SceneObject.h"

namespace Ogre
{
	class SceneManager;
	class Entity;
}

class Liquid : public SceneObject
{
public:
	Liquid(Ogre::SceneManager *sceneManager);

	Ogre::Entity *getEntity();

	std::string getMaterial() { return material; }
	void setMaterial(std::string material) { this->material = material; }

	void create(std::string name, std::string path);
	void destroy();

private:
	Ogre::Entity *entity;
	std::string material;
};
