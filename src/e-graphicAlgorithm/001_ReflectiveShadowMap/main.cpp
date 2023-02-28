//--------------------------------------------------------------------------------------
// Copyright(c) longlongwaytogo.All rights reserved.
// longlongway2012@hotmail.com
//--------------------------------------------------------------------------------------

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

#include <common/common.h>

int main(int argc, char** argv)
{
    osg::ArgumentParser arguments(&argc, argv);
    osg::ref_ptr<osg::Node> root = osgDB::readRefNodeFiles(arguments);
    if (root == NULL)
    {
        osg::notify(osg::FATAL) << "Unable to load model from command line." << std::endl;
        std::string saveFilePath = Comm::GlobalVars::getInstance().getDataPath() + "/sponza/Scale300Sponza.osgb";
        if(osgDB::fileExists(saveFilePath))
            root = osgDB::readNodeFile(saveFilePath);
        else {
            std::string filePath = Comm::GlobalVars::getInstance().getDataPath() + "/sponza/Scale300Sponza.obj";
            root = osgDB::readNodeFile(filePath);
            osgDB::writeNodeFile(*root, saveFilePath);
        }
    }

   
    osgUtil::Optimizer optimizer;
    optimizer.optimize(root.get(), osgUtil::Optimizer::ALL_OPTIMIZATIONS | osgUtil::Optimizer::TESSELLATE_GEOMETRY);
    osg::DisplaySettings::instance()->setMinimumNumStencilBits(8);
#define USE_GL3 1
#if USE_GL3
//    configureShaders(root->getOrCreateStateSet());
//#else
 if (root)
    {
        osgUtil::ShaderGenVisitor shaderGen;
        shaderGen.assignUberProgram(root->getOrCreateStateSet());
         root->getOrCreateStateSet()->setDefine("GL_LIGHTING");
         root->getOrCreateStateSet()->setDefine("GL_TEXTURE_2D");
    }
#endif 
    const int width(1024), height(768);
    const std::string version("3.0");
    osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
    traits->x = 20; traits->y = 30;
    traits->width = width; traits->height = height;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->glContextVersion = version;
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
        return(1);
    }

#if USE_GL3

    gc->getState()->resetVertexAttributeAlias(false); // 保留兼容的布局

#endif 

    osgViewer::Viewer viewer;

    // Create a Camera that uses the above OpenGL context.
    osg::Camera* cam = viewer.getCamera();
    cam->setGraphicsContext(gc.get());
    // Must set perspective projection for fovy and aspect.
    cam->setProjectionMatrix(osg::Matrix::perspective(30., (double)width / (double)height, 1., 100.));
    // Unlike OpenGL, OSG viewport does *not* default to window dimensions.
    cam->setViewport(new osg::Viewport(0, 0, width, height));

    // add the state manipulator
    viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));

    // add the thread model handler
    viewer.addEventHandler(new osgViewer::ThreadingHandler);

    // add the window size toggle handler
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);

    // add the stats handler
    viewer.addEventHandler(new osgViewer::StatsHandler);

    viewer.setSceneData(root);

    // for non GL3/GL4 and non GLES2 platforms we need enable the osg_ uniforms that the shaders will use,
    // you don't need thse two lines on GL3/GL4 and GLES2 specific builds as these will be enable by default.
    gc->getState()->setUseModelViewAndProjectionUniforms(true);
    gc->getState()->setUseVertexAttributeAliasing(true);

    return(viewer.run());
}