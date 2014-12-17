#include "stdafx.h"
#include "Editor.h"
#include "MiscUtils.h"
#include "d3dx9.h"

HBITMAP MiscUtils::loadBitmap(CString filename, int size)
{
	if(filename.GetLength() == 0 && size != 0)
	{ // °×Î»Í¼
		BITMAPINFO bitmapInfo;
 		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
 		bitmapInfo.bmiHeader.biBitCount = 24;
 		bitmapInfo.bmiHeader.biPlanes = 1;
 		bitmapInfo.bmiHeader.biSizeImage = 0;
 		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biWidth = size;
		bitmapInfo.bmiHeader.biHeight = size;
 		bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
 		bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
 		bitmapInfo.bmiHeader.biClrUsed = 0;
 		bitmapInfo.bmiHeader.biClrImportant = 0;
 
 		unsigned char *bitmapData = NULL;
 		HBITMAP bitmap = ::CreateDIBSection(0, &bitmapInfo, DIB_RGB_COLORS, (void**)&bitmapData, 0, 0);
		memset(bitmapData, 225, size * size * 3);

		return bitmap;
	}
	else
	{
		IDirect3DDevice9 *device = theApp.getDevice();
		
		IDirect3DTexture9 *texture = NULL; 
		D3DSURFACE_DESC d3dsd; D3DLOCKED_RECT d3dlr;
		D3DXCreateTextureFromFileEx(device, filename, size != 0 ? size : D3DX_DEFAULT, size != 0 ? size : D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &texture);
		texture->GetLevelDesc(0, &d3dsd);

		BITMAPINFO bitmapInfo;
 		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
 		bitmapInfo.bmiHeader.biBitCount = 32;
 		bitmapInfo.bmiHeader.biPlanes = 1;
 		bitmapInfo.bmiHeader.biSizeImage = 0;
 		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biWidth = size != 0 ? size : d3dsd.Width;
		bitmapInfo.bmiHeader.biHeight = size != 0 ? size : d3dsd.Height;
 		bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
 		bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
 		bitmapInfo.bmiHeader.biClrUsed = 0;
 		bitmapInfo.bmiHeader.biClrImportant = 0;

		unsigned char *bitmapData = NULL;
 		HBITMAP bitmap = ::CreateDIBSection(0, &bitmapInfo, DIB_RGB_COLORS, (void**)&bitmapData, 0, 0);

		texture->LockRect(0, &d3dlr, NULL, D3DLOCK_READONLY);
			memcpy(bitmapData, d3dlr.pBits, bitmapInfo.bmiHeader.biWidth * bitmapInfo.bmiHeader.biHeight * bitmapInfo.bmiHeader.biBitCount/8);
		texture->UnlockRect(0);
		texture->Release();

		return bitmap;
	}
	return NULL;
}
