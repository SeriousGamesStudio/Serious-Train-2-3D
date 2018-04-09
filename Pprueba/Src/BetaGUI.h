/// Betajaen's GUI 015 Uncompressed
/// Written by Robin "Betajaen" Southern 07-Nov-2006, http://www.ogre3d.org/wiki/BetaGUI
/// This code is under the Whatevar! licence. Do what you want; but keep the original copyright header.
#include "Ogre.h"
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>
#include <OgreFontManager.h>
#include <vector>
#include <OIS.h>

using namespace std;
namespace BetaGUI {

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
		PT_ABSOLUTE = 0,
		PT_REL= 1,
	};

	enum ANCHOR_TYPE {
		AT_LEFT_TOP		,
		AT_LEFT_MIDDLE	,
		AT_LEFT_BOTTOM	,
		AT_MIDDLE_TOP	,
		AT_MIDDLE_MIDDLE,
		AT_MIDDLE_BOTTOM,
		AT_RIGHT_TOP	,
		AT_RIGHT_MIDDLE	,
		AT_RIGHT_BOTTOM	,
	};

	

	class GUI {
	public:
		friend class Window;
		friend class Button;
		friend class TextInput;


		GUI(const Ogre::String & font, const Ogre::uint & fontSize);
		~GUI();
		bool injectMouse(const Ogre::uint & x, const Ogre::uint & y, const bool & LMB);
		bool injectKey(const Ogre::String &  key, const Ogre::uint & x, const Ogre::uint & y);
		void injectBackspace(const Ogre::uint & x, const Ogre::uint & y);
		Window*     createWindow(const Ogre::Vector4 &Dimensions , const Ogre::String & Material, const WINDOW_TYPE & type,const Ogre::String & caption = "");
		void destroyWindow(Window *window) {
			mXW = window;
		}
		Ogre::OverlayContainer* createOverlay(const Ogre::String & name, const Ogre::Vector2 & position, const Ogre::Vector2 & dimensions, const Ogre::String & material = "", const Ogre::String & caption = "", const bool & autoAdd = true);
		MousePointer* createMousePointer(const Ogre::Vector2 & dimensions, const Ogre::String & material);

	protected:
		Ogre::Overlay* mO;                        // Main sheet overlay
		std::vector<Window*>WN;                    // Windows
		Window *mXW;                        // Window to destroy
		MousePointer* mousePointer;                // Mouse Pointer Overlay
		Ogre::String mFont;
		Ogre::uint mFontSize;
		Ogre::uint wc, bc, tc;
	};
	
	class MousePointer: public OIS::MouseListener
	{
	public:
		MousePointer(GUI * gui, const Ogre::Vector2 & dimensions, const Ogre::String & material);
		virtual ~MousePointer();
		inline bool isActive() { return _active; }
		void setActive(const bool &active);
	protected:
		GUI * _GUI;
		bool _active;
		Ogre::OverlayContainer* mMP;                // Mouse Pointer Overlay
		virtual bool mouseMoved(const OIS::MouseEvent &arg);
		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	};

	class Window {
	public:
		friend class Button;
		friend class TextInput;
		friend class GUI;

		Button*        createButton(const Ogre::Vector4 &Dimensions , const Ogre::String & Material, const Ogre::String & Text, Callback callback);
		TextInput*    createTextInput(const Ogre::Vector4 &Dimensions , const Ogre::String & Material, const Ogre::String & value, const Ogre::uint & length);
		void        createStaticText(const Ogre::Vector4 &Dimensions , const Ogre::String & Text);

		void hide() {
			mO->hide();
		}

		void show() {
			mO->show();
		}

		bool isVisible() {
			return mO->isVisible();
		}
	protected:
		Window(const Ogre::Vector4 & Dimensions,const Ogre::String &Material,const WINDOW_TYPE &type, const Ogre::String & caption, GUI *gui);
		~Window();

		bool check(const Ogre::uint & x, const Ogre::uint & y, const bool & LMB); 
		bool checkKey(const Ogre::String & key,const Ogre::uint & x,const Ogre::uint & y);
		TextInput* mATI;                // Active TextInput
		Button *mRZ, *mAB, *mTB;            // Resizer, ActiveButton, Titlebar
		Ogre::uint x, y, w, h;                    // Dimensions
		GUI *mGUI;                        // mGUI pointer
		Ogre::OverlayContainer* mO;            // Overlay
		std::vector<BetaGUI::Button*> mB;    // Buttons
		std::vector<BetaGUI::TextInput*> mT;    // TextInputs
	};

	class BetaGUIListener {
	public:
		virtual void onButtonPress(Button *ref) = 0;
	};

	class Callback {
	public:
		friend class Window;
		friend class Button;

		Callback() {
			t = 0;
		}

		Callback(void(*functionPointer)(Button *ref)) {
			t = 1;
			fp = functionPointer;
		}

		Callback(BetaGUIListener *l) {
			t = 2;
			LS = l;
		}
	protected:
		Ogre::uchar t;                    // Type of callback: 0 - None, 1 - FunctionPointer 2 - GUIListener, 3 - Move Window, 4 - Resize
		void(*fp)(Button *ref);        // Function pointer (if 1)
		BetaGUIListener *LS;        // GuiListener (if 2)
	};

	class Button {
	public:
		friend class Window;

		Button(const Ogre::Vector4 &Dimensions , const Ogre::String & Material, const Ogre::String & Text, Callback callback, Window *parent);
		~Button() {
			mO->getParent()->removeChild(mO->getName());
			mCP->getParent()->removeChild(mCP->getName());
		}
	protected:
		inline void activate(const bool & a) {
			if (!a && mmn != "")
				mO->setMaterialName(mmn);
			if (a && mma != "")
				mO->setMaterialName(mma);
		}
		inline bool in(const Ogre::uint &mx, const Ogre::uint& my, const Ogre::uint &px, const Ogre::uint& py) {
			return (!(mx >= x + px && my >= y + py)) || (!(mx <= x + px + w && my <= y + py + h));
		}
		Ogre::OverlayContainer* mO, *mCP;            // Main Overlay and Caption
		Ogre::String mmn, mma;                        // MaterialName Normal, MaterialName Active
		BetaGUI::Callback callback;            // Callback to use
		Ogre::uint x, y, w, h;                        // Dimensions.
	};

	class TextInput {
	public:
		friend class Window;

		TextInput(const Ogre::Vector4 &Dimensions , const Ogre::String & Material, const Ogre::String & value, const Ogre::uint & length, Window *parent);
		~TextInput() {}
		Ogre::String getValue() {
			return value;
		}

		inline void setValue(const Ogre::String & v) {
			mCP->setCaption(value = v);
		}

	protected:
		inline bool in(const Ogre::uint &mx, const Ogre::uint& my, const Ogre::uint& px, const Ogre::uint& py) {
			return (!(mx >= x + px && my >= y + py)) || (!(mx <= x + px + w && my <= y + py + h));
		}
		Ogre::OverlayContainer* mO, *mCP;
		Ogre::String mmn, mma, value;
		Ogre::uint x, y, w, h, length;
	};
} // End of Namespace
