/*****************************************************************************
 *
 * MODULE:              ZLL Commissoning Cluster
 *
 * COMPONENT:          zll_utility.h
 *
 * AUTHOR:
 *
 * DESCRIPTION:        Header for  Cluster
 *
 * $HeadURL: https://www.collabnet.nxp.com/svn/lprf_sware/Projects/SmartEnergy/Branches/UnifiedZCL_ZLL102_HA122_Maintenance/Clusters/LightLink/Include/zll_utility.h $
 *
 * $Revision: 81942 $
 *
 * $LastChangedBy: nxp29772 $
 *
 * $LastChangedDate: 2016-07-29 13:22:21 +0100 (Fri, 29 Jul 2016) $
 *
 * $Id: zll_utility.h 81942 2016-07-29 12:22:21Z nxp29772 $
 *
 *****************************************************************************
 *
 * This software is owned by NXP B.V. and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on NXP products  [NXP Microcontrollers such as JN5168, JN5164,
 * JN5161, JN5148, JN5142, JN5139]. 
 * You, and any third parties must reproduce the copyright and warranty notice 
 * and any other legend of ownership on each  copy or partial copy of the software.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE. 
 *
 * Copyright NXP B.V. 2012. All rights reserved
 *
 ****************************************************************************/

#ifndef ZLL_UTILITY_H
#define ZLL_UTILITY_H

#include <jendefs.h>
#include "dlist.h"
#include "zcl.h"
#include "zcl_customcommand.h"
#include "zcl_options.h"
#include "zll.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Cluster ID's */
#define ZLL_CLUSTER_ID_UTILITY    0x1000

#define ZLL_MAX_GROUP_RECORDS          21
#define ZLL_MAX_ENDPOINT_RECORDS       8





/****************************************************************************/
/*              Cluster - Optional Attributes                */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes.                               */
/****************************************************************************/

/* There are no attributes */

/* End of optional attributes */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
    uint16 u16NwkAddr;
    uint16 u16ProfileId;
    uint16 u16DeviceId;
    uint8 u8Endpoint;
    uint8 u8Version;
} tsZllEndpointInfoRecord;

typedef struct {
    uint8 u8NumRecords;
    uint8 u8CurrentLight;
    tsZllEndpointInfoRecord asEndpointRecords[NUM_ENDPOINT_RECORDS];
    uint8 au8PingCount[NUM_ENDPOINT_RECORDS];
} tsZllEndpointInfoTable;

typedef struct {
    uint16 u16GroupId;
    uint8 u8GroupType;
} tsZllGroupInfoRecord;

typedef struct {
    uint8 u8NumRecords;
    tsZllGroupInfoRecord asGroupRecords[NUM_GROUP_RECORDS];
} tsZllGroupInfoTable;

#if 1
/* Attribute ID's */
typedef enum 
{
    E_CLD_OH = 0x00,
    // there are no attributes
} teCLD_ZllUtility_ClusterID;

/* Identify Cluster */
typedef struct
{
    uint8 u8Huh;
} tsCLD_ZllUtility;


#endif

/* Command codes */
typedef enum 
{
    E_CLD_UTILITY_CMD_ENDPOINT_INFO = 0x40,          /* Mandatory (received by client) 0x40 */
    E_CLD_UTILITY_CMD_GET_GROUP_ID_REQ_RSP,          /* Mandatory (received by server) 0x41 */
    E_CLD_UTILITY_CMD_GET_ENDPOINT_LIST_REQ_RSP,     /* Mandatory (received by server) 0x42 */
} teCLD_ZllUtility_Command;

/* Endpoint Information command payload */
typedef struct
{
    uint64  u64IEEEAddr;
    uint16  u16NwkAddr;
    uint8   u8Endpoint;
    uint16  u16ProfileID;
    uint16  u16DeviceID;
    uint8   u8Version;
} tsCLD_ZllUtility_EndpointInformationCommandPayload;


/* Get Group identifiers request command payload */
typedef struct
{
    uint8   u8StartIndex;
} tsCLD_ZllUtility_GetGroupIdReqCommandPayload;

typedef struct {
    uint16  u16GroupID;
    uint8   u8GroupType;
} tsCLD_ZllGroupIdRecord;

/* Get Group identifiers Response command payload */
typedef struct
{
    uint8   u8Total;
    uint8   u8StartIndex;
    uint8   u8Count;
    tsCLD_ZllGroupIdRecord asGroupRecords[ZLL_MAX_GROUP_RECORDS];
} tsCLD_ZllUtility_GetGroupIdRspCommandPayload;

/* Get Endpoint List request command payload */
typedef struct
{
    uint8   u8StartIndex;
} tsCLD_ZllUtility_GetEndpointListReqCommandPayload;

typedef struct {
    uint16  u16NwkAddr;
    uint8   u8Endpoint;
    uint16  u16ProfileID;
    uint16  u16DeviceID;
    uint8   u8Version;
} tsCLD_ZllEndpointlistRecord;

/* Get Endpoint List Response command payload */
typedef struct
{
    uint8   u8Total;
    uint8   u8StartIndex;
    uint8   u8Count;
    tsCLD_ZllEndpointlistRecord asEndpointRecords[ZLL_MAX_ENDPOINT_RECORDS];
} tsCLD_ZllUtility_GetEndpointListRspCommandPayload;


/* Definition of Call back Event Structure */
typedef struct
{
    uint8   u8CommandId;
    union
    {
        tsCLD_ZllUtility_EndpointInformationCommandPayload       *psEndpointInfoPayload;
        tsCLD_ZllUtility_GetGroupIdReqCommandPayload             *psGetGroupIdReqPayload;
        tsCLD_ZllUtility_GetGroupIdRspCommandPayload             *psGetGroupIdRspPayload;
        tsCLD_ZllUtility_GetEndpointListReqCommandPayload        *psGetEndpointlistReqPayload;
        tsCLD_ZllUtility_GetEndpointListRspCommandPayload        *psGetEndpointListRspPayload;
    } uMessage;
} tsCLD_ZllUtilityCallBackMessage;


/* Custom data structure */
typedef struct
{
    tsZCL_ReceiveEventAddress                sRxAddr;
    tsZCL_CallBackEvent                      sCustomCallBackEvent;
    tsCLD_ZllUtilityCallBackMessage       sCallBackMessage;
} tsCLD_ZllUtilityCustomDataStructure;


#if 0
typedef struct
{
    teCLD_ZllUtility_Command eCommand;
    //ZPS_tsInterPanAddress       sSrcAddr;
    union
    {
???
    }uPayload;
} tsZllMessage;
#endif


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_ZllUtilityCreateUtility(
                    tsZCL_ClusterInstance              *psClusterInstance,
                    bool_t                              bIsServer,
                    tsZCL_ClusterDefinition            *psClusterDefinition,
                    void                               *pvSharedStructPtr,
                    uint8              *pu8AttributeControlBits,
                    tsCLD_ZllUtilityCustomDataStructure  *psCustomDataStructure);



PUBLIC teZCL_Status eCLD_ZllUtilityCommandEndpointInformationCommandSend(
                    uint8 u8SrcEndpoint,
                    uint8 u8DstEndpoint,
                    tsZCL_Address               *psDestinationAddress,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_ZllUtility_EndpointInformationCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_ZllUtilityCommandGetGroupIdReqCommandSend(
                    uint8   u8Srcendpoint,
                    uint8   u8DstEndpoint,
                    tsZCL_Address               *psDestinationAddress,
                    uint8                       *pu8TransactionSequenceNumber,
                    uint8                       u8StartIndex);

PUBLIC teZCL_Status eCLD_ZllUtilityCommandGetGroupIdRspCommandSend(
                    uint8 u8SrcEndpoint,
                    uint8 u8DstEndpoint,
                    tsZCL_Address               *psDestinationAddress,
                    uint8                       *pu8TransactionSequenceNumber,
                    uint8                       u8StartIndex);

PUBLIC teZCL_Status eCLD_ZllUtilityCommandGetEndpointListReqCommandSend(

                    uint8 u8SrcEndpoint,
                    uint8 u8DstEndpoint,
                    tsZCL_Address               *psDestinationAddress,
                    uint8                       *pu8TransactionSequenceNumber,
                    uint8                       u8StartIndex);

PUBLIC teZCL_Status eCLD_ZllUtilityCommandGetEndpointListRspCommandSend(
                    uint8                       u8SrcEndpoint,
                    uint8                       u8DstEndpoint,
                    tsZCL_Address               *psDestinationAddress,
                    uint8                       *pu8TransactionSequenceNumber,
                    uint8                       u8StartIndex);

PUBLIC teZCL_Status eCLD_ZllUtilityCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);


/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/


extern tsZCL_AttributeDefinition asCLD_ZllUtilityClusterAttributeDefinitions[];
extern tsZCL_ClusterDefinition sCLD_ZllUtility;

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* ZLL_UTILITY_H */
