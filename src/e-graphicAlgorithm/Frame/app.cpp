
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
        m_root = new osg::Group;
        m_scene = new osg::Group;
        m_pipeline = new osg::Group;
        m_viewer = new osgViewer::Viewer;
    }
    bool App::init(int argc, char** argv)
    {
        osg::ArgumentParser arguments(&argc, argv);
        osg::ref_ptr<osg::Node> model = osgDB::readRefNodeFiles(arguments);
        if (model == NULL)
        {
            osg::notify(osg::FATAL) << "Unable to load model from command line." << std::endl;
            model = Frame::getDefaultModel();
            if(model)
                m_scene->addChild(model);
        }
        m_root->addChild(m_pipeline);
        m_pipeline->addChild(m_scene);
        return initCamera();
    }
    int App::run()
    {
        return(m_viewer->run());
    }

    bool App::initCamera()
    {
        osgUtil::Optimizer optimizer;
        optimizer.optimize(m_root.get(), osgUtil::Optimizer::ALL_OPTIMIZATIONS | osgUtil::Optimizer::TESSELLATE_GEOMETRY);
        osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);

#if USE_GL3
        //    configureShaders(root->getOrCreateStateSet());
        //#else
        if (m_root)
        {
            osgUtil::ShaderGenVisitor shaderGen;
            shaderGen.assignUberProgram(m_root->getOrCreateStateSet());
            m_root->getOrCreateStateSet()->setDefine("GL_LIGHTING");
            m_root->getOrCreateStateSet()->setDefine("GL_TEXTURE_2D");
        }
#endif 
        const int width(1024), height(768);
        const std::string version("4.3");
        osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
        traits->x = 20; traits->y = 30;
        traits->width = width; traits->height = height;
        traits->windowDecoration = true;
        traits->doubleBuffer = true;
        traits->glContextVersion = version;
#if USE_GL3
        // ʹ��coreģʽ��������ͼ������ɫ�Ǻ�ɫ��
        // ʹ�ü���ģʽ����������ͼ,�ǻ�ɫ����
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
        gc->getState()->resetVertexAttributeAlias(false); // �������ݵĲ���
#endif 
    // Create a Camera that uses the above OpenGL context.
        osg::Camera* cam = m_viewer->getCamera();
        cam->setGraphicsContext(gc.get());
        // Must set perspective projection for fovy and aspect.
        cam->setProjectionMatrix(osg::Matrix::perspective(30., (double)width / (double)height, 1., 100.));
        // Unlike OpenGL, OSG viewport does *not* default to window dimensions.
        cam->setViewport(new osg::Viewport(0, 0, width, height));

        // add the state manipulator
        m_viewer->addEventHandler(new osgGA::StateSetManipulator(m_viewer->getCamera()->getOrCreateStateSet()));

        // add the thread model handler
        m_viewer->addEventHandler(new osgViewer::ThreadingHandler);

        // add the window size toggle handler
        m_viewer->addEventHandler(new osgViewer::WindowSizeHandler);

        // add the stats handler
        m_viewer->addEventHandler(new osgViewer::StatsHandler);

        m_viewer->setSceneData(m_root.get());

        // for non GL3/GL4 and non GLES2 platforms we need enable the osg_ uniforms that the shaders will use,
        // you don't need thse two lines on GL3/GL4 and GLES2 specific builds as these will be enable by default.
        gc->getState()->setUseModelViewAndProjectionUniforms(true);
        gc->getState()->setUseVertexAttributeAliasing(true);

        initScene();
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