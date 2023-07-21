#include "FARigidSphere.h"

namespace FAPhysicsShapes
{
	//-----------------------------------------------------------------------------------------------------------------------------
	//RigidSphere Properties

	RigidSphere::RigidSphere() : mRadius{ 1.0f }
	{}

	void RigidSphere::InitializeRigidSphere(float radius, float massDensity,
		const FAMath::Vector4D& initialPosition, const FAMath::Quaternion& initialOrientation, const FAColor::Color& color,
		const std::vector<FAShapes::Vertex>& vertices, const std::vector<FAShapes::Triangle>& triangles)
	{
		mRadius = radius;

		mShape.InitializeThreeDimensionalShape(initialPosition, initialOrientation, color);

		//Need to scale the vertices to get it to the proper size so the computed mass is dependent on the volume of the object.
		std::vector<FAShapes::Vertex> tempVertexList = vertices;
		std::vector<FAShapes::Triangle> tempTriangleList = triangles;
		FAMath::Matrix4x4 scale(FAMath::Scale(FAMath::Matrix4x4(), radius, radius, radius));

		for (auto& i : tempVertexList)
		{
			i.position = i.position * scale;
		}

		//Set the vertex list the triangles in the temp triangle vector point to to the temp vertex list.
		for (auto& i : tempTriangleList)
		{
			i.SetVertexList(tempVertexList.data());
		}

		mRigidBody.InitializeRigidBody(massDensity, initialOrientation, tempTriangleList);

		//Formula used is:
		//local origin world location = center of mass world locaiton - offset at t = 0
		//so offset = center of mass world locaiton - local origin world location at t = 0.
		//At t = 0 the local origin world location is (0, 0, 0) so offset = center of mass world locaiton.
		mOffset = mRigidBody.GetCenterOfMass();
		mOffset.SetW(0.0f);

		//Move the center of mass to its initial location in the world.
		mRigidBody.SetCenterOfMass(mShape.GetPosition() + mOffset);
	}

	float RigidSphere::GetRadius() const
	{
		return mRadius;
	}

	void RigidSphere::SetRadius(float radius)
	{
		mRadius = radius;
	}

	const FAPhysics::RigidBody& RigidSphere::GetRigidBody() const
	{
		return mRigidBody;
	}

	FAPhysics::RigidBody& RigidSphere::GetRigidBody()
	{
		return mRigidBody;
	}

	const FAShapes::ThreeDimensionalShape& RigidSphere::GetShape() const
	{
		return mShape;
	}

	FAShapes::ThreeDimensionalShape& RigidSphere::GetShape()
	{
		return mShape;
	}

	void RigidSphere::SetPosition(const FAMath::Vector4D& position)
	{
		mShape.SetPosition(position);
		mRigidBody.SetCenterOfMass(position + mOffset);
	}

	void RigidSphere::UpdateModelMatrix()
	{
		mShape.SetPosition(mRigidBody.GetCenterOfMass() - mOffset);
		mShape.SetOrientation(mRigidBody.GetOrientation());

		FAMath::Matrix4x4 scale{ Scale(FAMath::Matrix4x4(), mRadius, mRadius, mRadius) };

		FAMath::Matrix4x4 localRotation(FAMath::QuaternionToRotationMatrixRow(mShape.GetOrientation()));

		FAMath::Matrix4x4 translation{ Translate(FAMath::Matrix4x4(), mShape.GetPosition()) };

		mShape.SetModelMatrix(scale * localRotation * translation);
	}

	float RigidSphere::Volume()
	{
		return (4.0f / 3.0f) * PI * mRadius * mRadius * mRadius;
	}
}