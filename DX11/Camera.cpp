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
	return viewMatrix * projectionMatrix;
}

DirectX::SimpleMath::Vector3 Camera::GetPosition() const
{
	//DirectX::SimpleMath::Matrix inv;
	//viewMatrix.Invert(inv);
	//return inv.Translation();
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
		(float)3.14159265358979323846f / 2.0f,
		(float)(game->appDisplay->screenWidth / (float)game->appDisplay->screenHeight),
		0.1f, //near plane
		10000.0f); //far plane
}