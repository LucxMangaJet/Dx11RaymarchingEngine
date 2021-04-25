#include "RMObject.h"


RMObject::RMObject()
{
	Name = std::string();
	Position = XMFLOAT3();
	EularAngles = XMFLOAT3();
	Scale = XMFLOAT3(1, 1, 1);
	Repetition = XMFLOAT3();

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
