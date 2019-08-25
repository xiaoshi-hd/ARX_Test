#include "stdafx.h"
#include "CAuxiliary.h"
#include "CCreateEnt.h"
#include "CModifyEnt.h"
#include "CCalculation.h"

vector<CMyPoint> CAuxiliary::vec_pt;//初始化静态列表
vector<CMyTriangle> CAuxiliary::vec_tri;//初始化静态列表
CString CAuxiliary::result = "";

CAuxiliary::CAuxiliary()
{
} 


CAuxiliary::~CAuxiliary()
{
}

void CAuxiliary::GetWidth(ads_real *width_begin, ads_real *width_end)
{
	if (RTNORM == acedGetReal(_T("\n输入起点线宽:"), width_begin))
	{
		if (RTNORM == acedGetReal(_T("\n输入终点线宽:"), width_end))
		{
			return;
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}
}

int CAuxiliary::GetColorIndex()
{
	int nColorIndex = 0;
	if (RTNORM != acedGetInt(_T("\n输入颜色索引值(0～256):"), &nColorIndex))
	{
		return 0;
	}

	/// 处理颜色索引值无效的情况
	while (nColorIndex < 0 || nColorIndex > 256)
	{
		acedPrompt(_T("\n输入了无效的颜色索引."));
		if (RTNORM != acedGetInt(_T("\n输入颜色索引值(0～256):"), &nColorIndex))
		{
			return 0;
		}
	}
	
	return nColorIndex;
}

void CAuxiliary::drawTriangle(vector<CMyPoint> pt, CString strLayerName)
{
#pragma region 初始化
	CAuxiliary A;//这里必须实例化类才能使用成员变量,因为函数是静态函数，在外部可以调用
	A.vec_tri.clear();
	//A.vec_v.clear();
	A.xmax = 0;
	A.ymax = 0;
	A.xmin = 100000000000;
	A.ymin = 100000000000;
#pragma endregion

#pragma region 生成初始三角网
	for (int i = 0; i < pt.size(); i++)
	{
		if (A.xmax < pt[i].pt.x)
			A.xmax = pt[i].pt.x;
		if (A.ymax < pt[i].pt.y)
			A.ymax = pt[i].pt.y;
		if (A.xmin > pt[i].pt.x)
			A.xmin = pt[i].pt.x;
		if (A.ymin > pt[i].pt.y)
			A.ymin = pt[i].pt.y;
	}

	AcGePoint3d pt1, pt2, pt3, pt4;
	pt1.x = A.xmin - 1;
	pt1.y = A.ymin - 1;
	pt2.x = A.xmin - 1;
	pt2.y = A.ymax + 1;
	pt3.x = A.xmax + 1;
	pt3.y = A.ymax + 1;
	pt4.x = A.xmax + 1;
	pt4.y = A.ymin - 1;

	CMyTriangle tri1;
	tri1.pt1 = pt1;
	tri1.pt2 = pt2;
	tri1.pt3 = pt3;

	A.vec_tri.push_back(tri1);

	CMyTriangle tri2;
	tri2.pt1 = pt1;
	tri2.pt2 = pt3;
	tri2.pt3 = pt4;

	A.vec_tri.push_back(tri2);
#pragma endregion

#pragma region 生成平面三角网
	double x0, y0, r;
	for (int i = 0; i < CAuxiliary::vec_pt.size(); i++)//对每个离散点进行遍历
	{
		vector<CMyTriangle> tri;//T2三角形列表
		vector<int> b;//记录T1中要剪切的三角形索引
		vector<CMyLine> vec_line;//用来存储直线

		double x = CAuxiliary::vec_pt[i].pt.x;
		double y = CAuxiliary::vec_pt[i].pt.y;

		for (int j = 0; j < A.vec_tri.size(); j++)//按顺序从T1中取出一个三角形ABC，设其顶点为A, B, C并计算该三角形外接圆的圆心O及半径r
		{
			x0 = CCalculation::X0(A.vec_tri[j].pt1.x, A.vec_tri[j].pt1.y, A.vec_tri[j].pt2.x,
				A.vec_tri[j].pt2.y, A.vec_tri[j].pt3.x, A.vec_tri[j].pt3.y);
			y0 = CCalculation::Y0(A.vec_tri[j].pt1.x, A.vec_tri[j].pt1.y, A.vec_tri[j].pt2.x,
				A.vec_tri[j].pt2.y, A.vec_tri[j].pt3.x, A.vec_tri[j].pt3.y);
			r = CCalculation::R(x0, y0, A.vec_tri[j].pt1.x, A.vec_tri[j].pt1.y);

			if ((x - x0) * (x - x0) + (y - y0) * (y - y0) < r * r)//在三角形内部
			{
				CMyLine line1;
				CMyLine line2;
				CMyLine line3;
				line1.ptBegin = A.vec_tri[j].pt1;
				line1.ptEnd = A.vec_tri[j].pt2;
				line2.ptBegin = A.vec_tri[j].pt2;
				line2.ptEnd = A.vec_tri[j].pt3;
				line3.ptBegin = A.vec_tri[j].pt3;
				line3.ptEnd = A.vec_tri[j].pt1;
				vec_line.push_back(line1);
				vec_line.push_back(line2);
				vec_line.push_back(line3);//把tri中所有三角形的边信息存储到边列表中

				tri.push_back(A.vec_tri[j]);//T2添加三角形
				b.push_back(j);
			}
		}
		for (int j = (int)b.size() - 1; j >= 0; j--)//列表数据的结构决定了必须从后往前移除数据
		{
			A.vec_tri.erase(A.vec_tri.begin() + b[j]);//T1移除三角形，实现T1向T2剪切
		}

		/*CString str;
		str.Format(_T("%d"), A.vec_tri.size());
		acedAlert(str + _T("VEC"));
		str.Format(_T("%d"), tri.size());
		acedAlert(str);*/

		for (int j = 0; j < vec_line.size(); j++)//在T2的三角形中寻找所有公共边，并删除这些公共边，再将剩下的边加入到边列表S中，然后清空T2
		{
			for (int k = 0; k < vec_line.size(); k++)
			{
				if (j != k)//不同边相互比较
				{
					if (vec_line[j].ptBegin == vec_line[k].ptBegin && vec_line[j].ptEnd == vec_line[k].ptEnd
						|| vec_line[j].ptBegin == vec_line[k].ptEnd && vec_line[j].ptEnd == vec_line[k].ptBegin)//判断两条边是否重合，重合就删掉
					{
						vec_line.erase(vec_line.begin() + k);//j < k
						vec_line.erase(vec_line.begin() + j);
						if (j == 0)//为了减少循环次数
						{
							j = 0;
						}
						else
						{
							j--;
						}
					}
				}
			}
		}
		for (int j = 0; j < vec_line.size(); j++)//将S中的每条边的端点与P点连接，得到多个新的三角形，并将它们添加到三角形列表T1中
		{
			CMyTriangle triAngle;
			triAngle.pt1 = CAuxiliary::vec_pt[i].pt;
			triAngle.pt2 = vec_line[j].ptBegin;
			triAngle.pt3 = vec_line[j].ptEnd;
			A.vec_tri.push_back(triAngle);
		}
	}
#pragma endregion

#pragma region 构成不规则三角网
	vector<int> a;//存储要删除的三角形的索引信息
	for (int i = 0; i < A.vec_tri.size(); i++)
	{
		if (A.vec_tri[i].pt1 == pt1 || A.vec_tri[i].pt1 == pt2 || A.vec_tri[i].pt1 == pt3 || A.vec_tri[i].pt1 == pt4
			|| A.vec_tri[i].pt2 == pt1 || A.vec_tri[i].pt2 == pt2 || A.vec_tri[i].pt2 == pt3 || A.vec_tri[i].pt2 == pt4
			|| A.vec_tri[i].pt3 == pt1 || A.vec_tri[i].pt3 == pt2 || A.vec_tri[i].pt3 == pt3 || A.vec_tri[i].pt3 == pt4)
		{
			a.push_back(i);
		}
	}
	for (int j = (int)a.size() - 1; j >= 0; j--)
	{
		A.vec_tri.erase(A.vec_tri.begin() + a[j]);
	}
#pragma endregion
	/*CString str;
	str.Format(_T("%d"), CAuxiliary::vec_tri.size());//对于静态变量，对其实例化时创建的内容，可以直接以静态的方式访问
	acedAlert(_T("三角形的个数为：") + str);*/

	AcDbObjectId triangleId;

	for (int i = 0; i < A.vec_tri.size(); i++)
	{
		triangleId = CCreateEnt::CreateTriangle(A.vec_tri[i].pt1, A.vec_tri[i].pt2, A.vec_tri[i].pt3);
		CModifyEnt::ChangeLayer(triangleId, strLayerName);//更改三角网的图层
	}
}

CString CAuxiliary::calculateV(vector<CMyTriangle> vec_tri, int gaocheng)
{
	vector<double> vec_v;//用来存储三棱柱体积信息

	for (int i = 0; i < vec_tri.size(); i++)
	{
		double SS, H;
		SS = CCalculation::SS(vec_tri[i]);
		H = CCalculation::H(vec_tri[i], gaocheng);
		vec_v.push_back(SS * H);
	}
	//体积排序
	for (int i = 0; i < vec_v.size(); i++)//冒泡排序算法
	{
		for (int j = 0; j < vec_v.size() - 1 - i; j++)
		{
			if (vec_v[j] > vec_v[j + 1])
			{
				double temp = vec_v[j + 1];
				vec_v[j + 1] = vec_v[j];
				vec_v[j] = temp;
			}
		}
	}

	//计算报告
	CString str;
	CString str_gaocheng;
	str_gaocheng.Format(_T("%d"),gaocheng);
	CString str_number;
	str_number.Format(_T("%d"), CAuxiliary::vec_tri.size());
	CString str_V;
	double V_sum = 0;
	for (int i = 0; i < vec_v.size(); i++)
	{
		V_sum += vec_v[i];
	}
	str_V.Format(_T("%.3f"),V_sum);

	str = "不规则三角网体积计算\n\n";
	str += "---------------三角网基本信息---------------\n";
	str += "基准高程：  " + str_gaocheng + "m\n";
	str += "三角形个数：  " + str_number + "\n";
	str += "体积：  " + str_V + "\n\n";

	str += "---------------20个三角形说明---------------\n";
	str += "序号\t三个顶点\n";
	CString str_i;
	for (int i = 0; i < 20; i++)
	{
		str_i.Format(_T("%d"),i + 1);
		str += str_i + "\t\n";
		//str += vec_tri[i].pt1.dianhao + vec_tri[i].pt2.dianhao + vec_tri[i].pt3.dianhao + "\n";
	}//这里没有考虑到，应该设置成CMyPoint类型的
	str += "\n---------------体积最小的5个三棱柱体积---------------\n";
	CString str_vi;
	for (int i = 0; i < 5; i++)
	{
		str_i.Format(_T("%d"), i + 1);
		str += str_i + "\t";
		str_vi.Format(_T("%.3f"), vec_v[i]);
		str += str_vi + "\n";
	}
	str += "\n---------------体积最大的5个三棱柱体积---------------\n";
	for (int i = 0; i < 5; i++)
	{
		str_i.Format(_T("%d"), i + 1);
		str += str_i + "\t";
		str_vi.Format(_T("%.3f"), vec_v[vec_v.size() - 1 - i]);
		str += str_vi + "\n";
	}
	
	acedAlert(str);

	return str;
}

