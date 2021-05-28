#include "GameTransform.h"
#include "SimpleMath.h"
#include "GridComponent.h"
#include "Keys.h"

using namespace DirectX::SimpleMath;

GridComponent* grid;//�����
GridComponent* basis;
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

	basis = new GridComponent(device, context, {
		Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //��� �
		Vector4(1.0f,  0.0f,  0.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f),

		Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), //��� �
		Vector4(0.0f,  1.0f,  0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),

		Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), //��� z
		Vector4(0.0f,  0.0f,  1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),},
		{0, 1, 2, 3, 4, 5},
		gameCamera);
	 
	grid = new GridComponent(device, context, {
		Vector4(-4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), //������������ �����
		Vector4(-4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-3.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-3.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-2.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-2.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-1.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-1.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(1.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(1.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(2.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(2.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 
		Vector4(3.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(3.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 
		//
		Vector4(-4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), //�������������� 
		Vector4(4.0f, 0.0f, 4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, 3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, 3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, 2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, 2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, -1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, -2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, -2.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, -3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, -3.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(4.0f, 0.0f, -4.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 
		},
		{ 0,1, 2,3, 4,5, 6,7, 8,9, 10,11, 12,13, 14,15, 16,17, 18,19, 20,21, 22,23, 24,25, 26,27, 28,29, 30,31, 32,33, 34,35}, 
		gameCamera);


	//���������
	//triangleObject = new TriangleComponent(device, context, { 
	//	Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
	//	Vector4(-0.5f, -0.5f, 0.5f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, 0.5f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),}, gameCamera);

	//triangleObject = new TriangleComponent(device, context, {
	//	Vector4(50.0f, 0.0f, 50.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
	//	Vector4(-50.0f, -0.0f, 50.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(50.0f, -0.0f, 50.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(-50.0f, 0.0f, 50.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), }, gameCamera);
	
	//�����
	triangleObject = new TriangleComponent(device, context, { 
		Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, -0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
		Vector4(0.5f, -0.5f, -0.5f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, -0.5f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 
		}, gameCamera);

	//triangleObject = new TriangleComponent(device, context, {
	//	Vector4(0.5f, 0.0f, 0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
	//	Vector4(-0.5f, -0.0f, 0.5f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, -0.0f, 0.5f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, 0.0f, 0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, 0.0f, -0.5f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
	//	Vector4(0.5f, -0.0f, -0.5f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, 0.0f, -0.5f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, -0.0f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), }, gameCamera);
	
	//triangleObject = new TriangleComponent(device, context, {
	//	Vector4(0.0f, 1.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(-1.0f, 0.0f, -1.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(1.0f, 0.0f, -1.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	//	Vector4(-1.0f, 0.0f, 1.0f, 1.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f),
	//	Vector4(1.0f, 0.0f, 1.0f, 1.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) }, gameCamera);


	trianglObjects.emplace_back(grid);
	trianglObjects.emplace_back(basis);
	trianglObjects.emplace_back(triangleObject);
}

void GameTransform::Update(float deltaTime)//1
{
	gameCameraCtrl->Update(deltaTime);

	float velocity = 5.0f;

	//����������� �������
	//if (inputDevice->IsKeyDown(Keys::Left))//37
	//{
	//	//triangleObject->objectPosition += velocity * Vector3::Left * deltaTime;
	//	basis->objectPosition += velocity * Vector3::Left * deltaTime;
	//	std::cout << "Move odject left" << std::endl;
	//}
	//if (inputDevice->IsKeyDown(Keys::Right))//39
	//{
	//	//triangleObject->objectPosition += velocity * Vector3::Right * deltaTime;
	//	basis->objectPosition += velocity * Vector3::Right * deltaTime;
	//	std::cout << "Move odject right" << std::endl;
	//}
	//if (inputDevice->IsKeyDown(Keys::Up))//38
	//{
	//	//triangleObject->objectPosition += velocity * Vector3::Up * deltaTime;
	//	basis->objectPosition += velocity * Vector3::Down * deltaTime;
	//	std::cout << "Move odject up" << std::endl;
	//}
	//if (inputDevice->IsKeyDown(Keys::Down))//40
	//{
	//	//triangleObject->objectPosition += velocity * Vector3::Down * deltaTime;
	//	basis->objectPosition += velocity * Vector3::Up * deltaTime;
	//	std::cout << "Move odject down" << std::endl;
	//}

	//if (inputDevice->IsKeyDown(Keys::Escape))
	//{
	//	//Exit();
	//	std::cout << "Exit sas" << std::endl;//
	//}

	Game::Update(deltaTime);//���������� ������ ��� ���� �����������
}

void GameTransform::OnMouseMove(InputDevice::MouseMoveEventArgs& args)
{
	//std::cout << args.Position.x << " " << args.Position.y << std::endl; //������������ �������
}