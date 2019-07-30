/*
The MIT License (MIT)

Copyright (c) 2017- cms(hsc)

Author: ���û������/kisslovecsh@foxmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//ѭ���ڴ���� �ڴ���밴˳��������ͷ�
//������������ý������������ʹ�� 
//��������Ƶ��֡(�������Ϣ֡)��metaData�����������֡ 
//����ѭ���ڴ��ڿ���
#ifndef __CMS_CYCLE_MEM_H__
#define __CMS_CYCLE_MEM_H__

#include <mem/cms_mem_common.h>
#include <mem/cms_mf_mem.h>
#include <common/cms_type.h>
#include <core/cms_lock.h>

//�ڴ�����ڵ㣬�ڵ�ᶯ̬�������ڴ�������� �Զ�����������ĩβ
//�ڴ治������ ���Զ�����
typedef struct _CmsCycleNode
{
	uint32 begin;  //�ڴ������ʼλ��
	uint32 end;    //�ڴ�������λ��
	uint32 size;   //buf size
	byte  *buf;
	struct _CmsCycleNode *prev;
	struct _CmsCycleNode *next;
}CmsCycleNode;

//���ص��ڴ����ڴ�ͷ�ṹ
typedef struct _CmsCycleChunk
{
	uint32 g;  //����glic�ڴ�
	uint32 b;    //struct _CmsBufNode buf ��ʼλ��
	uint32 e;    //struct _CmsBufNode buf ����λ��	
	uint32 size; //��С e-b >= size ���Ƕ������	
	struct _CmsCycleNode *node;
}CmsCycleChunk;

typedef struct _CmsCycleMem
{
	uint32  nodeSize;
	uint32  totalMemSize;
	CLock  *lock;
	struct _CmsCycleNode *releaseNode;		//�ͷŵ��ڴ��Ȼ���ڸýڵ�
	struct _CmsCycleNode *curent;	//��ǰ�����node ��Ȼָ�������һ��
}CmsCycleMem;


CmsCycleMem *mallocCycleMem(uint32 nodeSize, const char *file, int line);
void freeCycleMem(CmsCycleMem *m);
void *mallocCycleBuf(CmsCycleMem *m, uint32 size, int g, const char *file, int line);
void freeCycleBuf(CmsCycleMem *m, void *p);
//���ڴ�ֻ��Ӧ����˳���ڴ��У���������ڴ��
//��Ҫ�����ͷ��ڴ棬����øú��������ܵ���Free�ͷţ���������
void umallocCycleBuf(CmsCycleMem *m, void *p);

#define xmallocCycleMem(nodeSize) mallocCycleMem((nodeSize), __FILE__, __LINE__)
#define xfreeCycleMem(m) freeCycleMem((m))
#define xmallocCycleBuf(m, size, g) mallocCycleBuf((m), (size), (g), __FILE__, __LINE__)
#define xfreeCycleBuf(m, p) freeCycleBuf((m), (p))
#define xumallocCycleBuf(m, p) freeCycleBuf((m), (p))

#endif


