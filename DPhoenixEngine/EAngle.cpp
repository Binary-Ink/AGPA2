#include "D3DUtil.h"

//this calculates a look at vector for the camera
//based on the yaw and pitch values
XMFLOAT3 DPhoenix::EAngle::ToFloat3() const
{
	XMFLOAT3 result;

	result.x = cos(y) * cos(p);
	result.y = sin(p);
	result.z = sin(y) * cos(p);

	return result;
}

//here we provide boundings for the Euler angles
//focusing on pitch and yaw
//(you might want to experiment with roll...)
void DPhoenix::EAngle::Normalize()
{
	if (p > 89) p = 89;
	if (p < -89) p = -89;

	while (y < -180) y += 360;
	while (y > 180) y -= 360;

}
