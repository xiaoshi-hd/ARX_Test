#pragma once
#include <vector> 
#include "CMyPoint.h"
using namespace std;

class CAuxiliary
{
public:
	CAuxiliary();
	~CAuxiliary();

	//获取用户输入的线宽
	static void GetWidth(ads_real *width_begin, ads_real *width_end);

	//提示用户输入颜色索引值
	static int GetColorIndex();

	static vector<CMyPoint> vec_pt;//用来存储散点
	static vector<CMyTriangle> vec_tri;//用来存储三角形

	double xmax, xmin, ymax, ymin;//存储初始矩形信息

	static CString result;//保存计算报告

	//按逗号分隔字符串
	/*BOOL GetFieldText(CString strLineText, CStringArray &fields)
	{
		if (strLineText.Find(",", 0) == -1) // 如果找不到英文逗号，函数退出
		{
		return FALSE;
		}
		int nLeftPos = 0, nRightPos = 0; // 查找分隔符的起始位置
		while ((nRightPos = strLineText.Find(",", nRightPos)) != -1)
		{
			fields.Add(strLineText.Mid(nLeftPos, nRightPos - nLeftPos));
			nLeftPos = nRightPos + 1;
			nRightPos++;
		}
		// 最后一个列的数据
		fields.Add(strLineText.Mid(nLeftPos));
		return TRUE;
	}*/

	//绘制三角网
	static void drawTriangle(vector<CMyPoint> vec_pt, CString strLayerName);

	//计算TIN体积
	static CString calculateV(vector<CMyTriangle> vec_tri, int gaocheng);

};

