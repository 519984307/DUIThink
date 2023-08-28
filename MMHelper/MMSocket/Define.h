#ifndef __MM_DEFINESOCKET_H__
#define __MM_DEFINESOCKET_H__

#pragma once

//////////////////////////////////////////////////////////////////////////
#define SOCKET_TCP_BUFFER				(16384)								//���绺��

#define MaskClient_PC					(0x01)								//PC��
#define MaskClient_Mobile				(0x02)								//�ֻ���
#define MaskClient_All					(0xff)								//���ж�

#define MDM_CmdInner_Start				(1)
#define MDM_CmdInner_Finish				(10)

//////////////////////////////////////////////////////////////////////////////////
//shut reason
enum enSocketShutReason
{
	SocketShut_Inside,					//�ڲ�ԭ��
	SocketShut_Normal,					//�����ر�
	SocketShut_Remote,					//Զ�̹ر�
	SocketShut_TimeOut,					//���糬ʱ
	SocketShut_Exception,				//�쳣�ر�
};

//socket status
enum enSocketStatus
{
	SocketStatus_Idle,					 //����״̬
	SocketStatus_Wait,					 //�ȴ�״̬
	SocketStatus_Connect				 //����״̬
};

//connect code
enum enSocketConnectCode
{
	SocketConnect_Success,				//���ӳɹ�
	SocketConnect_Fail,					//����ʧ��
	SocketConnect_Exception,			//�����쳣
	SocketConnect_ProxyFail,			//����ʧ��
	SocketConnect_ProxyUserInvalid,		//�û���Ч									
};

//////////////////////////////////////////////////////////////////////////
//pack head
struct tagTCPHead
{
	WORD								wPacketSize = 0;
	WORD								wMainCmd = 0;
	WORD								wSubCmd = 0;
	DWORD								dwReserve = 0;
};

//////////////////////////////////////////////////////////////////////////

#endif