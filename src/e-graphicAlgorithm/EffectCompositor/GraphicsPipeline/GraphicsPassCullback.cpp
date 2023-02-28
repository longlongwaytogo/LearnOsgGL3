#include "GraphicsPipeline/GraphicsPassCullback.h"


void Effect::PassCullCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    osg::Camera* camera = static_cast<osg::Camera*>( node );
    if ( !camera || !nv )
    {
        traverse( node, nv );
        return;
    }

    if ( nv->getVisitorType()==osg::NodeVisitor::CULL_VISITOR )
    {
        osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>(nv);
        if ( _type== FORWARD_PASS )
        {
            // Forward pass will traverse the scene normally
            _compositor->osg::Group::traverse( *nv );

            // We obtain the actual near/far values at the end of forward pass traversing
            double znear = cv->getCalculatedNearPlane();
            double zfar = cv->getCalculatedFarPlane();
            osg::Matrixd projection = *(cv->getProjectionMatrix());
            cv->clampProjectionMatrix( projection, znear, zfar );

            const osg::FrameStamp* fs = cv->getFrameStamp();
            if ( fs ) _compositor->setPreservedNearAndFar( fs->getFrameNumber(), znear, zfar );
        }
        else if ( camera->getNumChildren()>0 )  // Use camera's own children as display surface
            camera->osg::Group::traverse( *nv );
        else                                    // Render to a fullscreen quad
            _compositor->getOrCreateQuad()->accept( *nv );
    }
    else
        traverse( node, nv );
}

Effect::PassCullCallback::PassCullCallback(EffectCompositor* compositor, PassType type) : _compositor(compositor), _type(type)
{

}
//
//Effect::GraphicPassCallback::GraphicPassCallback(GraphicsPipeline* graphicsPipeline,PassType type):_graphicPipeline(graphicsPipeline),_type(type)
//{
//
//}
//
//void Effect::GraphicPassCallback::operator()(osg::Node* node,osg::NodeVisitor* nv)
//{
//    osg::Camera* camera = static_cast<osg::Camera*>( node );
//    if ( !camera || !nv )
//    {
//        traverse( node, nv );
//        return;
//    }
//
//    if ( nv->getVisitorType()==osg::NodeVisitor::CULL_VISITOR )
//    {
//        osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>(nv);
//        if ( _type== FORWARD_PASS )
//        {
//            // Forward pass will traverse the scene normally
//            _graphicPipeline->osg::Group::traverse( *nv );
//
//            // We obtain the actual near/far values at the end of forward pass traversing
//            double znear = cv->getCalculatedNearPlane();
//            double zfar = cv->getCalculatedFarPlane();
//            osg::Matrixd projection = *(cv->getProjectionMatrix());
//            cv->clampProjectionMatrix( projection, znear, zfar );
//
//            const osg::FrameStamp* fs = cv->getFrameStamp();
//            if ( fs ) _graphicPipeline->setPreservedNearAndFar( fs->getFrameNumber(), znear, zfar );
//        }
//        else if ( camera->getNumChildren()>0 )  // Use camera's own children as display surface
//            camera->osg::Group::traverse( *nv );
//        else                                    // Render to a fullscreen quad
//            _graphicPipeline->getOrCreateQuad()->accept( *nv );
//    }
//    else
//        traverse( node, nv );
//}
