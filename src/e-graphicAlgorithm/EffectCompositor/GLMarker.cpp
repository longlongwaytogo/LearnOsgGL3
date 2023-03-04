#include "GLMarker.h"
#include <osg/GLExtensions>
namespace Effect
{

	typedef void (APIENTRY* func_glPushDebugGroup) (GLenum source, GLuint id, GLsizei length, const GLchar *message);
	typedef void (APIENTRY* func_glPopDebugGroup) (void);
	void GLMarker::beginGroupMarker(const char* pInfo)
	{
	  
	static func_glPushDebugGroup _glPushDebugGroup =
		osg::convertPointerType<func_glPushDebugGroup, void*>(osg::getGLExtensionFuncPtr("glPushDebugGroup"));
	if(_glPushDebugGroup)
			_glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION,0,-1,pInfo);
	}

	void GLMarker::endGroupMarker()
	{
	 static func_glPopDebugGroup _glPopDebugGroup =
		osg::convertPointerType<func_glPopDebugGroup, void*>(osg::getGLExtensionFuncPtr("glPopDebugGroup"));
	if(_glPopDebugGroup)
			_glPopDebugGroup();
	}
	
	
 
	void  CameraPreDrawCallbackMarker::operator() (const osg::Camera &camera) const
	{
		std::string info = "begin Pass:" + camera.getName();
		GLMarker::beginGroupMarker(info.c_str());
		//GLMarker::beginGroupMarker("abc-");
	}
 
	
	  
	void  CameraPostDrawCallbackMarker::operator() (const osg::Camera &camera) const 
	{
		GLMarker::endGroupMarker();
		//GLMarker::endGroupMarker();
	}
	
		
	 
 
	void  DrawableCallbackMarker::drawImplementation (osg::RenderInfo &renderInfo, const osg::Drawable *drawable) const 
	{
		std::string info = "begin drawable:" + drawable->getName();
		GLMarker::beginGroupMarker(info.c_str());
		//GLMarker::beginGroupMarker("abc");
		drawable->drawImplementation(renderInfo);
		GLMarker::endGroupMarker();
		//GLMarker::endGroupMarker();
	}
	EFFECTCOMPOSITOR_API void registerCameraMarker(osg::Camera* camera)
	{
		if (!camera)
			return;

		camera->addPreDrawCallback(new CameraPreDrawCallbackMarker);
		camera->addPostDrawCallback(new CameraPostDrawCallbackMarker);
	}
}
