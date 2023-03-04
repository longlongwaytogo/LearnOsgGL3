//--------------------------------------------------------------------------------------
// Copyright(c) longlongwaytogo.All rights reserved.
// longlongway2012@hotmail.com
//--------------------------------------------------------------------------------------

/*
* usage:============================================
*
		#include <Frame/app.h>
		class AppImpl : public Frame::App
		{
		public:
			virtual void initScene()
			{

			}
		};

		int main(int argc, char** argv)
		{
			runMain<AppImpl>(argc,argv);
		}
*/

#pragma once
#include <Frame/Frame_export.h>

#include <osgViewer/Viewer>
#include <osg/Group>
#include <osg/StateSet>

#include <memory>
#include <Frame/GraphicsPipeline.h>
#include <EffectCompositor/GraphicsPipeline/GraphicsPipeline.h>
namespace Frame
{
	class  FRAME_API App
	{
	public:
		App();

		// default init
		virtual  bool init(int argc, char** argv);
		// subclass master override initscene();
		virtual void initScene() = 0;
		virtual void initCamera() {};
		virtual int run();
		void setWindowSize(int w, int h)
		{
			m_h = h;
			m_w = w;
		}


		void addModel(osg::Node* pNode);
		void addRenderPass(osg::Camera* passNode);

		
	protected:
		bool _initCamera();
	protected:
		osg::ref_ptr<osg::Group> m_root;
		osg::ref_ptr<osg::Group> m_scene;
		///osg::ref_ptr<Frame::GraphicsPipeline> m_pipeline;
		osg::ref_ptr<Effect::GraphicsPipeline> m_pipeline;
		osg::ref_ptr<osgViewer::Viewer> m_viewer;
		int m_h;
		int m_w;
	};
}


template<class T> 
int runMain(int argc, char** argv)
{
	std::shared_ptr<T> app = std::make_shared<T>();
	app->init(argc, argv);
	return app->run();
}


