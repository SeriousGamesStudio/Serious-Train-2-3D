#include "Camera_c.h"

Camera_c::Camera_c(Entity * entity, GraphicsManager * graphicsManager): 
	Component(Component(entity,ComponentType::CAMERA))
{
	camNode_ = graphicsManager_->createNewNode(NULL);
	cam = graphicsManager_->getSceneManager()->createCamera("MainCam");
	cam->setPosition(0, 0, 80);
	cam->lookAt(0, 0, -300);
	cam->setNearClipDistance(5);

	//ViewPort Addition
	vp = graphicsManager_->getWindow()->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	cam->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));
	foreward = left = right = backward = false;

}

Camera_c::~Camera_c()
{

}

void Camera_c::update()
{
	if (foreward) {
		cam->setPosition(cam->getPosition().x, cam->getPosition().y,
			cam->getPosition().z - 1);
	}
	if (backward) {
		cam->setPosition(cam->getPosition().x, cam->getPosition().y,
			cam->getPosition().z + 1);
	}
	if (left) {
		cam->setPosition(cam->getPosition().x - 1, cam->getPosition().y,
			cam->getPosition().z);
	}
	if (right) {
		cam->setPosition(cam->getPosition().x + 1, cam->getPosition().y,
			cam->getPosition().z);
	}
}

void Camera_c::listen(Msg_Base * msg)
{
	Msg::CameraMove * p = static_cast<Msg::CameraMove*>(msg);
	switch (msg->id)		//tipo de mensaje
	{
	case MsgId::CAMERA_MOVED: 
		switch (p->info)		//info del tipo del mensaje
		{
		case Msg::CameraMove::Move::FOREWARD:
			foreward = true;
			break;
		case Msg::CameraMove::Move::BACKWARD:
			backward = true;
			break;
		case Msg::CameraMove::Move::LEFT:
			left = true;
			break;
		case Msg::CameraMove::Move::RIGHT:
			right = true;
			break;
		default:
			break;
		}
	case MsgId::CAMERA_STOPPED:
//		Msg::CameraMove * p = static_cast<Msg::CameraMove*>(msg);
		switch (p->info)		//info del tipo del mensaje
		{
		case Msg::CameraMove::Move::FOREWARD:
			foreward = false;
			break;
		case Msg::CameraMove::Move::BACKWARD:
			backward = false;
			break;
		case Msg::CameraMove::Move::LEFT:
			left = false;
			break;
		case Msg::CameraMove::Move::RIGHT:
			right = false;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

}
