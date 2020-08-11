#pragma	   once
/*
 定点数据文件格式 
 
 文件头{头大小，块格式，保留数据}

 块头1{块ID=1,块的顶点个数,块的三角面个数,顶点是否有发向量}
 块数据:顶点数据,然后是三角面数据,如果有发向量接下来是发向量数据

 块头2{块ID=2,块的顶点个数,块的三角面个数,顶点是否有发向量}
 块数据:顶点数据,然后是三角面数据,如果有发向量接下来是发向量数据

 ......

*/

//定点数据头文件
typedef struct _VECTORFILEHEAD{
	int Version;
	int ChunkNum;
	int Resverd1;
	int Resverd2;
	int HeadSize;
}VECTORFILEHEAD;


//大块数据文件
typedef struct _CHUNKHEAD{
	long ChunkID;
	long VectorNum;
	long FaceNum;
	long Normal;
}CHUNKHEAD;

