#include "RMObject.h"
#include <DirectXMath.h>

using namespace DirectX;

RMObject::RMObject()
{
	Name = std::string();
	Position = V3();
	EularAngles = V3();
	Scale = V3(1, 1, 1);
	Repetition = V3();

	Type = RMObjectType::None;
	Operation = RMOperation::Union;
}

RMObjectData RMObject::GetObjectData()
{
	RMObjectData data;
	data.Scale = Scale;
	data.Type = (float)Type;
	data.Operation = (float)Operation;
	data.Repetition = Repetition;

	XMVECTOR xmvRotation = XMLoadFloat3(&EularAngles);
	XMVECTOR xmvTranslation = XMLoadFloat3(&Position);

	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(-xmvRotation);
	XMMATRIX translation = XMMatrixTranslationFromVector(-xmvTranslation);

	XMMATRIX comb = translation * rotation;
	XMStoreFloat4x4(&data.TranslationRotationMatrix, comb);

	return data;
}
