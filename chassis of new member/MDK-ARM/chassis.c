#include "main.h"
#include "chassis.h"
 
void chassis(int16_t Vx,int16_t Vy,int16_t Vw,int16_t kx ,int16_t ky,int16_t kw)
{
	V1=-Vx*kx+Vy*ky+Vw*kw;
	V2=Vx*kx+Vy*ky+Vw*kw;
	V3=Vx*kx-Vy*ky+Vw*kw;
	V4=-Vx*kx-Vy*ky+Vw*kw;
}