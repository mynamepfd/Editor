#pragma once

class MiscUtils
{
public:
	/** filename为空，size不为0，则返回size大小的灰BMP；filename不为空，size为0，则返回原大小的BMP，否则返回size大小的BMP
	*/
	static HBITMAP loadBitmap(CString filename, int size);
};
