#include "InputOutputVertices.hlsl"
#include "ConstantBuffers.hlsl"

vertexOutput vsMain(vertexInput vin)
{
	vertexOutput vout;

	float4x4 MVP = mul(objectConstantData.localToWorldMatrix, mul(passConstantData.viewMatrix, passConstantData.projectionMatrix));

	vout.outputPosition = mul(vin.inputPosition, MVP);

	return vout;
}