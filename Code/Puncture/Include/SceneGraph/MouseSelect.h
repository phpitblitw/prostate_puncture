/*_____________________________________________________
 |
 |	Intra3D Layer3 :  Scene Graph 节点的鼠标拾取
 |                                          
 |  文件：MouseSelect.h
 |
 |  功能：用鼠标拾取可见节点(ShapeNode, LightNode)
 |
 |  开发：林锐 ，1999/06/03
 |
 |	源程序测试：进行了应用测试
 |_____________________________________________________*/

/*_____________________________________________________
 |  
 |	基于 OpenGL 的交互式三维图形软件开发工具
 |
 |	**   Intra3D 2.0 for Windows 9X/NT  **
 |
 |	          著作权人：林锐 
 |	
 |	浙江大学 CAD&CG 国家重点实验室 (310027)
 |_____________________________________________________*/


#if !defined (MOUSE_SELECT_H)
#define       MOUSE_SELECT_H

#include "Enum.h"

_declspec(dllexport) void MouseSelectBegin(void);

_declspec(dllexport) BOOL MouseSelectEnd(char *selectedName);

void LoadNameForMouseSelect(char *name);


#endif