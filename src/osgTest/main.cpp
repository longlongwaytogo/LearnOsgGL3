#include <iostream>
#include <osg/Node>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>

void main()
{


	osgViewer::Viewer viewer;

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(osgDB::readNodeFile("cow.osg"));

	viewer.setSceneData(root);
	viewer.run();


}