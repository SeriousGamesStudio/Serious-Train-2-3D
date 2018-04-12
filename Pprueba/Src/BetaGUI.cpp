#include "BetaGUI.h"
#include "InputManager.h"
using namespace Ogre;
using namespace std;
namespace GUIndilla {
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
		delete mousePointer;
		for (uint i = 0; i < WN.size(); i++)
			delete WN[i];
		WN.clear();
	}

	bool GUI::injectMouse(const uint & x, const uint & y, const bool & LMB = false) {


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

		for (uint i = 0; i < WN.size(); i++) {
			if (WN[i]->check(x, y, LMB)) {
				return true;
			}
		}

		return false;
	}


	OverlayContainer* GUI::createOverlay(const String & name, const Vector2 &position, const Vector2 &dimensions, const Ogre::String & material, const Ogre::String & caption,const bool &autoAdd) {
		String type = "Panel";
		OverlayContainer* e = static_cast<OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement(type, name));

		e->setMetricsMode(Ogre::GMM_RELATIVE);
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
		Window* window = new GUIndilla::Window(Dimensions, Material, type, caption, this);
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

	}

	Window::~Window() {
		for (uint i = 0; i<mB.size(); i++)
			delete mB[i];

		for (uint i = 0; i<mT.size(); i++)
			delete mT[i];

		mGUI->mO->remove2D(mO);
	}

	

	bool Window::check(const uint & px, const uint&  py, const bool & LMB) {
		
		if (!mO->isVisible())
			return false;
		if (!LMB)
			return true;

		

		return true;
	}

	

	MousePointer::MousePointer(GUI * gui, const Ogre::Vector2 & dimensions, const Ogre::String & material)
	{
		_active = false;
		_GUI = gui;
		Overlay* o = OverlayManager::getSingleton().create("BetaGUI.MP");
		o->setZOrder(649);

		mMP = static_cast<OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement("Panel", "bg.mp"));

		mMP->setMetricsMode(Ogre::GMM_PIXELS);
		mMP->setDimensions(dimensions.x, dimensions.y);
		mMP->setPosition(0, 0);
		mMP->setMaterialName(material);
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
		_GUI->injectMouse(arg.state.X.abs, arg.state.Y.abs);
		return true;
	}

	bool MousePointer::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
	{
		mMP->setPosition(arg.state.X.abs, arg.state.Y.abs);
		_GUI->injectMouse(arg.state.X.abs, arg.state.Y.abs,arg.state.buttonDown(id));
		return true;
	}

	bool MousePointer::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
	{
		return true;
	}

} // End of Betajaen's GUI. Normal programming can resume now.