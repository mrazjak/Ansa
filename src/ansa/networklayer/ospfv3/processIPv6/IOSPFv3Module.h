/*
 * IOSPFv3Module.h
 *
 *  Created on: 2014
 *      Author: Jakub Mrazek
 */

#ifndef IOSPFV3MODULE_H_
#define IOSPFV3MODULE_H_

//#include "EigrpMessage_m.h"
//#include "OSPFv3InterfaceTableIPv6.h"
//#include "OSPFv3Interface.h"
//#include "OSPFv3Area.h"

//namespace OSPFv3 {

//class OSPFv3Area;
//class OSPFv3IntefaceIPv6;

class IOSPFv3Module
{
  public:
    virtual ~IOSPFv3Module() {}
//    virtual void addInterface(OSPFv3InterfaceIPv6 *intf) = 0;
//    virtual void addArea(OSPFv3Area *area) = 0;

    virtual void setProcessID(int prcssId) = 0;
    virtual void setRouterID(IPv4Address routerID) = 0;
//    virtual void setKValues(const EigrpKValues& kValues) = 0;

//    virtual void setHelloInt(int interval, int ifaceId) = 0;
//    virtual void setHoldInt(int interval, int ifaceId) = 0;
};
//} // namespace OSPFv3
#endif /* IOSPFV3MODULE_H_ */
