#pragma once

#include "InputResolver.h"
#include "cinder/app/App.h"

static const std::string MOUSE_DOWN_NAME = "mouse_down";
static const std::string MOUSE_X_NAME = "mouse_x";
static const std::string MOUSE_Y_NAME = "mouse_y";

class MouseInputResolver : virtual public input::InputResolver {
public:
	virtual void onMouseDown(ci::app::MouseEvent);
	virtual void onMouseUp(ci::app::MouseEvent);
	virtual void onMouseMove(ci::app::MouseEvent);
	virtual int parseInputType(std::string type);
	virtual bool isTexture(const input::InputParameters&);
	virtual bool isFloat(const input::InputParameters&);
	virtual float getFloat(const input::InputParameters& ip);
	virtual ci::gl::TextureRef getTexture(const input::InputParameters& ip);

private:
	float mMouseDown;
	ci::vec2 mMousePos;
};
