#include "GameTransform.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

void GameTransform::Initialize()
{
	gameCamera = new Camera();
	gameCamCtrl = new CameraController();

	//plane = new PlaneComponent();
	//triangleObj = new TriangleComponent();

	//trianglObjects.emplace_back(plane);
	//trianglObjects.emplace_back(triangleObj);
}

void GameTransform::Update(float deltaTime)
{
	//gameCamCtrl->Update(deltaTime);

	//float velocity = 500.0f;

	//if (InDevice->IsKeyDown(Keys::A))
	//{
	//	triangleObj->position += velocity * DirectX::SimpleMath::Vector3::Left * deltaTime;
	//}
	//if (InDevice->IsKeyDown(Keys::D))
	//{
	//	triangleObj->position += velocity * DirectX::SimpleMath::Vector3::Right * deltaTime;
	//}
	//if (InDevice->IsKeyDown(Keys::W))
	//{
	//	triangleObj->position += velocity * DirectX::SimpleMath::Vector3::Up * deltaTime;
	//}
	//if (InDevice->IsKeyDown(Keys::S))
	//{
	//	triangleObj->position += velocity * DirectX::SimpleMath::Vector3::Down * deltaTime;
	//}

	//if (InDevice->IsKeyDown(Keys::Escape))
	//{
	//	Exit();
	//}

	//Game::Update();//вызывкется апдейт для всех компонентов
}