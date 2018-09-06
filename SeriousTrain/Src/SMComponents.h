#ifndef _SCENE_MAKER_COMPONENTS_
#define _SCENE_MAKER_COMPONENTS_

#include <string>
#include "SMstringOperations.h"
namespace SceneMaker {
	using Attrs = std::vector<SceneMaker::KeyValue>;
	namespace Components {
		class Component {
		public:
			Component() {};
			~Component() {};

			virtual std::string getXML() = 0;
		};

		//COMPONENTS////////////////////////////////////////////////////
		class PlayerController :
			public Component
		{
		public:
			PlayerController() : Component() {};
			~PlayerController() {};
			std::string getXML()
			{
				using namespace SceneMaker::StringOperations;
				Attrs attrs =
				{
					{"type", "PlayerController"}
				};
				return makeSelfClosingItem("Component", attrs);
			};
		};
		class Camera :
			public Component
		{
		public:
			Camera() :Component() {};
			~Camera() {};

			std::string getXML()
			{
				using namespace SceneMaker::StringOperations;
				Attrs attrs =
				{
					{"type", "Camera"}
				};
				return makeSelfClosingItem("Component", attrs);
			};

		};
		class CameraController :
			public Component
		{
		public:
			CameraController() : Component() {};
			~CameraController() {};
			std::string getXML()
			{
				using namespace SceneMaker::StringOperations;
				Attrs attrs =
				{
					{"type" , "CameraController"}
				};
				return makeSelfClosingItem("Component", attrs);
			}
		};
		class MeshRenderer :
			public Component
		{
		public:
			MeshRenderer() :
				Component(), _meshName("Not setted Mesh!") {};
			~MeshRenderer() {};
			void setAttrs(std::string meshName) { _meshName = meshName; };
			std::string getXML()
			{
				using namespace SceneMaker::StringOperations;
				Attrs attrs =
				{
					{"type", "MeshRenderer"},
					{"MeshName", _meshName}
				};
				return makeSelfClosingItem("Component", attrs);
			}
		private:
			std::string _meshName;
		};
		class Walker :
			public Component
		{
		public:
			Walker() : Component() {};
			~Walker() {};
			std::string getXML()
			{
				using namespace SceneMaker::StringOperations;
				Attrs attrs =
				{
					{"type", "Walker"}
				};
				return makeSelfClosingItem("Component", attrs);
			}

		};
		class PlaneRenderer :
			public Component
		{
		public:
			PlaneRenderer() : Component() {};
			~PlaneRenderer() {};

			std::string getXML()
			{
				using namespace SceneMaker::StringOperations;
				Attrs attrs =
				{
					{"type", "PlaneRenderer"}
				};
				return makeSelfClosingItem("Component", attrs);
			}

		};
		class RigidBody :
			public Component
		{
		public:
			struct Info
			{
				class CollisionShape
				{
				public:
					enum class Type { SPHERE, BOX, CYLINDER, CAPSULE };
					CollisionShape() { printf("Esta constructora NO debe utilizarse"); };
					CollisionShape(CollisionShape::Type type, std::initializer_list<float> params) : _type(type)
					{
						switch (_type)
						{
						case RigidBody::Info::CollisionShape::Type::SPHERE:
							dim.x = *(params.begin());
							break;
						case RigidBody::Info::CollisionShape::Type::BOX:
							dim.x = *(params.begin() + 0);
							dim.y = *(params.begin() + 1);
							dim.z = *(params.begin() + 2);
							break;
						case RigidBody::Info::CollisionShape::Type::CYLINDER:
							dim.x = *(params.begin() + 0);
							dim.y = *(params.begin() + 1);
							dim.z = *(params.begin() + 2);
							break;
						case RigidBody::Info::CollisionShape::Type::CAPSULE:
							dim.x = *(params.begin() + 0);
							dim.y = *(params.begin() + 1);
							break;
						default:
							break;
						}

					};
					string getXML()
					{
						string d = "<Dimensions ";
						switch (_type)
						{
						case RigidBody::Info::CollisionShape::Type::SPHERE:
							d += "\"radius\" = \"" + std::to_string(dim.x) + "\" />\n";
							break;
						case RigidBody::Info::CollisionShape::Type::BOX:
							d += "\"height\" = \"" + std::to_string(dim.y) +
								"\" \"width\" = \"" + std::to_string(dim.x) +
								"\" \"depth\" = \"" + std::to_string(dim.z) +
								"\" />\n";
							break;
						case RigidBody::Info::CollisionShape::Type::CYLINDER:
							d += "\"height\" = \"" + std::to_string(dim.y) +
								"\" \"width\" = \"" + std::to_string(dim.x) +
								"\" \"depth\" = \"" + std::to_string(dim.z) +
								"\" />\n";
							break;
						case RigidBody::Info::CollisionShape::Type::CAPSULE:
							break;
							d += "\"radius\" = \"" + std::to_string(dim.x) +
								"\" \"height\" = \"" + std::to_string(dim.y) +
								"\" />\n";
						default:
							break;
						}
						return d;
					}
					CollisionShape::Type getType() { return _type; };
				private:
					union Dimensions
					{
						float x;
						float y;
						float z;
					};
					CollisionShape::Type _type;
					Dimensions dim;
				};
				CollisionShape collision;
				float posX, posY, posZ;
				float Qx, Qy, Qz, Qw;
				float mass;
				float inertiaX, inertiaY, inertiaZ;

			};
		public:
			RigidBody() :Component() {};
			~RigidBody() {};
			std::string getXML()
			{
				using namespace SceneMaker::StringOperations;
				using namespace std;
				Attrs attrs =
				{
					{"type", "Rigidbody"},
				};
				string data = string();
				//Add mass
				data += makeClosingItem("Mass", std::to_string(_info.mass));
				//Add Position
				data += makeSelfClosingItem("Position",
					{
						{"x", to_string(_info.posX)},
						{"y", to_string(_info.posY)},
						{"z", to_string(_info.posZ)}
					}
				);
				//Add Inertia
				data += makeSelfClosingItem("Inertia",
					{
						{ "x", to_string(_info.inertiaX) },
						{ "y", to_string(_info.inertiaY) },
						{ "z", to_string(_info.inertiaZ) }
					}
				);
				//Add rotation
				data += makeSelfClosingItem("Rotation",
					{
						{ "x", to_string(_info.Qx) },
						{ "y", to_string(_info.Qy) },
						{ "z", to_string(_info.Qz) },
						{ "w", to_string(_info.Qw) }
					}
				);
				//Add CollisionShape info
				data += makeGroup("CollisionShape",
					{
						makeSelfClosingItem("type", {{"value", to_string((int)_info.collision.getType())}}),
						_info.collision.getXML()
					}
				);

				return makeClosingItem("Component", attrs, data);
			}
			void setAttrs(Info& info) { _info = info; };
			Info _info;
		};
		//COMPONENTS////////////////////////////////////////////////////

		enum Type
		{
			PLAYER_CONTROLLER,
			CAMERA_CONTROLLER,
			CAMERA,
			WALKER,
			MESH_RENDERER,
			PLANE_RENDERER,
			RIGIDBODY
		};
		static SceneMaker::Components::Component* factory(Type type)
		{
			SceneMaker::Components::Component* newComponent = nullptr;
			switch (type)
			{
			case PLAYER_CONTROLLER:
				newComponent = new SceneMaker::Components::PlayerController();
				break;
			case CAMERA_CONTROLLER:
				newComponent = new SceneMaker::Components::CameraController();
				break;
			case CAMERA:
				newComponent = new SceneMaker::Components::Camera();
				break;
			case WALKER:
				newComponent = new SceneMaker::Components::Walker();
				break;
			case MESH_RENDERER:
				newComponent = new SceneMaker::Components::MeshRenderer();
				break;
			case PLANE_RENDERER:
				newComponent = new SceneMaker::Components::PlaneRenderer();
				break;
			case RIGIDBODY:
				newComponent = new SceneMaker::Components::RigidBody();
				break;
			default: break;
			}
			return newComponent;
		}
	}
}

#endif // !_SCENE_MAKER_COMPONENTS_