//////////////////////////////////////////////////////////////////////////////////////////////////
//@file: GraphicsPipelineStageCallback.h
//@brief: 渲染阶段回调处理类
//@author: longlongwaytogo
//@date: 2021/02/07


#pragma once
#include <osg/NodeVisitor>
#include <osg/Referenced>
#include <GraphicsPipeline/GraphicsPipelinePass.h>
#include <initializer_list>
namespace Effect
{
    //class GraphicsPipelineStage;
    class GraphicsPipelineStage;
    class GraphicsPipelineStageCallback :public osg::Referenced
    {
    public:
      
        GraphicsPipelineStageCallback():m_bLoadXml(true)
        {
        }

        virtual ~GraphicsPipelineStageCallback()
        {
        }

		void setGraphicPipelineStage(GraphicsPipelineStage* stage);
		virtual bool init();
		virtual bool update();
        virtual void traverse(GraphicsPipelineStage* node,osg::NodeVisitor& nv);

    protected:
        GraphicsPipelineStage* _pStage;
		bool m_bLoadXml;
    };
	// for create from code 
	enum class ETextureAttachmentType
	{
		ColorTexture, // colorbuffer
		DepthTexture, // depthbuffer
		CubeDepthTexture, // cubemapBuffer
		StencilTexture,
		DepthAndStencilTexture,
		DepthArrayTexture
	};

#define ATTACHMENTTYPE "AttachMentType"
    class SceneStageCallback :public GraphicsPipelineStageCallback
    {
    public:
		osg::Camera* createNewPass(PassType type, const std::string& name);
		void setTextureAttachmentType(osg::Texture* texture, ETextureAttachmentType type)
		{
			texture->setUserValue(ATTACHMENTTYPE, (int)type);
		}
		ETextureAttachmentType getTextureAttatchmentType(osg::Texture* tex)
		{
			int nType =(int) ETextureAttachmentType::ColorTexture;
			tex->getUserValue(ATTACHMENTTYPE, nType);
			return (ETextureAttachmentType)nType;
		}
		void attachCamera(osg::Camera* camera,const std::initializer_list<osg::ref_ptr<osg::Texture>>& vioTextureAttachments);;
		void registerSharedData(const std::string& name, osg::Texture* tex);
		osg::Texture* getShaderedData(const std::string& name);

		virtual bool init() 
        {
            return true;
         }

          virtual void traverse(GraphicsPipelineStage* node,osg::NodeVisitor& nv)
          {
               __super::traverse(node,nv);
          }
    };


    // tiledShading
    class TiledShadingCallback: public GraphicsPipelineStageCallback
    {

    public:
        virtual bool init();

        virtual void traverse(GraphicsPipelineStage* node,osg::NodeVisitor& nv)
        {
            __super::traverse(node,nv);
        }
    };

    class DisplayStageCallback :public GraphicsPipelineStageCallback
    {
    public:

         virtual bool init()
        {
          return true;
        }

       virtual void traverse(GraphicsPipelineStage* node,osg::NodeVisitor& nv);
    };
}