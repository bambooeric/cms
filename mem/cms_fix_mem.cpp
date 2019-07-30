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

#include <mem/cms_fix_mem.h>
#include <mem/cms_mf_mem.h>
#include <mem/cms_mem_common.h>
#include <assert.h>

#define CMS_FIX_MEM_EMPTY_NUM 0

CmsFixChunk *mallocFixChunk(int nodeSize, int count, const char * file, int line)
{
	int nodeHeadSize = sizeof(CmsDataHeader);
	int totalSize = sizeof(CmsFixChunk)
		+ sizeof(CmsFixNode)*count
#ifdef CMS_NEED_ALIGNMENT
		+ sizeof(CMS_VOID_PTR)*(count + 1)    //�����϶����ֽ����������õ�Ҳ�����ò����������ں����Խṹ�� CmsFixChunk CmsFixNode ���޸ģ�
#endif		
		+ (nodeHeadSize + nodeSize) * count   //data chunkͷ ָ���Ӧ�� CmsFixNode
#ifdef CMS_NEED_ALIGNMENT
		+ sizeof(CMS_VOID_PTR)*(count + 1);   //�����϶����ֽ����������õ�Ҳ�����ò����������ں����Խṹ�� CmsFixChunk CmsFixNode ���޸ģ�
#else
		;
#endif	

#ifdef _CMS_LEAK_CHECK_
	byte *ptr = (byte *)cms_xmalloc(totalSize, file, line);
#else
	byte *ptr = (byte *)xmalloc(totalSize);
#endif	
	CmsFixChunk *fc = (CmsFixChunk *)ptr;
	fc->available = count;
	fc->total = count;
	fc->curent = NULL;
	fc->next = NULL;
	fc->prev = NULL;

	byte *fnPtr = ptr + sizeof(CmsFixChunk);
	byte *preData = ptr + totalSize;

	byte *data = NULL;
	CmsFixNode *node = NULL;
	CmsDataHeader * cdh = NULL;
	byte *cdhPtr = NULL;
	//data chunk �����ʼ���� ��ο�ͷ�ļ��ṹͼ
	for (int i = 0; i < count; i++)
	{
		cdhPtr = preData - (nodeHeadSize + nodeSize);
		fnPtr = cms_align_ptr(fnPtr, sizeof(CMS_VOID_PTR)); // CmsFixNode *
		data = cms_align_ptr(cdhPtr, sizeof(CMS_VOID_PTR));
		if (data > cdhPtr)
		{
			data -= sizeof(CMS_VOID_PTR);
		}
		assert(fnPtr + sizeof(CmsFixNode) <= data);
		if (fc->curent == NULL)
		{
			node = (CmsFixNode *)fnPtr;
			cdh = (CmsDataHeader *)data;
			cdh->fc = fc;
			cdh->node = node; //data header
			node->begin = data + nodeHeadSize;
			node->end = node->begin + nodeSize;
			node->prev = NULL;
			node->next = NULL;
			fc->curent = node;
			fc->end = node->end;
		}
		else
		{
			node = (CmsFixNode *)fnPtr;
			cdh = (CmsDataHeader *)data;
			cdh->fc = fc;
			cdh->node = node; //data header
			node->begin = data + nodeHeadSize;
			node->end = node->begin + nodeSize;
			node->prev = NULL;
			node->next = fc->curent;
			fc->curent->prev = node;
			fc->curent = node;
		}
		if (i == count - 1)
		{
			fc->begin = data;
		}
		fnPtr += sizeof(CmsFixNode);
		preData = data;
	}
	return fc;
}

void freeFixChunk(CmsFixChunk *fc)
{
	xfree(fc);
}

int isFixChunkFull(CmsFixChunk *fc)
{
	return fc->available == 0 ? 1 : 0;
}

int isFixChunkEmpty(CmsFixChunk *fc)
{
	return fc->available == fc->total ? 1 : 0;
}


void *mallocFromFixChunk(CmsFixChunk *fc)
{
	void *ptr = fc->curent->begin;
	fc->available--;
	//ע������������ curent ������Ϊ�� free2FixChunkʱ��Ҫ�õ�
	if (fc->available)
	{
		fc->curent = fc->curent->next;
	}
	return ptr;
}

int free2FixChunk(CmsFixChunk *fc, CmsFixNode *node)
{
	if (node->next)
	{
		node->next->prev = node->prev;
	}
	if (node->prev)
	{
		node->prev->next = node->next;
	}
	node->next = NULL;
	node->prev = NULL;
	if (fc->available == 0)
	{
		//˵����chunk�Ѿ���������
		assert(fc->curent->next == NULL);
		if (node == fc->curent)
		{
			//���һ���ڴ������ͷ�
			//do nothing
		}
		else
		{
			fc->curent->next = node;
			node->prev = fc->curent;
			fc->curent = node;
		}
	}
	else
	{
		if (fc->curent->next)
		{
			fc->curent->next->prev = node;
			node->next = fc->curent->next;
		}
		node->prev = fc->curent;
		fc->curent->next = node;
	}
	fc->available++;
	assert(fc->available <= fc->total);
	return isFixChunkEmpty(fc); //��ȫ����
}


CmsFixMem *mallocFixMem(int nodeSize, int count, const char * file, int line)
{
#ifdef _CMS_LEAK_CHECK_
	CmsFixMem *m = (CmsFixMem *)cms_xmalloc(sizeof(CmsFixMem), file, line);
#else
	CmsFixMem *m = (CmsFixMem *)xmalloc(sizeof(CmsFixMem));
#endif	

	// 	CmsFixChunk *fc = mallocFixChunk(nodeSize, count, file, line);
	// 	m->nodeSize = nodeSize;
	// 	m->nodeCount = count;
	// 	m->curent = fc;
	// 	m->emptyCount = 1;

	m->nodeSize = nodeSize;
	m->nodeCount = count;
	m->curent = NULL;
	m->emptyCount = 0;

	return m;
}

void freeFixMem(CmsFixMem *m)
{
	CmsFixChunk *curent = m->curent;
	CmsFixChunk *node = NULL;
	if (curent)
	{
		node = curent->next;
	}
	CmsFixChunk *next = NULL;
	for (; node != NULL;)
	{
		//˳��ɾ��
		next = node->next;
		xfree(node);
		node = next;
	}
	if (curent)
	{
		node = curent->prev;
	}
	CmsFixChunk *prev = NULL;
	for (; node != NULL;)
	{
		//����ɾ��
		prev = node->prev;
		xfree(node);
		node = prev;
	}
	if (curent)
	{
		xfree(curent);
	}
	xfree(m);
}

void *mallocFix(CmsFixMem *m, const char * file, int line)
{
	int isNew = 0;
	CmsFixChunk *node = NULL;
	if (m->curent == NULL)
	{
		isNew = 1;
		CmsFixChunk *fc = mallocFixChunk(m->nodeSize, m->nodeCount, file, line);		
		m->curent = fc;
	}
	else if (m->curent->available == 0)
	{
		//��ǰ����
		node = m->curent->next;
		//ҪôΪ�� Ҫô���Է���
		assert(!node || node->available);
		if (node == NULL)
		{
			isNew = 1;
			CmsFixChunk *fc = mallocFixChunk(m->nodeSize, m->nodeCount, file, line);
			m->curent->next = fc;
			fc->prev = m->curent;
			m->curent = fc;
		}
		else
		{
			m->curent = node;
		}
	}
	else
	{
		//�ӵ�ǰ�ڵ����
	}
	if (!isNew)
	{
		//�����´�����chunk ���ж��Ƿ���ȫû�з����
		if (isFixChunkEmpty(m->curent))
		{
			assert(m->emptyCount);
			m->emptyCount--;
		}
	}
	return mallocFromFixChunk(m->curent);
}

void freeFix(CmsFixMem *m, void *p)
{
	CmsDataHeader *cdh = (CmsDataHeader *)((byte *)p - sizeof(CmsDataHeader));
	CmsFixChunk *fc = cdh->fc;
	int isLastFull = isFixChunkFull(fc);
	int isEmpty = free2FixChunk(fc, cdh->node);
	if (isEmpty)
	{
		if (CMS_FIX_MEM_EMPTY_NUM == 0 || (m->emptyCount >= CMS_FIX_MEM_EMPTY_NUM &&
			m->curent != fc))
		{
			if (m->curent == fc)
			{
				m->curent = m->curent->next;
			}
			if (fc->next)
			{
				fc->next->prev = fc->prev;
			}
			if (fc->prev)
			{
				fc->prev->next = fc->next;
			}
			freeFixChunk(fc);			
			return;
		}
		else
		{
			m->emptyCount++;
		}
	}
	if (isLastFull)
	{
		//�ӷ�����ɿɷ��� ����λ��
		//���� ����Ҫ���� ��Ϊ�ýڵ��Ȼ�� m->curent ֮��
		if (m->curent != fc)
		{
			if (fc->next)
			{
				fc->next->prev = fc->prev;
			}
			if (fc->prev)
			{
				fc->prev->next = fc->next;
			}
			fc->next = NULL;
			fc->prev = NULL;
			if (m->curent->next)
			{
				m->curent->next->prev = fc;
				fc->next = m->curent->next;
			}
			m->curent->next = fc;
			fc->prev = m->curent;
// 			if (isFixChunkFull(m->curent))
// 			{
// 				m->curent = fc;
// 			}
		}
	}
}



