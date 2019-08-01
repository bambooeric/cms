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
#ifndef __CMS_TS_COMMON_H__
#define __CMS_TS_COMMON_H__
#include <common/cms_type.h>
#include <mem/cms_mf_mem.h>

#define PATpid	0
#define PMTpid	0x1000
#define Apid	0x101
#define Vpid	0x100
#define PCRpid	0x100

#define CMS_TS_TIMEOUT_MILSECOND 0.1

//FLVͷ������Ϣ
typedef struct _SHead {
	byte	version;	//
	byte	streamInfo; //����Ϣ 4-a 1-v 5-a/v
	int		lenght;		//ͷ����
}SHead;
//FLV Tagͷ������Ϣ
typedef struct _STagHead {
	byte	tagType;      //tag����
	int		dataSize;     //tag����
	uint32	timeStamp;	  //ʱ���
	int		streamId;     //��ID
	int		deviation;    //ʱ���ƫ����
}STagHead;
//FLV ��ƵTag��Ϣ
typedef struct _SAudioInfo
{
	byte	codeType;	//��������
	byte	rate;		//������
	byte	precision;	//����
	byte	audioType;	//��Ƶ����
}SAudioInfo;
//FLV ��ƵTag��Ϣ
typedef struct _SVideoInfo
{
	byte	framType; //֡����
	byte	codeId;   //��������
}SVideoInfo;
//FLV ScriptTag��Ϣ
typedef struct _SDataInfo
{
	int duration;		//ʱ��
	int width;			//��Ƶ���
	int height;			//��Ƶ�߶�
	int videodatarate;	//��Ƶ����
	int framerate;		//��Ƶ֡��
	int videocodecid;	//��Ƶ���뷽ʽ
	int audiosamplerate;//��Ƶ������
	int audiosamplesize;//��Ƶ��������
	int stereo;			//�Ƿ�Ϊ������
	int audiocodecid;	//��Ƶ���뷽ʽ
	int filesize;		//�ļ���С
}SDataInfo;
//AAC������Ϣ
typedef struct _SAudioSpecificConfig
{
	byte ObjectType;		//5
	byte SamplerateIndex;	//4
	byte Channels;			//4
	byte FramLengthFlag;	//1
	byte DependOnCCoder;	//1
	byte ExtensionFlag;		//1
}SAudioSpecificConfig;
//����Tag��Ϣ
typedef struct _STagInfo
{
	STagHead	head;
	byte		flag; //v:video a:audio
	SAudioInfo	audio;
	SVideoInfo	video;
}STagInfo;

//hls mgr msg
typedef struct _HlsMissionMsg
{	
	int act;
	int tsDuration;
	int tsNum;
	int tsSaveNum;
	uint32 hasIdx;
	HASH hash;
	std::string url;

	OperatorNewDelete
}HlsMissionMsg;

#define CMS_HLS_CREATE 0x01
#define CMS_HLS_DELETE 0x02

#endif
