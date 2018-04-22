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

		std::string meshPath;
	private:
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

		std::string meshPath;
		std::string texturePath;

	private:
		void parse(rapidxml::xml_node<>* src)
		{
			meshPath = src->first_node("MeshPath")->value();
			texturePath = src->first_node("TexturePath")->value();
		};
	};

	class RigidBody :
		public ComponentConstructor 
	{
	public: 
		enum Shape {SPHERE, BOX, CYLINDER, CAPSULE};
		std::string shapeToString[4] = {"SPHERE", "BOX", "CYLINDER", "CAPSULE" };
		Shape stringToShape(std::string s) 
		{
			for (int i = 0; i < 4; i++)
				if (s == shapeToString[i])
					return (Shape)i;
		}
		struct Dimensions { float x = 0; float y = 0; float z = 0; };

	public:
		RigidBody(rapidxml::xml_node<>* src): ComponentConstructor()
		{
			parse(src);
		}
		Shape shape;
		Dimensions dimensions;
		Dimensions position;
		Dimensions rotationAxis;
		float rotationAngle;
		float mass;
		Dimensions inertia;
	private:
		void parse(rapidxml::xml_node<>* src)
		{
			//Get the shape
			shape = stringToShape(src->first_node("Shape")->value());
			//Get the Collision Shape dimensions 
			switch (shape)
			{
			case ComponentConstructors::RigidBody::SPHERE:
				dimensions.x = std::stof(src->first_node("Radious")->value());
				break;
			case ComponentConstructors::RigidBody::BOX:
				dimensions.x = std::stof(src->first_node("Width")->value());
				dimensions.y = std::stof(src->first_node("Height")->value());
				dimensions.z = std::stof(src->first_node("Depth")->value());
				break;
			case ComponentConstructors::RigidBody::CYLINDER:
				if (!src->first_node("RadiousX") || !src->first_node("RadiousZ"))
					dimensions.x = dimensions.z = std::stof(src->first_node("Radious")->value());
				else
				{
					dimensions.x = std::stof(src->first_node("RadiousX")->value());
					dimensions.z = std::stof(src->first_node("RadiousZ")->value());
				}
				dimensions.y = std::stof(src->first_node("Height")->value());
				break;
			case ComponentConstructors::RigidBody::CAPSULE:
				dimensions.x = std::stof(src->first_node("Radious")->value());
				dimensions.y = std::stof(src->first_node("Height")->value());
				break;
			default:
				break;
			}
			//Get position
			position.x = std::stof(src->first_node("Position")->first_node("x")->value());
			position.y = std::stof(src->first_node("Position")->first_node("y")->value());
			position.z = std::stof(src->first_node("Position")->first_node("z")->value());
			//Get rotation
			rotationAxis.x = std::stof(src->first_node("Rotation")->first_node("x")->value());
			rotationAxis.y = std::stof(src->first_node("Rotation")->first_node("y")->value());
			rotationAxis.z = std::stof(src->first_node("Rotation")->first_node("z")->value());
			rotationAngle  = std::stof(src->first_node("Rotation")->first_node("angle")->value());
			//Get Mass
			mass = std::stof(src->first_node("Mass")->value());
			//Get Inertia
			inertia.x = std::stof(src->first_node("Inertia")->first_node("x")->value());
			inertia.y = std::stof(src->first_node("Inertia")->first_node("y")->value());
			inertia.z = std::stof(src->first_node("Inertia")->first_node("z")->value());
		}
	};

	class Collider :
		public ComponentConstructor
	{
	public:
		enum Shape { SPHERE, BOX, CYLINDER, CAPSULE };
		std::string shapeToString[4] = { "SPHERE", "BOX", "CYLINDER", "CAPSULE" };
		Shape stringToShape(std::string s)
		{
			for (int i = 0; i < 4; i++)
				if (s == shapeToString[i])
					return (Shape)i;
		}
		struct Dimensions { float x = 0; float y = 0; float z = 0; };
		Collider(rapidxml::xml_node<>* src) : ComponentConstructor()
		{
			parse(src);
		};
		~Collider() {};

		Shape shape;
		Dimensions dimensions;
		Dimensions position;
		Dimensions rotationAxis;
		float rotationAngle;
	private:
		void parse(rapidxml::xml_node<>* src)
		{
			//Get the shape
			shape = stringToShape(src->first_node("Shape")->value());
			//Get the Collision Shape dimensions 
			switch (shape)
			{
			case ComponentConstructors::RigidBody::SPHERE:
				dimensions.x = std::stof(src->first_node("Radious")->value());
				break;
			case ComponentConstructors::RigidBody::BOX:
				dimensions.x = std::stof(src->first_node("Width")->value());
				dimensions.y = std::stof(src->first_node("Height")->value());
				dimensions.z = std::stof(src->first_node("Deep")->value());
				break;
			case ComponentConstructors::RigidBody::CYLINDER:
				if (!src->first_node("RadiousX") || !src->first_node("RadiousZ"))
					dimensions.x = dimensions.z = std::stof(src->first_node("Radious")->value());
				else
				{
					dimensions.x = std::stof(src->first_node("RadiousX")->value());
					dimensions.z = std::stof(src->first_node("RadiousZ")->value());
				}
				dimensions.y = std::stof(src->first_node("Height")->value());
				break;
			case ComponentConstructors::RigidBody::CAPSULE:
				dimensions.x = std::stof(src->first_node("Radious")->value());
				dimensions.y = std::stof(src->first_node("Height")->value());
				break;
			default:
				break;
			}
			//Get position
			position.x = std::stof(src->first_node("Position")->first_node("x")->value());
			position.y = std::stof(src->first_node("Position")->first_node("y")->value());
			position.z = std::stof(src->first_node("Position")->first_node("z")->value());
			//Get rotation
			rotationAxis.x = std::stof(src->first_node("Rotation")->first_node("x")->value());
			rotationAxis.y = std::stof(src->first_node("Rotation")->first_node("y")->value());
			rotationAxis.z = std::stof(src->first_node("Rotation")->first_node("z")->value());
			rotationAngle = std::stof(src->first_node("Rotation")->first_node("angle")->value());
		}

	};

	class Transform :
		public ComponentConstructor
	{
	public:
		Transform(rapidxml::xml_node<>* src) : ComponentConstructor()
		{
			parse(src);
		}
		~Transform() {};

		float position[3];
		float rotation[4];
	private:
		void parse(rapidxml::xml_node<>* src)
		{
			position[0] = std::stof(src->first_node("Position")->first_node("x")->value());
			position[1] = std::stof(src->first_node("Position")->first_node("y")->value());
			position[2] = std::stof(src->first_node("Position")->first_node("z")->value());

			rotation[0] = std::stof(src->first_node("Rotation")->first_node("x")->value());
			rotation[1] = std::stof(src->first_node("Rotation")->first_node("y")->value());
			rotation[2] = std::stof(src->first_node("Rotation")->first_node("z")->value());
			rotation[3] = std::stof(src->first_node("Rotation")->first_node("w")->value());
		}
	};

};
#endif // !_COMPONENTS_CONSTRUCTORS_

