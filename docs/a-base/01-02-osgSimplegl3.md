# osgSimplegl3结合RenderDoc工具

	author: longlongwaytogo
	date: 2022/07/12	
	email: longlongway2012@hotmail.com		
	repo: https://github.com/longlongwaytogo/LearnOsgGL3

# 简介
前面完成了osg GL3环境的搭建,这里主要分析下osg下如何结合gl3进行开发工作。

# 例子详解
- 创建osg gl3环境
```cpp
        const int width( 800 ), height( 450 );
        const std::string version( "3.0" );
        osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
        traits->x = 20; traits->y = 30;
        traits->width = width; traits->height = height;
        traits->windowDecoration = true;
        traits->doubleBuffer = true;
        traits->glContextVersion = version;
        traits->readDISPLAY();
        traits->setUndefinedScreenDetailsToDefaultScreen();
        osg::ref_ptr< osg::GraphicsContext > gc = osg::GraphicsContext::createGraphicsContext( traits.get() );
        if( !gc.valid() )
        {
            osg::notify( osg::FATAL ) << "Unable to create OpenGL v" << version << " context." << std::endl;
            return( 1 );
        }
```

以上代码主要针对osg::GraphicsContext::traits对象进行参数设置，比较关键的是指定gl版本为3.0。

 - 将traits参数应用到相机：
 ```cpp
    osg::Camera* cam = viewer.getCamera();
    cam->setGraphicsContext( gc.get() );
```

- 将shader参数应用给场景根节点
```cpp
    void configureShaders( osg::StateSet* stateSet )
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
        osg::Shader* vShader = new osg::Shader( osg::Shader::VERTEX, vertexSource );

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
        osg::Shader* fShader = new osg::Shader( osg::Shader::FRAGMENT, fragmentSource );

        osg::Program* program = new osg::Program;
        program->addShader( vShader );
        program->addShader( fShader );
        stateSet->setAttribute( program );

        osg::Vec3f lightDir( 0., 0.5, 1. );
        lightDir.normalize();
        stateSet->addUniform( new osg::Uniform( "ecLightDir", lightDir ) );
    }
  ```

  以上代码，使用的shader明显比较老，真正使用最好使用3.3以后的版本声明比较好，这样可以使用renderdoc进行抓帧调试（renderdoc的使用在后面的文章中会详细介绍）。


- 设置投影矩阵和顶点属性引用，但这句代码只在gl3以前的版本中需要，真正开发gl3版本的osg程序不需要以下代码
  
```cpp
   // for non GL3/GL4 and non GLES2 platforms we need enable the osg_ uniforms that the shaders will use,
    // you don't need thse two lines on GL3/GL4 and GLES2 specific builds as these will be enable by default.
    gc->getState()->setUseModelViewAndProjectionUniforms(true);
    gc->getState()->setUseVertexAttributeAliasing(true);
```
- 总结：
  
虽然osgSimplegl3例子是演示gl3特性的，但它编写的环境是在非完全开启GL3核心模式下运行，即运行在gl3的兼容模式下，这样可以运行version版本130的shader代码。

该例子只是为了简单演示如何运行gl3的环境。对osg编译没有要求，即使osg在编译设置中没有设置gl shader版本为高版本，也可以使用高版本的shader特性。但建议还是在设置gl3的环境下运行gl3程序，主要原因如下：

  - 第一：可以使用高版本的特性；
  - 第二：可以有更高效的api接口，可以提升性能；
  - 第三：可以使用第三方调试工具（renderdoc、gpa、NSight等）调试错误。下一篇文章，将讲解osg和gl 3.3以上版本的配置。
