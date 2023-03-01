//--------------------------------------------------------------------------------------
// Copyright(c) longlongwaytogo.All rights reserved.
// longlongway2012@hotmail.com
//--------------------------------------------------------------------------------------

#include <osgViewer/Viewer>
#include <osg/Group>
#include <osg/StateSet>

#include <Frame/Frame_export.h>
#include <memory>
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
		virtual int run();

		void addModel(osg::Node* pNode);
		void addRenderPass(osg::Camera* passNode);

		
	protected:
		bool initCamera();
	protected:
		osg::ref_ptr<osg::Group> m_root;
		osg::ref_ptr<osg::Group> m_scene;
		osg::ref_ptr<osg::Group> m_pipeline;
		osg::ref_ptr<osgViewer::Viewer> m_viewer;
	};
}


template<class T> 
int runMain(int argc, char** argv)
{
	std::shared_ptr<T> app = std::make_shared<T>();
	app->init(argc, argv);
	return app->run();
}
