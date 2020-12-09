#include "OpenGLWidget3D.h"
#include <string>
#include <qstring.h>
#include <qmessagebox.h>
#include <qvector3d.h>
#pragma execution_character_set("utf-8")  //设置默认编码格式 避免中文乱码



OpenGLWidget3D::OpenGLWidget3D(QWidget* parent):
	m_vboAnnotation(QOpenGLBuffer::VertexBuffer),  //定义m_vbo为顶点缓冲对象,作为顶点着色器的输入(对象在create()处才会创建)
	m_vboProstate(QOpenGLBuffer::VertexBuffer),  
	m_vboLesion(QOpenGLBuffer::VertexBuffer),
	m_vboPlane(QOpenGLBuffer::VertexBuffer),
	m_eboAnnotation(QOpenGLBuffer::IndexBuffer),  //定义m_ebo为索引缓冲对象,作为图元着色器的输入(对象在create()处才会创建)
	m_eboProstate(QOpenGLBuffer::IndexBuffer),  
	m_eboLesion(QOpenGLBuffer::IndexBuffer),
	m_eboPlane(QOpenGLBuffer::IndexBuffer)
{
	this->setParent(parent);

	m_colorAnnotation = QVector4D(1.0f, 0.0f, 0.0f, 1.0f);  //设置标记颜色为红色不透明
	m_colorProstate = QVector4D(0.0f, 0.5f, 1.0f, 0.5f);  //设置前列腺颜色为浅蓝色半透明
	m_colorLesion = QVector4D(1.0f, 0.0f, 0.0f, 1.0f);  //设置病灶颜色为红色不透明
	m_colorLight = QVector3D(1.0f, 1.0f, 1.0f);  //光源设为白色
	m_colorPlane = QVector4D(0.5, 0.5, 0.5, 0.5);  //平面示意颜色为灰色不透明
	m_bLeftActive = false;  //初始状态左键未按下
	m_fLinearSpeed = 10; 
	m_fRotateSpeed = 100;
	memset(m_verticesPlane, 0, sizeof(GLfloat) * 8 * 6);  //初始状态平面上的点全为0
	m_fTrackballRadius = 1.0;  //trackball的半径默认为1

	this->setVisible(true);  //设置为可见 启动绘制
}


OpenGLWidget3D::~OpenGLWidget3D()
{
	this->makeCurrent();  //令当前context为OpenGL输出的目标

	//销毁顶点缓冲对象(从显存中删除数据)
	m_vboAnnotation.destroy();
	m_vboProstate.destroy();
	m_vboLesion.destroy();
	m_vboPlane.destroy();
	//销毁索引缓冲区(从显存中删除数据)
	m_eboAnnotation.destroy();
	m_eboProstate.destroy(); 
	m_eboLesion.destroy();
	m_eboPlane.destroy();
	//销毁顶点数组对象(从内存中删除数据)
	m_vaoAnnotation.destroy();
	m_vaoProstate.destroy();  
	m_vaoLesion.destroy();
	m_vaoPlane.destroy();

	this->doneCurrent();  //解除对当前context的绑定
}

void OpenGLWidget3D::initializeGL()
{
	this->initializeOpenGLFunctions();

	/************编译着色器*******************/
	this->CreateShader();

	/**************配置模型着色器********************/
	QMatrix4x4 mtxIdentity;  //单位阵 作为着色器各个矩阵的默认值
	m_shaderObject.bind();  //将当前着色器程序绑定到opengl上下文
	//设置模型矩阵(用于将boundbox移至中心)
	m_shaderObject.setUniformValue("model", mtxIdentity);
	//设置观察矩阵
	m_shaderObject.setUniformValue("view", mtxIdentity);
	//设置投影矩阵
	m_shaderObject.setUniformValue("projection", mtxIdentity);
	//设置光源颜色
	m_shaderObject.setUniformValue("lightColor", m_colorLight);
	m_shaderObject.release();
	/**************配置标注着色器********************/
	m_shaderAnnotation.bind();  //将当前着色器程序绑定到opengl上下文
	//设置模型矩阵
	m_shaderAnnotation.setUniformValue("model", mtxIdentity);
	//设置观察矩阵
	m_shaderAnnotation.setUniformValue("view", mtxIdentity);
	//设置投影矩阵
	m_shaderAnnotation.setUniformValue("projection", mtxIdentity);
	m_shaderAnnotation.release();

	/*************载入数据到显存****************/
	this->LoadPlaneToGPU();

	m_shaderObject.release();
}

void OpenGLWidget3D::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}

void OpenGLWidget3D::paintGL()
{
	glEnable(GL_DEPTH_TEST);  //启用深度测试
	glEnable(GL_BLEND);  //启用混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //设置背景颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //清除颜色缓冲以及深度缓冲

	//总是保持深度测试开启
	//先绘制不透明物体，同时将其深度信息写入深度缓冲
	//再绘制半透明物体，不将其深度信息写入缓冲，半透明物体不会遮挡其后的物体
	/*********绘制不透明物体***********/
	glDepthMask(GL_TRUE);  //将深度缓冲区设置为可读可写形式
	m_mutexRender.lock();  //开始渲染过程
	m_shaderAnnotation.bind();  //将标注着色器与opengl上下文绑定
	m_shaderAnnotation.setUniformValue("view", m_mtxView);
	m_shaderAnnotation.setUniformValue("color", m_colorAnnotation);
	//绘制boundbox
	if(m_vboAnnotation.isCreated())
	{
		m_vboAnnotation.bind();
		m_eboAnnotation.bind();
		unsigned int eboData[24];
		m_eboAnnotation.read(0, eboData, sizeof(eboData));
		m_vboAnnotation.release();
		m_eboAnnotation.release();
		QOpenGLVertexArrayObject::Binder vaoBind(&m_vaoAnnotation);
		m_shaderAnnotation.setUniformValue("color", m_colorAnnotation);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);  //绘制长方体的12条棱
		vaoBind.release();
	}
	m_shaderAnnotation.release();

	m_shaderObject.bind();  //将模型着色器与opengl上下文绑定
	m_shaderObject.setUniformValue("view", m_mtxView);  //设置观察矩阵
	//绘制病灶
	if(m_vboLesion.isCreated())
	{
		QOpenGLVertexArrayObject::Binder vaoBind(&m_vaoLesion);
		m_shaderObject.setUniformValue("objectColor", m_colorLesion);  //设置病灶颜色
		glDrawElements(GL_TRIANGLES, m_lesionObjPtr->GetTriangleNum() * 3, GL_UNSIGNED_INT, 0);  //绘制三角面片
		vaoBind.release();
	}

	/*********绘制半透明物体*************/
	glDepthMask(GL_FALSE);  //将深度缓冲区设置为只读形式
	//绘制前列腺
	if(m_vboProstate.isCreated())
	{
		QOpenGLVertexArrayObject::Binder vaoBind(&m_vaoProstate);
		m_shaderObject.setUniformValue("objectColor", m_colorProstate);  //设置前列腺颜色
		glDrawElements(GL_TRIANGLES, m_prostateObjPtr->GetTriangleNum() * 3, GL_UNSIGNED_INT, 0);  //绘制三角面片
		vaoBind.release();
	}
	//绘制示意平面
	if(m_vboPlane.isCreated())
	{
		//TODO
		QOpenGLVertexArrayObject::Binder vaoBind(&m_vaoPlane);
		m_shaderObject.setUniformValue("objectColor", m_colorPlane);  //设置示意平面颜色
		glDrawElements(GL_TRIANGLES, 4 * 3, GL_UNSIGNED_INT, 0);  //绘制双平面 共4个三角面片
		vaoBind.release();
	}
	glDepthMask(GL_TRUE);  //绘制完成后  一定要将深度缓冲区设置为可读可写形式，否则会引发未知的错误
	m_shaderObject.release();
	m_mutexRender.unlock();  //结束渲染过程
}

void OpenGLWidget3D::LoadProstateObj(ObjDataPtr prostateObjPtr)
{
	m_mutexRender.lock();  //避免开始渲染
	m_prostateObjPtr = prostateObjPtr;
	m_prostateObjPtr->GetBoundBox(m_fX1, m_fX2, m_fY1, m_fY2, m_fZ1, m_fZ2);
	m_mtxModel = QMatrix4x4();
	m_mtxModel.translate(QVector3D(-(m_fX1 + m_fX2) / 2, -(m_fY1 + m_fY2) / 2, -(m_fZ1 + m_fZ2) / 2));  //相当于把模型移动到以boundbox为中心
	m_fRadius = 1.5*sqrt(pow(m_fX2 - m_fX1, 2) + pow(m_fY2 - m_fY1, 2) + pow(m_fZ2 - m_fZ1, 2));  //摄像机球半径 设定为boundBox对角线长度的1.5倍
	this->UpdateShader();  //使用病人前列腺数据 更新着色器参数
	this->LoadAnnotationToGPU();  //将标记(boundBox)数据载入GPU
	this->LoadObjToGPU(m_prostateObjPtr, m_vaoProstate, m_vboProstate, m_eboProstate);  //将prostate数据载入GPU
	m_mutexRender.unlock();  //可以开始渲染
}

void OpenGLWidget3D::LoadLesionObj(ObjDataPtr lesionObjPtr)
{
	m_mutexRender.lock();  //避免开始渲染
	m_lesionObjPtr = lesionObjPtr;
	this->LoadObjToGPU(m_lesionObjPtr, m_vaoLesion, m_vboLesion, m_eboLesion);
	m_mutexRender.unlock();  //可以开始渲染
}

//void OpenGLWidget3D::SetWorldSize(float width, float height, float depth)
//{
//	m_fWidth = width;
//	m_fHeight = height;
//	m_fDepth = depth;
//	//将摄像机球半径设置为对角线长度的一半
	//m_fRadius = sqrt(width*width + height * height + depth * depth);
	//m_fRadius = width > height ? width : height;
	//m_fRadius = m_fRadius > depth ? m_fRadius : depth;
//}

void OpenGLWidget3D::SetTransversePlane(Coordinate cLeftTop, Coordinate cRightTop, Coordinate cLeftBottom, Coordinate cRightBottom)
{
	int i;
	Coordinate v1, v2, vNormal;
	v1 = cLeftTop - cLeftBottom;
	v2 = cLeftTop - cRightTop;
	vNormal = v1.GetCrossProduct(v2);
	vNormal.w = 0;  //normal是向量
	vNormal.Normalize();
	//设置点坐标
	m_verticesPlane[0] = cLeftTop.x;
	m_verticesPlane[1] = cLeftTop.y;
	m_verticesPlane[2] = cLeftTop.z;
	m_verticesPlane[6] = cRightTop.x;
	m_verticesPlane[7] = cRightTop.y;
	m_verticesPlane[8] = cRightTop.z;
	m_verticesPlane[12] = cLeftBottom.x;
	m_verticesPlane[13] = cLeftBottom.y;
	m_verticesPlane[14] = cLeftBottom.z;
	m_verticesPlane[18] = cRightBottom.x;
	m_verticesPlane[19] = cRightBottom.y;
	m_verticesPlane[20] = cRightBottom.z;
	//设置法向量
	for (i = 0; i < 4; i++)
	{
		m_verticesPlane[i * 6 + 3] = vNormal.x;
		m_verticesPlane[i * 6 + 4] = vNormal.y;
		m_verticesPlane[i * 6 + 5] = vNormal.z;
	}
	//更新至显存
	m_mutexRender.lock();  //避免开始渲染
	if (!m_vboPlane.isCreated())  //如果vboPlane尚未被创建，则数据也无法更新至显存
		return;
	m_vboPlane.bind();  
	m_vboPlane.allocate(m_verticesPlane, sizeof(m_verticesPlane));  
	m_vboPlane.release();
	m_mutexRender.unlock();  //可以开始渲染
}

void OpenGLWidget3D::SetSagittalPlane(Coordinate cLeftTop, Coordinate cRightTop, Coordinate cLeftBottom, Coordinate cRightBottom)
{
	int i;
	Coordinate v1, v2, vNormal;
	v1 = cLeftTop - cLeftBottom;
	v2 = cLeftTop - cRightTop;
	vNormal = v1.GetCrossProduct(v2);
	vNormal.w = 0;  //normal是向量
	vNormal.Normalize();
	//设置点坐标
	m_verticesPlane[24] = cLeftTop.x;
	m_verticesPlane[25] = cLeftTop.y;
	m_verticesPlane[26] = cLeftTop.z;
	m_verticesPlane[30] = cRightTop.x;
	m_verticesPlane[31] = cRightTop.y;
	m_verticesPlane[32] = cRightTop.z;
	m_verticesPlane[36] = cLeftBottom.x;
	m_verticesPlane[37] = cLeftBottom.y;
	m_verticesPlane[38] = cLeftBottom.z;
	m_verticesPlane[42] = cRightBottom.x;
	m_verticesPlane[43] = cRightBottom.y;
	m_verticesPlane[44] = cRightBottom.z;
	//设置法向量
	for (i = 5; i < 8; i++)
	{
		m_verticesPlane[i * 6 + 3] = vNormal.x;
		m_verticesPlane[i * 6 + 4] = vNormal.y;
		m_verticesPlane[i * 6 + 5] = vNormal.z;
	}
	//更新至显存
	m_mutexRender.lock();  //避免开始渲染
	if (!m_vboPlane.isCreated())  //如果vboPlane尚未被创建，则数据也无法更新至显存
		return;
	m_vboPlane.bind();  
	m_vboPlane.allocate(m_verticesPlane, sizeof(m_verticesPlane));
	m_vboPlane.release();
	m_mutexRender.unlock();  //可以开始渲染
}

void OpenGLWidget3D::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bLeftActive = true;
		m_mouseLastPos = event->pos();
		int temp = 0;
	}
}

void OpenGLWidget3D::mouseReleaseEvent(QMouseEvent * event)
{
	Q_UNUSED(event);  //event用不上
	m_bLeftActive = false;
}

void OpenGLWidget3D::mouseMoveEvent(QMouseEvent * event)
{
	//TODO
	m_mouseCurPos = event->pos();

	if (m_bLeftActive)  //鼠标左键拖动 使用trackball旋转视角
	{
		QVector3D dirWorld,dirLocal;  //旋转轴
		float angle;  //旋转角
		this->calRotateParameters(m_mouseLastPos, m_mouseCurPos, dirWorld, angle);  //计算旋转轴和旋转角
		dirLocal = m_mtxView.inverted().mapVector(dirWorld);  //将trackball的旋转轴映射为opengl坐标系下的旋转轴
		
		angle *= m_fRotateSpeed;
		m_mtxView.rotate(angle, dirLocal);  //对应旋转观察矩阵
		qDebug() << "dir" << dirLocal;
		this->update();  //重绘
	}

	m_mouseLastPos = m_mouseCurPos;
}

void OpenGLWidget3D::wheelEvent(QWheelEvent * event)
{
	//TODO
}

QVector3D OpenGLWidget3D::projectToTrackball(const QPoint & screenCoords) const
{
	float sCoordsX = screenCoords.x(), sCoordsY = this->height() - screenCoords.y();  //将以左上角为原点的2d坐标，变为以左下角为原点的2D坐标
	float z = 0.0f;  //映射到的trackball上的z坐标
	float r2 = m_fTrackballRadius * m_fTrackballRadius;

	QVector2D pos2D(sCoordsX / this->width() - 0.5f, sCoordsY / this->height() - 0.5f);  //将2D的opengl坐标x、y 映射到[-0.5,0.5]的区间内
	//qDebug()<<"pos2D"<< pos2D;
	if (pos2D.lengthSquared() <= r2 * 0.5f) {  //映射为球面范围内的点
		z = sqrt(r2 - pos2D.lengthSquared());
	}
	else {  //映射为双曲面上的点
		z = r2 * 0.5f / pos2D.length();
	}
	QVector3D pos3D(pos2D, z);
	//qDebug()<<p3d;
	return pos3D;
}

void OpenGLWidget3D::calRotateParameters(const QPoint & firstPoint, const QPoint & nextPoint, QVector3D & dir, float & angle)
{
	QVector3D lastPos3D = projectToTrackball(firstPoint).normalized();  //将鼠标输入的2D点变为trackball表面坐标
	QVector3D currentPos3D = projectToTrackball(nextPoint).normalized();  //将鼠标输入的2D点变为3d trackball表面坐标

	// 计算旋转轴方向
	dir = QVector3D::crossProduct(lastPos3D, currentPos3D);  //旋转轴方向是两个法向量的叉乘

	// 计算旋转角
	float dotVal;
	dotVal = QVector3D::dotProduct(lastPos3D, currentPos3D);
	dotVal = dotVal < 1 ? dotVal : 1;
	dotVal = dotVal > -1 ? dotVal : 1;
	angle = acos(dotVal);  //旋转角为两个法向量的夹角(弧度制)

	//qDebug()<<"dir:"<<dir<<"angle:"<<angle;
}

void OpenGLWidget3D::CreateShader()
{
	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);
	std::string strResDir = exePath;
	strResDir = strResDir.substr(0, strResDir.rfind("\\")) + "\\Res\\";
	QString strFileName;
	/***********编译用于标注的着色器程序**********/
	strFileName = QString::fromStdString(strResDir + "annotation.vert");
	if (!m_shaderAnnotation.addShaderFromSourceFile(QOpenGLShader::Vertex, strFileName))  //编译顶点着色器
	{
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_shaderAnnotation.log();
		QMessageBox::information(0, "错误", "读取标注着色器vert文件失败");
	}
	strFileName = QString::fromStdString(strResDir + "annotation.frag");
	if (!m_shaderAnnotation.addShaderFromSourceFile(QOpenGLShader::Fragment, strFileName))  //编译片段着色器
	{
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_shaderAnnotation.log();
		QMessageBox::information(0, "错误", "读取标注着色器frag文件失败");
	}
	if (!m_shaderAnnotation.link())  //链接上述着色器 生成着色器对象
	{
		qDebug() << "shaderProgram link failed!" << m_shaderAnnotation.log();
		QMessageBox::information(0, "错误", "链接标注着色器失败");
	}
	/***********编译用于模型的着色器程序**********/
	strFileName = QString::fromStdString(strResDir + "object.vert");
	if (!m_shaderObject.addShaderFromSourceFile(QOpenGLShader::Vertex, strFileName))  //编译顶点着色器
	{
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_shaderObject.log();
		QMessageBox::information(0, "错误", "读取模型着色器vert文件失败");
	}
	strFileName = QString::fromStdString(strResDir + "object.frag");
	if (!m_shaderObject.addShaderFromSourceFile(QOpenGLShader::Fragment, strFileName))  //编译片段着色器
	{
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_shaderObject.log();
		QMessageBox::information(0, "错误", "读取模型着色器frag文件失败");
	}
	if (!m_shaderObject.link())  //链接上述着色器 生成着色器对象
	{
		qDebug() << "shaderProgram link failed!" << m_shaderObject.log();
		QMessageBox::information(0, "错误", "链接模型着色器失败");
	}
}

void OpenGLWidget3D::UpdateShader()
{
	m_shaderObject.bind();  //将当前着色器程序绑定到opengl上下文
	//设置默认的相机参数
	m_defaultViewerPos = QVector3D(0.577*m_fRadius, 0.577*m_fRadius, 0.577*m_fRadius);
	m_defaultUpDir = QVector3D(0.0f, 1.0f, 0.0f);
	//设置模型矩阵(用于将boundbox移至中心)
	m_shaderObject.setUniformValue("model", m_mtxModel);
	//设置观察矩阵
	//m_mtxView.lookAt(QVector3D(0, 0, -m_fRadius), QVector3D(0, 0, 0), m_defaultUpDir);  //正前方
	m_mtxView.lookAt(m_defaultViewerPos, QVector3D(0.0f, 0.0f, 0.0f), m_defaultUpDir);  //45度角
	m_shaderObject.setUniformValue("view", m_mtxView);
	//设置投影矩阵
	m_mtxProjection.perspective(45.0f, 1.0f*this->width() / this->height(), 0.1f, 500.0f);  //设置透视投影
	//m_mtxProjection.ortho(-m_fRadius, m_fRadius, -m_fRadius, m_fRadius, -2*m_fRadius, 2*m_fRadius);  //设置正交投影
	m_shaderObject.setUniformValue("projection", m_mtxProjection);
	//设置观察者的位置(仅用于计算光照)
	m_shaderObject.setUniformValue("viewerPos", m_defaultViewerPos);
	m_shaderObject.release();
	/**************配置标注着色器********************/
	m_shaderAnnotation.bind();  //将当前着色器程序绑定到opengl上下文
	//设置模型矩阵
	m_shaderAnnotation.setUniformValue("model", m_mtxModel);
	//设置观察矩阵
	m_shaderAnnotation.setUniformValue("view", m_mtxView);
	//设置投影矩阵
	m_shaderAnnotation.setUniformValue("projection", m_mtxProjection);
	m_shaderAnnotation.release();
}

void OpenGLWidget3D::LoadAnnotationToGPU()
{
	this->makeCurrent();
	//boundbox长方体的8个顶点
	GLfloat vertices[] = {
		m_fX1,m_fY1,m_fZ1,
		m_fX1,m_fY1,m_fZ2,
		m_fX1,m_fY2,m_fZ1,
		m_fX1,m_fY2,m_fZ2,
		m_fX2,m_fY1,m_fZ1,
		m_fX2,m_fY1,m_fZ2,
		m_fX2,m_fY2,m_fZ1,
		m_fX2,m_fY2,m_fZ2
	};
	//长方体的12条棱
	unsigned int indices[] = {
	0,1,  //z方向
	2,3,
	4,5,
	6,7,
	0,2,  //y方向
	1,3,
	4,6,
	5,7,
	0,4,  //x方向
	1,5,
	2,6,
	3,7
	};

	QOpenGLVertexArrayObject::Binder vaoBind(&m_vaoAnnotation);
	m_vboAnnotation.create();  //在opengl服务器中创建该顶点buffer对象
	m_vboAnnotation.bind();  //将与此对象关联的缓冲区绑定到当前OpenGL上下文
	m_vboAnnotation.allocate(vertices, sizeof(vertices));  //载入模型顶点数据到显存
	m_eboAnnotation.create();  //在opengl服务器中创建该索引buffer对象
	m_eboAnnotation.bind();  //将与此对象关联的缓冲器绑定到当前opengl上下文
	m_eboAnnotation.allocate(indices, sizeof(indices));  //载入模型索引数据到显存

	//通知vao如何解析vbo数据
	int attr = -1;
	attr = m_shaderAnnotation.attributeLocation("aPos");  //为vao设置 如何解析顶点缓冲对象vbo内的顶点坐标数据
	m_shaderAnnotation.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);  //为vao设置 如何解析顶点缓冲对象vbo内的顶点坐标数据
	m_shaderAnnotation.enableAttributeArray(attr);  //使位置attr处的数据 即位置数组，可被着色器使用

	//解除与opengl上下文的绑定(数据仍然在显存中)
	vaoBind.release();
	m_vboAnnotation.release();
	this->doneCurrent();
}

void OpenGLWidget3D::LoadPlaneToGPU()
{
	this->makeCurrent();
	unsigned int indices[] = {  //双平面 三角面片索引。一经设置 不会改动
	0,1,2,  //横断面
	1,2,3,
	4,5,6,  //矢状面
	5,6,7
	};

	QOpenGLVertexArrayObject::Binder vaoBind(&m_vaoPlane);  //借助binder绑定，从而使用该顶点数组对象vao,且binder对象析构时会自动调用vao.release()
	m_vboPlane.create();  //在opengl服务器中创建该顶点buffer对象
	m_vboPlane.bind();  //将与此对象关联的缓冲区绑定到当前OpenGL上下文
	m_vboPlane.allocate(m_verticesPlane, sizeof(m_verticesPlane));  //载入模型顶点及法线数据到显存
	m_eboPlane.create();  //在opengl服务器中创建该索引buffer对象
	m_eboPlane.bind();  //将与此对象关联的缓冲器绑定到当前opengl上下文
	m_eboPlane.allocate(indices, sizeof(indices));  //载入模型索引数据到显存

	//通知vao如何解析vbo数据
	int attr = -1;
	attr = m_shaderObject.attributeLocation("aPos");
	m_shaderObject.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);  //为vao设置 如何解析顶点缓冲对象vbo内的顶点坐标数据
	m_shaderObject.enableAttributeArray(attr);  //使位置attr处的数据 即位置数组，可被着色器使用
	attr = m_shaderObject.attributeLocation("aNormal");
	m_shaderObject.setAttributeBuffer(attr, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);  //为vao设置 如何解析顶点缓冲对象vbo内的法向量数据
	m_shaderObject.enableAttributeArray(attr);  //使位置attr处的数据 即法向量数组，可被着色器使用

	//解除与opengl上下文的绑定(数据仍然在显存中)
	vaoBind.release();
	m_vboPlane.release();
	m_eboPlane.release();
	this->doneCurrent();
}

void OpenGLWidget3D::LoadObjToGPU(ObjDataPtr objPtr, QOpenGLVertexArrayObject & vao, QOpenGLBuffer & vbo, QOpenGLBuffer & ebo)
{
	this->makeCurrent();
	QOpenGLVertexArrayObject::Binder vaoBind(&vao);  //借助binder绑定，从而使用该顶点数组对象vao,且binder对象析构时会自动调用vao.release()

	vbo.create();  //在opengl服务器中创建该顶点buffer对象
	vbo.bind();  //将与此对象关联的缓冲区绑定到当前OpenGL上下文
	vbo.allocate(objPtr->GetVboArray(), objPtr->GetVertexNum() * sizeof(float) * 6);  //载入模型顶点及法线数据到显存

	ebo.create();  //在opengl服务器中创建该索引buffer对象
	ebo.bind();  //将与此对象关联的缓冲器绑定到当前opengl上下文
	ebo.allocate(objPtr->GetEboArray(), objPtr->GetTriangleNum() * sizeof(unsigned int) * 3);  //载入模型索引数据到显存

	int attr = -1;
	//通知vao如何解析vbo数据
	attr = m_shaderObject.attributeLocation("aPos");
	m_shaderObject.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);  //为vao设置 如何解析顶点缓冲对象vbo内的顶点坐标数据
	m_shaderObject.enableAttributeArray(attr);  //使位置attr处的数据 即位置数组，可被着色器使用
	attr = m_shaderObject.attributeLocation("aNormal");
	m_shaderObject.setAttributeBuffer(attr, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);  //为vao设置 如何解析顶点缓冲对象vbo内的法向量数据
	m_shaderObject.enableAttributeArray(attr);  //使位置attr处的数据 即法向量数组，可被着色器使用

	//解除与opengl上下文的绑定(数据仍然在显存中)
	vaoBind.release();
	vbo.release();
	ebo.release();

	this->doneCurrent();
}
