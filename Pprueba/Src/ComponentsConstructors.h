#ifndef _COMPONENTS_CONSTRUCTORS_
#define _COMPONENTS_CONSTRUCTORS_

#include <string>
#include "rapidxml.hpp"
#include "Components.h"

namespace ComponentConstructors {

	class ComponentConstructor 
	{
	public:
		ComponentConstructor() {};
		virtual ~ComponentConstructor() {};

		//virtual rapidxml::xml_node<>* getXML() = 0;
	protected:
		//Parses xml node to init the info needed to construct the component
		virtual void parse(rapidxml::xml_node<>* src) {};
	};
	
	class PlayerController :
		public ComponentConstructor
	{
	public:
		PlayerController() : ComponentConstructor() {};
		~PlayerController() {};
		/*rapidxml::xml_node<>* getXML()
		{
			return rapidxml::memory_pool<char>::allocate_node();
		};*/
	};

	class Camera :
		public ComponentConstructor
	{
	public:
		Camera(): ComponentConstructor() {};
		~Camera() {};
		/*rapidxml::xml_node<> getXML()
		{
			return "<Camera/>\n";
		}*/
	};

	class CameraController:
		public ComponentConstructor
	{
	public:
		CameraController() : ComponentConstructor() {};
		~CameraController() {};

		/*rapidxml::xml_node<> getXML()
		{
			return "<CameraController/>\n";
		}*/
	};

	class Walker :
		public ComponentConstructor 
	{
	public:
		Walker() : ComponentConstructor(){};
		~Walker() {};
		/*rapidxml::xml_node<> getXML()
		{
			return "<Walker/>\n";
		}*/
	};

	class MeshRenderer :
		public ComponentConstructor
	{
	public:
		MeshRenderer(rapidxml::xml_node<>* src) : ComponentConstructor(), meshPath("")
		{
			parse(src);
		}
		~MeshRenderer() {};

		/*rapidxml::xml_node<> getXML()
		{
			std::string s = "";
			s += "<MeshRenderer>\n";
			s += "\t<MeshPath>" + meshPath + "<MeshPath/>\n";
			s += "<MeshRenderer/>\n";
			return s;
		}*/

	private:
		std::string meshPath;

		void parse(rapidxml::xml_node<>* src)
		{
			meshPath = src->first_node("MeshPath")->value();
		}
	};

	class PlaneRenderer :
		public ComponentConstructor
	{
	public:
		PlaneRenderer(rapidxml::xml_node<>* src):
			ComponentConstructor() { parse(src); };
		~PlaneRenderer() {};

		/*rapidxml::xml_node<>	getXML()
		{
			std::string s = "";
			s += "<PlaneRenderer>\n";
			s += "\t<MeshPath>" + meshPath + "<MeshPath/>\n";
			s += "\t<TexturePath>" + texturePath + "<TexturePath/>\n";
			s += "<PlaneRenderer/>\n";
			return s;
		}*/

	private:
		std::string meshPath;
		std::string texturePath;

		void parse(rapidxml::xml_node<>* src)
		{
			meshPath = src->first_node("MeshPath")->value();
			texturePath = src->first_node("TexturePath")->value();
		};
	};


};
#endif // !_COMPONENTS_CONSTRUCTORS_

