#include "stdafx.h"
#include "NewSceneDlg.h"
#include "TextureProp.h"

IMPLEMENT_DYNAMIC(NewSceneDlg, CBCGPDialog)

NewSceneDlg::NewSceneDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(IDD_NEW_SCENE, pParent)
{
}

NewSceneDlg::~NewSceneDlg()
{
}

_variant_t NewSceneDlg::getProperty(int type)
{
	return (_variant_t)values[type];
}

BEGIN_MESSAGE_MAP(NewSceneDlg, CBCGPDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

void NewSceneDlg::OnBnClickedOk()
{
	for(int i=0; i<_COUNT; i++)
		values[i] = props[i]->GetValue();
	CBCGPDialog::OnOK();
}

LRESULT NewSceneDlg::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

BOOL NewSceneDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	EnableVisualManagerStyle();

	CRect rectPropList;
	propListFrame.GetClientRect(rectPropList);
	propListFrame.MapWindowPoints(this, &rectPropList);
	propList.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER, rectPropList, this, -1);
	propList.EnableHeaderCtrl(FALSE);
	propList.MarkModifiedProperties();
	propList.SetVSDotNetLook();

	CBCGPProp *prop = NULL, *subProp = NULL;

	props[SCENE_NAME] = new CBCGPProp("Scene name", (_variant_t)"NewScene", "", SCENE_NAME);
	propList.AddProperty(props[SCENE_NAME]);

	props[AMBIENT_LIGHT] = new CBCGPColorProp("Ambient light", RGB(0xff, 0xff, 0xff), NULL, "", AMBIENT_LIGHT);
	((CBCGPColorProp*)props[AMBIENT_LIGHT])->EnableOtherButton("Other");
	propList.AddProperty(props[AMBIENT_LIGHT]);

	//props[SHADOW_TECHNIQUE] = new CBCGPProp("Shadow technique", (_variant_t)"None", "", SHADOW_TECHNIQUE);
	//props[SHADOW_TECHNIQUE]->AllowEdit(FALSE);
	//props[SHADOW_TECHNIQUE]->AddOption("None");
	//props[SHADOW_TECHNIQUE]->AddOption("Colour Shadows");
	//props[SHADOW_TECHNIQUE]->AddOption("Depth Shadows");
	//propList.AddProperty(props[SHADOW_TECHNIQUE]);

	//////////////////////////////////////////////////
	// Sky
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Sky");

	props[SKY_TYPE] = new CBCGPProp("Type", (_variant_t)"Box", "", SKY_TYPE);
	props[SKY_TYPE]->AllowEdit(FALSE);
	props[SKY_TYPE]->AddOption("None");
	props[SKY_TYPE]->AddOption("Box");
	props[SKY_TYPE]->AddOption("Dome");
	props[SKY_TYPE]->AddOption("Plane");
	prop->AddSubItem(props[SKY_TYPE]);

	props[SKY_MATERIAL] = new CBCGPProp("Material", (_variant_t)"CloudyNoonSkyBox", "", SKY_MATERIAL);
	prop->AddSubItem(props[SKY_MATERIAL]);

	propList.AddProperty(prop);

	//////////////////////////////////////////////////
	// Fog
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Fog");

	props[FOG_MODE] = new CBCGPProp("Mode", "None", "", FOG_MODE);
	props[FOG_MODE]->AllowEdit(FALSE);
	props[FOG_MODE]->AddOption("None");
	props[FOG_MODE]->AddOption("Linear");
	props[FOG_MODE]->AddOption("Exp");
	props[FOG_MODE]->AddOption("Exp2");
	prop->AddSubItem(props[FOG_MODE]);

	props[FOG_COLOUR] = new CBCGPColorProp("Color", RGB(0xff, 0xff, 0xff), NULL, "", FOG_COLOUR);
	((CBCGPColorProp*)props[FOG_COLOUR])->EnableOtherButton("Other");
	prop->AddSubItem(props[FOG_COLOUR]);

	props[FOG_DENSITY] = new CBCGPProp("Density", (_variant_t)0.0f, "", FOG_DENSITY);
	prop->AddSubItem(props[FOG_DENSITY]);

	props[FOG_START] = new CBCGPProp("Start", (_variant_t)0.0f, "", FOG_START);
	prop->AddSubItem(props[FOG_START]);

	props[FOG_END] = new CBCGPProp("End", (_variant_t)10000.0f, "", FOG_END);
	prop->AddSubItem(props[FOG_END]);

	propList.AddProperty(prop);

	//////////////////////////////////////////////////
	// Terrain
	//////////////////////////////////////////////////

	prop = new CBCGPProp("Terrain");

	subProp = new CBCGPProp("Light map direction");
	props[LIGHT_MAP_DIRECTION_X] = new CBCGPProp("X", (_variant_t)1.0f, "", LIGHT_MAP_DIRECTION_X);
	subProp->AddSubItem(props[LIGHT_MAP_DIRECTION_X]);
	props[LIGHT_MAP_DIRECTION_Y] = new CBCGPProp("Y", (_variant_t)-1.0f, "", LIGHT_MAP_DIRECTION_Y);
	subProp->AddSubItem(props[LIGHT_MAP_DIRECTION_Y]);
	props[LIGHT_MAP_DIRECTION_Z] = new CBCGPProp("Z", (_variant_t)0.0f, "", LIGHT_MAP_DIRECTION_Z);
	subProp->AddSubItem(props[LIGHT_MAP_DIRECTION_Z]);
	prop->AddSubItem(subProp);

	props[LIGHT_MAP_SIZE] = new CBCGPProp("Light map size", (_variant_t)1024, "", LIGHT_MAP_SIZE);
	prop->AddSubItem(props[LIGHT_MAP_SIZE]);

	props[COMPOSITE_MAP_DIFFUSE] = new CBCGPColorProp("Composite map diffuse", RGB(0xff, 0xff, 0xff), NULL, "", COMPOSITE_MAP_DIFFUSE);
	((CBCGPColorProp*)props[COMPOSITE_MAP_DIFFUSE])->EnableOtherButton("Other");
	prop->AddSubItem(props[COMPOSITE_MAP_DIFFUSE]);

	props[COMPOSITE_MAP_DISTANCE] = new CBCGPProp("Composite map distance", (_variant_t)12800.0f, "", COMPOSITE_MAP_DISTANCE);
	prop->AddSubItem(props[COMPOSITE_MAP_DISTANCE]);

	props[COMPOSITE_MAP_SIZE] = new CBCGPProp("Composite map size", (_variant_t)1024, "", COMPOSITE_MAP_SIZE);
	prop->AddSubItem(props[COMPOSITE_MAP_SIZE]);

	// Default import settings
	subProp = new CBCGPProp("Filename conversion");
	props[FILENAME_PREFIX] = new CBCGPProp("Filename prefix", (_variant_t)"../../TestMedia/Scene/NewScene", "", FILENAME_PREFIX);
	subProp->AddSubItem(props[FILENAME_PREFIX]);
	props[FILENAME_EXTENSION] = new CBCGPProp("Filename extension", (_variant_t)"dat", "", FILENAME_EXTENSION);
	subProp->AddSubItem(props[FILENAME_EXTENSION]);
	prop->AddSubItem(subProp);

	props[TERRAIN_SIZE] = new CBCGPProp("Terrain size", (_variant_t)129, "", TERRAIN_SIZE);
	prop->AddSubItem(props[TERRAIN_SIZE]);
	
	props[WORLD_SIZE] = new CBCGPProp("World size", (_variant_t)6400.0f, "", WORLD_SIZE);
	prop->AddSubItem(props[WORLD_SIZE]);
	
	props[INPUT_SCALE] = new CBCGPProp("Input scale", (_variant_t)600, "", INPUT_SCALE);
	prop->AddSubItem(props[INPUT_SCALE]);

	props[MIN_BATCH_SIZE] = new CBCGPProp("Min batch size", (_variant_t)33, "", MIN_BATCH_SIZE);
	prop->AddSubItem(props[MIN_BATCH_SIZE]);
	
	props[MAX_BATCH_SIZE] = new CBCGPProp("Max batch size", (_variant_t)65, "", MAX_BATCH_SIZE);
	prop->AddSubItem(props[MAX_BATCH_SIZE]);

	props[TEXTURE_WORLD_SIZE] = new CBCGPProp("Texture world size", (_variant_t)128.0f, "", TEXTURE_WORLD_SIZE);
	prop->AddSubItem(props[TEXTURE_WORLD_SIZE]);

	props[DIFFUSE_SPECULAR] = new TextureProp("Diffuse specular texture", "dirt_grayrocky_diffusespecular.dds", "", DIFFUSE_SPECULAR);
	prop->AddSubItem(props[DIFFUSE_SPECULAR]);

	props[NORMAL_HEIGHT] = new TextureProp("Normal height texture", "dirt_grayrocky_normalheight.dds", "", NORMAL_HEIGHT);
	prop->AddSubItem(props[NORMAL_HEIGHT]);

	props[HEIGHT_MAP] = new TextureProp("Height map", "", "", HEIGHT_MAP);
	prop->AddSubItem(props[HEIGHT_MAP]);

	propList.AddProperty(prop);

	return TRUE;
}

void NewSceneDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROPLIST_FRAME, propListFrame);
}
