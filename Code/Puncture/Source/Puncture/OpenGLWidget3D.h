#pragma once
//参考https://blog.csdn.net/p942005405/article/details/103770259
//参考https://www.jianshu.com/p/bccc565b5248
//绘制半透明物体 参考https://blog.csdn.net/u014386971/article/details/22492631?utm_medium=distribute.pc_relevant.none-task-blog-title-2&spm=1001.2101.3001.4242
#define AFX_EXT_CLASS __declspec(dllimport)  //因为本项目是qt应用程序，未使用MFC，需要调用其他MFC项目中的类（fsutility::Coordinate）,所以简单地将MFC导出/导入dll的宏AFX_EXT_CLASS在本项目中定义为导入
#include <qopenglwidget.h>
#include <qopenglfunctions.h>
#include <qopenglshaderprogram.h>
#include <qopenglbuffer.h>
#include <qopenglvertexarrayobject.h>
#include <Qt3DInput/qkeyevent.h>
#include <mutex>
#include "SurgicalPlan/ObjData.h"


using namespace SURGICALPLAN;


class OpenGLWidget3D :
	public QOpenGLWidget,
	protected QOpenGLFunctions
{
	Q_OBJECT
public:
	OpenGLWidget3D(QWidget* parent = nullptr);
	~OpenGLWidget3D();

	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;
	
	void LoadProstateObj(ObjDataPtr prostateObjPtr);  //获取prostate模型指针，载入prostate数据，并更新3D显示参数  //TODO
	void LoadLesionObj(ObjDataPtr lesionObjPtr);  //获取lesion模型指针，载入lesion数据
	//void SetWorldSize(float width, float height, float depth);  //设置obj物体所在的世界空间坐标的大小(mm)
	void SetTransversePlane(Coordinate cLeftTop, Coordinate cRightTop, Coordinate cLeftBottom, Coordinate cRightBottom);  //更新横断面四个点的位置  //TODO(if iscreated())
	void SetSagittalPlane(Coordinate cLeftTop, Coordinate cRightTop, Coordinate cLeftBottom, Coordinate cRightBottom);  //更新矢状面四个点的位置  //TODO

protected:
	void mousePressEvent(QMouseEvent *event) override;  //鼠标拖动trackball 移动视角
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;  //鼠标滚轮缩放

	QVector3D projectToTrackball(const QPoint &screenCoords) const;  //将2D的屏幕点坐标，转换为3D的trackball球坐标
	void calRotateParameters(const QPoint &firstPoint,const QPoint &nextPoint, QVector3D &dir, float &angle);  //计算旋转参数

private:
	void CreateShader();  //使用硬盘文件 编译着色器
	void UpdateShader();  //根据病人数据 更新着色器参数
	void LoadAnnotationToGPU();  //构造并载入标记数据到显存
	void LoadPlaneToGPU();  //构造并载入平面数据到显存
	void LoadObjToGPU(ObjDataPtr objPtr, QOpenGLVertexArrayObject& vao, QOpenGLBuffer& vbo, QOpenGLBuffer& ebo);  //使用指定的vao、vbo、ebo对象，将模型载入GPU

private:
	QOpenGLShaderProgram m_shaderObject,m_shaderAnnotation;  //用于 模型/标注的着色器程序
	QOpenGLVertexArrayObject m_vaoAnnotation,m_vaoProstate, m_vaoLesion,m_vaoPlane;  //顶点数组对象(vao并不实际存储数据 只是存储了vbo、ebo的引用，以及解析数据的方式)
	QOpenGLBuffer m_vboAnnotation,m_vboProstate, m_vboLesion,m_vboPlane;  //顶点缓冲对象
	QOpenGLBuffer m_eboAnnotation,m_eboProstate, m_eboLesion,m_eboPlane;  //索引缓冲对象
	ObjDataPtr m_prostateObjPtr, m_lesionObjPtr;  //模型对象指针

	QMatrix4x4 m_mtxModel;  //模型矩阵  将prostate的boundbox移至中心
	QMatrix4x4 m_mtxView;  //观察矩阵
	QMatrix4x4 m_mtxProjection;  //投影矩阵
	QVector3D m_defaultViewerPos;  //默认的观察者位置
	QVector3D m_defaultUpDir;  //默认的上方

	//float m_fWidth;  //世界空间宽度(mm)
	//float m_fHeight;  //世界空间高度(mm)
	//float m_fDepth;  //世界空间深度(mm)
	float m_fRadius;  //摄像机所在的球半径(mm) 与boundbox的大小相关
	QVector4D m_colorAnnotation;  //标记颜色
	QVector4D m_colorProstate;  //前列腺颜色
	QVector4D m_colorLesion;  //病灶颜色
	QVector4D m_colorPlane;  //平面示意颜色
	QVector3D m_colorLight;  //光源颜色
	
	bool m_bLeftActive;  //左键被按下
	QPoint m_mouseLastPos,m_mouseCurPos;  //鼠标位置
	float m_fLinearSpeed;  //摄像机的平移速度
	float m_fRotateSpeed;  //摄像机的旋转速度
	float m_fTrackballRadius;  //trackball的半径(实际上是相对于屏幕尺寸的比例)
	float m_fX1, m_fX2, m_fY1, m_fY2, m_fZ1, m_fZ2;  //boundbox
	GLfloat m_verticesPlane[8*6];  //双平面示意 共8个点,每个点包含坐标数据+法向量数据

	std::mutex m_mutexRender;  //渲染 更新数据 互斥量
};

