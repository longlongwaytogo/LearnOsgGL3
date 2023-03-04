#pragma once

#include <osgGA/FirstPersonManipulator>

namespace Frame
{
	class FirstPersonManipulator : public osgGA::FirstPersonManipulator
	{
	public:
		using osgGA::FirstPersonManipulator::FirstPersonManipulator;
		/// Handles GUIEventAdapter::KEYUP event.
		bool handleKeyDown(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
		inline void setSpeed(float speed) { m_speed = speed; }
		inline float getSpeed() { return m_speed; }
		void customKeyDown();
	private:
			float m_speed = 1.0f;
			float m_lastTime = 0.0;
			float m_deltaTime = 0.0;
	};
}