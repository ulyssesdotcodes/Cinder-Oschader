#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "EffectRect.h"
#include "FadeEffect.h"
#include "FragmentProgram.h"
#include "PassthroughProgram.h"
#include "ProgramState.h"

#include "MouseInputResolver.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicOschaderApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void mouseUp( MouseEvent event ) override;
	void mouseMove( MouseEvent event ) override;
	void update() override;
	void draw() override;

private:
	std::shared_ptr<ProgramState> mState;
	gl::FboRef mBase, mExtra;
	std::shared_ptr<MouseInputResolver> mResolver;
};

void BasicOschaderApp::setup()
{
	mBase = gl::Fbo::create(app::getWindowWidth(), app::getWindowHeight());
	mExtra = gl::Fbo::create(app::getWindowWidth(), app::getWindowHeight());

	mResolver = std::make_shared<MouseInputResolver>(MouseInputResolver());

	mState = std::make_shared<ProgramState>(ProgramState());

	mState->setProgram("circles", "circles", FragmentProgram::create(mState, "circles.frag"));
	mState->setProgram("translate", "translate", EffectRect::create(mState, "translate.frag"));
	mState->getProgram("translate")->updateInputUniform("translate_x", MOUSE_X_NAME, { -1, 0.5 });// Adjust for gl position by (* -1) and (+ 1)
	mState->getProgram("translate")->updateInputUniform("translate_y", MOUSE_Y_NAME, { 1, -0.5 }); 
	mState->getProgram("circles")->setEffect("translate");
	mState->setProgram("sine", "sine", FragmentProgram::create(mState, "sine.frag"));
	mState->setProgram("default", "passthrough", PassthroughProgram::create(mState));
	mState->setProgram("fade", "fade", FadeEffect::create(mState, "fade.frag"));
	mState->getProgram("fade")->updateInputUniform("fade", MOUSE_DOWN_NAME, std::vector<float>());
	mState->getProgram("default")->setEffect("fade");
}

void BasicOschaderApp::mouseDown( MouseEvent event )
{
	mResolver->onMouseDown(event);
}

void BasicOschaderApp::mouseUp( MouseEvent event )
{
	mResolver->onMouseUp(event);
}

void BasicOschaderApp::mouseMove(MouseEvent event)
{
	mResolver->onMouseMove(event);
}

void BasicOschaderApp::update()
{
	mState->update([&](ProgramRef p) {
		p->update(mResolver);
	});

	mState->getProgram("circles")->updateUniform("time", (float) app::getElapsedSeconds());
	mState->getProgram("sine")->updateUniform("time", (float) app::getElapsedSeconds());
	mState->getProgram("default")->updateUniform("program", (app::getElapsedFrames() / 60) % 2 == 0 ? "circles" : "sine");
}

void BasicOschaderApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 

	gl::draw(mState->getProgram("default")->getColorTexture(mBase, mExtra));
}

CINDER_APP( BasicOschaderApp, RendererGl )
