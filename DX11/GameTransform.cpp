#include "GameTransform.h"
#include "SimpleMath.h"
#include "GridComponent.h"
#include "Keys.h"

using namespace DirectX::SimpleMath;

GridComponent* grid;
TriangleComponent* triangleObject;

void GameTransform::Initialize()
{
	gameCamera = new Camera();
	gameCameraCtrl = new CameraController();

	grid = new GridComponent(device, context, {}, gameCamera); /////////////////////////////////////
	triangleObject = new TriangleComponent(device, context, { Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //позиция (от -1 до 1) //цвет
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), }, gameCamera);

	trianglObjects.emplace_back(grid);
	trianglObjects.emplace_back(triangleObject);
}

void GameTransform::Update(float deltaTime)
{
	gameCameraCtrl->Update(deltaTime);

	float velocity = 500.0f;

	/*if (inputDevice->IsKeyDown(Keys::A))
	{
		triangleObject->objectPosition += velocity * Vector3::Left * deltaTime;
	}
	if (inputDevice->IsKeyDown(Keys::D))
	{
		triangleObject->objectPosition += velocity * Vector3::Right * deltaTime;
	}
	if (inputDevice->IsKeyDown(Keys::W))
	{
		triangleObject->objectPosition += velocity * Vector3::Up * deltaTime;
	}
	if (inputDevice->IsKeyDown(Keys::S))
	{
		triangleObject->objectPosition += velocity * Vector3::Down * deltaTime;
	}

	if (inputDevice->IsKeyDown(Keys::Escape))
	{
		Exit();
	}*/

	Game::Update(deltaTime);//вызывкется апдейт для всех компонентов
}