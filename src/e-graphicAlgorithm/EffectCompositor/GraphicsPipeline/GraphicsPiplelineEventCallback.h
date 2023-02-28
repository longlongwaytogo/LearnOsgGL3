//////////////////////////////////////////////////////////////////////////
//@file: GraphicPipelineEventCallback.h
//@brief: 处理渲染管线更新事件等交互
//@author: longlongwaytogo
//@date: 2021/01/19

#ifndef  __EFFECT_GRAPHIC_PIPELINE_EVENT_CALLBACK_H__
#define __EFFECT_GRAPHIC_PIPELINE_EVENT_CALLBACK_H__

#include <osg/Callback>
#include <osg/NodeVisitor>
#include <osgGA/EventVisitor>
#include <osgGA/GUIActionAdapter>
#include <osgViewer/Viewer>


namespace Effect
{
    class  GraphicPipelineEventCallback: public osg::NodeCallback
    {
        /** Callback method called by the NodeVisitor when visiting a node.*/
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

        virtual bool handle(osg::Node* node,const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

    };

}
#endif // __EFFECT_GRAPHIC_PIPELINE_EVENT_CALLBACK_H__
