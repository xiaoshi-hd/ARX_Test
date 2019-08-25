#include "stdafx.h"
#include "CModifyEnt.h"
#include "CCalculation.h"


CModifyEnt::CModifyEnt()
{
}


CModifyEnt::~CModifyEnt()
{
}
//Acad::ErrorStatus 的目的就相当于一般使用BOOL 类型作为
//函数的返回值，用来标识函数是否执行成功。

Acad::ErrorStatus CModifyEnt::ChangeColor(AcDbObjectId entId, Adesk::UInt16
	colorIndex)
{
	AcDbEntity *pEntity;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForWrite);
	// 修改实体的颜色
	pEntity->setColorIndex(colorIndex);
	// 用完之后，及时关闭
	pEntity->close();

	return Acad::eOk;
}

Acad::ErrorStatus CModifyEnt::ChangeLayer(AcDbObjectId entId, CString strLayerName)
{
	AcDbEntity *pEntity;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForWrite);
	// 修改实体的图层
	pEntity->setLayer(strLayerName);
	// 用完之后，及时关闭
	pEntity->close();

	return Acad::eOk;
}

Acad::ErrorStatus CModifyEnt::ChangeLinetype(AcDbObjectId entId, CString strLinetype)
{
	AcDbEntity *pEntity;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForWrite);
	// 修改实体的线型
	pEntity->setLayer(strLinetype);
	// 用完之后，及时关闭
	pEntity->close();

	return Acad::eOk;
}

Acad::ErrorStatus CModifyEnt::Rotate(AcDbObjectId entId, AcGePoint2d ptBase, double rotation)
{
	AcGeMatrix3d xform;//创建一个几何变换矩阵
	AcGeVector3d vec(0, 0, 1);
	xform.setToRotation(rotation, vec, CCalculation::Pt2dTo3d(ptBase));//设置旋转
	AcDbEntity *pEnt;//创建实体
	Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite, false);
	pEnt->transformBy(xform);
	pEnt->close();
	return es;
}
Acad::ErrorStatus CModifyEnt::Move(AcDbObjectId entId, AcGePoint3d ptBase,AcGePoint3d ptDest)
{
	// 设置变换矩阵的参数
	AcGeMatrix3d xform;
	AcGeVector3d vec(ptDest.x - ptBase.x, ptDest.y - ptBase.y, ptDest.z - ptBase.z);
	xform.setToTranslation(vec);
	AcDbEntity *pEnt;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite,false);
	pEnt->transformBy(xform);
	pEnt->close();
	return es;
}
Acad::ErrorStatus CModifyEnt::Scale(AcDbObjectId entId,AcGePoint3d ptBase, double scaleFactor)
{
	// 设置变换矩阵的参数
	AcGeMatrix3d xform;
	xform.setToScaling(scaleFactor, ptBase);
	AcDbEntity *pEnt;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite,false);
	pEnt->transformBy(xform);
	pEnt->close();
	return es;
}