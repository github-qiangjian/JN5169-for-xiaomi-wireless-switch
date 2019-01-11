/*****************************************************************************
 *
 * MODULE:             Micro MAC
 *
 * DESCRIPTION:
 * Low-level functions for MAC/BBC control
 *
 ****************************************************************************
 *
 * This software is owned by NXP B.V. and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on NXP products [NXP Microcontrollers such as JN5148, JN5142, JN5139].
 * You, and any third parties must reproduce the copyright and warranty notice
 * and any other legend of ownership on each copy or partial copy of the
 * software.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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
 * Copyright NXP B.V. 2013. All rights reserved
 *
 ***************************************************************************/
#ifndef MICRO_MAC_H
#define MICRO_MAC_H

#if defined __cplusplus
extern "C" {
#endif


/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "jendefs.h"

/****************************************************************************/
/***        Macro/Type Definitions                                        ***/
/****************************************************************************/
typedef struct
{
    uint32 u32L;  /**< Low word */
    uint32 u32H;  /**< High word */
} tsExtAddr;

typedef union
{
    uint16    u16Short;
    tsExtAddr sExt;
} tuAddr;

/* Structure for building a MAC frame, where the MAC header alignment is
   handled by the hardware */
typedef struct
{
    uint8           u8PayloadLength;
    uint8           u8SequenceNum;
    uint16          u16FCF;
    uint16          u16DestPAN;
    uint16          u16SrcPAN;
    tuAddr          uDestAddr;
    tuAddr          uSrcAddr;
    uint16          u16FCS;
    uint16          u16Unused;
    union
    {
        uint8     au8Byte[127]; /* Payload as both bytes and words */
        uint32    au32Word[32];
    } uPayload;
} tsMacFrame;

/* Structure for building a PHY frame, where the MAC header format is
   undefined */
typedef struct
{
    uint8           u8PayloadLength;
    uint8           au8Padding[3];
    union
    {
        uint8     au8Byte[127]; /* Payload as both bytes and words */
        uint32    au32Word[32];
    } uPayload;
} tsPhyFrame;

/* Options for reception, to pass to vMMAC_StartReceive. User should select
   one from each pair of options, and logical OR the options together */
typedef enum
{
    /* Receive start time: now or delayed */
    E_MMAC_RX_START_NOW        = 0x0002,
    E_MMAC_RX_DELAY_START      = 0x0003,

    /* Timing alignment for auto ack transmission: normal or aligned to
       backoff clock (used in CAP period in beacon networks) */
    E_MMAC_RX_ALIGN_NORMAL     = 0x0000,
    E_MMAC_RX_ALIGNED          = 0x0004,

    /* Wait for auto ack and retry: don't use or use */
    E_MMAC_RX_NO_AUTO_ACK      = 0x0000,
    E_MMAC_RX_USE_AUTO_ACK     = 0x0008,

    /* Malformed packets: reject or accept */
    E_MMAC_RX_NO_MALFORMED     = 0x0000,
    E_MMAC_RX_ALLOW_MALFORMED  = 0x0400,

    /* Frame Check Sequence errors: reject or accept */
    E_MMAC_RX_NO_FCS_ERROR     = 0x0000,
    E_MMAC_RX_ALLOW_FCS_ERROR  = 0x0200,

    /* Address matching: enable or disable */
    E_MMAC_RX_NO_ADDRESS_MATCH = 0x0000,
    E_MMAC_RX_ADDRESS_MATCH    = 0x0100

} teRxOption;

/* Options for transmission, to pass to vMMAC_StartMacTransmit or
   vMMAC_StartPhyTransmit. User should select one from each set of options,
   and logical OR the options together */
typedef enum
{
    /* Transmit start time: now or delayed */
    E_MMAC_TX_START_NOW       = 0x02,
    E_MMAC_TX_DELAY_START     = 0x03,

    /* Wait for auto ack and retry: don't use or use */
    E_MMAC_TX_NO_AUTO_ACK     = 0x00,
    E_MMAC_TX_USE_AUTO_ACK    = 0x08,

    /* Clear channel assessment: don't use or use, plus option to align to
       backoff clock */
    E_MMAC_TX_NO_CCA          = 0x00,
    E_MMAC_TX_USE_CCA         = 0x10,
    E_MMAC_TX_USE_CCA_ALIGNED = 0x20

} teTxOption;

/* Flags for receive status, as returned by u32MMAC_GetRxErrors */
typedef enum
{
    E_MMAC_RXSTAT_ERROR     = 0x01, /* Frame check sequence error */
    E_MMAC_RXSTAT_ABORTED   = 0x02, /* Reception aborted by user */
    E_MMAC_RXSTAT_MALFORMED = 0x20  /* Frame was malformed */
} teRxStatus;

/* Flags for transmit status, as returned by u32MMAC_GetTxErrors */
typedef enum
{
    E_MMAC_TXSTAT_CCA_BUSY  = 0x01, /* Channel wasn't free */
    E_MMAC_TXSTAT_NO_ACK    = 0x02, /* Ack requested but not seen */
    E_MMAC_TXSTAT_ABORTED   = 0x04  /* Transmission aborted by user */
} teTxStatus;

/* Flags for interrupt status, as returned to handler registered with
   vMMAC_EnableInterrupts and as used in the mask passed to
   vMMAC_ConfigureInterruptSources, u32MMAC_PollInterruptSource,
   u32MMAC_PollInterruptSourceUntilFired */
typedef enum
{
    E_MMAC_INT_TX_COMPLETE  = 0x01, /* Transmission attempt has finished */
    E_MMAC_INT_RX_HEADER    = 0x02, /* MAC header has been received */
    E_MMAC_INT_RX_COMPLETE  = 0x04  /* Complete frame has been received */
} teIntStatus;

/* CCA mode to use when transmitting. Use with vMMAC_SetCcaMode(). Default is
   E_MMAC_CCAMODE_ENERGY */
typedef enum
{
    E_MMAC_CCAMODE_ENERGY            = 0x01, /* Energy above threshold */
    E_MMAC_CCAMODE_CARRIER           = 0x02, /* Carrier sense */
    E_MMAC_CCAMODE_ENERGY_OR_CARRIER = 0x03  /* Either energy or carrier */
} teCcaMode;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/* Initialisation */
PUBLIC void vMMAC_Enable(void);
PUBLIC void vMMAC_ConfigureRadio(void);
PUBLIC void vMMAC_SetChannel(uint8 u8Channel);
PUBLIC void vMMAC_SetChannelAndPower(uint8 u8Channel, int i8TxPower);
PUBLIC int8 i8MMAC_GetTxPowerLevel(void);

/* Interrupt control */
PUBLIC void vMMAC_EnableInterrupts(void (*prHandler)(uint32));
PUBLIC void vMMAC_ConfigureInterruptSources(uint32 u32Mask);
PUBLIC uint32 u32MMAC_PollInterruptSource(uint32 u32Mask);
PUBLIC uint32 u32MMAC_PollInterruptSourceUntilFired(uint32 u32Mask);

/* Miscellaneous */
PUBLIC uint32 u32MMAC_GetTime(void);
PUBLIC void vMMAC_RadioOff(void);
PUBLIC void vMMAC_SetCutOffTimer(uint32 u32CutOffTime, bool_t bEnable);
PUBLIC void vMMAC_SynchroniseBackoffClock(bool_t bEnable);
PUBLIC void vMMAC_GetMacAddress(tsExtAddr *psMacAddr);
PUBLIC uint8 u8MMAC_EnergyDetect(uint32 u32DurationSymbols);

/* Receive */
PUBLIC void vMMAC_SetRxAddress(uint32 u32PanId, uint16 u16Short,
                               tsExtAddr *psMacAddr);
PUBLIC void vMMAC_SetRxStartTime(uint32 u32Time);
PUBLIC void vMMAC_StartMacReceive(tsMacFrame *psFrame, teRxOption eOptions);
PUBLIC void vMMAC_StartPhyReceive(tsPhyFrame *psFrame, teRxOption eOptions);
PUBLIC bool_t bMMAC_RxDetected(void);
PUBLIC uint32 u32MMAC_GetRxErrors(void);
PUBLIC uint32 u32MMAC_GetRxTime(void);
PUBLIC uint8 u8MMAC_GetRxLqi(uint8 *pu8Msq);

/* Transmit */
PUBLIC void vMMAC_SetTxParameters(uint8 u8Attempts, uint8 u8MinBE,
                                  uint8 u8MaxBE, uint8 u8MaxBackoffs);
PUBLIC void vMMAC_SetTxStartTime(uint32 u32Time);
PUBLIC void vMMAC_SetCcaMode(teCcaMode eCcaMode);
PUBLIC void vMMAC_StartMacTransmit(tsMacFrame *psFrame, teTxOption eOptions);
PUBLIC void vMMAC_StartPhyTransmit(tsPhyFrame *psFrame, teTxOption eOptions);
PUBLIC uint32 u32MMAC_GetTxErrors(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /* #ifndef MICRO_MAC_H */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
