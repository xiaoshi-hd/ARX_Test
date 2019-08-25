#pragma once
#include "CMyPoint.h"

class CCalculation
{
public:
	CCalculation();
	~CCalculation();

	static AcGePoint3d Pt2dTo3d(AcGePoint2d pt);//二维点转三维点
	static AcGePoint2d Pt3dTo2d(AcGePoint3d pt);//三维点转二维点

	static double PI();

	static double Min(double a, double b);
	static double Max(double a, double b);

	static double hudutodu(double angle);
	static double dutohudu(double angle);

	//构建TIN的函数
	//三角形外接圆的圆心
	static double X0(double x1, double y1, double x2, double y2, double x3, double y3);
	static double Y0(double x1, double y1, double x2, double y2, double x3, double y3);
	//三角形外接圆的半径
	static double R(double x0, double y0, double x1, double y1);
	//斜三棱柱计算
	static double SS(CMyTriangle san);
	static double H(CMyTriangle san, double gaocheng);
};
