#ifndef _COMPONENTS_CONSTRUCTORS_
#define _COMPONENTS_CONSTRUCTORS_

#include <string>

namespace ComponentConstructors {

	class ComponentConstructor 
	{
	public:
		ComponentConstructor() {};
		virtual ~ComponentConstructor() {};

		virtual std::string getXML() = 0;
	protected:
		virtual void parse(std::string src) {};
	};


	class PlayerController :
		public ComponentConstructor
	{
	public:
		PlayerController() : ComponentConstructor() {};
		~PlayerController() {};
		std::string getXML()
		{
			return "<PlayerController/>\n";
		};
	};

	class Camera :
		public ComponentConstructor
	{
	public:
		Camera(): ComponentConstructor() {};
		~Camera() {};
		std::string getXML()
		{
			return "<Camera/>\n";
		}
	};

	class CameraController:
		public ComponentConstructor
	{
		CameraController() : ComponentConstructor() {};
		~CameraController() {};

		std::string getXML()
		{
			return "<CameraController/>\n";
		}
	};

	class Walker :
		public ComponentConstructor 
	{
		Walker() : ComponentConstructor(){};
		~Walker() {};
		std::string getXML()
		{
			return "<Walker/>\n";
		}
	};

	class MeshRenderer :
		public ComponentConstructor
	{
		MeshRenderer(std::string src) : ComponentConstructor(), meshPath(std::string())
		{
			parse(src);
		}
		~MeshRenderer() {};

		std::string getXML()
		{
			std::string s = "";
			s += "<MeshRenderer>\n";
			s += "\t<MeshPath>" + meshPath + "<MeshPath/>\n";
			s += "<MeshRenderer/>\n";
			return s;
		}

	private:
		std::string meshPath;

		void parse(std::string src)
		{
			//NOT IMPLEMENTED
		}
	};

	class PlaneRenderer :
		public ComponentConstructor
	{
		PlaneRenderer(std::string src):
			ComponentConstructor() { parse(src); };
		~PlaneRenderer() {};

		std::string	getXML()
		{
			std::string s = "";
			s += "<PlaneRenderer>\n";
			s += "\t<MeshPath>" + meshPath + "<MeshPath/>\n";
			s += "\t<TexturePath>" + texturePath + "<TexturePath/>\n";
			s += "<PlaneRenderer/>\n";
			return s;
		}

	private:
		std::string meshPath;
		std::string texturePath;

		void parse(std::string src)
		{
			//NotImplemented
		};
	};


};
#endif // !_COMPONENTS_CONSTRUCTORS_

