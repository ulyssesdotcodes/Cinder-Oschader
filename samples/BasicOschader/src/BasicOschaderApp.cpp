#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BasicOschaderApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void BasicOschaderApp::setup()
{
}

void BasicOschaderApp::mouseDown( MouseEvent event )
{
}

void BasicOschaderApp::update()
{
}

void BasicOschaderApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( BasicOschaderApp, RendererGl )
