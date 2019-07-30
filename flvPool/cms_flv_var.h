#ifndef __CMS_FLV_VAR_H__
#define __CMS_FLV_VAR_H__
#include <common/cms_type.h>
#include <core/cms_lock.h>
#include <mem/cms_mf_mem.h>
#ifdef __CMS_CYCLE_MEM__
#include <mem/cms_cycle_mem.h>
#endif
#include <string>
#include <vector>
#include <map>

enum FlvPoolCode
{
	FlvPoolCodeError = -1,
	FlvPoolCodeOK,
	FlvPoolCodeNoData,
	FlvPoolCodeRestart,
	FlvPoolCodeTaskNotExist
};

enum FlvPoolDataType
{
	DATA_TYPE_NONE = -0x01,
	DATA_TYPE_AUDIO = 0x00,
	DATA_TYPE_VIDEO = 0x01,
	DATA_TYPE_VIDEO_AUDIO = 0x02,
	DATA_TYPE_FIRST_AUDIO = 0x03,
	DATA_TYPE_FIRST_VIDEO = 0x04,
	DATA_TYPE_FIRST_VIDEO_AUDIO = 0x05,
	DATA_TYPE_DATA_SLICE = 0X06
};

#define FLV_TAG_AUDIO		0x08
#define FLV_TAG_VIDEO		0x09
#define FLV_TAG_SCRIPT		0x12

#define OFFSET_FIRST_VIDEO_FRAME 0x05

#define DropVideoMinSeconds 1500

struct StreamSlice;

typedef struct Slice //�ýṹ�岻�ܳ����й��캯���ı���!!!!!!!!!!
{
	uint32			midxFixMem;			//�̶��ڴ����ڶ���������
	int				mionly;				//0 ��ʾû��ʹ�ã�����0��ʾ���ڱ�ʹ�ô���	
	bool            misHaveMediaInfo;   //�Ƿ����޸Ĺ�����Ϣ
	bool			misPushTask;
	bool			misNoTimeout;
	bool			misMetaData;		//�������Ƿ���metaData
	bool			misRemove;			//ɾ�������־
	bool			misRealTimeStream;	//�Ƿ�����µ����ݷ���
	bool			misKeyFrame;
	bool			misResetStreamTimestamp;
	bool			misH264;
	bool			misH265;
	FlvPoolDataType miDataType;			//��������
	int64			mllP2PIndex;		//p2p������
	int64			mllIndex;           //��slice��Ӧ�����к�
	int64			mllOffset;			//ƫ��λ�ã�������
	int64			mllStartTime;		//����ʼʱ��
	int64			mllCacheTT;			//����ʱ�� ����
	uint32			muiTimestamp;	    //��slice���ݶ�Ӧrtmp��ʱ���
	int				miNotPlayTimeout;	//��ʱʱ�䣬����	
	int				miMediaRate;
	int				miVideoRate;		//��Ƶ����
	int				miAudioRate;		//��Ƶ����
	int				miVideoFrameRate;	//��Ƶ֡��
	int				miAudioFrameRate;	//��Ƶ֡��	
	int				miAudioChannelID;	//��������ƵID
	int				miPlayStreamTimeout;		//���û���ų�ʱʱ��	
	int				miFirstPlaySkipMilSecond;	//�ײ���֡ʱ��
	int				miAutoBitRateMode;			//��̬��֡ģʽ(0/1/2)
	int				miAutoBitRateFactor;		//��̬������ϵ��
	int				miAutoFrameFactor;			//��̬��֡ϵ��
	int				miBufferAbsolutely;			//buffer�ٷֱ�
	int				miLiveStreamTimeout;
	int				miNoHashTimeout;
	int             miDataLen;
	char			*mData;				//����	
	char			*mpMajorHash;		//����ת�����񣬸�hash��ʾԴ��hash
	char			*mpHash;			//��ǰ����hash
	char			*mpUrl;
	char			*mpVideoType;		//��Ƶ����
	char			*mpAudioType;		//��Ƶ����
	char			*mpReferUrl;
	char			*mpRemoteIP;
	char			*mpHost;
#ifdef __CMS_CYCLE_MEM__
	CmsCycleMem     *mcycMem;
	struct StreamSlice * mss;
#endif
}Slice;

struct TTandKK
{
	int64			mllIndex;		//��ͨ��Ƶ����
	int64			mllKeyIndex;
	uint32			muiTimestamp;	//ʱ���
};

struct StreamSlice
{
#ifdef __CMS_CYCLE_MEM__
	int							mionly;				//0 ��ʾû��ʹ�ã�����0��ʾ���ڱ�ʹ�ô���
													//ʹ��ѭ���ڴ�ʱ���ᱻSlice����
#endif
	//idΨһ�� ���ڷ�������ʱ �ж������Ƿ�����
	CRWlock						mLock;
	std::vector<TTandKK *>		msliceTTKK;	//��ʱ�������
	std::string					mstrUrl;
	std::string					mstrReferUrl;
	std::string					mstrVideoType;		//��Ƶ����
	std::string					mstrAudioType;		//��Ƶ����
	std::string					mstrRemoteIP;
	std::string					mstrHost;
	std::vector<Slice *>		mavSlice;
	std::vector<int64>			mavSliceIdx;
	std::vector<int64>			mvKeyFrameIdx;		//�ؼ�֡λ��
	std::vector<int64>			mvP2PKeyFrameIdx;	//�ؼ�֡λ��
	std::map<int64, int64>		mp2pIdx2PacketIdx;
	HASH						mhMajorHash;

	uint64						muid;
	//������ʱ����
	int64						maxRelativeDuration;
	int64						minRelativeDuration;	
	int64						mllNearKeyFrameIdx;	
	int64						mllAccessTime;		//��¼ʱ�������һ��ʱ��û���û����ʣ�ɾ��
	int64						mllCreateTime;		//���񴴽�ʱ��
	uint32						muiTheLastVideoTimestamp;	
	int							miNotPlayTimeout;	//��ʱʱ�䣬����	
	int							miVideoFrameCount;
	int							miAudioFrameCount;
	int							miMediaRate;
	int							miVideoRate;		//��Ƶ����
	int							miAudioRate;		//��Ƶ����
	int							miVideoFrameRate;	//��Ƶ֡��
	int							miAudioFrameRate;	//��Ƶ֡��
	bool						misPushTask;		//�Ƿ�����������
	bool						mnoTimeout;			//�����Ƿ񲻳�ʱ
	bool						misH264;
	bool						misH265;
	
	uint32						muiKeyFrameDistance;
	uint32						muiLastKeyFrameDistance;
	int64						mllLastSliceIdx;
	int64						mllFirstVideoIdx;
	int64						mllFirstAudioIdx;
	Slice						*mfirstVideoSlice;	
	Slice						*mfirstAudioSlice;
	Slice						*mmetaDataSlice;
	
	int64						mllVideoAbsoluteTimestamp;	//���ڼ��㻺������
	int64						mllAudioAbsoluteTimestamp;	//���ڼ��㻺������	
	int64						mllMetaDataIdx;
	int64						mllMetaDataP2PIdx;

	bool						misNoTimeout;
	bool						misHaveMetaData;
	bool						misRealTimeStream;			//�Ƿ�����µ����ݷ���
	bool						misResetStreamTimestamp;
	int64						mllLastMemSize;
	int64						mllMemSize;
	int64						mllMemSizeTick;	
	int64						mllCacheTT;					//����ʱ�� ����
	int							miPlayStreamTimeout;		//���û���ų�ʱʱ��	
	int							miFirstPlaySkipMilSecond;	//�ײ���֡ʱ��
	int							miAutoBitRateMode;			//��̬��֡ģʽ(0/1/2)
	int							miAutoBitRateFactor;		//��̬������ϵ��
	int							miAutoFrameFactor;			//��̬��֡ϵ��
	int							miBufferAbsolutely;			//buffer�ٷֱ�
	

	//��Ե�Ż��õ��ı������Ͽ�ʱ��״̬
	bool						misNeedJustTimestamp;
	bool						misRemove;
	bool						misHasBeenRemove;
	int64						mllRemoveTimestamp;
	uint32						muiLastVideoTimestamp;
	uint32						muiLastAudioTimestamp;
	uint32						muiLast2VideoTimestamp;
	uint32						muiLast2AudioTimestamp;

	int64						mllUniqueID;

	//���Ʋ���Ч
	int							miLiveStreamTimeout;
	int							miNoHashTimeout;

	OperatorNewDelete
};
#endif
