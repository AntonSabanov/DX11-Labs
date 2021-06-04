#include "CameraController.h"
#include "Camera.h"//
#include "Game.h"//

using namespace DirectX::SimpleMath;

CameraController::CameraController(Game* gameInst, Camera* camera)
{
	game = gameInst;
	gameCamera = camera;
	yaw = 0;
	pitch = 0;
	//CameraController& controller = *this;
	cameraPosition = DirectX::SimpleMath::Vector3(0, 1, 1);//стартовая позиция камеры
	//cameraPosition = DirectX::SimpleMath::Vector3(1, 1, 1);
	game->inputDevice->MouseMove.AddRaw(this, &CameraController::OnMouseMove); //подписываемся на движение мыши
}

void CameraController::Update(float deltaTime)//2
{
	auto rotationMatrix = Matrix::CreateFromYawPitchRoll(yaw, pitch, 0); //создаем матрицу вращения (определяет, куда мы смотрим

	//движение камеры
	auto moveDirection = Vector3::Zero;
	if (game->inputDevice->IsKeyDown(Keys::W)) moveDirection += Vector3(1.0f, 0.0f, 0.0f);//движемся вперед
	if (game->inputDevice->IsKeyDown(Keys::S)) moveDirection += Vector3(-1.0f, 0.0f, 0.0f);//движемся назад
	if (game->inputDevice->IsKeyDown(Keys::A)) moveDirection += Vector3(0.0f, 0.0f, -1.0f);//движемся влево
	if (game->inputDevice->IsKeyDown(Keys::D)) moveDirection += Vector3(0.0f, 0.0f, 1.0f);//движемся вправо

	if (game->inputDevice->IsKeyDown(Keys::Space)) moveDirection += Vector3(0.0f, 1.0f, 0.0f);//движемся вверх
	if (game->inputDevice->IsKeyDown(Keys::C)) moveDirection += Vector3(0.0f, -1.0f, 0.0f);//движемся вниз

	moveDirection.Normalize();//нормализация

	auto moveDirectionWorld = rotationMatrix.Forward() * moveDirection.x + Vector3::Up * moveDirection.y + rotationMatrix.Right() * moveDirection.z;
	if (moveDirectionWorld.Length() != 0) 
	{
		moveDirectionWorld.Normalize();
	}

	cameraPosition = cameraPosition + moveDirectionWorld * velocityMagnitude * deltaTime; //deltaTime - нужно чтобы отвязаться от частоты кадров
	//вращение камеры
	gameCamera->viewMatrix = Matrix::CreateLookAt(cameraPosition, cameraPosition + rotationMatrix.Forward(), rotationMatrix.Up());
	gameCamera->projectionMatrix = gameCamera->UpdateProjectionMatrix();

	//gameCamera->UpdateProjectionMatrix();
}

void CameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
	if (game->inputDevice->IsKeyDown(Keys::LeftShift)) return;
	
	yaw -= args.Offset.x * 0.003f * mouseSensitivity;//поворот
	pitch -= args.Offset.y * 0.003f * mouseSensitivity;//наклон

	if (args.WheelDelta == 0) return;

	if (args.WheelDelta > 0) 
		velocityMagnitude += 1;//увеличение скорости
	if (args.WheelDelta < 0) 
		velocityMagnitude -= 1;//уменьшение скорости
}