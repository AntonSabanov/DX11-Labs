#include "GameTransform.h"
#include "SimpleMath.h"
#include "GridComponent.h"
#include "TextureObjComponent.h"
#include "LightObjComponent.h"
#include "Keys.h"

using namespace DirectX::SimpleMath;

GridComponent* grid;				//�����
GridComponent* basis;				//������� �����
TriangleComponent* triangleObject;	//������ �� �������������
TextureObjComponent* texObj;		//������ � ���������
LightObjComponent* texLightObj;		//������ �� ������

void GameTransform::Initialize()//�������� ������ � ��������
{
	gameCamera = new Camera(this);
	gameCameraCtrl = new CameraController(this, gameCamera);

	texLoader = new TextureLoader(this);

	//������� �����
	basis = new GridComponent(device, context, {
		Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //��� �
		Vector4(1.0f,  0.0f,  0.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f),

		Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f), //��� �
		Vector4(0.0f,  1.0f,  0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),

		Vector4(0.0f,  0.0f,  0.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f), //��� z
		Vector4(0.0f,  0.0f,  1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),},
		{0, 1, 2, 3, 4, 5},
		gameCamera);
	 
	//�����
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
	//
	//triangleObject = new TriangleComponent(device, context, {
	//	Vector4(50.0f, 0.0f, 50.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
	//	Vector4(-50.0f, -0.0f, 50.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(50.0f, -0.0f, 50.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(-50.0f, 0.0f, 50.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), }, gameCamera);
	
	//�����
	triangleObject = new TriangleComponent(device, context, { 
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, -0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f), //������� (�� -1 �� 1) //����
		Vector4(0.5f, -0.5f, -0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, -0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector4(-0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 
		}, { //������ �������� ��� ������
		0,1,2, 1,0,3, 4,2,5, 2,4,0, 6,5,7, 5,6,4, 3,7,1, 7,3,6, 4,3,0, 3,4,6, 2,7,5, 7,2,1 }, gameCamera);

	//����� � ���������
	texObj = new TextureObjComponent(this, device, context, (Vector3::Zero + Vector3(0, -1, 0)), {
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 0.0f),		//0
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f),		//1
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 0.0f, 0.0f),		//2
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 0.0f, 0.0f),		//3
		Vector4(0.5f, 0.5f, -0.5f, 1.0f),	Vector4(0.0f, 0.0f, 0.0f, 0.0f),		//4
		Vector4(0.5f, -0.5f, -0.5f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 0.0f),		//5
		Vector4(-0.5f, 0.5f, -0.5f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 0.0f),		//6
		Vector4(-0.5f, -0.5f, -0.5f, 1.0f), Vector4(1.0f, 1.0f, 0.0f, 0.0f), },		//7
		{ 0,1,2, 1,0,3, 4,2,5, 2,4,0, 6,5,7, 5,6,4, 3,7,1, 7,3,6, 4,3,0, 3,4,6, 2,7,5, 7,2,1 },//������ �������� ��� ������
		L"Obj.obj", L"WallTex3.png", gameCamera);

	//����� � ��������� � ������ //���
	//texLightObj = new LightObjComponent(this, device, context, (Vector3::Zero + Vector3(0, 1, 0)), {
	//	Vector4(-0.5f, 0.5f, -0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, -0.5f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, -0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, -1.0f, 0.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, -0.5f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, -1.0f, 0.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, -1.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, -1.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(-1.0f, 0.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, -0.5f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),	Vector4(-1.0f, 0.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, -0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),	Vector4(-1.0f, 0.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),	Vector4(-1.0f, 0.0f, 0.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f),//
	//	Vector4(0.5f, -0.5f, -0.5f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f),//
	//	Vector4(0.5f, 0.5f, -0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, -0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, -1.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, -0.5f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, -1.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, -0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, -1.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, -0.5f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, -1.0f, 1.0f),
	//	Vector4(-0.5f, -0.5f, 0.5f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, -0.5f, 0.5f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(0.5f, 0.5f, 0.5f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
	//	Vector4(-0.5f, 0.5f, 0.5f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f), },
	//	{ 3,1,0, 2,1,3, 6,4,5, 7,4,6, 11,9,8, 10,9,11, 14,12,13, 15,12,14, 19,17,16, 18,17,19, 22,20,21, 23,20,22},
	//	L"Obj.obj", L"WallTex.png", gameCamera);

	//��� � �������, �� � ������� ������������
	texLightObj = new LightObjComponent(this, device, context, (Vector3::Zero + Vector3(0, 1, 0)), {
		Vector4(-0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, -1.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, -0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, -0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),		Vector4(-1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, -0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),	
		Vector4(0.5f, 0.5f, -0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, -0.5f, 1.0f),		Vector4(0.0f, 0.0f, -1.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, -0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		Vector4(0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.5f, 0.5f, 0.5f, 1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f),	Vector4(0.0f, 1.0f, 1.0f, 1.0f), },
		{ 3,1,0, 2,1,3, 6,4,5, 7,4,6, 11,9,8, 10,9,11, 14,12,13, 15,12,14, 19,17,16, 18,17,19, 22,20,21, 23,20,22 },
		L"Obj.obj", L"WallTex.png", gameCamera);

	components.emplace_back(grid);
	components.emplace_back(basis);
	components.emplace_back(triangleObject);
	components.emplace_back(texObj);
	components.emplace_back(texLightObj);
}

void GameTransform::Update(float deltaTime)//1
{
	gameCameraCtrl->Update(deltaTime);

	float objVelocity = 3.0f;
	float lightVelocity = 3.0f;

	auto mat = Matrix::CreateRotationX(DirectX::XMConvertToRadians(10));

	//Matrix::

	//����������� �������
	if (inputDevice->IsKeyDown(Keys::Left))//37
	{
		//texLightObj->objectPosition += objVelocity * Vector3::Left * deltaTime;
		texLightObj->light_x += lightVelocity * -1 * deltaTime;
		std::cout << "Move odject left" << std::endl;
	}
	if (inputDevice->IsKeyDown(Keys::Right))//39
	{	
		//texObj->objectPosition += objVelocity * Vector3::Right * deltaTime;
		texLightObj->light_x += lightVelocity * 1 * deltaTime;
		std::cout << "Move odject right" << std::endl;
	}
	if (inputDevice->IsKeyDown(Keys::Up))//38
	{
		//texObj->objectPosition += objVelocity * Vector3::Up * deltaTime;
		texLightObj->light_z += lightVelocity * -1 * deltaTime;
		std::cout << "Move odject up" << std::endl;
	}
	if (inputDevice->IsKeyDown(Keys::Down))//40
	{
		//texLightObj->objectPosition += objVelocity * Vector3::Down * deltaTime;
		texLightObj->light_z += lightVelocity * 1 * deltaTime;
		std::cout << "Move odject down" << std::endl;
	}

	if (inputDevice->IsKeyDown(Keys::Escape))
	{
		std::cout << "Exit sas" << std::endl;//
	}

	Game::Update(deltaTime);//���������� ������ ��� ���� �����������
}

void GameTransform::OnMouseMove(InputDevice::MouseMoveEventArgs& args)
{
	//std::cout << args.Position.x << " " << args.Position.y << std::endl; //������������ �������
}