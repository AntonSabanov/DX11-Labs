#include "pch.h"
#include "Camera.h"
#include "Game.h"

Camera::Camera(Game* gameInst)
{
	game = gameInst;
	viewMatrix = DirectX::SimpleMath::Matrix::Identity;
	projectionMatrix = UpdateProjectionMatrix();
}

DirectX::SimpleMath::Matrix Camera::GetCameraMatrix() const
{
	//DirectX::SimpleMath::Matrix  sas = -1.0f * viewMatrix.Translation();
	return projectionMatrix * viewMatrix;
}

DirectX::SimpleMath::Vector3 Camera::GetPosition() const
{
	/*DirectX::SimpleMath::Matrix inv;
	viewMatrix.Invert(inv);
	return inv.Translation();*/
	return -viewMatrix.Translation();
}

DirectX::SimpleMath::Vector3 Camera::GetForwardVector() const
{
	DirectX::SimpleMath::Matrix inv;
	viewMatrix.Invert(inv);
	return inv.Forward();
}

DirectX::SimpleMath::Matrix Camera::UpdateProjectionMatrix()
{
	return DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		(float)3.141592653f / 2.0f, 
		(float)(game->appDisplay->screenWidth / (float)game->appDisplay->screenHeight),
		0.1f, //near plane
		10000.0f); //far plane
}
