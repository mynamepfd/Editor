/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2013 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef __GLTEXTURE_H__
#define __GLTEXTURE_H__

#include "OgreGLPrerequisites.h"
#include "OgrePlatform.h"
#include "OgreRenderTexture.h"
#include "OgreTexture.h"
#include "OgreGLSupport.h"
#include "OgreHardwarePixelBuffer.h"

namespace Ogre {

    class _OgreGLExport GLTexture : public Texture
    {
    public:
        // Constructor
        GLTexture(ResourceManager* creator, const String& name, ResourceHandle handle,
            const String& group, bool isManual, ManualResourceLoader* loader, 
            GLSupport& support);

        virtual ~GLTexture();      

        void createRenderTexture();
			
		/// @copydoc Texture::getBuffer
		HardwarePixelBufferSharedPtr getBuffer(size_t face, size_t mipmap);

        /// Takes the OGRE texture type (1d/2d/3d/cube) and returns the appropriate GL one
        GLenum getGLTextureTarget(void) const;

        GLuint getGLID() const
        {
            return mTextureID;
        }
		
		void getCustomAttribute(const String& name, void* pData);

    protected:
		/// @copydoc Texture::createInternalResourcesImpl
		void createInternalResourcesImpl(void);
        /// @copydoc Resource::prepareImpl
        void prepareImpl(void);
        /// @copydoc Resource::unprepareImpl
        void unprepareImpl(void);
        /// @copydoc Resource::loadImpl
        void loadImpl(void);
        /// @copydoc Texture::freeInternalResourcesImpl
        void freeInternalResourcesImpl(void);

		/** internal method, create GLHardwarePixelBuffers for every face and
			 mipmap level. This method must be called after the GL texture object was created,
			the number of mipmaps was set (GL_TEXTURE_MAX_LEVEL) and glTexImageXD was called to
			actually allocate the buffer
		*/
		void _createSurfaceList();

        /// Used to hold images between calls to prepare and load.
        typedef SharedPtr<vector<Image>::type > LoadedImages;

        /** Vector of images that were pulled from disk by
            prepareLoad but have yet to be pushed into texture memory
            by loadImpl.  Images should be deleted by loadImpl and unprepareImpl.
        */
        LoadedImages mLoadedImages;


    private:
        GLuint mTextureID;
        GLSupport& mGLSupport;
		
		/// Vector of pointers to subsurfaces
		typedef vector<HardwarePixelBufferSharedPtr>::type SurfaceList;
		SurfaceList	mSurfaceList;
    };
}

#endif // __GLTEXTURE_H__
