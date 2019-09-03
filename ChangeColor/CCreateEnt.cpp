#include "stdafx.h"
#include "CCreateEnt.h"
#include "CCalculation.h"
#include "CModifyEnt.h"
#include "CAuxiliary.h"
#include <math.h>
#include "dbspline.h"//创建样条曲线
#include "dbhatch.h"//创建填充

CCreateEnt::CCreateEnt()
{
}


CCreateEnt::~CCreateEnt()
{
}
//创建点
AcDbObjectId CCreateEnt::CreatePoint(AcGePoint3d pt)
{

	AcDbPoint *pPoint = new AcDbPoint(pt);

	// 将实体添加到图形数据库
	AcDbObjectId pointId;
	pointId = CCreateEnt::PostToModelSpace(pPoint);
	return pointId;
}

// 创建直线
AcDbObjectId CCreateEnt::CreateLine(AcGePoint3d ptStart,AcGePoint3d ptEnd)
{

	AcDbLine *pLine = new AcDbLine(ptStart, ptEnd);

	// 将实体添加到图形数据库
	AcDbObjectId lineId;
	lineId = CCreateEnt::PostToModelSpace(pLine);
	return lineId;
}

//创建圆
AcDbObjectId CCreateEnt::CreateCircle(AcGePoint3d ptCenter, double radius)
{
	AcGeVector3d vec(0, 0, 1);//添加一个创建位于XOY 平面上的圆

	AcDbCircle *pCircle = new AcDbCircle(ptCenter, vec, radius);

	// 将实体添加到图形数据库
	AcDbObjectId circleId;
	circleId = CCreateEnt::PostToModelSpace(pCircle);
	return circleId;
}
//多线的方式创建圆
AcDbObjectId CCreateEnt::CreatePolyCircle(AcGePoint2d ptCenter,double radius, double width)
{
	// 计算顶点的位置
	AcGePoint2d pt1, pt2, pt3;
	pt1.x = ptCenter.x + radius;
	pt1.y = ptCenter.y;
	pt2.x = ptCenter.x - radius;
	pt2.y = ptCenter.y;
	pt3.x = ptCenter.x + radius;
	pt3.y = ptCenter.y;
	// 创建多段线
	AcDbPolyline *pPoly = new AcDbPolyline(3);
	pPoly->addVertexAt(0, pt1, 1, width, width);
	pPoly->addVertexAt(1, pt2, 1, width, width);
	pPoly->addVertexAt(2, pt3, 1, width, width);
	pPoly->setClosed(Adesk::kTrue);
	// 将多段线添加到模型空间
	AcDbObjectId polyId;
	polyId = CCreateEnt::PostToModelSpace(pPoly);
	return polyId;
}

//用圆心加半径加起终点角度的方法创建一个圆弧
AcDbObjectId CCreateEnt::CreateArc(AcGePoint2d ptCenter, double radius, double startAngle, double endAngle)
{
	AcGeVector3d vec(0, 0, 1);//添加一个创建位于XOY 平面上的圆
	AcGePoint3d pt3dCenter = CCalculation::Pt2dTo3d(ptCenter);

	AcDbArc *pArc = new AcDbArc(pt3dCenter, vec, radius, startAngle, endAngle);

	// 将实体添加到图形数据库
	AcDbObjectId arcId;
	arcId = CCreateEnt::PostToModelSpace(pArc);
	return arcId;
}
//用创建多线的方式创建圆弧
AcDbObjectId CCreateEnt::CreatePolyArc(AcGePoint2d ptCenter, double radius,
	double angleStart, double angleEnd, double width)
{
	// 计算顶点的位置
	AcGePoint2d pt1, pt2;
	pt1.x = ptCenter.x + radius * cos(angleStart);
	pt1.y = ptCenter.y + radius * sin(angleStart);
	pt2.x = ptCenter.x + radius * cos(angleEnd);
	pt2.y = ptCenter.y + radius * sin(angleEnd);
	// 创建多段线
	AcDbPolyline *pPoly = new AcDbPolyline(3);

	pPoly->addVertexAt(0, pt1, tan((angleEnd - angleStart) / 4), width,
		width);
	pPoly->addVertexAt(1, pt2, 0, width, width);
	// 将多段线添加到模型空间
	AcDbObjectId polyId;
	polyId = CCreateEnt::PostToModelSpace(pPoly);
	return polyId;
}

//创建椭圆
AcDbObjectId CCreateEnt::CreateEllipse(AcGePoint3d ptCenter,
	AcGeVector3d vecNormal, AcGeVector3d majorAxis, double
	ratio)
{
	AcDbEllipse *pEllipse = new AcDbEllipse(ptCenter, vecNormal,
		majorAxis, ratio);
	return CCreateEnt::PostToModelSpace(pEllipse);
}

//根据外接矩形创建椭圆（函数的重载）
AcDbObjectId CCreateEnt::CreateEllipse(AcGePoint2d pt1, AcGePoint2d pt2)
{
	// 计算椭圆的中心点
	AcGePoint3d ptCenter;
	ptCenter = CCalculation::MiddlePoint(CCalculation::Pt2dTo3d(pt1),
		CCalculation::Pt2dTo3d(pt2));
	AcGeVector3d vecNormal(0, 0, 1);
	AcGeVector3d majorAxis(fabs(pt1.x - pt2.x) / 2, 0, 0);
	double ratio = fabs((pt1.y - pt2.y) / (pt1.x - pt2.x));
	return CCreateEnt::CreateEllipse(ptCenter, vecNormal, majorAxis,
		ratio);
}

//创建样条曲线
AcDbObjectId CCreateEnt::CreateSpline(const AcGePoint3dArray& points,
	int order, double fitTolerance)
{
	assert(order >= 2 && order <= 26);//assert 函数用于判断一个变量或表达式的值
	//是否为true，如果为false 则弹出一个错误对话框，并且终止程序的运行
	AcDbSpline *pSpline = new AcDbSpline(points, order, fitTolerance);
	AcDbObjectId splineId;
	splineId = CCreateEnt::PostToModelSpace(pSpline);
	return splineId;
}

//根据起终点切线的方法创建样条曲线
AcDbObjectId CCreateEnt::CreateSpline(const AcGePoint3dArray& points,
	const AcGeVector3d& startTangent, const AcGeVector3d& endTangent,
	int order, double fitTolerance)
{
	assert(order >= 2 && order <= 26);
	AcDbSpline *pSpline = new AcDbSpline(points, startTangent,
		endTangent, order, fitTolerance);
	return CCreateEnt::PostToModelSpace(pSpline);
}

//根据二维点列表创建多线
AcDbObjectId CCreateEnt::CreatePolyline(AcGePoint2dArray points, double width)
{
	int numVertices = points.length();
	AcDbPolyline *pPoly = new AcDbPolyline(numVertices);
	for (int i = 0; i < numVertices; i++)
	{
		pPoly->addVertexAt(i, points.at(i), 0, width, width);
	}
	// 将实体添加到图形数据库
	AcDbObjectId polyId;
	polyId = CCreateEnt::PostToModelSpace(pPoly);
	return polyId;
}

//创建二维矩形
AcDbObjectId CCreateEnt::CreateRect(AcGePoint2d pt1, AcGePoint2d pt2, double width)
{
	// 提取两个角点的坐标值
	double x1 = pt1.x, x2 = pt2.x;
	double y1 = pt1.y, y2 = pt2.y;

	// 计算矩形的角点
	AcGePoint2d ptLeftBottom(CCalculation::Min(x1, x2),
		CCalculation::Min(y1, y2));
	AcGePoint2d ptRightBottom(CCalculation::Max(x1, x2),
		CCalculation::Min(y1, y2));
	AcGePoint2d ptRightTop(CCalculation::Max(x1, x2),
		CCalculation::Max(y1, y2));
	AcGePoint2d ptLeftTop(CCalculation::Min(x1, x2),
		CCalculation::Max(y1, y2));

	// 创建对应的多段线
	AcDbPolyline *pPloy = new AcDbPolyline(4);
	pPloy->addVertexAt(0, ptLeftBottom, 0, width, width);
	pPloy->addVertexAt(1, ptRightBottom, 0, width, width);
	pPloy->addVertexAt(2, ptRightTop, 0, width, width);
	pPloy->addVertexAt(3, ptLeftTop, 0, width, width);
	pPloy->setClosed(Adesk::kTrue);

	// 将多段线添加到模型空间
	AcDbObjectId polyId;
	polyId = CCreateEnt::PostToModelSpace(pPloy);
	return polyId;
}

//创建三维三角形
AcDbObjectId CCreateEnt::CreateTriangle(AcGePoint3d pt1, AcGePoint3d pt2, AcGePoint3d pt3, double width)
{
	AcGePoint3dArray points;
	points.append(pt1);
	points.append(pt2);
	points.append(pt3);

	// 创建对应的多段线
	AcDb3dPolyline *p3dPloy = new AcDb3dPolyline(AcDb::k3dSimplePoly,points,true);//闭合多线段

	// 将多段线添加到模型空间
	AcDbObjectId polyId;
	polyId = CCreateEnt::PostToModelSpace(p3dPloy);
	return polyId;
}
//用户交互创建多线
void CCreateEnt::CreatePoly()
{
	int colorIndex = 0; // 颜色索引值
	ads_real width_begin = 0; // 多段线的起点线宽
	ads_real width_end = 0; // 多段线的终点线宽
	int nIndex = 2; // 当前输入点的次数
	ads_point ptStart; // 起点
	ads_point ptPrevious;// 前一个参考点
	ads_point ptCurrent; //当前拾取的点
	AcDbObjectId polyId; //多段线ID

	// 提示用户输入起点
	if (RTNORM != acedGetPoint(NULL, _T("\n输入第一点:"), ptStart))
	{
		return;
	}
	acdbPointSet(ptStart, ptPrevious);//复制起点的坐标值
	// 输入第二点
	acedInitGet(NULL, _T("W C B F"));//限定输入的条件

	int rc = acedGetPoint(ptPrevious, _T("\n输入下一点 [宽度(W)/颜色(C)/闭合(B)]<完成(F)>:"), ptCurrent);

	while (rc == RTNORM || rc == RTKWORD)//返回值为成功或者用户输入了关键字
	{
		if (RTKWORD == rc) // 如果用户输入了关键字
		{
			ACHAR kword[20];//这里要用ACHAR定义关键字
			if (acedGetInput(kword) != RTNORM)//获取用户输入的关键字，不成功就返回
				return;
			if (0 == _tcscmp(kword, _T("W")))//判断关键字
			{
				CAuxiliary::GetWidth(&width_begin,&width_end);//获取宽度
			}
			else if (0 == _tcscmp(kword, _T("C")))
			{
				colorIndex = CAuxiliary::GetColorIndex(); //获取颜色索引

				AcDbEntity *pEnt;
				acdbOpenAcDbEntity(pEnt, polyId, AcDb::kForWrite);//根据对象id获取数据库中的实体的指针

				if (pEnt->isKindOf(AcDbPolyline::desc()) == Adesk::kTrue)//
				{
					AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);//将其转化为指向AcDbPolyline 的指针
					if (pPoly != NULL)
					{
						pPoly->setColorIndex(colorIndex);
					}
				}
				pEnt->close();
			}
			else if (0 == _tcscmp(kword, _T("B")))
			{
				if (nIndex < 3)
				{
					acutPrintf(_T("\n输入点个数不足，不能执行闭合操作"));
					return;
				}

				AcDbEntity *pEnt;
				acdbOpenAcDbEntity(pEnt, polyId, AcDb::kForWrite);//根据对象id获取数据库中的实体的指针

				if (pEnt->isKindOf(AcDbPolyline::desc()) == Adesk::kTrue)//
				{
					AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);//将其转化为指向AcDbPolyline 的指针
					if (pPoly != NULL)
					{
						pPoly->setClosed(Adesk::kTrue);
					}
				}
				pEnt->close();

				return;
			}
			else if (0 == _tcscmp(kword, _T("F")))
			{
				return;
			}
		}
		else if (RTNORM == rc)  // 用户输入了点
		{
			acutPrintf(_T("\n输入点的坐标是(%.2f, %.2f, %.2f)"), ptCurrent[X], ptCurrent[Y], ptCurrent[Z]);

			if (2 == nIndex)
			{
				// 创建多段线
				AcDbPolyline *pPoly = new AcDbPolyline(2);
				AcGePoint2d ptGe1, ptGe2; // 两个节点
				ptGe1[X] = ptPrevious[X];
				ptGe1[Y] = ptPrevious[Y];
				ptGe2[X] = ptCurrent[X];
				ptGe2[Y] = ptCurrent[Y];
				pPoly->addVertexAt(0, ptGe1, 0, width_begin, width_end);
				pPoly->addVertexAt(1, ptGe2, 0, width_begin, width_end);

				// 修改多段线的颜色和线宽
				//pPoly->setConstantWidth(width);
				//pPoly->setColorIndex(colorIndex);
				// 添加到模型空间
				polyId = PostToModelSpace(pPoly);
			}
			else if (nIndex > 2)
			{
				// 修改多段线，添加最后一个顶点
				AcDbPolyline *pPoly;
				acdbOpenObject(pPoly, polyId, AcDb::kForWrite);//获取数据库中的多线
				AcGePoint2d ptGe; // 增加的节点
				ptGe[X] = ptCurrent[X];
				ptGe[Y] = ptCurrent[Y];
				pPoly->addVertexAt(nIndex - 1, ptGe, 0, width_begin, width_end);
				// 修改多段线的颜色和线宽
				//pPoly->setConstantWidth(width);
				//pPoly->setColorIndex(colorIndex);
				pPoly->close();
			}

			++nIndex;
			acdbPointSet(ptCurrent, ptPrevious);//acdbPointSet宏： ads_point 变量值的复制
		}
		// 提示用户输入新的节点
		acedInitGet(NULL, _T("W C B F"));//限定输入的条件
		rc = acedGetPoint(ptPrevious, _T("\n输入下一点 [宽度(W)/颜色(C)/闭合(B)]<完成(F)>:"), ptCurrent);
	}
}

//创建正多边形
AcDbObjectId CCreateEnt::CreatePolygon(AcGePoint2d ptCenter, int number,
	double radius, double rotation, double width)
{
	AcGePoint2dArray points;
	double angle = 2 * CCalculation::PI() / (double)number;

	for (int i = 0; i < number; i++)
	{
		AcGePoint2d pt;
		pt.x = ptCenter.x + radius * cos(i * angle);//正多边行的顶点在坐标轴上
		pt.y = ptCenter.y + radius * sin(i * angle);
		points.append(pt);
	}

	AcDbObjectId polyId = CCreateEnt::CreatePolyline(points, width);

	// 将其闭合
	AcDbEntity *pEnt;
	acdbOpenAcDbEntity(pEnt, polyId, AcDb::kForWrite);//根据对象id获取数据库中的实体的指针

	if (pEnt->isKindOf(AcDbPolyline::desc()) == Adesk::kTrue)//
	{
		AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);//将其转化为指向AcDbPolyline 的指针
		if (pPoly != NULL)
		{
			pPoly->setClosed(Adesk::kTrue);
		}
	}
	
	pEnt->close();
	CModifyEnt::Rotate(polyId, ptCenter, rotation);//旋转
	return polyId;
}

//创建单行文字
AcDbObjectId CCreateEnt::CreateText(const AcGePoint3d& ptInsert,
	const ACHAR* text, AcDbObjectId style,
	double height, double rotation)
{
	AcDbText *pText = new AcDbText(ptInsert, text, style, height, rotation);
	return CCreateEnt::PostToModelSpace(pText);
}
AcDbObjectId CCreateEnt::CreateMText(const AcGePoint3d& ptInsert,
	const ACHAR* text, AcDbObjectId style,
	double height, double width)
{
	AcDbMText *pMText = new AcDbMText();
	// 设置多行文字的特性
	pMText->setTextStyle(style);
	pMText->setContents(text);
	pMText->setLocation(ptInsert);
	pMText->setTextHeight(height);
	pMText->setWidth(width);
	pMText->setAttachment(AcDbMText::kBottomLeft);//修改文字对齐方式

	return CCreateEnt::PostToModelSpace(pMText);
}

//创建填充
AcDbObjectId CCreateEnt::CreateHatch(AcDbObjectIdArray objIds,
	const ACHAR* patName, bool bAssociative)
{
	Acad::ErrorStatus es;
	AcDbHatch *pHatch = new AcDbHatch();
	// 设置填充平面
	AcGeVector3d normal(0, 0, 1);
	pHatch->setNormal(normal);
	pHatch->setElevation(0);
	// 设置关联性
	pHatch->setAssociative(bAssociative);
	// 设置填充图案
	pHatch->setPattern(AcDbHatch::kPreDefined, patName);
	// 添加填充边界
	es = pHatch->appendLoop(AcDbHatch::kExternal, objIds);
	// 显示填充对象
	es = pHatch->evaluateHatch();
	// 添加到模型空间
	AcDbObjectId hatchId;
	hatchId = CCreateEnt::PostToModelSpace(pHatch);
	// 如果是关联性的填充，将填充对象与边界绑定，以便使其能获得边界
	//对象修改的通知
		if (bAssociative)
		{
			AcDbEntity *pEnt;
			for (int i = 0; i < objIds.length(); i++)
			{
				es = acdbOpenAcDbEntity(pEnt, objIds[i],
					AcDb::kForWrite);
				if (es == Acad::eOk)
				{
					// 添加一个永久反应器
					pEnt->addPersistentReactor(hatchId);
					pEnt->close();
				}
			}
		}
	return hatchId;
}

CString CCreateEnt::NewLayer()//新建一个图层
{
	// 提示用户输入新建图层的名称
	ACHAR layerName[100];
	if (acedGetString(Adesk::kFalse, _T("\n输入新图层的名称："), layerName) != RTNORM)
	{
		return _T("0");
	}
	// 获得当前图形的层表
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getLayerTable(pLayerTbl, AcDb::kForWrite);//以写入的方式打开层表

	// 是否已经包含指定的层表记录
	if (pLayerTbl->has(layerName))
	{
		pLayerTbl->close();
		return _T("0");
	}

	// 创建新的层表记录
	AcDbLayerTableRecord *pLayerTblRcd;
	pLayerTblRcd = new AcDbLayerTableRecord();
	pLayerTblRcd->setName(layerName);//设置名称

	// 将新建的层表记录添加到层表中返回图层id
	AcDbObjectId layerTblRcdId;
	pLayerTbl->add(layerTblRcdId, pLayerTblRcd);

	acdbHostApplicationServices()->workingDatabase()
		->setClayer(layerTblRcdId);//设置图形的当前图层，但是好像没什么用

	pLayerTblRcd->close();
	pLayerTbl->close();

	return layerName;
}
CString CCreateEnt::SetLayer()//更改图层特性
{
	// 提示用户输入要修改的图层名称
	ACHAR layerName[100];
	if (acedGetString(Adesk::kFalse, _T("\n输入图层的名称："),
		layerName) != RTNORM)
	{
		return _T("0");
	}

	// 获得当前图形的层表
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getLayerTable(pLayerTbl, AcDb::kForRead);

	// 判断是否包含指定名称的层表记录
	if (!pLayerTbl->has(layerName))
	{
		pLayerTbl->close();
		return _T("0");
	}

	// 获得指定层表记录的指针
	AcDbLayerTableRecord *pLayerTblRcd;
	pLayerTbl->getAt(layerName, pLayerTblRcd, AcDb::kForWrite);

	// 弹出“颜色”对话框
	AcCmColor oldColor = pLayerTblRcd->color();
	int nCurColor = oldColor.colorIndex(); // 图层修改前的颜色
	int nNewColor = oldColor.colorIndex(); // 用户选择的颜色
	if (acedSetColorDialog(nNewColor, Adesk::kFalse, nCurColor))
	{
		AcCmColor color;
		color.setColorIndex(nNewColor);
		pLayerTblRcd->setColor(color);//设置图层颜色
	}


	// 设置线型
	ACHAR lineType[100];
	if (acedGetString(Adesk::kFalse, _T("\n输入线形的名称（点滑线 CENTER；虚线 DASHED）："),
		lineType) != RTNORM)
	{
		return _T("0");
	}

	AcDbLinetypeTable *pLinetypeTbl;
	AcDbObjectId linetypeId;
	//获取当前的线型表
	acdbHostApplicationServices()->workingDatabase()
		->getLinetypeTable(pLinetypeTbl, AcDb::kForRead);

	if (pLinetypeTbl->has(lineType))
	{
		pLinetypeTbl->getAt(lineType, linetypeId);
	}
	else
	{
		acutPrintf(_T("您输入的线型不存在！"));
		pLinetypeTbl->getAt(_T("Continuous"), linetypeId);//不存在用户选择的线形就选择实线
	}
	pLayerTblRcd->setLinetypeObjectId(linetypeId);
	pLinetypeTbl->close();

	// 设置线宽
	ACHAR linewidth[10];
	if (acedGetString(Adesk::kFalse, _T("\n输入线宽："), linewidth) != RTNORM)
	{
		return _T("0");
	}
	AcDb::LineWeight lineWeight = (AcDb::LineWeight)_ttol(linewidth);
	pLayerTblRcd->setLineWeight(lineWeight);

	pLayerTblRcd->close();
	pLayerTbl->close();

	return layerName;
}

CString CCreateEnt::ChooseLayer()//选择图层
{
	// 提示用户输入要修改的图层名称
	ACHAR layerName[100];
	if (acedGetString(Adesk::kFalse, _T("\n输入图层的名称："),
		layerName) != RTNORM)
	{
		return _T("0");
	}

	// 获得当前图形的层表
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getLayerTable(pLayerTbl, AcDb::kForRead);

	// 判断是否包含指定名称的层表记录
	if (!pLayerTbl->has(layerName))
	{
		pLayerTbl->close();
		acutPrintf(_T("您输入的图层不存在！"));
		return _T("0");
	}
	else
	{
		return layerName;
	}
	pLayerTbl->close();
}

CString CCreateEnt::DelLayer()//删除图层
{
	// 提示用户输入要修改的图层名称
	ACHAR layerName[100];
	if (acedGetString(Adesk::kFalse, _T("\n输入图层的名称："), layerName) != RTNORM)
	{
		return  _T("0");
	}
	// 获得当前图形的层表
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getLayerTable(pLayerTbl, AcDb::kForRead);
	// 判断是否包含指定名称的层表记录
	if (!pLayerTbl->has(layerName))
	{
		pLayerTbl->close();
		return  _T("0");
	}
	// 获得指定层表记录的指针
	AcDbLayerTableRecord *pLayerTblRcd;
	pLayerTbl->getAt(layerName, pLayerTblRcd, AcDb::kForWrite);

	pLayerTblRcd->erase(); // 为其设置“删除”标记
	pLayerTblRcd->close();
	pLayerTbl->close();

	return layerName;
}

// 将实体添加到图形数据库的模型空间
AcDbObjectId CCreateEnt::PostToModelSpace(AcDbEntity* pEnt)
{
	//图形数据库块表
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlockTable, AcDb::kForRead);

	//获取块表中特定的记录
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
		AcDb::kForWrite);

	//将实体添加到块表记录
	AcDbObjectId entId;
	pBlockTableRecord->appendAcDbEntity(entId, pEnt);

	//关闭块表，块表记录，实体
	pBlockTable->close();
	pBlockTableRecord->close();
	pEnt->close();

	return entId;
}