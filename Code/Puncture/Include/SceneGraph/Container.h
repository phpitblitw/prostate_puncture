/*_____________________________________________________
 |
 |	Intra3D Layer1 :  Container 基类
 |                                          
 |  文件：Container.h
 |
 |  功能：管理对象的引用、删除
 |
 |  开发：林锐 ，1999/05/17
 |
 |	源程序测试：进行了应用测试
 |_____________________________________________________*/

/*_____________________________________________________
 |
 |  2003－10－30 唐慧 修改
 |_____________________________________________________*/



#if !defined (CONTAINER_H)
#define       CONTAINER_H

/*________________________________________________________________

					对象引用计数的内存管理技术

	正确处理计数器是万里长征的第一步，请参考 Microsoft COM 技术
  ________________________________________________________________*/

class AFX_EXT_CLASS Container
{
public:
	// 在创建对象时，计数器为 0
	Container(void){ m_cRefrenceCount=0; }
	virtual ~Container(){}; //析构函数设置为虚函数，在delete this 的时候才会调用继承类的析构函数，唐慧

	// 被引用时，增加计数器
    void AddRef(void){ m_cRefrenceCount++; }

	// 如果计数器为0，则删除自身
    void Release(void)
	{ 
		m_cRefrenceCount--; 
		if(m_cRefrenceCount <= 0) 
		{
			FinalRelease();	// 用于释放所包容的对象
			delete this;
		}
	}

protected:

	// 用于释放所包容的对象
	virtual void FinalRelease(void);

protected:
	int	m_cRefrenceCount;	// 计数器
};


#endif

/*___________________________________________________________________________

					Why - Container - Class

The Container class is the base class for all objects in a scene; 
where an object is an entity that you can place in the scene graph. 
A Container provides reference counting for all its children.

Many kinds of data objects in Intra3D can be placed in a hierarchical 
scene graph. Using instancing, an object can be referenced multiple times. 
Scene graphs can become quite complex, which can cause problems if you're 
not careful. Deleting objects can be a particularly dangerous operation, 
for example, if you delete an object that another object still references.

Within each Container is a counter that keeps track of the number of objects 
referencing a particular instance. Reference counting provides a bookkeeping 
mechanism that makes object deletion safe: an object should never be deleted 
if its reference count is greater than zero. In general, you should only 
unreference an object in case it is referenced by another object.

It is just as important, however, not to unreference an object that has not 
been referenced. 

Each Container is created with a reference count of 1. 

When object A is attached to object B, the reference count of A is incremented. 
Additionally, if A replaces a previously referenced object C, the reference 
count of C is decremented. 

See Also SGI Cosmo3D. Microsoft COM
							Understand ?
_____________________________________________________________________________*/

