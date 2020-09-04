#include "I2cDriver.hpp"
#include "I2cStatus.hpp"

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>

namespace I2C
{
	/* Mid Level Functions */
	
	void I2cDriver::readBytes(uint8_t * buffer, uint8_t size) const
	{
		#ifdef I2C_CHECK_SUCCESS
		volatile bool success = false;
		#endif
		
		sendStart();
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_START);
		#endif
		
		sendAddress(false);
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_SLAR_ACK);
		#endif
		
		// read and acknowledge n-1 bytes
		for(;size > 1; size--)
		{
			*buffer = readByte(true);
			buffer++;
			
			#ifdef I2C_CHECK_SUCCESS
			success = checkStatus(STATUS_DATA_REC_ACK);
			#endif
			
		}
		
		// read and don't acknowledge n-th byte
		*buffer = readByte(false);
		
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_DATA_REC_NACK);
		#endif
		
		sendStop();
	}

	void I2cDriver::readBytes(uint8_t subAddress, uint8_t * buffer, uint8_t size) const
	{
		#ifdef I2C_CHECK_SUCCESS
		volatile bool success = false;
		#endif
		
		sendStart();
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_START);
		#endif
		
		sendAddress(true);
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_SLAW_ACK);
		#endif
		
		sendByte(subAddress);
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_DATA_TRA_ACK);
		#endif
		
		// repeated start to initiate read operation
		sendStart();
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_START);
		#endif
		
		sendAddress(false);
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_SLAR_ACK);
		#endif
		
		// read and acknowledge n-1 bytes
		for(;size > 1; size--)
		{
			*buffer = readByte(true);
			buffer++;
			#ifdef I2C_CHECK_SUCCESS
			success = checkStatus(STATUS_DATA_REC_ACK);
			#endif
		}
		
		// read and don't acknowledge n-th byte
		*buffer = readByte(false);
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_DATA_REC_NACK);
		#endif
		
		sendStop();
	}
	
	void I2cDriver::writeBytes(const uint8_t * data, uint8_t size) const
	{
		#ifdef I2C_CHECK_SUCCESS
		volatile bool success = false;
		#endif
		sendStart();
				#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_START);
		#endif
		
		sendAddress(true);
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_SLAW_ACK);
		#endif
		
		for(;size > 0; size--)
		{
			sendByte(*data++);
			#ifdef I2C_CHECK_SUCCESS
			success = checkStatus(STATUS_DATA_TRA_ACK);
			#endif
		}
		
		sendStop();
	}
	
	void I2cDriver::writeBytes(uint8_t subAddress, const uint8_t * data, uint8_t size) const
	{
		#ifdef I2C_CHECK_SUCCESS
		volatile bool success = false;
		#endif
		sendStart();
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_START);
		#endif
		
		sendAddress(true);
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_SLAW_ACK);
		#endif
		
		sendByte(subAddress);
		#ifdef I2C_CHECK_SUCCESS
		success = checkStatus(STATUS_DATA_TRA_ACK);
		#endif

		for(;size > 0; size--)
		{
			sendByte(*data++);
			#ifdef I2C_CHECK_SUCCESS
			success = checkStatus(STATUS_DATA_TRA_ACK);
			#endif
		}

		sendStop();
	}
	
	void I2cDriver::sendAddress(bool write) const
	{
		// shift address and add R/W bit
		uint8_t address = (m_address << 1) | !write;
		
		// send combined byte
		sendByte(address);
	}
	
	void I2cDriver::sendStart() const
	{
		// clear interrupt, send start, enable TWI
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
		
		// spinlock: wait for TWI
		while (!(TWCR & (1<<TWINT)));
	}
	
	void I2cDriver::sendStop() const
	{
		// clear interrupt, send stop, enable TWI
		TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
		
		// spinlock: wait for STOP to be sent
		while (TWCR & (1<<TWSTO))
		{
			// in case of bus error
			if ((TWSR & STATUS_MASK)  == 0x00)
			{
				// send STOP again to reset bus
				TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
			}
		}
	}
	
	void I2cDriver::sendByte(uint8_t data) const
	{
		// copy data
		TWDR = data;
		
		// clear interrupt, enable TWI
		TWCR = (1 << TWINT) | (1 << TWEN);
		
		// spinlock: wait for TWI
		while (!(TWCR & (1<<TWINT)));
	}
	
	uint8_t I2cDriver::readByte(bool acknowledgeNextByte) const
	{	
		// clear interrupt, set ACK, enable TWI
		TWCR = (1 << TWINT) | (acknowledgeNextByte << TWEA) | (1 << TWEN);
		
		// spinlock: wait for TWI
		while (!(TWCR & (1<<TWINT)));
		
		// return
		return TWDR;
	}
	
} // namespace
