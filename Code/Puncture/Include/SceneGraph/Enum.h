#if!defined(ENUM_H)
#define ENUM_H
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>

//字体列表数目
#define FONTLIST    256

//字体类型
enum GLFontType
{
	GL_FONT_SOLID=0,
	GL_FONT_LINE
}; 

enum RotateMode
{
	SG_NORMALMODE=0,
	SG_EASYMODE
};

enum InteractionMode
{
	IM_ROTATE_NORMAL = 0,
	IM_ROTATE_BOXONLY,
	IM_SELECT,
	IM_TRANSLATE,
};

//////////////////////////////
/*PI常量*/
const double PI=3.1415926; 
const float DELTA_ROT=0.00001f;//旋转最小步长，2004－2－6修改

/* 节点类型常量*/ 
//Scene Graph 节点基类
const int SCENE_NODE_BASE  =  0;

//材质节点
const int MATERIAL_NODE    =  1;

/********光源节点及其派生类****************/
const int LIGHT_NODE_BEGIN = 10; 
const int POINT_LIGHT_NODE = 12; //点光源节点
const int DIR_LIGHT_NODE   = 13; //平行光源节点
const int SPOT_LIGHT_NODE  = 14; //聚光源节点
//派生类...
const int LIGHT_NODE_END   = 19;
/*************************************/


/********相机节点及其派生类*************/
const int CAMERA_NODE_BEGIN= 20;
const int CAMERA_NODE      = 21;//相机节点
//派生类...
const int CAMERA_NODE_END  = 29;
/*************************************/

/***********变换节点类*************/
const int TRANSFORM_NODE   = 30;
/*************************************/

/*************组节点类*************/
const int GROUP_NODE       =40;
/*************************************/

/************形体节点及派生类********/
const int SCENEOBJECT_NODE = 50;
const int GEOMETRY_NODE    =51;
const int TEX_GEOMETRY_NODE=52;
const int MARK_NODE = 53;
const int PATH_NODE = 54;
const int CUBOID_NODE = 55;
const int SPHERE_NODE = 56;
const int KNIFE_NODE = 57;
const int RADARPOINTS_NODE = 58;
/**************************************/

//鼠标交互方式常量
const int NO_MOUSE_INTERACTION = 60;
const int MOUSE_SELECT = 61;
const int MOUSE_ROTATE = 62;
const int MOUSE_FEEDBACK = 63;
const int MOUSE_TRANSLATE = 64;

//文字节点
const int TEXT_NODE = 70;

/*__________________________________________________

        以下为Projection Mode常量
  __________________________________________________*/

typedef enum E_PROJ_MODE{ PERSPECTIVE = 0,
                        ORTHO
    }PROJ_MODE;

/////////////////////////////
typedef enum E_MOUSE_MODE{MM_MOUSE_HAND=0 , 
						  MM_MOUSE_MOVE,
					      MM_MYDEFAULT
	}MOUSE_MODE;

/*_____________________________________________________
  
	以下为长度的常量
  _____________________________________________________*/

const int NODE_NAME_LENGTH = 32;	// 节点名字的字符串长度
// 树的最大深度
static const int TREE_DEPTH=50;

typedef struct STRUCT_BOUND_BOX{
	float CenterX;
	float CenterY;
	float CenterZ;
	float XRange;
	float YRange;
	float ZRange;
}BOUND_BOX;

#endif