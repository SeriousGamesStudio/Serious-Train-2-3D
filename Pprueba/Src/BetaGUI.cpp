#include "BetaGUI.h"
#include "InputManager.h"
using namespace Ogre;
using namespace std;
namespace BetaGUI {
	GUI::GUI(const String & font, const uint & fontSize)
		:mXW(0),
		mousePointer(0),
		mFont(font),
		mFontSize(fontSize),
		wc(0),
		bc(0),
		tc(0)
	{
		mO = OverlayManager::getSingleton().create("BetaGUI");
		mO->show();
	}

	GUI::~GUI() {
		for (uint i = 0; i < WN.size(); i++)
			delete WN[i];
		WN.clear();
	}

	bool GUI::injectMouse(const uint & x,const uint & y, const bool & LMB) {


		if (mXW) {
			for (std::vector<Window*>::iterator i = WN.begin(); i != WN.end(); i++) {
				if (mXW == (*i)) {
					delete mXW;
					WN.erase(i);
					mXW = 0;
					return false;
				}
			}
		}

		for (uint i = 0; i<WN.size(); i++) {
			if (WN[i]->check(x, y, LMB)) {
				return true;
			}
		}

		return false;
	}
	bool GUI::injectKey(const String & key, const uint & x, const uint &y) {
		for (uint i = 0; i<WN.size(); i++) {
			if (WN[i]->checkKey(key, x, y)) {
				return true;
			}
		}
		return false;
	}

	void GUI::injectBackspace(const uint & x, const uint & y) {
		injectKey("!b", x, y);
	}

	OverlayContainer* GUI::createOverlay(const String & name, const Vector2 &position, const Vector2 &dimensions, const Ogre::String & material, const Ogre::String & caption,const bool &autoAdd) {
		String type = "Panel";

		if (caption != "")
			type = "TextArea";

		OverlayContainer* e = static_cast<OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement(type, name));

		e->setMetricsMode(Ogre::GMM_PIXELS);
		e->setDimensions(dimensions.x, dimensions.y);
		e->setPosition(position.x, position.y);

		if (material != "")
			e->setMaterialName(material);

		if (caption != "") {
			e->setCaption(caption);
			e->setParameter("font_name", mFont);
			e->setParameter("char_height", StringConverter::toString(mFontSize));
		}

		if (autoAdd) {
			mO->add2D(e);
			e->show();
		}

		return e;
	}

	MousePointer* GUI::createMousePointer(const Vector2 & d, const String & m) {
		
		if (!mousePointer) {
			mousePointer = new MousePointer(this, d, m);
		}

		return mousePointer;
	}

	Window* GUI::createWindow(const Ogre::Vector4 & Dimensions,const Ogre::String & Material, const WINDOW_TYPE &type, const Ogre::String & caption) {
		Window* window = new BetaGUI::Window(Dimensions, Material, type, caption, this);
		WN.push_back(window);
		return window;
	}

	Window::Window(const Ogre::Vector4 & Dimensions,const Ogre::String & Material, const WINDOW_TYPE & t, const Ogre::String & caption, GUI *gui)
		: x(Dimensions.x),
		y(Dimensions.y),
		w(Dimensions.z),
		h(Dimensions.w),
		mGUI(gui),
		mTB(0),
		mRZ(0),
		mATI(0),
		mAB(0)
	{
		mO = gui->createOverlay("BetaGUI.w" + StringConverter::toString(gui->wc++), Vector2(Dimensions.x, Dimensions.y), Vector2(Dimensions.z, Dimensions.w), Material);
		if (t >= 2) {
			Callback c; c.t = 4;
			mRZ = createButton(Vector4(Dimensions.z - 16, Dimensions.w - 16, 16, 16), Material + ".resize", "", c);
		}

		if (t == 1 || t == 3) {
			Callback c; c.t = 3;
		}
		mTB = createButton(Vector4(0, 0, Dimensions.z, 22), Material + ".titlebar", caption, Callback());

	}

	Window::~Window() {
		for (uint i = 0; i<mB.size(); i++)
			delete mB[i];

		for (uint i = 0; i<mT.size(); i++)
			delete mT[i];

		mGUI->mO->remove2D(mO);
	}

	Button* Window::createButton(const Vector4 & D, const String & M, const String & T, Callback C) {
		Button *x = new Button(D, M, T, C, this);
		mB.push_back(x);
		return x;
	}

	Button::Button(const Ogre::Vector4 & Dimensions, const String & m, const String & Text, Callback cb, Window *parent)
		: x(Dimensions.x),
		y(Dimensions.y),
		w(Dimensions.z),
		h(Dimensions.w),
		mmn(m),
		mma(m)
	{

		Ogre::ResourcePtr ma = Ogre::MaterialManager::getSingleton().getByName(mmn + ".active");
		if (!ma.isNull())
			mma += ".active";

		mO = parent->mGUI->createOverlay(parent->mO->getName() + "b" + StringConverter::toString(parent->mGUI->bc++), Vector2(x, y), Vector2(w, h), m, "", false);
		mCP = parent->mGUI->createOverlay(mO->getName() + "c", Vector2(4, (h - parent->mGUI->mFontSize) / 2), Vector2(w, h), "", Text, false);
		parent->mO->addChild(mO);
		mO->show();
		mO->addChild(mCP);
		mCP->show();
		callback = cb;
	}

	TextInput* Window::createTextInput(const Vector4 &D,const String & M, const String&  V, const uint & L) {
		TextInput *x = new TextInput(D, M, V, L, this);
		mT.push_back(x);
		return x;
	}

	void Window::createStaticText(const Vector4 &  D, const String & T) {
		OverlayContainer* x = mGUI->createOverlay(mO->getName() + StringConverter::toString(mGUI->tc++), Vector2(D.x, D.y), Vector2(D.z, D.w), "", T, false);
		mO->addChild(x);
		x->show();
	}

	TextInput::TextInput(const Vector4 & D, const String & M, const String & V, const uint & L, Window *P)
		: x(D.x), y(D.y), w(D.z), h(D.w), value(V), mmn(M), mma(M), length(L)
	{
		ResourcePtr ma = Ogre::MaterialManager::getSingleton().getByName(mmn + ".active");
		if (!ma.isNull())
			mma += ".active";

		mO = P->mGUI->createOverlay(P->mO->getName() + "t" + StringConverter::toString(P->mGUI->tc++), Vector2(x, y), Vector2(w, h), M, "", false);
		mCP = P->mGUI->createOverlay(mO->getName() + "c", Vector2(4, (h - P->mGUI->mFontSize) / 2), Vector2(w, h), "", V, false);
		P->mO->addChild(mO);
		mO->show();
		mO->addChild(mCP);
		mCP->show();
	}

	bool Window::checkKey(const String & k, const uint & px, const uint & py) {

		if (!mO->isVisible())
			return false;

		if (!(px >= x&&py >= y) || !(px <= x + w&&py <= y + h))return false;

		if (mATI == 0)
			return false;

		if (k == "!b") {
			mATI->setValue(mATI->value.substr(0, mATI->value.length() - 1));
			return true;
		}

		if (mATI->value.length() >= mATI->length)
			return true;

		mATI->mCP->setCaption(mATI->value += k);
		return true;
	}

	bool Window::check(const uint & px, const uint&  py, const bool & LMB) {
		
		if (!mO->isVisible())
			return false;

		if (!(px >= x && py >= y) || !(px <= x + w && py <= y + h)) {
			if (mAB) {
				mAB->activate(false);
			}
			return false;
		}

		if (mAB) {
			mAB->activate(false);
		}

		for (uint i = 0; i < mB.size(); i++) {
			if (mB[i]->in(px, py, x, y))
				continue;

			if (mAB) {
				mAB->activate(false);
			}

			mAB = mB[i];
			mAB->activate(true);

			if (!LMB)
				return true;

			if (mATI) {
				mATI->mO->setMaterialName(mATI->mmn);
				mATI = 0;
			}

			switch (mAB->callback.t) {
			default: return true;
			case 1:
				mAB->callback.fp(mAB);
				return true;
			case 2:
				mAB->callback.LS->onButtonPress(mAB);
				return true;
			case 3:
				mO->setPosition(x = px - (mAB->w / 2), y = py - (mAB->h / 2));
				return true;
			case 4:
				mO->setDimensions(w = px - x + 8, h = py - y + 8);
				mRZ->mO->setPosition(mRZ->x = w - 16, mRZ->y = h - 16);
				if (mTB) {
					mTB->mO->setWidth(mTB->w = w);
				}
				return true;
			}

		}


		if (!LMB)
			return true;

		for (uint i = 0; i<mT.size(); i++) {

			if (mT[i]->in(px, py, x, y))
				continue;

			mATI = mT[i];
			mATI->mO->setMaterialName(mATI->mma);
			return true;
		}

		if (mATI) {
			mATI->mO->setMaterialName(mATI->mmn);
			mATI = 0;
		}

		return true;
	}

	

	MousePointer::MousePointer(GUI * gui, const Ogre::Vector2 & dimensions, const Ogre::String & material)
	{
		_active = false;
		_GUI = gui;
		Overlay* o = OverlayManager::getSingleton().create("BetaGUI.MP");
		o->setZOrder(649);
		mMP = _GUI->createOverlay("bg.mp", Vector2(0, 0), dimensions, material, "", false);
		o->add2D(mMP);
		o->show();
		mMP->show();
		setActive(true);
	}

	MousePointer::~MousePointer()
	{
	}

	void MousePointer::setActive(const bool & active)
	{
		_active = active;
		if (_active) 
		{
			InputManager::getInstance()->addMouseListener(this, "GUI-Mouse");
			mMP->show();
		}
		else
		{
			InputManager::getInstance()->removeMouseListener(this);
			mMP->hide();
		}
	}

	bool MousePointer::mouseMoved(const OIS::MouseEvent & arg)
	{
		mMP->setPosition(arg.state.X.abs, arg.state.Y.abs);
		return true;
	}

	bool MousePointer::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
	{
		return true;
	}

	bool MousePointer::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
	{
		return true;
	}

} // End of Betajaen's GUI. Normal programming can resume now.