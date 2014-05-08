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

#include "OSPFv3InterfaceTableIPv6.h"

#include <algorithm>

Define_Module(OSPFv3InterfaceTableIPv6);

OSPFv3InterfaceTableIPv6::~OSPFv3InterfaceTableIPv6()
{
    int cnt = OSPFv3Interfaces.size();
    OSPFv3InterfaceIPv6 *iface;

    for (int i = 0; i < cnt; i++)
    {
        iface = OSPFv3Interfaces[i];

        OSPFv3Interfaces[i] = NULL;
        delete iface;
    }
    OSPFv3Interfaces.clear();
}

void OSPFv3InterfaceTableIPv6::initialize(int stage)
{
    if (stage == 3)
    {
//        WATCH_PTRVECTOR(eigrpInterfaces);
    }
}

void OSPFv3InterfaceTableIPv6::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
/*
void EigrpInterfaceTable::cancelHelloTimer(EigrpInterface *iface)
{
    EigrpTimer *timer;

    if ((timer = iface->getHelloTimer()) != NULL)
    {
        if (timer->isScheduled())
            cancelEvent(timer);
    }
}
*/
void OSPFv3InterfaceTableIPv6::addInterface(OSPFv3InterfaceIPv6 *interface)
{
    //TODO check duplicity
    OSPFv3Interfaces.push_back(interface);
}
/*
EigrpInterface *EigrpInterfaceTable::removeInterface(EigrpInterface *iface)
{
    InterfaceVector::iterator it;
    it = std::find(eigrpInterfaces.begin(), eigrpInterfaces.end(), iface);

    if (it != eigrpInterfaces.end())
    {
        eigrpInterfaces.erase(it);
        return iface;
    }

    return NULL;
}
*/
OSPFv3InterfaceIPv6 *OSPFv3InterfaceTableIPv6::getInterfaceById(const int ifaceId)
{
    OSPFv3InterfaceVector::iterator it;

    for (it = OSPFv3Interfaces.begin(); it != OSPFv3Interfaces.end(); it++)
    {
        if ((*it)->getInterfaceId() == ifaceId)
        {
            return *it;
        }
    }
    return NULL;
}

