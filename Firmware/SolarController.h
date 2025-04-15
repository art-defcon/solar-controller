/** \addtogroup LDR
 *  @{
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "LRD.h"

class SolarController
{
	public:
		SolarController();
		void adjust();

	private:
		LDR rightLightReader;
		LDR leftLightReader;

};
#endif
