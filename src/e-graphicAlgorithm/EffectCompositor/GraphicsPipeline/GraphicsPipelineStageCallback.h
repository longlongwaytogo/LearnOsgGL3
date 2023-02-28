//////////////////////////////////////////////////////////////////////////////////////////////////
//@file: GraphicsPipelineStageCallback.h
//@brief: 渲染阶段回调处理类
//@author: longlongwaytogo
//@date: 2021/02/07


#pragma once
#include <osg/NodeVisitor>
#include <osg/Referenced>

namespace Effect
{
    //class GraphicsPipelineStage;
    class GraphicsPipelineStage;
    class GraphicsPipelineStageCallback :public osg::Referenced
    {
    public:
      
        GraphicsPipelineStageCallback()
        {
        }

        virtual ~GraphicsPipelineStageCallback()
        {
        }

        void setGraphicPipelineStage(GraphicsPipelineStage* stage)
        {
            _pStage = stage;
        }
        virtual bool init()
        {
            return true;
        }

        virtual void traverse(GraphicsPipelineStage* node,osg::NodeVisitor& nv);

    protected:
        GraphicsPipelineStage* _pStage;
    };

    class SceneStageCallback :public GraphicsPipelineStageCallback
    {
    public:

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