/*
* I2cStatus.h
*
* Created: 21/11/2018 19:04:11
*  Author: SelfBiasedCode
*/


#ifndef I2CSTATUS_H_
#define I2CSTATUS_H_

namespace I2C
{
	#define STATUS_MASK				0xF8 // mask prescaler bits
	
	/* defines shared and for Master Transmission Mode */
	#define STATUS_START			0x08 // START sent
	#define STATUS_RESTART			0x10 // repeated START sent
	#define STATUS_SLAW_ACK			0x18 // SLA+W sent, ACK received
	#define STATUS_SLAW_NACK		0x20 // SLA+W sent, NACK received
	#define STATUS_DATA_TRA_ACK		0x28 // Data sent, ACK received
	#define STATUS_DATA_TRA_NACK	0x30 // DATA sent, NACK received
	#define STATUS_ARBLOST			0x38 // Arbitration lost
	
	/* extra defines for Master Receiver Mode */
	#define STATUS_SLAR_ACK			0x40 // SLA+R sent, ACK received
	#define STATUS_SLAR_NACK		0x48 // SLA+R sent, NACK received
	#define STATUS_DATA_REC_ACK		0x50 // Data + ACK received
	#define STATUS_DATA_REC_NACK	0x58 // Data + NACK received

}

#endif /* I2CSTATUS_H_ */
