//--------------------------------------------------------------------------------------
// Copyright (c) longlongwaytogo. All rights reserved.
// longlongway2012@hotmail.com
//--------------------------------------------------------------------------------------

#include <osg/Notify>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osg/GraphicsContext>
#include <osg/Camera>
#include <osg/Viewport>
#include <osg/StateSet>
#include <osg/Program>
#include <osg/Shader>
#include <osgUtil/Optimizer>
#include <osgUtil/ShaderGen>

#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <EffectCompositor/GraphicsPipeline/CustomPipeline.h>
#include <EffectCompositor/GLMarker.h>
#include "app.h"
#include "utils.h"
#define USE_GL3 1

namespace Frame
{
    void configureShaders(osg::StateSet* stateSet)
    {
        const std::string vertexSource =
            "#version 130 \n"
            " \n"
            "uniform mat4 osg_ModelViewProjectionMatrix; \n"
            "uniform mat3 osg_NormalMatrix; \n"
            "uniform vec3 ecLightDir; \n"
            " \n"
            "in vec4 osg_Vertex; \n"
            "in vec3 osg_Normal; \n"
            "out vec4 color; \n"
            " \n"
            "void main() \n"
            "{ \n"
            "    vec3 ecNormal = normalize( osg_NormalMatrix * osg_Normal ); \n"
            "    float diffuse = max( dot( ecLightDir, ecNormal ), 0. ); \n"
            "    color = vec4( vec3( diffuse ), 1. ); \n"
            " \n"
            "    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; \n"
            "} \n";
        osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX, vertexSource);

        const std::string fragmentSource =
            "#version 130 \n"
            " \n"
            "in vec4 color; \n"
            "out vec4 fragData; \n"
            " \n"
            "void main() \n"
            "{ \n"
            "    fragData = color; \n"
            "} \n";
        osg::Shader* fShader = new osg::Shader(osg::Shader::FRAGMENT, fragmentSource);

        osg::Program* program = new osg::Program;
        program->addShader(vShader);
        program->addShader(fShader);
        stateSet->setAttribute(program);

        osg::Vec3f lightDir(0., 0.5, 1.);
        lightDir.normalize();
        stateSet->addUniform(new osg::Uniform("ecLightDir", lightDir));
    }

    App::App()
    {
		m_w = 1024;
		m_h = 768;
        m_root = new osg::Group;
        m_scene = new osg::Group;
		m_pipeline = new Effect::CustomPipeline;// new Frame::GraphicsPipeline;
        m_viewer = new osgViewer::Viewer;
    }
    bool App::init(int argc, char** argv)
    {
        m_root->addChild(m_pipeline);
        m_pipeline->addChild(m_scene);

		osg::setNotifyLevel(osg::NotifySeverity::ALWAYS);
        initScene();

        if (m_scene->getNumChildren() == 0)
        {
            osg::ArgumentParser arguments(&argc, argv);
            osg::ref_ptr<osg::Node> model = osgDB::readRefNodeFiles(arguments);
            if (model == NULL)
            {
                osg::notify(osg::FATAL) << "Unable to load model from command line." << std::endl;
                model = Frame::getDefaultModel();
                if (model)
                    m_scene->addChild(model);
            }
        }
       
		m_pipeline->init();
         _initCamera();
		 initCamera();
		 return true;
    }
    int App::run()
    {
        return(m_viewer->run());
    }

    bool App::_initCamera()
    {
        osgUtil::Optimizer optimizer;
        optimizer.optimize(m_root.get(), osgUtil::Optimizer::ALL_OPTIMIZATIONS | osgUtil::Optimizer::TESSELLATE_GEOMETRY);
        osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);

#if USE_GL3
        //    configureShaders(root->getOrCreateStateSet());
        //#else
        if (m_root)
        {
           /* osgUtil::ShaderGenVisitor shaderGen;
            shaderGen.assignUberProgram(m_root->getOrCreateStateSet());
            m_root->getOrCreateStateSet()->setDefine("GL_LIGHTING");
            m_root->getOrCreateStateSet()->setDefine("GL_TEXTURE_2D");*/
        }
#endif 
        const int width(m_w), height(m_h);
        const std::string version("4.5");
        osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
        traits->x = 20; traits->y = 30;
        traits->width = width; traits->height = height;
        traits->windowDecoration = true;
        traits->doubleBuffer = true;
       // traits->glContextVersion = version;
#if USE_GL3
        // 使用core模式，输出深度图，背景色是红色的
        // 使用兼容模式，输出的深度图,是灰色渐变
        traits->glContextProfileMask = GL_CONTEXT_CORE_PROFILE_BIT;// 0x1;// 
       // traits->glContextProfileMask = 0x1;// GL_CONTEXT_COMPATIBILITY_PROFILE_BIT;// 0x1;// 
#endif 
        traits->readDISPLAY();
        traits->setUndefinedScreenDetailsToDefaultScreen();
        osg::ref_ptr< osg::GraphicsContext > gc = osg::GraphicsContext::createGraphicsContext(traits.get());
        if (!gc.valid())
        {
            osg::notify(osg::FATAL) << "Unable to create OpenGL v" << version << " context." << std::endl;
            return false;
        }

#if USE_GL3
        gc->getState()->resetVertexAttributeAlias(false); // 保留兼容的布局
#endif 
    // Create a Camera that uses the above OpenGL context.
        osg::Camera* cam = m_viewer->getCamera();
		cam->setName("MainCameraPass");
        cam->setGraphicsContext(gc.get());
        // Must set perspective projection for fovy and aspect.
        cam->setProjectionMatrix(osg::Matrix::perspective(45., (double)width / (double)height, 1., 100.));
        // Unlike OpenGL, OSG viewport does *not* default to window dimensions.
        cam->setViewport(new osg::Viewport(0, 0, width, height));

		Effect::registerCameraMarker(cam);
        // add the state manipulator
		/*
			_keyEventToggleBackfaceCulling('b'),
			_keyEventToggleLighting('l'),
			_keyEventToggleTexturing('t'),
			_keyEventCyclePolygonMode('w')
		*/
		auto stateSetManipulator = new osgGA::StateSetManipulator(m_viewer->getCamera()->getOrCreateStateSet());
		stateSetManipulator->setKeyEventCyclePolygonMode('p'); // plygon
		m_viewer->addEventHandler(stateSetManipulator);

        // add the thread model handler
        m_viewer->addEventHandler(new osgViewer::ThreadingHandler);

        // add the window size toggle handler
        //m_viewer->addEventHandler(new osgViewer::WindowSizeHandler);

        // add the stats handler
		auto statsHandler = new osgViewer::StatsHandler;
		statsHandler->setKeyEventTogglesOnScreenStats('o');
        m_viewer->addEventHandler(statsHandler);

        m_viewer->setSceneData(m_root.get());

		//m_viewer->setRunMaxFrameRate(120.0);
		//m_viewer->setRunFrameScheme(osgViewer::ViewerBase::ON_DEMAND);
        // for non GL3/GL4 and non GLES2 platforms we need enable the osg_ uniforms that the shaders will use,
        // you don't need thse two lines on GL3/GL4 and GLES2 specific builds as these will be enable by default.
        gc->getState()->setUseModelViewAndProjectionUniforms(true);
        gc->getState()->setUseVertexAttributeAliasing(true);

      
        return  true;
    }

    void App::addModel(osg::Node* pNode)
    {
        m_scene->addChild(pNode);

    }
    void App::addRenderPass(osg::Camera* passNode)
    {
        m_pipeline->addChild(passNode);
    }
}