#include "GameTransform.h"
#include "SimpleMath.h"
#include "GridComponent.h"
#include "Keys.h"

using namespace DirectX::SimpleMath;

//GridComponent* grid;//сетка
TriangleComponent* triangleObject;//объект из треугольников

//GameTransform::GameTransform(DisplayWindow* display)
//{
//	//gameInstance = this;
//	Game::appDisplay = display;
//	Game::name = name;
//}

void GameTransform::Initialize()//создание камеры и объектов
{
	gameCamera = new Camera(this);
	gameCameraCtrl = new CameraController(this, gameCamera);

	//grid = new GridComponent(device, context, {}, gameCamera); /////////////////////////////////////
	triangleObject = new TriangleComponent(device, context, { Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //позиция (от -1 до 1) //цвет
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), }, gameCamera);

	//trianglObjects.emplace_back(grid);
	trianglObjects.emplace_back(triangleObject);
}

void GameTransform::Update(float deltaTime)//1
{
	gameCameraCtrl->Update(deltaTime);

	float velocity = 10.0f;

	//перемещение объекта
	if (inputDevice->IsKeyDown(Keys::Left))//37
	{
		triangleObject->objectPosition += velocity * Vector3::Left * deltaTime;
		std::cout << "Move odject left" << std::endl;
	}
	if (inputDevice->IsKeyDown(Keys::Right))//39
	{
		triangleObject->objectPosition += velocity * Vector3::Right * deltaTime;
		std::cout << "Move odject right" << std::endl;
	}
	if (inputDevice->IsKeyDown(Keys::Up))//38
	{
		triangleObject->objectPosition += velocity * Vector3::Up * deltaTime;
		std::cout << "Move odject up" << std::endl;
	}
	if (inputDevice->IsKeyDown(Keys::Down))//40
	{
		triangleObject->objectPosition += velocity * Vector3::Down * deltaTime;
		std::cout << "Move odject down" << std::endl;
	}

	//if (inputDevice->IsKeyDown(Keys::Escape))
	//{
	//	//Exit();
	//	std::cout << "Exit sas" << std::endl;//
	//}

	Game::Update(deltaTime);//вызывкется апдейт для всех компонентов
}

void GameTransform::OnMouseMove(InputDevice::MouseMoveEventArgs& args)
{
	std::cout << args.Position.x << " " << args.Position.y << std::endl; //отслеживание курсора
}