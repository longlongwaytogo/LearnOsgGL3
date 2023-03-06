#pragma once

#include <osg/NodeCallback>
#include <osgUtil/CullVisitor>
#include "EffectCompositor"
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Effect
{

    class PassCullCallback : public osg::NodeCallback
    {
    public:
        PassCullCallback( EffectCompositor* compositor, PassType type );

        virtual void operator()( osg::Node* node, osg::NodeVisitor* nv );

    protected:
        osg::observer_ptr<EffectCompositor> _compositor;
        PassType _type;
    };

  /*  class GraphicPassCallback :public osg::NodeCallback
    {

    public: 
        GraphicPassCallback(GraphicsPipeline* graphicsPipeline,PassType type);

        virtual void operator()(osg::Node* node,osg::NodeVisitor* nv);

    private:
        osg::observer_ptr<GraphicsPipeline> _graphicPipeline;
        PassType _type;
    };
*/
}