//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __INET_OSPFV3INTERFACETABLEIPV6_H_
#define __INET_OSPFV3INTERFACETABLEIPV6_H_

#include <omnetpp.h>

#include "OSPFv3Timer_m.h"
#include "InterfaceEntry.h"

/**
 * TODO nastavovat BW, DLY dle typu linky, Reliability pocitat
 * na zacatku simulace, Load prubezne po 5 min (melo by byt konfigurovatelne)
 */
class OSPFv3InterfaceIPv6: public cObject
{
  protected:
    int interfaceId;        /**< ID of interface*/
    char instanceId;
    int areaId;             /**< ID of area*/
    int helloInt;           /**< Hello interval in seconds (<1-65535>) */
    int deadInt;            /**< Router's Dead interval in seconds (<1-65535>) */
    int routerPriority;     /**< Router Priority (0-255)*/
    int hopLimit;           /**< Hop Limit typically 1 and different virtual line*/
    OSPFv3Timer *hellot;    /**< pointer to hello timer */
    bool enabled;           /**< true, if OSPFv3 is enabled on interface */

    double bandwidth; /**< Bandwidth in Kbps (<1-10 000 000>) */
    double delay;     /**< Delay in us (<1-16 777 215>) */
    int reliability; /**< Reliability in percent (<1-255>) */
    int load;      /**< Load in percent (<1-255>) */

  public:
    OSPFv3InterfaceIPv6(InterfaceEntry* iface, int area/*, int linkId*/, bool enabled);
    ~OSPFv3InterfaceIPv6();

    bool operator==(const OSPFv3InterfaceIPv6& iface) const
    {
        return interfaceId == iface.getInterfaceId();
    }

    /**< Set identifier of interface. */
    void setInterfaceId(int id) { interfaceId = id; }
    /**< Set pointer to hello timer. */
    void setHelloTimerPtr(OSPFv3Timer *timer) { this->hellot = timer; }
    void setHelloInt(int helloInt) { this->helloInt = helloInt; }
    void setDeadInt(int deadInt) { this->deadInt = deadInt; }
    void setEnabling(bool enabled) { this->enabled = enabled; }
//    void setLinkId(int id) { this->linkId= id; }
    void setInstanceId(int instId) { this->instanceId= instId; }
    void setAreaId(int areaId) { this->areaId = areaId; }
    void setRouterPriorityId(int rp) { this->routerPriority = rp; }
    void setHopLimit(int hplmt) { this->hopLimit = hplmt; }

    void setBandwidth(int bw) { this->bandwidth = bw; }
    void setDelay(int dly) { this->delay = dly; }
    void setReliability(int rel) { this->reliability = rel; }
    void setLoad(int load) { this->load = load; }

    /**< Return identifier of interface. */
    int getInterfaceId() const { return interfaceId; }
    /**< Return pointer to hello timer. */
    OSPFv3Timer *getHelloTimer() const { return this->hellot; }
    int getHelloInt() const { return this->helloInt; }
    int getDeadInt() const { return this->deadInt; }
    bool isEnabled() const { return this->enabled; }
//    int getLinkId() const { return this->linkId; }
    int getInstanceId() const { return this->instanceId; }
    int getAreaId() const { return this->areaId; }
    int getRouterPriorityId() const {return this->routerPriority; }
    int getHopLimit() const { return this->hopLimit; }

    unsigned int getBandwidth() const { return bandwidth; }
    unsigned int getDelay() const { return delay; }
    unsigned int getReliability() const { return reliability; }
    unsigned int getLoad() const { return load; }
};

/**
 * TODO - Generated class
 */
class OSPFv3InterfaceTableIPv6 : public cSimpleModule
{
  protected:
    typedef typename std::vector<OSPFv3InterfaceIPv6 *> OSPFv3InterfaceVector;

    OSPFv3InterfaceVector OSPFv3Interfaces;

    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const { return 4; }

//    void cancelHelloTimer(EigrpInterface *iface);

  public:
    ~OSPFv3InterfaceTableIPv6();

    /**< Adds interface to table. */
    void addInterface(OSPFv3InterfaceIPv6 *interface);
    /**< Removes interface from table */
//    EigrpInterface *removeInterface(EigrpInterface *iface);

    /**< Gets interface from table by interface ID */
    OSPFv3InterfaceIPv6 *getInterfaceById(int ifaceId);
    /**< Gets interface from table by interface name */
//    EigrpInterface *findInterfaceByName(const char *ifaceName);
};

#endif // __INET_OSPFV3INTERFACETABLEIPV6_H_
