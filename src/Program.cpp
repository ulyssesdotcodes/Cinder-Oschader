#include "Program.h"

using namespace ci;
using namespace input;

Program::Program(gl::BatchRef batch, ProgramStateRef state) : mBatch(batch), mState(state), mOverflowCheck(std::make_pair(0, 0))
{
	if(mBatch) {
		mBatch->getGlslProg()->uniform("i_resolution", (vec2) app::getWindowSize());
	}
}

ci::gl::BatchRef Program::batch()
{
	return mBatch;
}

void Program::updateUniform(std::string name, float val) {
	auto iter = mInputUniforms.find(name);
	if (iter != mInputUniforms.end()) {
		mInputUniforms.erase(name);
	}

	if(mBatch) {
		mBatch->getGlslProg()->uniform("i_" + name, val);
	}

	onUpdateUniform(name, val);
}

void Program::updateInputUniform(std::string name, std::string val, std::vector<float> modifiers) {
	mInputUniforms.insert_or_assign(name, input::InputParameters(val, modifiers));
}

void Program::updateUniform(std::string name, std::string)
{
}

void Program::updateUniform(std::string name, int val) {
	if(mBatch) {
		mBatch->getGlslProg()->uniform("i_" + name, val);
	}
}

void Program::onUpdateUniform(std::string name, float val)
{
}

// Note that it uses fbo a
gl::Texture2dRef Program::getColorTexture(ci::gl::FboRef base, ci::gl::FboRef _)
{
	{
		gl::ScopedFramebuffer fbo(base);
		gl::clear(Color::black());

		draw();
	}

	auto effect = getEffect();
	if (effect) {
		return effect->getColorTexture(base, _);
	}

	return base->getColorTexture();
}

void Program::draw() 
{
	bindInputTexes(mBatch->getGlslProg());

	gl::pushViewport();
	gl::pushMatrices();

	auto cam = camera();
	std::shared_ptr<ci::ivec2> win = matrixWindow();
	if (cam) {
		gl::setMatrices(*cam);
	}
	else if (win) {
		gl::setMatricesWindow(*win);
	}

	mBatch->draw();

	gl::popMatrices();
	gl::popViewport();
}

ProgramRef Program::getEffect()
{
	if (mOverflowCheck.first < app::getElapsedFrames()) {
		mOverflowCheck.first = app::getElapsedFrames();
		mOverflowCheck.second = 1;
	}
	else {
		mOverflowCheck.second++;
	}

	if(mEffect && mOverflowCheck.second < 20) {
		return mState->getProgram(*mEffect);
	}

	return nullptr; 
}

ProgramRef Program::getProgram(std::string id)
{
	return mState->getProgram(id);
}

void Program::setEffect(std::string eff)
{
	mEffect = std::make_shared<std::string>(eff);
}

void Program::clearEffect()
{
	mEffect = nullptr;
}

void Program::addLayer(std::string)
{
	throw Exception("Can't add a layer to a regular prog.");
}

void Program::clearLayers()
{
}

void Program::update(std::shared_ptr<input::InputResolver> r)
{
	mLastInputTextures.clear();

	for (std::pair<std::string, InputParameters> e : mInputUniforms) {
		if(r->isFloat(e.second)) {
			float val = r->getFloat(e.second);
			if(mBatch) {
				mBatch->getGlslProg()->uniform("i_" + e.first, val);
			}
			onUpdateUniform(e.first, val);
		} else if(r->isTexture(e.second)) {
			gl::TextureRef tex = r->getTexture(e.second);
			mLastInputTextures.insert(std::make_pair(e.first, tex));
		}
	}


	onUpdate();
}

void Program::onUpdate() { }

void Program::bindInputTexes(ci::gl::GlslProgRef prog)
{
	int i = 2; // Leave room for effect and layer
	for (auto iter = mLastInputTextures.begin(); iter != mLastInputTextures.end(); iter++) {
		iter->second->bind(i);
		prog->uniform("i_" + iter->first, i);
		++i;
	}
}

gl::TextureRef Program::getInputTex(std::string it) {
	auto iter = mLastInputTextures.find(it);
	return iter == mLastInputTextures.end() ? nullptr : iter->second;
}
