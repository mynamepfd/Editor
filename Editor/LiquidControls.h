#pragma once

#include <string>
#include <vector>
#include <map>

enum ShaderValueType
{
	GPU_VERTEX,
	GPU_FRAGMENT,
	MAT_SPECULAR,
	MAT_DIFFUSE,
	MAT_AMBIENT,
	MAT_SHININESS,
	MAT_EMISSIVE
};

class ShaderControl
{
public:
	std::string name;
	std::string paramName;
	ShaderValueType valueType;
	float minValue;
	float maxValue;
	int elementIndex;
	int physicalIndex; // For update
};

class LiquidControls
{
public:
	static LiquidControls *shared();

	void initialize();
	std::vector<ShaderControl> *getControls(std::string materialName) { return &controls[materialName]; }

private:
	LiquidControls() {}
	~LiquidControls() {}

	void loadLiquidControlsFile(std::string filename);
	ShaderControl parseDataString(std::string dataString);
	std::map<std::string, std::vector<ShaderControl>> controls;

	static LiquidControls *self;
};
