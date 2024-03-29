﻿#include "FirstPersonMainipulator.h"

#if defined(WIN32)
#include <Windows.h>
#endif 
/// Handles GUIEventAdapter::KEYUP event.

bool Frame::FirstPersonManipulator::handleKeyDown(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	return false;
	osg::Vec3d eye, center, up;
	getTransformation(eye, center, up);
	// move
	osg::Vec3d front = center - eye;
	osg::Vec3d right = front ^ up;
	float len = front.length();
	right.normalize();
	front.normalize();
	
	
	double speed = m_deltaTime * m_speed;
	printf("speed:%f\n", speed);
	switch (auto key = ea.getKey())
	{
	case osgGA::GUIEventAdapter::KEY_Left:
	{
		eye += -right * speed;
		center = eye + front*len;
		break;
	}
	case osgGA::GUIEventAdapter::KEY_Right:
	{
		eye += right * speed;
		center = eye + front*len;
		break;
	}
	case osgGA::GUIEventAdapter::KEY_Up:
	{
		eye += front * speed;
		center = eye + front*len;
		break;
	}
	case osgGA::GUIEventAdapter::KEY_Down:
	{
		eye += -front * speed;
		center = eye + front * len;
		break;
	}

	}
	setTransformation(eye, center, up);

	return false;
}

#define KEY_DOWN(key_name) ((GetAsyncKeyState(key_name)&0x8000)?1:0)
// osg keydown 消息不连贯使用window自有api获取
void Frame::FirstPersonManipulator::customKeyDown()
{
	osg::Vec3d eye, center, up;
	getTransformation(eye, center, up);
	// move
	osg::Vec3d front = center - eye;
	osg::Vec3d right = front ^ up;
	float len = front.length();
	right.normalize();
	front.normalize();
	float a = 1.0;
	if (KEY_DOWN(VK_SHIFT))
		a = 1.4;
	double speed = m_deltaTime * m_speed * a;
	 
	if(KEY_DOWN('a') || KEY_DOWN('A') || KEY_DOWN(VK_LEFT))
	{
		eye += -right * speed;
		center = eye + front * len;
	}
	if (KEY_DOWN('d')||KEY_DOWN('D')|| KEY_DOWN(VK_RIGHT))
	{
		eye += right * speed;
		center = eye + front * len;
	}
	if (KEY_DOWN('w')||KEY_DOWN('W')|| KEY_DOWN(VK_UP))
	{
		eye += front * speed;
		center = eye + front * len;
	}
	if (KEY_DOWN('s')||KEY_DOWN('S')|| KEY_DOWN(VK_DOWN))
	{
		eye += -front * speed;
		center = eye + front * len;
	}
	if (KEY_DOWN('q') || KEY_DOWN('Q')) // 上
	{
		eye += osg::Vec3(0,0,1) * speed;
		center = eye + front * len;
	}
	if (KEY_DOWN('e') || KEY_DOWN('E')) // 下
	{
		eye += -osg::Vec3(0, 0, 1) * speed;
		center = eye + front * len;
	}

	if (KEY_DOWN('z') || KEY_DOWN('Z')) // 下
	{
		m_speed *= 1.2f;
	}

	if (KEY_DOWN('c') || KEY_DOWN('C')) // 下
	{
		m_speed /= 1.2f;
	}
	setTransformation(eye, center, up);
}
bool Frame::FirstPersonManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	switch (ea.getEventType())
	{
	case  osgGA::GUIEventAdapter::FRAME:
	{
		float curTime = us.asView()->getFrameStamp()->getSimulationTime();
		m_deltaTime = curTime - m_lastTime;
		m_lastTime = curTime;
		printf("delta time:%f\n", m_deltaTime);
		customKeyDown();
		return handleFrame(ea, us);
	}

	case  osgGA::GUIEventAdapter::RESIZE:
		return handleResize(ea, us);

	default:
		break;
	}

	if (ea.getHandled())
		return false;

	switch (ea.getEventType())
	{
	case  osgGA::GUIEventAdapter::MOVE:
		return handleMouseMove(ea, us);

	case  osgGA::GUIEventAdapter::DRAG:
		return handleMouseDrag(ea, us);

	case  osgGA::GUIEventAdapter::PUSH:
		return handleMousePush(ea, us);

	case  osgGA::GUIEventAdapter::RELEASE:
		return handleMouseRelease(ea, us);

	case  osgGA::GUIEventAdapter::KEYDOWN:
		return handleKeyDown(ea, us);

	case  osgGA::GUIEventAdapter::KEYUP:
		return handleKeyUp(ea, us);

	case  osgGA::GUIEventAdapter::SCROLL:
		if (_flags & PROCESS_MOUSE_WHEEL)
			return handleMouseWheel(ea, us);
		else
			return false;

	default:
		return false;
	}
}
