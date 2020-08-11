// SceneGroup.h: interface for the SceneGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENEGROUP_H__E75E9D4E_626C_4740_A149_6EA7C1F0598F__INCLUDED_)
#define AFX_SCENEGROUP_H__E75E9D4E_626C_4740_A149_6EA7C1F0598F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SceneNode.h"
#include "TextNode.h"

//// GroupNode 中的链表数据结构
class GroupLink
{
public:
	GroupLink(void);
	~GroupLink(void);
	void SetSceneNode(SceneNode * node);

public:
	GroupLink *m_pNext;
	SceneNode *m_pSceneNode;
};

class AFX_EXT_CLASS SceneGroup : public SceneNode  
{
public:
	SceneGroup();
	virtual ~SceneGroup();

	//获得当前节点类型
    virtual int getType(){return GROUP_NODE;}

	// 搜索名为 name 的节点（整个 Group）
	virtual SceneNode*	Search(char *name);	

	//绘制
	virtual void Draw(void);
	//绘制包围盒
	//virtual void DrawBoundBox(void);

	// 为 Mouse Select 操作绘制当前节点
	virtual void DrawForMouseSelect(void);

	void	AddChild(SceneNode *child);			// 加入子节点
    void	setChild(int i, SceneNode *child);	// 设置第 i 号子节点

    int		getCount();					// 获取子节点总数
    int		getChildIndex(char *name);	// 获取名为 name 子节点的索引号
	SceneNode*	getChild(int i);		// 获取第 i 号子节点
	SceneNode*	getChild(char *name);	// 获取名为 name 子节点

    void	DeleteChild(int i);			// 删除第 i 号子节点
    void	DeleteChild(char *name);	// 删除名为 name 子节点

protected:
	//  用于释放所包容的对象
	virtual void FinalRelease(void);

public:
	GroupLink * m_pListHead;
};


#endif // !defined(AFX_SCENEGROUP_H__E75E9D4E_626C_4740_A149_6EA7C1F0598F__INCLUDED_)
