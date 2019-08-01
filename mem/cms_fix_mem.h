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
//�̶��ڴ������ ���ڹ̶���С�ڴ�Ƶ����������
#ifndef __CMS_FIX_MEM_H__
#define __CMS_FIX_MEM_H__
#include <common/cms_type.h>
#include <mem/cms_mem_common.h>

/*
//by water 2019-07-26
														               struct _CmsFixChunk  ��ʼ��ʾ��ͼ
																		
																		   +-+-+-+-+-+-+-+-+-+-+-+-+-+ --------+
																		   |          begin          | --------+-------------------------------------+
																		   +-+-+-+-+-+-+-+-+-+-+-+-+-+         |                                     |
																		   |          end            | --------+---------------------------------+   |
																		   +-+-+-+-+-+-+-+-+-+-+-+-+-+         |                                 |   |
																	+----- |          curent         |         |---------> struct _CmsFixChunk   |   |
																	|	   +-+-+-+-+-+-+-+-+-+-+-+-+-+         |                                 |   |
																	|	   |          prev           |         |                                 |   |
																	|	   +-+-+-+-+-+-+-+-+-+-+-+-+-+         |                                 |   |
																	|	   |          next           |         |                                 |   |
														            +----> +-+-+-+-+-+-+-+-+-+-+-+-+-+ --------+  <------------------------------+---+---------------+
										+--------------------------------- |          begin          |         |                                 |   |               |
										|			                       +-+-+-+-+-+-+-+-+-+-+-+-+-+         |                                 |   |               |
										|	 +---------------------------- |          end            |         |                                 |   |               |
										|	 |                             +-+-+-+-+-+-+-+-+-+-+-+-+-+         |---------> struct _CmsFixNode    |   |               |
										|	 |                             |          prev           |         |                                 |   |               |
										|	 |                             +-+-+-+-+-+-+-+-+-+-+-+-+-+         |                                 |   |               |
										|	 |                             |          next           |         |                                 |   |               |
										|	 |                             +-+-+-+-+-+-+-+-+-+-+-+-+-+ --------+  <------------------------------+---+------+        |
										|	 |   +------------------------ |          begin          |         |                                 |   |      |        |
										|	 |   |                         +-+-+-+-+-+-+-+-+-+-+-+-+-+         |                                 |   |      |        |
										|	 |   |     +------------------ |          end            |         |                                 |   |      |        |
										|	 |   |     |                   +-+-+-+-+-+-+-+-+-+-+-+-+-+         |---------> struct _CmsFixNode    |   |      |        |
										|	 |   |     |                   |          prev           |         |                                 |   |      |        |
										|	 |   |     |                   +-+-+-+-+-+-+-+-+-+-+-+-+-+         |                                 |   |      |        |
										|	 |   |     |                   |          next           |         |                                 |   |      |        |
										|	 |   |     |                   +-+-+-+-+-+-+-+-+-+-+-+-+-+ --------+  <--+                           |   |      |        |
										|	 |   |     |    +------------- |          begin          |         |     |                           |   |      |        |
										|	 |   |     |    |              +-+-+-+-+-+-+-+-+-+-+-+-+-+     +---+-----+                           |   |      |        |
										|	 |   |     |    |      +------ |          end            |     |   |                                 |   |      |        |
										|	 |   |     |    |      |       +-+-+-+-+-+-+-+-+-+-+-+-+-+     |   |---------> struct _CmsFixNode    |   |      |        |
										|	 |   |     |    |      |       |          prev           |     |   |                                 |   |      |        |
										|	 |   |     |    |      |       +-+-+-+-+-+-+-+-+-+-+-+-+-+     |   |                                 |   |      |        |
										|	 |   |     |    |      |       |          next           |     |   |                                 |   |      |        |
										|	 |   |     |    |      |       +-+-+-+-+-+-+-+-+-+-+-+-+-+ ----+---+                                 |   |      |        |
										|	 |   |     |    |      |       |        .......          |     |                                     |   |      |        |
										|	 |   |     |    |      |       +-+-+-+-+-+-+-+-+-+-+-+-+-+ ----+---+ <-------------------------------+---+      |        |
										|	 |   |     |    |      |       |          fc             |     |   |                                 |          |        |
										|	 |   |     |    |      |       +-+-+-+-+-+-+-+-+-+-+-+-+-+     |   |           struct _CmsDataHeader |          |        |
										|	 |   |     |    |      |       |          node           | ----+   |---------> and                   |          |        |
										|	 |   |     |    +------------> +-+-+-+-+-+-+-+-+-+-+-+-+-+         |           data                  |          |        |
										|	 |   |     |           |       |          data           |         |                                 |          |        |
										|	 |   |     |           +-----> +-+-+-+-+-+-+-+-+-+-+-+-+-+ --------+                                 |          |        |
										|	 |   |     |                   |          fc             |                                           |          |        |
										|	 |   |     |                   +-+-+-+-+-+-+-+-+-+-+-+-+-+                                           |          |        |
										|	 |   |     |                   |          node           | ------------------------------------------+----------+        |
										|	 |   +-----+-----------------> +-+-+-+-+-+-+-+-+-+-+-+-+-+                                           |                   |
										|	 |         |                   |          data           |                                           |                   |
										|	 |         +-----------------> +-+-+-+-+-+-+-+-+-+-+-+-+-+                                           |                   |
										|	 |                             |          fc             |                                           |                   |
										|	 |                             +-+-+-+-+-+-+-+-+-+-+-+-+-+                                           |                   |
										|	 |                             |          node           | ------------------------------------------+-------------------+
										+--------------------------------> +-+-+-+-+-+-+-+-+-+-+-+-+-+                                           |
											 |                             |          data           |                                           |
										     +---------------------------> +-+-+-+-+-+-+-+-+-+-+-+-+-+ <-----------------------------------------+
																			
*/


typedef struct _CmsFixNode
{
	byte *begin;			//С���ڴ濪ʼλ��
	byte *end;				//С���ڴ����λ��	
	struct _CmsFixNode *prev;
	struct _CmsFixNode *next;
}CmsFixNode;

typedef struct _CmsFixChunk
{
	int  available;			//���ÿ���
	int  total;				//�ܿ���
	byte *begin;			//�����ڴ濪ʼλ��
	byte *end;				//�����ڴ����λ��
	CmsFixNode *curent;		//��ǰ����С��

	struct _CmsFixChunk *prev;
	struct _CmsFixChunk *next;
}CmsFixChunk;

typedef struct _CmsDataHeader
{
	CmsFixChunk *fc;
	CmsFixNode *node;	
}CmsDataHeader;

typedef struct _CmsFixMem
{
	int idx;				//��Ҫ��Ϊ�˲���
	int nodeSize;			//ÿ��������ڴ��С
	int nodeCount;			//ÿ��chunk�������ٸ�node
	int emptyCount;			//����chunk��
	int curNum;				//��ʱʹ��
	CmsFixChunk *curent;	//��ǰ���ÿ�
}CmsFixMem;

CmsFixMem *mallocFixMem(int nodeSize, int count, const char * file, int line);
void freeFixMem(CmsFixMem *m);
void *mallocFix(CmsFixMem *m, const char * file, int line);
void freeFix(CmsFixMem *m, void *p);

#define xmallocFixMem(nodeSize, count) mallocFixMem((nodeSize), (count), (__FILE__), (__LINE__))
#define xfreeFixMem(m) freeFixMem((m))
#define xmallocFix(m) mallocFix((m), (__FILE__), (__LINE__))
#define xfreeFix(m, p) freeFix((m),(p))

#endif //__CMS_FIX_MEM_H__


