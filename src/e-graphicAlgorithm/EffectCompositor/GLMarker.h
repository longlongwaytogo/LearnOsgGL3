
/*
*
*  为OpenGL 添加渲染调试标记
*/
#ifndef __EFFECT_GLMARKER_H__
#define __EFFECT_GLMARKER_H__
#include "EffectCompositor_export.h"
#include <osg/Drawable>
#include <osg/Camera>
namespace Effect
{
	class EFFECTCOMPOSITOR_API GLMarker
	{
		public:
		static void beginGroupMarker(const char* pInfo);
		
		static void endGroupMarker();
	};
	
	class EFFECTCOMPOSITOR_API CameraPreDrawCallbackMarker : public osg::Camera::DrawCallback
	{
		public:
		
		virtual void  operator() (const osg::Camera &) const ;
		
	};
	
	class EFFECTCOMPOSITOR_API CameraPostDrawCallbackMarker : public osg::Camera::DrawCallback
	{
		public:
		
		virtual void  operator() (const osg::Camera &) const ;
	};
	
	EFFECTCOMPOSITOR_API void RegisterCameraMarker(osg::Camera* camera);
	
	class EFFECTCOMPOSITOR_API DrawableCallbackMarker :public osg::Drawable::DrawCallback
	{
		public:
		virtual void  drawImplementation (osg::RenderInfo &, const osg::Drawable *) const ;
	};
	
	
}


#endif // __EFFECT_GLMARKER_H__