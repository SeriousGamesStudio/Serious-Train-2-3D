#ifndef _H_GUINDILLA_H_
#define _H_GUINDILLA_H_

/// Betajaen's GUI 015 Uncompressed
/// Written by Robin "Betajaen" Southern 07-Nov-2006, http://www.ogre3d.org/wiki/BetaGUI
/// This code is under the Whatevar! licence. Do what you want; but keep the original copyright header.
#include "Ogre.h"
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>
#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <vector>
#include <OIS.h>
#include <functional>


using namespace std;
namespace GUIndilla {

	class GUI;
	class MousePointer;
	class Window;
	class Button;
	class TextInput;
	class Callback;

	enum WINDOW_TYPE {
		WT_NONE = 0,
		WT_MOVE = 1,
		WT_RESIZE = 2,
		WT_RESIZE_AND_MOVE = 3
	};

	enum POSITION_TYPE { 
		PT_ABSOLUTE = Ogre::GuiMetricsMode::GMM_PIXELS,
		PT_REL= Ogre::GuiMetricsMode::GMM_RELATIVE,
	};

	enum HORINZONTAL_ANCHOR 
	{
		HA_LEFT = Ogre::GuiHorizontalAlignment::GHA_LEFT,
		HA_CENTER = Ogre::GuiHorizontalAlignment::GHA_CENTER,
		HA_RIGHT = Ogre::GuiHorizontalAlignment::GHA_RIGHT

	};
	enum VERTICAL_ANCHOR
	{
		VA_TOP= Ogre::GuiVerticalAlignment::GVA_TOP,
		VA_CENTER = Ogre::GuiVerticalAlignment::GVA_CENTER,
		VA_BOTTOM = Ogre::GuiVerticalAlignment::GVA_BOTTOM

	};

	

	class GUI {
	public:
		

		GUI(const Ogre::String & font, const Ogre::uint & fontSize);
		~GUI();
		bool injectMouse();
		void mousePressed();
				
		Ogre::OverlayContainer* createOverlay(const Ogre::String & name, const Ogre::Vector2 & position, const Ogre::Vector2 & dimensions , const POSITION_TYPE & posType, const VERTICAL_ANCHOR & vertAnch, const HORINZONTAL_ANCHOR & horAnchor,const Ogre::String & material = "", const Ogre::String & caption = "");
		
		MousePointer* createMousePointer(const Ogre::Vector2 & dimensions, const Ogre::String & material);
		Button * createButton(const Ogre::Vector4 & Dimensions, const Ogre::String & M, const Ogre::String & T, const Callback & C,  const POSITION_TYPE & posType, const VERTICAL_ANCHOR & vertAnch = VERTICAL_ANCHOR::VA_TOP , const HORINZONTAL_ANCHOR & horAnchor= HORINZONTAL_ANCHOR::HA_LEFT);
		Button * createStaticImage(const Ogre::Vector4 & Dimensions, const Ogre::String & Material,const POSITION_TYPE & posType, const VERTICAL_ANCHOR & vertAnch = VERTICAL_ANCHOR::VA_TOP, const HORINZONTAL_ANCHOR & horAnchor = HORINZONTAL_ANCHOR::HA_LEFT);
		Button * createStaticText(const Ogre::Vector4 & Dimensions, const Ogre::String & Text, const POSITION_TYPE & posType, const VERTICAL_ANCHOR & vertAnch = VERTICAL_ANCHOR::VA_TOP, const HORINZONTAL_ANCHOR & horAnchor = HORINZONTAL_ANCHOR::HA_LEFT);


		inline MousePointer* getMousePointer() { return mousePointer; }
		inline int getNBotones() { return nButtons; }
		void ClearGUI();

	protected:
		Button * activeButton;
		Ogre::Overlay* mO;                        // Main sheet overlay
		std::vector<Button*> Elementos;                    // Botones in
		std::vector<Button*> botones;                    // Botones in

		Window *mXW;                        // Window to destroy
		MousePointer* mousePointer;                // Mouse Pointer Overlay
		Ogre::String mFont;
		Ogre::uint mFontSize;

		int nButtons;
	};
	


	class MousePointer: public OIS::MouseListener
	{
	public:
		MousePointer(GUI * gui, const Ogre::Vector2 & dimensions, const Ogre::String & material);
		virtual ~MousePointer();
		inline bool isActive() { return _active; }
		void setActive(const bool &active);
		bool IsMouseOver(Ogre::OverlayContainer * over);
		inline float getLeft() { return mMP->_getLeft(); }
		inline float getTop() { return mMP->_getTop(); }

	protected:
		GUI * _GUI;
		bool _active;
		Ogre::OverlayContainer* mMP;                // Mouse Pointer Overlay
		virtual bool mouseMoved(const OIS::MouseEvent &arg);
		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	};

	

	class Callback {
	public:
		

		Callback() {
			_function = defaultCallback;
		}

		Callback(std::function<void()> function) {
			_function = function;
		}
		void onButtonPress() {
			_function();
		};
	protected:
		static void defaultCallback()
		{
			printf("This button dont give a shit when pressed\n");
		}
		std::function<void()> _function;
	};

	class GUIElement
	{
	public:
		virtual ~GUIElement();
		inline Ogre::OverlayContainer* getMainOverlay() { return mainOverlay; }
		void setMainOverlay(Ogre::OverlayContainer* newOverlay);

		void setPosition(const Ogre::Real & x, const Ogre::Real & y);
		Ogre::Real getLeft();
		Ogre::Real getTop();
		
		void setDimension(const Ogre::Real & w, const Ogre::Real & h);
		Ogre::Real  getWidht();
		Ogre::Real  getHeight();


		void setActive(const bool & active);
		bool getActive();
	protected:


		


	private:
		Ogre::OverlayContainer* mainOverlay;
	};

	class Button:
		public GUIElement
	{
	public:

		Button(const Ogre::Vector4 & Dimensions, const Ogre::String &  Material, const Ogre::String & Text, const Callback & callback, GUI*gui,  const VERTICAL_ANCHOR & vertAnch, const HORINZONTAL_ANCHOR & horAnchor);
		virtual ~Button();
		inline void activate(bool a) {
			if (!a && mmn != "")
				getMainOverlay()->setMaterialName(mmn);
			if (a && mma != "")
				getMainOverlay()->setMaterialName(mma);
		}
		Callback callback;            // Callback to use
	protected:
		
		Ogre::String mmn, mma;                        // MaterialName Normal, MaterialName Active
		                      // Dimensions.
	};

	class GUIText :
		public GUIElement
	{
	public:
		GUIText(const Ogre::Vector2 & position, const Ogre::String & caption, const int charsize, const POSITION_TYPE & posType, const VERTICAL_ANCHOR & vertAnch, const HORINZONTAL_ANCHOR & horAnchor);
		~GUIText();
		void setText(const Ogre::String & newText);
	private:
	};

} // End of Namespace
#endif // !_H_GUINDILLA_H_

