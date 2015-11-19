/*
 * Types.h
 *
 *  Created on: 14/nov/2015
 *      Author: stefano
 */

#ifndef TYPES_H_
#define TYPES_H_

#include "SupportDefs.h"

struct basemap {
	uint8 width;
	uint8 height;
	uint8 tiles[0];
} __attribute__((__packed__)) ;



#endif /* TYPES_H_ */
