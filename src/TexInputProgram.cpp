#include "TexInputProgram.h"

using namespace ci;
using namespace input;

ProgramRef TexInputProgram::create(ProgramStateRef state)
{
	return ProgramRef(new TexInputProgram(state));
}

ci::gl::Texture2dRef TexInputProgram::getColorTexture(ci::gl::FboRef base, ci::gl::FboRef extra)
{
	gl::TextureRef tex = getInputTex(mInput);
	if(tex) {
		gl::ScopedFramebuffer fbo(base);
		gl::draw(tex, tex->getBounds(), base->getBounds());
	}

	auto e = getEffect();
	if (e) {
		return e->getColorTexture(base, extra);
	}

	return base->getColorTexture();
}

TexInputProgram::TexInputProgram(ProgramStateRef state) : Program(nullptr, state)
{
}

std::shared_ptr<ci::Camera> TexInputProgram::camera()
{
	return std::shared_ptr<ci::Camera>();
}

std::shared_ptr<ci::ivec2> TexInputProgram::matrixWindow()
{
	return std::shared_ptr<ci::ivec2>();
}

void TexInputProgram::updateInputUniform(std::string name, std::string val, std::vector<float> modifiers)
{
	Program::updateInputUniform(name, val, modifiers);

	if (name.compare("program") != 0) {
		return;
	}

	mInput = name;
}