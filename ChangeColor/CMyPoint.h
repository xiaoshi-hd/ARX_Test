#pragma once
class CMyPoint
{
public:
	CMyPoint();
	~CMyPoint();

public:
	CString ptName;
	AcGePoint3d pt;
};

class CMyLine
{
public:
	CMyLine();
	~CMyLine();

public:
	AcGePoint3d ptBegin;
	AcGePoint3d ptEnd;
};

class CMyTriangle
{
public:
	CMyTriangle();
	~CMyTriangle();

public:
	AcGePoint3d pt1;
	AcGePoint3d pt2;
	AcGePoint3d pt3;
};
