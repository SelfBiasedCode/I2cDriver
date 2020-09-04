# I2cDriver

## Summary
This is a low level master I2C driver for the Atmel ATmega328p and related models written in C++. It is optimized for a balance of speed and code size. It also serves as the foundation of my high level drivers. 

## Technical Details
No inlining is used in regular driver functions to let the compiler control optimization. If absolute speed priority is required, the driver can easily be optimized by moving functions from the .cpp to the .hpp and adding the *inline* keyword.
Debugging can be simplified by defining the preprocessor constant **I2C_CHECK_SUCCESS**, which creates an observable variable namend *success*. It can be monitored to check successful communication in larger contexts.

## Usage
- One instance of I2cDriver controls one I2C slave.
- Higher level drivers must make sure that they complete communication with the slave in one function. If this is not adhered to, multiple instances of I2cDriver could destructively influence each other.

## Known Issues
1. No destructor is present as teardowns had not been set as a requirement when writing this driver.
2. I2cDriver is not multithreading compatible.

