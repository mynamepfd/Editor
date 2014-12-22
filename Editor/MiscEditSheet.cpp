#include "stdafx.h"
#include "Editor.h"
#include "MiscEditSheet.h"

#include "TexturePreviewPage.h"
#include "MaterialPreviewPage.h"
#include "ParticlePreviewPage.h"
#include "TerrainEditPage.h"

IMPLEMENT_DYNAMIC(CMiscEditSheet, CBCGPTabWnd)

CMiscEditSheet::CMiscEditSheet()
{

}

CMiscEditSheet::~CMiscEditSheet()
{
}

BEGIN_MESSAGE_MAP(CMiscEditSheet, CBCGPTabWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CMiscEditSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CBCGPTabWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	tabs[TEXTURE_PREVIEW] = new CTexturePreviewPage;
	tabs[TEXTURE_PREVIEW]->Create(IDD_TEXTURE_PREVIEW, this);
	InsertTab(tabs[TEXTURE_PREVIEW], "Texture Preview", TEXTURE_PREVIEW);

	tabs[MATERIAL_PREVIEW] = new CMaterialPreviewPage;
	tabs[MATERIAL_PREVIEW]->Create(IDD_MATERIAL_PREVIEW, this);
	InsertTab(tabs[MATERIAL_PREVIEW], "Material Preview", MATERIAL_PREVIEW);

	tabs[PARTICLE_PREVIEW] = new ParticlePreviewPage;
	tabs[PARTICLE_PREVIEW]->Create(IDD_PARTICLE_PREVIEW, this);
	InsertTab(tabs[PARTICLE_PREVIEW], "Particle Preview", PARTICLE_PREVIEW);

	tabs[TERRAIN_EDIT] = new CTerrainEditPage;
	tabs[TERRAIN_EDIT]->Create(IDD_TERRAIN_EDIT, this);
	InsertTab(tabs[TERRAIN_EDIT], "Terrain Edit", TERRAIN_EDIT);

	return 0;
}
