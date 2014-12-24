#include "stdafx.h"
#include "LiquidControls.h"

#include "OgreResourceGroupManager.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"

LiquidControls *LiquidControls::self = NULL;
LiquidControls *LiquidControls::shared()
{
	if(self == NULL)
		self = new LiquidControls;
	return self;
}

void LiquidControls::initialize()
{
	Ogre::StringVectorPtr filenames = Ogre::ResourceGroupManager::getSingleton().findResourceNames("General", "*.controls");
	Ogre::StringVector::iterator filenameIter = filenames->begin();
	while(filenameIter != filenames->end())
	{
		loadLiquidControlsFile(*filenameIter);
		filenameIter++;
	}
}

void LiquidControls::loadLiquidControlsFile(std::string filename)
{
	Ogre::ConfigFile cf;
	cf.loadFromResourceSystem(filename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	Ogre::ConfigFile::SectionIterator secIter = cf.getSectionIterator();
	Ogre::String secName, typeName, materialName, dataString;

	while(secIter.hasMoreElements())
	{
		secName = secIter.peekNextKey();

		Ogre::ConfigFile::SettingsMultiMap *settings = secIter.getNext();
		materialName = cf.getSetting("material", secName);

		Ogre::ConfigFile::SettingsMultiMap::iterator settingIter = settings->begin();
		for(; settingIter != settings->end(); settingIter++)
		{
			typeName = settingIter->first;
			dataString = settingIter->second;
			if(typeName == "control")
				controls[materialName].push_back(parseDataString(dataString));
		}
	}
}

ShaderControl LiquidControls::parseDataString(std::string dataString)
{
	Ogre::StringVector params = Ogre::StringUtil::split(dataString, ",");

	ShaderControl shaderControl;

	Ogre::StringUtil::trim(params[0]);
	shaderControl.name = params[0];

	Ogre::StringUtil::trim(params[1]);
	shaderControl.paramName = params[1];

	Ogre::StringUtil::trim(params[2]);
	if(params[2] == "GPU_VERTEX")
	{
		shaderControl.valueType = GPU_VERTEX;
	} else
	if(params[2] == "GPU_FRAGMENT")
	{
		shaderControl.valueType = GPU_FRAGMENT;
	}

	shaderControl.minValue = Ogre::StringConverter::parseReal(params[3]);
	shaderControl.maxValue = Ogre::StringConverter::parseReal(params[4]);
	shaderControl.elementIndex = Ogre::StringConverter::parseInt(params[5]);

	return shaderControl;
}
