#ifndef _COMPONENTS_CONSTRUCTORS_
#define _COMPONENTS_CONSTRUCTORS_

#include <string>
#include "rapidxml.hpp"

namespace ComponentConstructors {

	class ComponentConstructor 
	{
	public:
		ComponentConstructor() {};
		virtual ~ComponentConstructor() {};

		virtual rapidxml::xml_node<> getXML() = 0;
	protected:
		virtual void parse(rapidxml::xml_node<>* src) {};
	};


	class PlayerController :
		public ComponentConstructor
	{
	public:
		PlayerController() : ComponentConstructor() {};
		~PlayerController() {};
		rapidxml::xml_node<> getXML()
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
		rapidxml::xml_node<> getXML()
		{
			return "<Camera/>\n";
		}
	};

	class CameraController:
		public ComponentConstructor
	{
	public:
		CameraController() : ComponentConstructor() {};
		~CameraController() {};

		rapidxml::xml_node<> getXML()
		{
			return "<CameraController/>\n";
		}
	};

	class Walker :
		public ComponentConstructor 
	{
	public:
		Walker() : ComponentConstructor(){};
		~Walker() {};
		rapidxml::xml_node<> getXML()
		{
			return "<Walker/>\n";
		}
	};

	class MeshRenderer :
		public ComponentConstructor
	{
	public:
		MeshRenderer(rapidxml::xml_node<>* src) : ComponentConstructor(), meshPath()
		{
			parse(src);
		}
		~MeshRenderer() {};

		rapidxml::xml_node<> getXML()
		{
			std::string s = "";
			s += "<MeshRenderer>\n";
			s += "\t<MeshPath>" + meshPath + "<MeshPath/>\n";
			s += "<MeshRenderer/>\n";
			return s;
		}

	private:
		rapidxml::xml_node<> meshPath;

		void parse(rapidxml::xml_node<>* src)
		{
			//NOT IMPLEMENTED
		}
	};

	class PlaneRenderer :
		public ComponentConstructor
	{
	public:
		PlaneRenderer(rapidxml::xml_node<>* src):
			ComponentConstructor() { parse(src); };
		~PlaneRenderer() {};

		rapidxml::xml_node<>	getXML()
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

		void parse(rapidxml::xml_node<>* src)
		{
			//NotImplemented
		};
	};


};
#endif // !_COMPONENTS_CONSTRUCTORS_

