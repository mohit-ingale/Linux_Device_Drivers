/*
 * ioct.h
 *
 *  Created on: 01-Nov-2016
 *      Author: mohit_linux
 */

#ifndef IOCT_H_
#define IOCT_H_

#define BASIC_TYPE 'x'
#define BASIC_RESET _IO(BASIC_TYPE,0)
#define BASIC_PRINT _IOW(BASIC_TYPE,1,int)

#endif /* IOCT_H_ */
