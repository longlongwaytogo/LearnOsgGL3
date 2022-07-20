// This is public domain software and comes with
// absolutely no warranty. Use of public domain software
// may vary between counties, but in general you are free
// to use and distribute this software for any purpose.


// Example: OSG using an OpenGL 3.0 context.
// The comment block at the end of the source describes building OSG
// for use with OpenGL 3.x.

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/GraphicsContext>
#include <osg/Camera>
#include <osg/Viewport>
#include <osg/StateSet>
#include <osg/Program>
#include <osg/Shader>
#include <osg/PolygonMode>
#include <osg/GL>
#include <osg/MatrixTransform>
#include <osg/Depth>
#include <osgUtil/Optimizer>

void configureShaderShowLine(osg::StateSet* stateSet)
{
    const std::string vs =
        "#version 330 \n"
        "uniform mat4 osg_ModelViewProjectionMatrix; \n"
        "uniform mat3 osg_NormalMatrix; \n"
        "in vec4 osg_Vertex; \n"
        "in vec3 osg_Normal; \n"
        "out vec4 color;\n"
        "void main() \n"
        "{ \n"
        "   vec3 ecNormal = normalize( osg_NormalMatrix * osg_Normal ); \n"
        "   gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; \n"
        "   vec4 pos = gl_Position/gl_Position.w;\n"
        // "   color =  vec4(pos.xyz,1.0);\n"
        "   color =  vec4(1.0,1.0,1.0,1.0);\n"
        "} \n";

    const std::string fs =
        "#version 330 \n"
        "in vec4 color;\n"
        "out vec4 fragData; \n"
        "void main() \n"
        "{ \n"
        "    fragData = color; \n"
        "} \n";

    if (stateSet)
    {
        osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX, vs);
        osg::Shader* fShader = new osg::Shader(osg::Shader::FRAGMENT, fs);
        osg::Program* pProgram = new osg::Program;
        pProgram->addShader(vShader);
        pProgram->addShader(fShader);
        stateSet->setAttribute(pProgram);
    }
}
void configureShaderShowNormal(osg::StateSet* stateSet)
{
    const std::string vs =
        "#version 330 \n"
        "uniform mat4 osg_ModelViewProjectionMatrix; \n"
        "uniform mat3 osg_NormalMatrix; \n"
        "in vec4 osg_Vertex; \n"
        "in vec3 osg_Normal; \n"
        "out vec4 color;\n"
        "void main() \n"
        "{ \n"
        "   vec3 ecNormal = normalize( osg_NormalMatrix * osg_Normal ); \n"
        "   gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; \n"
        "   vec4 pos = gl_Position/gl_Position.w;\n"
        // "   color =  vec4(pos.xyz,1.0);\n"
        "   color =  vec4(ecNormal,1.0);\n"
        "} \n";

    const std::string fs =
        "#version 330 \n"
        "in vec4 color;\n"
        "out vec4 fragData; \n"
        "void main() \n"
        "{ \n"
        "    fragData = color; \n"
        "} \n";
  
    if (stateSet)
    {
        osg::Shader* vShader = new osg::Shader(osg::Shader::VERTEX, vs);
        osg::Shader* fShader = new osg::Shader(osg::Shader::FRAGMENT, fs);
        osg::Program* pProgram = new osg::Program;
        pProgram->addShader(vShader);
        pProgram->addShader(fShader);
        stateSet->setAttribute(pProgram);
    }
}
void configureShaders( osg::StateSet* stateSet )
{
    const std::string vertexSource =
        "#version 330 \n"
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
    osg::Shader* vShader = new osg::Shader( osg::Shader::VERTEX, vertexSource );

    const std::string fragmentSource =
        "#version 330 \n"
        " \n"
        "in vec4 color; \n"
        "out vec4 fragData; \n"
        " \n"
        "void main() \n"
        "{ \n"
        "    fragData = color; \n"
        "} \n";
    osg::Shader* fShader = new osg::Shader( osg::Shader::FRAGMENT, fragmentSource );

    osg::Program* program = new osg::Program;
    program->addShader( vShader );
    program->addShader( fShader );
    stateSet->setAttribute( program );

    osg::Vec3f lightDir( 0., 0.5, 1. );
    lightDir.normalize();
    stateSet->addUniform( new osg::Uniform( "ecLightDir", lightDir ) );
}

osg::Camera* createNormalCamera()
{
    osg::Camera* pCamera = new osg::Camera;
    pCamera->setRenderOrder(osg::Camera::POST_RENDER);
    pCamera->setClearMask(0); // 不清除深度缓存和颜色缓存

    const std::string vs =
        "#version 330 \n"
        "uniform mat4 osg_ModelViewProjectionMatrix; \n"
        "uniform mat3 osg_NormalMatrix; \n"
        "in vec4 osg_Vertex; \n"
        "in vec3 osg_Normal; \n"
        "out vec3 local_normal;\n"
        "void main() \n"
        "{ \n"
        "   local_normal = osg_Normal; \n"
        "   gl_Position =  osg_Vertex; \n"
        "} \n";
    
    const std::string gs =

        "#version 330\n"
        "uniform mat4 osg_ModelViewProjectionMatrix;\n"
        "uniform mat3 osg_NormalMatrix;\n"
        "uniform float normal_length;\n"
        "in vec3 local_normal[];\n"
        "out vec4 color;\n"
        "layout(triangles) in;\n"
        "layout(line_strip, max_vertices = 6) out;\n"
        " void main()\n"
        "{\n"
        "   for (int i = 0; i < gl_in.length(); i++)\n"
        "   {\n"
        "       vec4 P0 = gl_in[i].gl_Position; \n"
        "            gl_Position = osg_ModelViewProjectionMatrix * P0; \n"
        "            color = vec4(0,1,0,1); \n"
        "            EmitVertex(); \n"
        "           vec4 N = vec4(local_normal[i],0); \n"
        "            vec4 P1 = gl_in[i].gl_Position + N * 1.0; \n"
        "        gl_Position = osg_ModelViewProjectionMatrix * P1; \n"
        "        color = vec4(1,0,0,1); \n"
        "        EmitVertex();\n"
        "   }\n"
        "  }\n";

    const std::string fs =
        "#version 330 \n"
        "in vec4 color;\n"
        "out vec4 fragData; \n"
        "void main() \n"
        "{ \n"
        "    fragData = color; \n"
        "} \n";
    osg::StateSet* pSS =  pCamera->getOrCreateStateSet();
    if (pSS)
    {
        osg::ref_ptr<osg::Shader> vShader = new osg::Shader(osg::Shader::VERTEX, vs);
        osg::ref_ptr<osg::Shader> gShader = new osg::Shader(osg::Shader::GEOMETRY, gs);
        gShader = osgDB::readRefShaderFile(shaderPath + std::string("/normal.geom"));
        osg::ref_ptr<osg::Shader> fShader = new osg::Shader(osg::Shader::FRAGMENT, fs);
        osg::Program* pProgram = new osg::Program;
        pProgram->addShader(vShader);
       
        pProgram->addShader(gShader);
        pProgram->addShader(fShader);
        pSS->setAttribute(pProgram);
    }

    return pCamera;
}
int main( int argc, char** argv )
{
    osg::setNotifyLevel(osg::NOTICE);
    //osg::setNotifyHandler(new osg::WinDebugNotifyHandler);
    osg::ArgumentParser arguments( &argc, argv );
    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<osg::Node> model = osgDB::readRefNodeFiles( arguments );
    if( root == NULL )
    {
        osg::notify( osg::FATAL ) << "Unable to load model from command line." << std::endl;
        return( 1 );
    }
    osgUtil::Optimizer optimizer;
    optimizer.optimize(root.get(), osgUtil::Optimizer::ALL_OPTIMIZATIONS  | osgUtil::Optimizer::TESSELLATE_GEOMETRY);

    const int width( 800 ), height( 450 );
    const std::string version( "3.3" );
    osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
    traits->x = 20; traits->y = 30;
    traits->width = width; traits->height = height;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->glContextVersion = version;
    traits->glContextProfileMask = GL_CONTEXT_CORE_PROFILE_BIT;// 0x1;// 
    traits->readDISPLAY();
    traits->setUndefinedScreenDetailsToDefaultScreen();
    osg::ref_ptr< osg::GraphicsContext > gc = osg::GraphicsContext::createGraphicsContext( traits.get() );
    if( !gc.valid() )
    {
        osg::notify( osg::FATAL ) << "Unable to create OpenGL v" << version << " context." << std::endl;
        return( 1 );
    }

    gc->getState()->resetVertexAttributeAlias(false);
    osgViewer::Viewer viewer;

    // Create a Camera that uses the above OpenGL context.
    osg::Camera* cam = viewer.getCamera();
    cam->setGraphicsContext( gc.get() );
    // Must set perspective projection for fovy and aspect.
    cam->setProjectionMatrix( osg::Matrix::perspective( 30., (double)width/(double)height, 1., 1000. ) );
    // Unlike OpenGL, OSG viewport does *not* default to window dimensions.
    cam->setViewport( new osg::Viewport( 0, 0, width, height ) );
  
    osg::Camera* pNormalCamera = createNormalCamera();
    osg::MatrixTransform* scribe = new osg::MatrixTransform;
    scribe->setMatrix(osg::Matrix::scale(1.02, 1.02, 1.02));
    scribe->addChild(model);
    scribe->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE),osg::StateAttribute::OVERRIDE);
    scribe->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::LEQUAL,0.0,1.0,false));
    root->addChild(model);
    root->addChild(scribe);
    float normal_length = 0.5f;
    root->getOrCreateStateSet()->addUniform(new osg::Uniform("normal_length", normal_length));
    configureShaderShowNormal(root->getOrCreateStateSet());
    configureShaderShowLine(scribe->getOrCreateStateSet());
    model->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL));
    root->addChild(pNormalCamera);
    pNormalCamera->addChild(model);
 
   
 
    viewer.setSceneData( root );

    return( viewer.run() );
}

