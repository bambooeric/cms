#ifndef __cms_http_request_h__
#define __cms_http_request_h__
#include <interface/cms_interf_conn.h>
#include <interface/cms_read_write.h>
#include <common/cms_type.h>
#include <protocol/cms_http.h>
#include <mem/cms_mf_mem.h>
#include <string>

class CHttpClient;

typedef int(*HttpCallBackFn)(const char *  data, int dataLen, CHttpClient *client, void *custom);

class CHttpClient : public Conn
{
public:
	CHttpClient(CReaderWriter *rw, std::string strUrl, std::string refer);
	~CHttpClient();

	Request		*httpRequest();
	Response	*httpResponse();

	//����
	int activateEV(void *base, struct ev_loop *evLoop);
	int doit();
	int handleEv(bool isRead);
	int handleTimeout();
	int stop(std::string reason);
	std::string getUrl();
	std::string getPushUrl();
	std::string &getRemoteIP();
	void down8upBytes();
	void reset();
	CReaderWriter *rwConn();
	//http�ײ�ص�
	int  doDecode();
	int  doReadData(); //http client
	int  doTransmission();
	int  sendBefore(const char *data, int len);
	bool isFinish();
	bool isWebsocket();

	// ����http���󣬷�������ǰ��Ҫ���� Request ��Ӧ������
	// ������
	// cb		�ص�����
	// custom	�Զ������� �ص�ʱ�����ظ�������
	bool		start(std::string postData, HttpCallBackFn cb, void *custom);
	void        saveData(const char* data, int len);
	std::string &getData();

	OperatorNewDelete
private:
	bool			misRequet;
	bool			misDecodeHeader;
	bool			misRedirect;
	CReaderWriter	*mrw;
	std::string		mredirectUrl;

	CHttp			*mhttp;
	CBufferReader	*mrdBuff;
	CBufferWriter	*mwrBuff;
	std::string		mpostData;
	std::string     msaveData;
	std::string     mremoteAddr;
	HttpCallBackFn mcb;
	void *mcustomData;

	bool  misStop;

	//�ٶ�ͳ��
	int32			mxSecdownBytes;
	int32			mxSecUpBytes;
	int32			mxSecTick;

	unsigned long  mspeedTick;
	int64		   mtimeoutTick;
};
#endif
