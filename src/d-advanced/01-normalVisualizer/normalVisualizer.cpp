//--------------------------------------------------------------------------------------
// Copyright (c) longlongwaytogo. All rights reserved.
// longlongway2012@hotmail.com
//--------------------------------------------------------------------------------------

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
#include <osg/Switch>
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
        "uniform mat4 osg_ViewMatrixInverse;\n"
        "uniform mat4 osg_ModelViewMatrix;\n"
        "in vec4 osg_Vertex; \n"
        "in vec3 osg_Normal; \n"
        "out vec4 color;\n"
        "void main() \n"
        "{ \n"
        "   vec3 ecNormal;// = normalize( osg_NormalMatrix * osg_Normal ); \n"
        "   gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex; \n"
        //"   vec4 pos = gl_Position/gl_Position.w;\n"
        // "   color =  vec4(pos.xyz,1.0);\n"
        "   mat4 modelMatrix = osg_ViewMatrixInverse * osg_ModelViewMatrix;\n"
        "   ecNormal = mat3(modelMatrix) * osg_Normal;\n"
        "   ecNormal = ecNormal*0.5 + 0.5;\n"
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
enum ShowNormalLineType
{
    VERTEX_LINE     = 1, // 顶点法线
    VERTEX_LINE_AVG = 2, // 平均法线
    FACE_LINE       = 4, // 面法线
};
enum {
    ROOT_NODE,
    ORIGIN_NODE,
    SWITCH_NODE,// 原始node和颜色法线
    LINES_NODE, // 线框模型
    NORMAL_LINE_NODE, // 法线显示节点

};
class Switcher : public osgGA::GUIEventHandler
{
public:

    std::map<int, osg::ref_ptr<osg::Node>> _lists;
    virtual bool handle(osgGA::Event* event, osg::Object* object, osg::NodeVisitor* nv)
    {
        if (osgGA::GUIEventAdapter* ea = dynamic_cast<osgGA::GUIEventAdapter*>(event))
        {
            if (ea->getEventType() == osgGA::GUIEventAdapter::KEYUP)
            {
                switch (ea->getKey())
                {
                case osgGA::GUIEventAdapter::KEY_F2: // 原始模型效果
                {
                    static int nShowMode = 0;
                    nShowMode++;
                    switch (nShowMode % 3)
                    {
                        case 0:
                        {
                            _lists[SWITCH_NODE]->asSwitch()->setSingleChildOn(0);
                            break;
                        }
                        case 1:
                        {
                            _lists[SWITCH_NODE]->asSwitch()->setSingleChildOn(1);
                            break;
                        }
                        case 2:
                        {
                            _lists[SWITCH_NODE]->asSwitch()->setAllChildrenOff();
                            break;
                        }
                    }
                    break;
                }
                case osgGA::GUIEventAdapter::KEY_F3: // line strip
                {
                    static bool bShowLine = true;
                    bShowLine = !bShowLine;
                    if (bShowLine)
                        _lists[LINES_NODE]->setNodeMask(~0u);
                    else
                        _lists[LINES_NODE]->setNodeMask(0u);
                    break;
                }
                case osgGA::GUIEventAdapter::KEY_F4: // 顶点法线
                {

                    static int nShowNormalMode = 0;
                    nShowNormalMode++;

                    _lists[NORMAL_LINE_NODE]->setNodeMask(~0u);
                    osg::StateSet* pSS = _lists[NORMAL_LINE_NODE]->getOrCreateStateSet();
                    switch (nShowNormalMode % 5)
                    {
                    case 0:
                    {
                        int show_normal_mode = VERTEX_LINE;
                        pSS->getUniform("show_normal_mode")->set(show_normal_mode);
                        break;
                    }

                    case 1:
                    {
                        int show_normal_mode = VERTEX_LINE_AVG;
                        pSS->getUniform("show_normal_mode")->set(show_normal_mode);
                        break;
                    }
                    case 2:
                    {
                        int show_normal_mode = FACE_LINE;
                        pSS->getUniform("show_normal_mode")->set(show_normal_mode);

                        break;
                    }
                    case 3:
                    {
                        int show_normal_mode = VERTEX_LINE | VERTEX_LINE_AVG | FACE_LINE;
                        pSS->getUniform("show_normal_mode")->set(show_normal_mode);
                        break;
                    }
                    case 4:
                    {
                        _lists[NORMAL_LINE_NODE]->setNodeMask(0u);
                        break;
                    }
                    default:
                        break;
                    }
                    break;
                }
               
                default:
                    break;
                }
            }

        }
        return false;
    }
};
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

    const int width( 1024 ), height( 768 );
    const std::string version( "3.3" );
    osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
    traits->x = 0; traits->y = 0;
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
  
    model->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, 
        osg::PolygonMode::FILL));

    osg::Switch* pSwitchNode = new osg::Switch;
    osg::MatrixTransform* original = new osg::MatrixTransform; // 原始效果
    osg::MatrixTransform* colorNormalNode = new osg::MatrixTransform; // 颜色法线
    original->addChild(model);
    colorNormalNode->addChild(model);
    configureShaders(original->getOrCreateStateSet());
    configureShaderShowNormal(colorNormalNode->getOrCreateStateSet());
    pSwitchNode->addChild(original);
    pSwitchNode->addChild(colorNormalNode);

    osg::MatrixTransform* scribe = new osg::MatrixTransform; // 线框显示模式
    scribe->addChild(model);
    scribe->setMatrix(osg::Matrix::scale(1.02, 1.02, 1.02));
    scribe->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, 
        osg::PolygonMode::LINE), osg::StateAttribute::OVERRIDE);
    scribe->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::LEQUAL, 0.0, 1.0, false));
    configureShaderShowLine(scribe->getOrCreateStateSet());

    osg::Camera* pNormalCamera = createNormalCamera();
    float normal_length = 0.4f;
    pNormalCamera->getOrCreateStateSet()->addUniform(new osg::Uniform("normal_length", normal_length));
    int show_normal_mode = 0;
    pNormalCamera->getOrCreateStateSet()->addUniform(new osg::Uniform("show_normal_mode", show_normal_mode));
    pNormalCamera->addChild(model);

    root->addChild(pSwitchNode);
    root->addChild(scribe);
    root->addChild(pNormalCamera);
   
    osg::ref_ptr<Switcher> swr = new Switcher;
    swr->_lists[ORIGIN_NODE] = model;
    swr->_lists[ROOT_NODE] = root;
    swr->_lists[LINES_NODE] = scribe;
    swr->_lists[SWITCH_NODE] = pSwitchNode;
    swr->_lists[NORMAL_LINE_NODE] = pNormalCamera;
 
    viewer.setSceneData( root );
    viewer.addEventHandler(swr);
    return( viewer.run() );
}

