// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "CCreateEnt.h"
#include "CModifyEnt.h"
#include "CCalculation.h"
#include "CAuxiliary.h"
#include <vector> 
#include"acedCmdNF.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("asdk")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CChangeColorApp : public AcRxArxApp {

public:
	CChangeColorApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CChangeColorApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	
	//在程序运行之前，要确保“strLayer”层和“虚线”线型的存在

	static void asdkMyGroupAddLine () {//更改实体的颜色
		// Put your command code here
		// 创建直线
		AcGePoint3d ptStart(0, 0, 0);
		AcGePoint3d ptEnd(100, 100, 0);
		AcDbObjectId lineId;
		lineId = CCreateEnt::CreateLine(ptStart,ptEnd);
		// 修改直线的颜色为红色
		CModifyEnt::ChangeColor(lineId, 1);
		// 修改直线的图层为strLayer
		CModifyEnt::ChangeLayer(lineId, _T("strLayer"));
		// 修改直线的线形为虚线
		CModifyEnt::ChangeLinetype(lineId, _T("虚线"));
	}

	static void asdkMyGroupAddCircle() {
		// Put your command code here
		// 创建圆
		AcGePoint3d ptCenter(0, 0, 0);//圆心坐标，半径100
		CCreateEnt::CreateCircle(ptCenter, 100);
		CCreateEnt::CreatePolyCircle(CCalculation::Pt3dTo2d(ptCenter), 200);
	}

	static void asdkMyGroupAddArc() {
	// Put your command code here
		// 创建圆弧
		AcGePoint2d ptCenter(0, 0);
		CCreateEnt::CreateArc(ptCenter, 1000, 0, CCalculation::dutohudu(145));
		CCreateEnt::CreatePolyArc(ptCenter, 500, 0, CCalculation::dutohudu(145));
	}

	static void asdkMyGroupAddPolyline() {
		// Put your command code here
		//用户交互创建多线
		CCreateEnt::CreatePoly();
	}

	static void asdkMyGroupAddRect()
	{
		ads_point ptStart; // 起点
		ads_point ptEnd; // 终点

		if (RTNORM != acedGetPoint(NULL, _T("\n输入第一点:"), ptStart))
		{
			return;
		}
		acutPrintf(_T("\n输入点的坐标是(%.2f, %.2f, %.2f)"), ptStart[X], ptStart[Y], ptStart[Z]);

		if (RTNORM != acedGetPoint(NULL, _T("\n输入第二点:"), ptEnd))
		{
			return;
		}
		acutPrintf(_T("\n输入点的坐标是(%.2f, %.2f, %.2f)"), ptEnd[X], ptEnd[Y], ptEnd[Z]);

		CCreateEnt::CreateRect(asPnt2d(ptStart), asPnt2d(ptEnd)
);
	}

	static void asdkMyGroupAddPolygon() {
		// Put your command code here
		// 创建正多边形
		CCreateEnt::CreatePolygon(AcGePoint2d::kOrigin, 6, 100, 0, 1);
		//输入参数为中心、边数、外接圆半径、旋转角度（弧度值）和线宽
	}

	static void asdkMyGroupEntInfo()
	{
		// 提示用户选择实体
		ads_name entName;
		ads_point pt;
		if (acedEntSel(_T("\n选择实体:"), entName, pt) != RTNORM)
			return;
		struct resbuf *rbEnt; // 保存实体数据的结果缓冲区
		struct resbuf *rb; // 用于遍历rbEnt的结果缓冲区
		// 从entName获得保存实体数据的结果缓冲区
		rbEnt = acdbEntGet(entName);
		rb = rbEnt;
		while (rb != NULL)
		{
			switch (rb->restype)
			{
			case -1: // 图元名
				acutPrintf(_T("\n图元名: %x"), rb->resval.rstring);
				break;
			case 0: // 图元类型
				acutPrintf(_T("\n图元类型: %s"), rb->resval.rstring);
				break;
			case 8: // 图层
				acutPrintf(_T("\n图层:%s"), rb->resval.rstring);
				break;
			case 10: // 圆心
				acutPrintf(_T("\n圆心:(%.2f, %.2f, %.2f)"),
					rb->resval.rpoint[X],
					rb->resval.rpoint[Y],
					rb->resval.rpoint[Z]);
				break;
			case 40: // 半径
				acutPrintf(_T("\n半径:%.4f"), rb->resval.rreal);
				break;
			case 210: // 圆所在平面的法向矢量
				acutPrintf(_T("\n平面的法向矢量:(%.2f, %.2f, %.2f)"),
					rb->resval.rpoint[X],
					rb->resval.rpoint[Y],
					rb->resval.rpoint[Z]);
				break;
			default:
				break;
			} // switch
			rb = rb->rbnext; // 切换到下一个节点
		} // while
		if (rbEnt != NULL)
		{
			acutRelRb(rbEnt);
		}
		/*如果没有使用
		辅助的指针rb 来遍历所有节点，而是直接使用rbEnt 来访问，最后就无法通过acutRelRb 函
		数释放结果缓冲区链表的存储空间（rbEnt 并未指向链表的头节点），造成内存泄漏。*/
	}

	static void asdkMyGroupAddText()
	{
		// 创建单行文字
		AcGePoint3d ptInsert(0, 12, 0);
		CCreateEnt::CreateText(ptInsert, _T("CAD文字"));
		// 创建多行文字
		ptInsert.set(0, 0, 0);
		CCreateEnt::CreateMText(ptInsert, _T("http://www.cadhelp.net\n哈哈"));
	}

	static void asdkMyGroupNewLayer()
	{
		CCreateEnt::NewLayer();
	}

	static void asdkMyGroupSetLayer()
	{
		CCreateEnt::SetLayer();
	}

	static void asdkMyGroupDelLayer()
	{
		CString layerName;
		layerName = CCreateEnt::DelLayer();
		if (layerName == _T("0"))
		{
			acutPrintf(_T("不存在您所选择的图层！"));
		}
		else
		{
			acutPrintf(_T("您刚才删除了图层：") + layerName);
		}
	}

	static void asdkMyGroupOpenFile1()
	{
		CAuxiliary::vec_pt.clear();//清除链表的数据

		const ACHAR* title = _T("选择数据文件");
		const ACHAR* path = _T("G:\\");
		struct resbuf *fileName;
		fileName = acutNewRb(RTSTR);
		if (acedGetFileD(title, path, _T("txt;bat"), 0, fileName) == RTNORM)
		{
			//acedAlert(fileName->resval.rstring);//对话框显示文件路径
			CStdioFile file(fileName->resval.rstring, CFile::modeRead | CFile::typeText);
			CString strLine, strTemp;
			int row = 0;
			try
			{
				while (file.ReadString(strLine))//每次读取一行
				{
					CMyPoint point;
					int n = strLine.Find(_T(","));//返回第一次出现的位置（索引位置，从0开始），如果没有，返回-1
					strTemp = strLine.Left(n);
					point.ptName = strTemp;
					strLine = strLine.Right(strLine.GetLength() - n - 1);//去掉第一个逗号及之前的内容
					n = strLine.Find(_T(","));
					/*strLine = strLine.Right(strLine.GetLength() - n - 1);//把第二个没数据的逗号去掉
					n = strLine.Find(_T(","));*/

					float* num = new float[3];//存储坐标值
					int i = 0;
					while (-1 != n)
					{
						strTemp = strLine.Left(n);
						//num[i] = (float)atof((char *)(LPTSTR)(LPCTSTR)strTemp);//这种强制转换丢失精度
						num[i] = (float)_ttof(strTemp);
						i++;
						strLine = strLine.Right(strLine.GetLength() - n - 1);
						n = strLine.Find(_T(","));
					}
					if (strLine.GetLength() > 0)//最后一个数据
					{
						num[i] = (float)_ttof(strLine);
					}
					point.pt.x = num[1];//测量坐标系下x，y坐标互换
					point.pt.y = num[0];
					point.pt.z = num[2];
					CAuxiliary::vec_pt.push_back(point);//为动态链表赋值
					row++;
				}
			}
			catch (...)//捕获所有异常
			{
				acedAlert(_T("请检查文件中的数据是否正确！\n数据导入失败！"));
				return;
			}
			file.Close();
		}
		acutRelRb(fileName);
		//acedAlert(_T("文件打开成功！"));

		AcDbObjectId textId;
		CString layerName;
		//layerName = CCreateEnt::NewLayer();//让用户新建一个图层
		layerName = CCreateEnt::ChooseLayer();//让用户选择图层特性

		for (int i = 0; i < CAuxiliary::vec_pt.size(); i++)
		{
			CCreateEnt::CreatePoint(CAuxiliary::vec_pt[i].pt);

			//CString 转CStringA 转string 再转 const char*
			/*CStringA strA(CAuxiliary::vec_pt[i].ptName.GetBuffer(0));
			CAuxiliary::vec_pt[i].ptName.ReleaseBuffer();
			string s = strA.GetBuffer(0);
			const char* pc = s.c_str();*/

			textId = CCreateEnt::CreateText(CAuxiliary::vec_pt[i].pt, CAuxiliary::vec_pt[i].ptName);
			CModifyEnt::ChangeLayer(textId,layerName);//更改文字注记的图层

			/*ads_point pt = {CAuxiliary::vec_pt[i].pt.x, CAuxiliary::vec_pt[i].pt.y, CAuxiliary::vec_pt[i].pt.z};
			acedCommandS(RTSTR, "POINT", // 命令
				RTPOINT, pt, // 位置
				RTNONE); // 结束命令*/
		}
		/*CString str = "";
		acedAlert(CAuxiliary::vec_pt[0].ptName);
		str.Format(_T("%.2f"), (float)CAuxiliary::vec_pt[0].pt.x);
		acedAlert(str);
		str.Format(_T("%.2f"), (float)CAuxiliary::vec_pt[0].pt.y);
		acedAlert(str);
		str.Format(_T("%.2f"), (float)CAuxiliary::vec_pt[0].pt.z);
		acedAlert(str);*/

		//CCreateEnt::CreateTriangle(CAuxiliary::vec_pt[0].pt, CAuxiliary::vec_pt[1].pt, CAuxiliary::vec_pt[2].pt);

		// 进行缩放
		acedCommandS(RTSTR, _T("Zoom"), RTSTR, _T("E"), RTNONE);

		acdbCurDwg()->setPdmode(34); //设置点样式

		//acdbCurDwg()->setPdsize(3); //设置大小
	}

	static void asdkMyGroupDrawTriange()
	{
		if (CAuxiliary::vec_pt.size() == 0)
		{
			acedAlert(_T("请先打开文件！"));
			return;
		}

		CString layerName;
		//layerName = CCreateEnt::NewLayer();//让用户新建一个图层
		layerName = CCreateEnt::ChooseLayer();//让用户选择图层特性

		CAuxiliary::drawTriangle(CAuxiliary::vec_pt, layerName);
	}

	static void asdkMyGroupCaculateV()
	{
		if (CAuxiliary::vec_tri.size() == 0)
		{
			acedAlert(_T("请先构建TIN！"));
			return;
		}

		int gaocheng;
		acedGetInt(_T("请输入基准高程: "), &gaocheng);
		CAuxiliary::result = CAuxiliary::calculateV(CAuxiliary::vec_tri, gaocheng);
	}

	static void asdkMyGroupSaveFile()
	{
		if (CAuxiliary::result == "")
		{
			acedAlert(_T("请先计算体积！"));
			return;
		}

		//acedAlert(CAuxiliary::result);

		CFileDialog fileDlg(FALSE);//文件保存
		fileDlg.m_ofn.lpstrTitle = _T("文件保存");
		fileDlg.m_ofn.lpstrFilter = _T("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0");
		fileDlg.m_ofn.lpstrDefExt = _T("txt");
		if (IDOK == fileDlg.DoModal())
		{
			CFile file(fileDlg.GetPathName()/*_T("G:\\")*/, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
			// | CFile::typeBinary是设置二进制模式读写，不能读取回车换行符
			//CStdioFile  file(_T("Report.txt"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

			//是文件保存为unicode格式
			//为了unicode编码的CString写入unicode编码的文件之后能正常显示不出现乱码
			//需要在文件的开头写上0xFEFF
			WORD unicode = 0xFEFF; //这句重要
			file.Write(&unicode, 2); //这句重要

			file.Write(CAuxiliary::result, CAuxiliary::result.GetLength() * 2);//这里乘以2应该是以ansi编码方式编码了
			file.Close();
			AfxMessageBox(_T("文件保存成功！"));
		}
	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void asdkMyGroupMyPickFirst () {
		ads_name result;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM )
		{
			// There are selected entities
			// Put your command using pickfirst set code here
		}
		else
		{
			// There are no selected entities
			// Put your command code here
		}
	}

	// Application Session Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void asdkMyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CChangeColorApp class.
	// The function may or may not take arguments and have to return RTNORM, RTERROR, RTCAN, RTFAIL, RTREJ to AutoCAD, but use
	// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal to return
	// a value to the Lisp interpreter.
	//
	// NOTE: ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid.
	
	//- ACED_ADSFUNCTION_ENTRY_AUTO(classname, name, regFunc) - this example
	//- ACED_ADSSYMBOL_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file
	//- ACED_ADSCOMMAND_ENTRY_AUTO(classname, name, regFunc) - a Lisp command (prefix C:)
	//- ACED_ADSCOMMAND_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file

	// Lisp Function is similar to ARX Command but it creates a lisp 
	// callable function. Many return types are supported not just string
	// or integer.
	// ACED_ADSFUNCTION_ENTRY_AUTO(CChangeColorApp, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CChangeColorApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, AddLine, AddLine, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, AddCircle, AddCircle, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, AddArc, AddArc, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, AddPolyline, AddPolyline, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, AddRect, AddRect, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, AddPolygon, AddPolygon, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, EntInfo, EntInfo, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, AddText, AddText, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, NewLayer, NewLayer, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, SetLayer, SetLayer, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, DelLayer, DelLayer, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, OpenFile1, OpenFile1, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, DrawTriange, DrawTriange, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, CaculateV, CaculateV, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, SaveFile, SaveFile, ACRX_CMD_MODAL, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CChangeColorApp, asdkMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CChangeColorApp, MyLispFunction, false)

