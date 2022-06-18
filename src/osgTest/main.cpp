/*
* @file: osgTest.cpp
* @date: 2022/06/16
* @author: longlongwaytogo
* @description: gl3 模式下最简单的osg例子
*/

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
	viewer.setUpViewInWindow(10, 10, 800, 600);
	viewer.run();
}