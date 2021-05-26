#include "GameTransform.h"
#include "SimpleMath.h"
#include "GridComponent.h"
#include "Keys.h"

using namespace DirectX::SimpleMath;

GridComponent* grid;//�����
TriangleComponent* triangleObject;//������ �� �������������

//GameTransform::GameTransform(DisplayWindow* display)
//{
//	//gameInstance = this;
//	Game::appDisplay = display;
//	Game::name = name;
//}

void GameTransform::Initialize()//�������� ������ � ��������
{
	gameCamera = new Camera(this);
	gameCameraCtrl = new CameraController(this, gameCamera);

	grid = new GridComponent(device, context, {}, gameCamera); /////////////////////////////////////
	triangleObject = new TriangleComponent(device, context, { Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), }, gameCamera);

	trianglObjects.emplace_back(grid);
	trianglObjects.emplace_back(triangleObject);
}

void GameTransform::Update(float deltaTime)//1
{
	gameCameraCtrl->Update(deltaTime);

	float velocity = 500.0f;

	//����������� �������
	if (inputDevice->IsKeyDown(Keys::A))
	{
		triangleObject->objectPosition += velocity * Vector3::Left * deltaTime;
		std::cout << "Move left" << std::endl;
	}
	if (inputDevice->IsKeyDown(Keys::D))
	{
		triangleObject->objectPosition += velocity * Vector3::Right * deltaTime;
		std::cout << "Move right" << std::endl;
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
		//Exit();
		std::cout << "Exit" << std::endl;//
	}

	Game::Update(deltaTime);//���������� ������ ��� ���� �����������
}

void GameTransform::OnMouseMove(InputDevice::MouseMoveEventArgs& args)
{
	std::cout << args.Position.x << " " << args.Position.y << std::endl; //������������ �������
}