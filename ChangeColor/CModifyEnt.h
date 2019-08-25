#pragma once
class CModifyEnt
{
public:
	CModifyEnt();
	~CModifyEnt();
	static Acad::ErrorStatus ChangeColor(AcDbObjectId entId, Adesk::UInt16 colorIndex);//改变实体颜色
	static Acad::ErrorStatus ChangeLayer(AcDbObjectId entId, CString strLayerName);//改变实体图层
	static Acad::ErrorStatus ChangeLinetype(AcDbObjectId entId, CString strLinetype);
	//改变实体线型（通过改变图层的方式）

	static Acad::ErrorStatus Rotate(AcDbObjectId entId, AcGePoint2d ptBase, double rotation);
	//按照指定的角度（用弧度值表示）旋转指定的实体
	static Acad::ErrorStatus Move(AcDbObjectId entId, AcGePoint3d ptBase, AcGePoint3d ptDest);
	//按照指定的坐标移动指定的实体
	static Acad::ErrorStatus Scale(AcDbObjectId entId, AcGePoint3d ptBase, double scaleFactor);
	//缩放实体
};

