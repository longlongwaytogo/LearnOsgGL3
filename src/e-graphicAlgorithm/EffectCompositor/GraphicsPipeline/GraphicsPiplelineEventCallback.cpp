#include <GraphicsPipeline/GraphicsPiplelineEventCallback.h>
#include <GraphicsPipeline/GraphicsPipeline.h>
//#include <RenderDocEvent/RenderDocEvent.h>

namespace Effect
{


    void GraphicPipelineEventCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
    {

        if(nv->getVisitorType() == osg::NodeVisitor::EVENT_VISITOR)
        {
            osg::ref_ptr<osgGA::EventVisitor> ev = dynamic_cast<osgGA::EventVisitor*>(nv);
            if(ev)
            {

                osgGA::GUIActionAdapter* aa = ev->getActionAdapter();
                osgGA::EventQueue::Events& events = ev->getEvents();

                for(auto itr = events.begin(); itr != events.end(); ++itr)
                {
                    handle(node,*osg::dynamic_pointer_cast<osgGA::GUIEventAdapter>(*itr),*(aa));
                  /*    handle(node,*(*itr),*(aa));*/
                }
            }
            
        }

    }

bool GraphicPipelineEventCallback::handle(osg::Node* node,const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
  {
      osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
      
      switch (ea.getEventType())
      {

      case osgGA::GUIEventAdapter::KEYDOWN:
          {
               OSG_INFO << "KEYDOWN" << std::endl;
              break;
          }

      case osgGA::GUIEventAdapter::KEYUP:
      {
          if(ea.getKey() == osgGA::GUIEventAdapter::KEY_F4)
          {
              OSG_INFO << "LaucherUI" << std::endl;
             //RenderDocEvent::LaunchReplayUI();
              break;
          }
          else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_F3)
          {
              OSG_INFO << "Capture" << std::endl;
             // RenderDocEvent::TriggerCapture();
              break;
          }
      }

      case osgGA::GUIEventAdapter::FRAME:
          {
              OSG_INFO << "on Frame" << std::endl;

              break;
          }

    


      case osgGA::GUIEventAdapter::RESIZE:
          {

              OSG_INFO << "Resize window" << std::endl;

              GraphicsPipeline* pPipeline = dynamic_cast<GraphicsPipeline*>(node);
             if(pPipeline)
                {
                    // todo
                   // pPipeline->resize(ea.getWindowWidth(),ea.getWindowHeight());
                   // pPipeline->reLoadFromXmlNode();
             }
              break;

          }
      default:
          break;
      }

      return true;
  }


}