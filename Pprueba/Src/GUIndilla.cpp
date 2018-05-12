#include "GUIndilla.h"

#include "InputManager.h"
using namespace Ogre;
using namespace std;

namespace GUIndilla {
	GUI::GUI(const String & font, const uint & fontSize)
		:mXW(0),
		mousePointer(0),
		mFont(font),
		mFontSize(fontSize),
		nButtons(0),
		activeButton(0)
	{
		mO = OverlayManager::getSingleton().create("BetaGUI");
		mO->show();
	}

	GUI::~GUI() {
		delete mousePointer;
		for (uint i = 0; i < Elementos.size(); i++)
			delete Elementos[i];
	}

	bool GUI::injectMouse() {
		if (activeButton) {
			activeButton->activate(false);
			activeButton = nullptr;
		}
		for (uint i = 0; i < botones.size(); i++) {
			if (botones[i]->getActive() && mousePointer->IsMouseOver(botones[i]->getMainOverlay())) {
				activeButton = botones[i];
				activeButton->activate(true);

			}
		}

		return false;
	}

	void GUI::mousePressed()
	{
		if (activeButton) {
			activeButton->callback.onButtonPress();
		}
	}


	OverlayContainer* GUI::createOverlay(const String & name, const Vector2 &position, const Vector2 &dimensions, const VERTICAL_ANCHOR & vertAnch, const HORINZONTAL_ANCHOR & horAnchor,const Ogre::String & material, const Ogre::String & caption) {
		String type = "Panel";
		OverlayContainer* e = static_cast<OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement(type, name));
		e->setMetricsMode(Ogre::GuiMetricsMode::GMM_PIXELS);
		e->setHorizontalAlignment((GuiHorizontalAlignment)horAnchor);
		e->setVerticalAlignment((GuiVerticalAlignment)vertAnch);
		e->setDimensions(dimensions.x, dimensions.y);
		e->setPosition(position.x, position.y);
		
		
		if (material != "")
			e->setMaterialName(material);

		if (caption != "") {
			TextAreaOverlayElement* textArea = static_cast<TextAreaOverlayElement*>(
				OverlayManager::getSingleton().createOverlayElement("TextArea", name+".caption"));
			textArea->setHorizontalAlignment(Ogre::GHA_CENTER);
			textArea->setVerticalAlignment(Ogre::GVA_CENTER);

			textArea->setMetricsMode(Ogre::GMM_PIXELS);
			textArea->setPosition(0, 0);
			textArea->setDimensions(dimensions.x, dimensions.y);
			textArea->setCaption(caption);
			textArea->setCharHeight(16);
			textArea->setFontName("Caption");
			textArea->setPosition(-textArea->getWidth()/2, -10);
			textArea->setColourBottom(ColourValue(0.3, 0.5, 0.3));
			textArea->setColourTop(ColourValue(0.5, 0.7, 0.5));
			e->addChild(textArea);
			
		}
		
			mO->add2D(e);
			e->show();
			mO->show();


		return e;
	}

	Ogre::OverlayContainer * GUI::createTextOverlay(const Ogre::String & name, const Ogre::Vector2 & position, const int & charSize,const VERTICAL_ANCHOR & vertAnch, const HORINZONTAL_ANCHOR & horAnchor, const Ogre::String & caption)
	{
		TextAreaOverlayElement* textArea = static_cast<TextAreaOverlayElement*>(
			OverlayManager::getSingleton().createOverlayElement("TextArea", name + ".caption"));
		textArea->setHorizontalAlignment(Ogre::GHA_CENTER);
		textArea->setVerticalAlignment(Ogre::GVA_CENTER);

		textArea->setMetricsMode(Ogre::GMM_PIXELS);
		textArea->setPosition(position.x,position.y);
		textArea->setCaption(caption);
		textArea->setCharHeight(charSize);
		textArea->setFontName("Caption");
		textArea->setColourBottom(ColourValue(0.3, 0.5, 0.3));
		textArea->setColourTop(ColourValue(0.5, 0.7, 0.5));
		mO->add2D((OverlayContainer*)textArea);
		textArea->show();
		mO->show();

		return (OverlayContainer*)textArea;
	}

	MousePointer* GUI::createMousePointer(const Vector2 & d, const String & m) {
		
		if (!mousePointer) {
			mousePointer = new MousePointer(this, d, m);
		}

		return mousePointer;
	}

	
	Button * GUI::createButton(const Ogre::Vector4 & D, const Ogre::String & M, const Ogre::String & T, const Callback & C, const POSITION_TYPE & posType, const VERTICAL_ANCHOR & vertAnch, const HORINZONTAL_ANCHOR & horAnchor)
	{
		nButtons++;
		Button *x = new Button(D, M, T, C,this,vertAnch,horAnchor);
		botones.push_back(x);
		Elementos.push_back(x);
		return x;
	}

	Button * GUI::createStaticImage(const Ogre::Vector4 & Dimensions, const Ogre::String & Material, const POSITION_TYPE & posType, const VERTICAL_ANCHOR & vertAnch, const HORINZONTAL_ANCHOR & horAnchor)
	{
		nButtons++;
		Button *x = new Button(Dimensions, Material, "", Callback(), this,vertAnch,horAnchor);
		Elementos.push_back(x);
		return x;
	}

	GUIText * GUI::createStaticText(const Ogre::Vector2 & position, const Ogre::String & Text, const int & charsize,  const VERTICAL_ANCHOR & vertAnch, const HORINZONTAL_ANCHOR & horAnchor)
	{
		nButtons++;
		GUIText *x = new GUIText(position, Text,charsize, vertAnch, horAnchor, this);
		Elementos.push_back(x);
		return x;
	}

	void GUI::ClearGUI()
	{
		mO->clear();
		botones.clear();
		Elementos.clear();
		nButtons = 0;
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

	bool MousePointer::IsMouseOver(Ogre::OverlayContainer * over)
	{
	
		double overX = over->_getLeft();
		double overY = over->_getTop();
		switch (over->getVerticalAlignment())
		{
		case Ogre::GVA_CENTER:
			overY += 0.5;
			break;
		case Ogre::GVA_BOTTOM:
			overY += 1.0;

			break;
		default:
			break;
		}

		switch (over->getHorizontalAlignment())
		{
		case Ogre::GHA_CENTER:
			overX += 0.5;

			break;
		case Ogre::GHA_RIGHT:
			overX += 1.0;

			break;
		default:
			break;
		}
		double overH = overY + over->_getRelativeHeight();
		double overW = overX +over->_getRelativeWidth();
		
		
		
		if (getTop() < overY)
			return false;
		if (getLeft() < overX)
			return false;
		if (getTop() > overH)
			return false;
		if (getLeft() > overW)
			return false;
		return true;
	}

	bool MousePointer::mouseMoved(const OIS::MouseEvent & arg)
	{
		mMP->setPosition(arg.state.X.abs, arg.state.Y.abs);
		_GUI->injectMouse();
		return true;
	}

	bool MousePointer::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
	{
		mMP->setPosition(arg.state.X.abs, arg.state.Y.abs);
		_GUI->injectMouse();
		_GUI->mousePressed();
		return true;
	}

	bool MousePointer::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
	{
		return true;
	}

	Button::Button(const Ogre::Vector4 & Dimensions, const Ogre::String & Material, const Ogre::String & Text, const Callback & _callback, GUI*gui, const VERTICAL_ANCHOR & vertAnch, const HORINZONTAL_ANCHOR & horAnchor)
		:
		mmn(Material),
		mma(Material)
	{

		Ogre::ResourcePtr ma = Ogre::MaterialManager::getSingleton().getByName(mmn + ".active");
		if (!ma.isNull())
			mma += ".active";
		Ogre::String name;
		name.append("b");
		name.append(std::to_string(gui->getNBotones()));
		setMainOverlay(gui->createOverlay(name, { Dimensions.x,Dimensions.y }, { Dimensions.z,Dimensions.w }, vertAnch, horAnchor, mmn, Text));
		
		
		getMainOverlay()->show();
		callback = _callback;
		
	}

	

	GUIElement::~GUIElement()
	{
		
	}

	void GUIElement::setMainOverlay(Ogre::OverlayContainer * newOverlay)
	{
		
		mainOverlay = newOverlay;
	}

	void GUIElement::setPosition(const Ogre::Real & x, const Ogre::Real & y)
	{
		mainOverlay->setPosition(x, y);
	}

	Ogre::Real GUIElement::getLeft()
	{
		return mainOverlay->getLeft();
	}

	Ogre::Real GUIElement::getTop()
	{
		return mainOverlay->getTop();
	}

	void GUIElement::setDimension(const Ogre::Real & w, const Ogre::Real & h)
	{
		mainOverlay->setDimensions(w, h);
	}

	Ogre::Real GUIElement::getWidht()
	{
		return mainOverlay->getWidth();
	}

	Ogre::Real GUIElement::getHeight()
	{
		return mainOverlay->getHeight();
	}

	void GUIElement::setActive(const bool & active)
	{
		(active) ?mainOverlay->show() : mainOverlay->hide();
	}

	bool GUIElement::getActive()
	{
		return mainOverlay->isVisible();
	}


	GUIText::GUIText(const Ogre::Vector2 & position, const Ogre::String & caption, const int & charsize, const VERTICAL_ANCHOR & vertAnch, const HORINZONTAL_ANCHOR & horAnchor, GUI* gui)
	{
		Ogre::String name;
		name.append("t");
		name.append(std::to_string(gui->getNBotones()));
		setMainOverlay(gui->createTextOverlay(name, { position.x,position.y },charsize , vertAnch, horAnchor, caption));
		getMainOverlay()->show();
	}

	GUIText::~GUIText()
	{
	}

	void GUIText::setText(const Ogre::String & newText)
	{
		
	}
}