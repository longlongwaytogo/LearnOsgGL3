#include "GraphicsPipeline/GraphicsPipeline.h"
#include "GraphicsPipeline/GraphicsPipelineStageCallback.h"
#include "GraphicsPipeline/GraphicsPiplelineEventCallback.h"


#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/XmlParser>
#include <common/common.h>

namespace Effect
{
    GraphicsPipeline::GraphicsPipeline()
    {
        _renderTargetResolution = osg::Vec3(1024,768,0);
		
		
    }

    GraphicsPipeline::GraphicsPipeline(const GraphicsPipeline& copy, const osg::CopyOp& copyop/*=osg::CopyOp::SHALLOW_COPY */)
        :EffectCompositor(copy,copyop)
    {
        m_stages = copy.m_stages;
    }

    void GraphicsPipeline::traverse(osg::NodeVisitor& nv)
     {
          for(auto itr = m_stages.begin(); itr != m_stages.end(); itr++)
         {
          
             if(itr->second)
                 itr->second->traverse(nv);
         }
     }

     void GraphicsPipeline::init()
     {
         bool bOk = false;
         // read prexml
         
        
        std::string path = Comm::GlobalVars::getInstance().getDataPath();
        std::string preXmlPath = path +  "/GFX/PostEffectProcessors/AllPreDef.xml";
        osgDB::FilePathList& filePaths = osgDB::getDataFilePathList();
        filePaths.push_back(osgDB::getFilePath(path));
        filePaths.push_back(osgDB::getFilePath(path + "/GFX/PostEffectProcessors/"));
        osg::ref_ptr<osgDB::XmlNode> preDefXmlRoot = loadXML(preXmlPath);
        bOk = preDefXmlRoot.valid();

         // create stages
        /*
         SceneGbuffer->tildShading ->postEffect ->toneMaping->dispaly->debug
        */
        // register statge and callback
         registerStage<SceneStageCallback>(STAGE_SCENE_BUFFER,"SceneGBuffer");
         registerStage<TiledShadingCallback>(STAGE_TILEDSHADING,"TiledShading");
         registerStage<SceneStageCallback>(STAGE_TONE_MAPPING,"ToneMapping");
         registerStage<DisplayStageCallback>(STAGE_DISPLAY,"Display");

         // for each stage
         for(auto itr = m_stages.begin(); itr != m_stages.end(); itr++)
         {
            /* if(*itr)
                 (*itr)->init();*/
             if(itr->second)
                 itr->second->init();
         }

         addEventCallback(new GraphicPipelineEventCallback());
     }

     void GraphicsPipeline::resize(int renderWidth, int renderHeight)
     {
        setRenderTargetResolution(osg::Vec3(renderWidth,renderHeight,0.0));
     }

    osg::ref_ptr<osgDB::XmlNode> GraphicsPipeline::loadXML( const std::string& filename, const osgDB::Options* options )
    {
		if(!options)
			options = _loadOptions.get();
    
        bool ret = false;
        osg::ref_ptr<osgDB::XmlNode> xmlRoot = osgDB::readXmlFile( filename, options );
        if ( xmlRoot.valid() )
        {
           ret = loadFromXML( xmlRoot.get(), options );
        }
        return xmlRoot;
    }

    bool GraphicsPipeline::reLoadFromXmlNode()
    {
        bool bRet = false;
       

        for(auto itr  = m_stages.begin(); itr != m_stages.end(); ++itr)
        {
            if(itr->second)
               bRet &= itr->second->init();
        }

        return bRet;
    }
//    osg::Camera* GraphicsPipeline::createNewPass( PassType type, const std::string& name,GraphicsPipelineStage* stage )
//    {
//    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
//    camera->setName( name );
//    camera->setClearMask( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
//    camera->setRenderOrder( osg::Camera::PRE_RENDER );
//    camera->setRenderTargetImplementation( _renderTargetImpl );
//    camera->setCullCallback( new PassCullCallback(this, type) );
//    
//    if ( type==DEFERRED_PASS )
//    {
//        // Deferred pass is absolutely facing the XOY plane in the range of [0, 1]
//        camera->setAllowEventFocus( false );
//        camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
//        camera->setProjectionMatrix( osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0) );
//        camera->setViewMatrix( osg::Matrix::identity() );
//        camera->addChild( createScreenQuad(1.0, 1.0) );
//    }
//    else
//    {
//        // Forward pass will follow the scene navigation result
//        camera->setReferenceFrame( osg::Transform::RELATIVE_RF );
//    }
//    
//    PassData newData;
//    newData.activated = true;
//    newData.type = type;
//    newData.name = name;
//    newData.pass = camera;
//    
//    if(stage)
//    {
//        stage->addPass(newData);
//    }
//    else
//    {
//         getPassList().push_back( newData );
//    }
//   
//    return camera.get();
//}

}