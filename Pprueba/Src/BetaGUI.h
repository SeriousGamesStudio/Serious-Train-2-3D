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


		GUI(const Ogre::String & font, const Ogre::uint & fontSize);
		~GUI();
		bool injectMouse(const Ogre::uint & x, const Ogre::uint & y, const bool & LMB);
		Window*     createWindow(const Ogre::Vector4 &Dimensions , const Ogre::String & Material, const WINDOW_TYPE & type=WINDOW_TYPE::WT_NONE,const Ogre::String & caption = "");
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
		TextInput* mATI;                // Active TextInput
		Button *mRZ, *mAB, *mTB;            // Resizer, ActiveButton, Titlebar
		Ogre::uint x, y, w, h;                    // Dimensions
		GUI *mGUI;                        // mGUI pointer
		Ogre::OverlayContainer* mO;            // Overlay
		std::vector<GUIndilla::Button*> mB;    // Buttons
		std::vector<GUIndilla::TextInput*> mT;    // TextInputs
	};

	class Callback {
	public:
		friend class Window;

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

} // End of Namespace
