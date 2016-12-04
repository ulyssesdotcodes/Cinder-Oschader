#include "MouseInputResolver.h"

void MouseInputResolver::onMouseDown(ci::app::MouseEvent m)
{
	mMouseDown = 1.0f;
}

void MouseInputResolver::onMouseUp(ci::app::MouseEvent m)
{
	mMouseDown = 0.0f;
}

void MouseInputResolver::onMouseMove(ci::app::MouseEvent m)
{
	mMousePos = (ci::vec2) m.getPos() / (ci::vec2) ci::app::getWindowSize();
}

int MouseInputResolver::parseInputType(std::string type)
{
	return 0;
}

bool MouseInputResolver::isTexture(const input::InputParameters &)
{
	return false;
}

bool MouseInputResolver::isFloat(const input::InputParameters & ip)
{
	if (ip.inputType.compare(MOUSE_DOWN_NAME) == 0 || 
		ip.inputType.compare(MOUSE_X_NAME) == 0 || 
		ip.inputType.compare(MOUSE_Y_NAME) == 0) {
		return true;
	}
	return false;
}

float MouseInputResolver::getFloat(const input::InputParameters & ip)
{
	float mult = ip.params.size() > 0 ? ip.params[0] : 1;
	float add = ip.params.size() > 1 ? ip.params[1] : 0;

	if (ip.inputType.compare(MOUSE_DOWN_NAME) == 0) {
		return mMouseDown * mult + add;
	}
	if (ip.inputType.compare(MOUSE_X_NAME) == 0) {
		return mMousePos.x * mult + add;
	}
	if (ip.inputType.compare(MOUSE_Y_NAME) == 0) {
		return mMousePos.y * mult + add;
	}
	return 0.0f;
}

ci::gl::TextureRef MouseInputResolver::getTexture(const input::InputParameters & ip)
{
	return ci::gl::TextureRef();
}
