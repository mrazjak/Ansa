/*
 * IOSPFv3Module.h
 *
 *  Created on: 2014
 *      Author: Jakub Mrazek
 */

#ifndef IOSPFV3MODULE_H_
#define IOSPFV3MODULE_H_

//#include "EigrpMessage_m.h"
#include "OSPFv3LinkTableIPv6.h"

class IOSPFv3Module
{
  public:
    virtual ~IOSPFv3Module() {}
    virtual void addInterface(int ifaceId, int area,/*int LinkId,*/ bool enabled) = 0;
//    virtual OSPFv3LinkIPv6 *addLink(IPv6Address address, int mask) = 0;
    virtual void setProcessID(int prcssId) = 0;
    virtual void setRouterID(IPv4Address routerID) = 0;
//    virtual void setKValues(const EigrpKValues& kValues) = 0;

//    virtual void setHelloInt(int interval, int ifaceId) = 0;
//    virtual void setHoldInt(int interval, int ifaceId) = 0;
};

#endif /* IOSPFV3MODULE_H_ */
