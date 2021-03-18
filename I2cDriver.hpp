#ifndef __I2CDRIVER_H__
#define __I2CDRIVER_H__

// check TWI status after each command
#ifdef DEBUG
#define I2C_CHECK_SUCCESS
#endif

// test values: 12,0 = 400 kHz, 12,1 = 100 kHz @ 16 MHz
#if !(defined(I2C_SPEED_DIV) && defined(I2C_SPEED_PRE))
#error "I2C speed not defined!"
#endif

// platform check
#if !(defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PA__))
#warning "Unsupported platform, please verify compatibility!"
#endif 

#include "I2cStatus.hpp"
#include <stdint.h>
#include <avr/io.h>

namespace I2C
{
	class I2cDriver
	{
		//variables
		public:
		protected:
		private:
		const uint8_t m_address;

		//functions
		public:
		
		/* minimizing class overhead through inline constructor */
		I2cDriver(const uint8_t address):m_address(address)
		{
			// set speed
			TWBR = I2C_SPEED_DIV;
			TWSR |= (I2C_SPEED_PRE & 0x03);
			
			// enable TWI
			TWCR = (1 << TWEN);
		}
		
		/* mid level */
		
		void readBytes(uint8_t * buffer, uint8_t size) const;
		void readBytes(uint8_t subAddress, uint8_t * buffer, uint8_t size) const;
		
		void writeBytes(const uint8_t * data, uint8_t size) const;
		void writeBytes(uint8_t subAddress, const uint8_t * data, uint8_t size) const;
		
		protected:
		/* low level */

		void sendAddress(bool write) const;
		void sendStart() const;
		void sendStop() const;
		void sendByte(uint8_t data) const;
		uint8_t readByte(bool acknowledgeNextByte) const;
		inline bool checkStatus(uint8_t expectedStatus) const
		{
			// return success indicator
			return ( (TWSR & (STATUS_MASK)) == expectedStatus);
		}
		private:

		// delete copy constructor and assignment operator
		I2cDriver( const I2cDriver &c ) = delete;
		I2cDriver& operator=( const I2cDriver &c ) = delete;

	}; //I2cDriver
} // namespace
#endif //__I2CDRIVER_H__
