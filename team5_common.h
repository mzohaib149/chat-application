#ifndef COMMON_H_
#define COMMON_H_

#define VERSION3TYPE2 0x382 //for JOIN
#define VERSION3TYPE3 0x383 //for FWD
#define VERSION3TYPE4 0x384 //for SEND
#define VERSION3TYPE5 0x385 //for NAK
#define VERSION3TYPE6 0x386 //for OFFLINE
#define VERSION3TYPE7 0x387 //for ACK
#define VERSION3TYPE8 0x388 //for ONLINE
#define VERSION3TYPE9 0x389 //for IDLE

#define MASK 0x007F

#define IDLE 0
#define ONLINE 1
#define OFFLINE 2

struct SBCP_Header
{
	unsigned short int Vrsn_and_Type;
	unsigned short int Length;

};

struct SBCP_Attribute_Header
{
	unsigned short int Type;
	unsigned short int Length;
};

/***************************************************************/
/*		Messages sent by client			       */
/***************************************************************/
struct SBCP_JOIN_Message
{
	struct SBCP_Header hdr;
	struct SBCP_Attribute_Header attr_hdr;
	char Username[16];

};

struct SBCP_SEND_Message
{
	struct SBCP_Header hdr;
	struct SBCP_Attribute_Header attr_hdr;
	char Message[512];

};


struct SBCP_IDLE_Message_Client
{
	struct SBCP_Header hdr;
};

/***************************************************************/
/*		Messages sent by server			       */
/***************************************************************/

struct SBCP_FWD_Message
{
	struct SBCP_Header hdr;
	struct SBCP_Attribute_Header attr_hdr1;
	char Username[16];
	struct SBCP_Attribute_Header attr_hdr2;
	char Message[512];
};

struct SBCP_ACK_Message
{
	struct SBCP_Header hdr;
	struct SBCP_Attribute_Header attr_hdr1;
	unsigned short int Client_Count;
	struct SBCP_Attribute_Header attr_hdr2;
	char Username[800];//list of user names appended

};

struct SBCP_NAK_Message
{
	struct SBCP_Header hdr;
	struct SBCP_Attribute_Header attr_hdr;
	char Reason[32];

};

struct SBCP_ONLINE_Message
{
	struct SBCP_Header hdr;
	struct SBCP_Attribute_Header attr_hdr;
	char Username[16];

};

struct SBCP_OFFLINE_Message
{
	struct SBCP_Header hdr;
	struct SBCP_Attribute_Header attr_hdr;
	char Username[16];

};

struct SBCP_IDLE_Message_Server
{
	struct SBCP_Header hdr;
	struct SBCP_Attribute_Header attr_hdr;
	char Username[16];

};
#endif
