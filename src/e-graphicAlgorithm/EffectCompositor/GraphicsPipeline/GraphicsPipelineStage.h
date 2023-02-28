//////////////////////////////////////////////////////////////////////////
//@file: GraphicPipelineStage.h
//@brif: 渲染管线阶段（步骤），包含多个渲染流程，用于分解渲染的不同流程
//       一个渲染阶段，即一个技术（technique），一个技术包含多个渲染通道（pass）
//      一个pass对应一个camera渲染，可以场景渲染，也可以是后处理渲染等
//@author: longlongwaytogo
//@date: 2021/01/20


#pragma once
#include <memory>
#include <string>
#include <osg/NodeVisitor>
#include <osgDB/XmlParser>

#include "EffectCompositor_export.h"
#include "GraphicsPipeline/GraphicsPipePass.h"
#include "GraphicsPipeline/GraphicsPipelineStageCallback.h"
namespace Effect
{
    class GraphicsPipeline;
    class EFFECTCOMPOSITOR_API GraphicsPipelineStage :public osg::Referenced
    {
    public:
          friend class GraphicsPipelineStageCallback;
        GraphicsPipelineStage(GraphicsPipeline* pipeline,int id,const std::string name,
        GraphicsPipelineStageCallback* callback=nullptr)
            :m_pipeline(pipeline),m_id(id),m_name(name),m_callback(callback)
        {
            if(m_callback)
            {
                m_callback->setGraphicPipelineStage(this);
            }
        }
        virtual bool init();

        virtual void traverse(osg::NodeVisitor& nv);

        bool addPass(PassData& passData);

        virtual bool getPassData( const std::string& name, PassData& data ) const;

        PassList& getPassList();
        GraphicsPipeline* getGraphicsPipeline(){ return m_pipeline;}
   
    protected:
       // GraphicsPipelinePass m_pass;
        int m_id;
        std::string m_name;
        GraphicsPipeline* m_pipeline;
        osg::ref_ptr<osgDB::XmlNode> _xmlRoot;
        osg::ref_ptr<GraphicsPipelineStageCallback> m_callback;
        PassList m_passList;
    };
}