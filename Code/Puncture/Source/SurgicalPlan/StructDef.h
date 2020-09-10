#pragma once

namespace SURGICALPLAN
{
#define img_Round(x) (int (x+0.5))
	const double img_EqualCriteria = 1e-5;

	typedef struct struct_imgColor
	{
		BYTE Blue, Green, Red;
	} img_COLOR;			// color defined following BMP order

	typedef struct struct_imgColorA
	{
		BYTE Blue, Green, Red, Alpha;
	} img_COLOR_A;			// color with alpha factor

	typedef struct struct_Pt3D
	{
		double x, y, z;
	} Point3D;

	typedef struct  struct_Pt3D_INT
	{
		int x, y, z;
	} Point3DInt;

	typedef enum img_DataType
	{
		img_DataTypeUndefined = 0,
		img_DataTypeMonochrom, //单色 0 or 1, 1 bit
		img_DataTypeHalfByte,// 4 bit
		img_DataTypeChar,//char
		img_DataTypeByte,// unsigned char
		img_DataTypeShort,// 2 bytes
		img_DataTypeWord,// 4 bytes
		img_DataTypeLong,// 4 bytes
		img_DataTypeDword,//8 bytes
		img_DataTypeInt,// 4 bytes
		img_DataTypeUint,// unsigned int 4 byte
		img_DataTypeFloat, // 4 bytes
		img_DataTypeDouble,// 8 bytes
		img_DataTypeString,// 64 BYTE
		img_DataTypeByteArray,// 1  unsigned char
		img_DataTypeWordArray,// 4
		img_DataTypeShortArray,// 2
		img_DataTypeLongArray,// 4
		img_DataTypeDwordArray,//4
		img_DataTypeFloatArray,//4
		img_DataTypeDoubleArray,//8
		img_DataTypeColor,  //RGB
		img_DataTypeColorA //RGBA
	} enum_img_DataType;

	typedef BYTE MaskDataType;	//Mask数据现在用的BYTE，后面改

	typedef short MRIDataType;	//MRI数据现在用的short，后面改

	typedef struct Size
	{
		int cx, cy;
	}ImageSize;

	typedef struct ThreeD_BOUND_BOX
	{
		float x1, x2, y1, y2, z1, z2;
	}ThreeD_BOUNDBOX;
}
