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

#include "OSPFv3LinkTableIPv6.h"

OSPFv3LinkTableIPv6::~OSPFv3LinkTableIPv6()
{
    int cnt = linkVec.size();

    for (int i = 0; i < cnt; i++)
    {
        delete linkVec[i];
    }
    linkVec.clear();
}

OSPFv3LinkIPv6 *OSPFv3LinkTableIPv6::addLink(IPv6Address& address, int& mask)
{
    OSPFv3LinkIPv6 *link = new OSPFv3LinkIPv6(address, mask, linkCnt++);
    linkVec.push_back(link);
    return link;
}
/*
EigrpNetwork *EigrpNetworkTable::findNetworkById(int netId)
{
    std::vector<EigrpNetwork *>::iterator it;

    for (it = networkVec.begin(); it != networkVec.end(); it++)
    {
        if ((*it)->getNetworkId() == netId)
        {
            return (*it);
        }
    }

    return NULL;
}

/**
 * @param resultNetId ID of network that belongs to the interface. If the interface does not
 * belong to any network, it has undefined value.
 */
/*
bool EigrpNetworkTable::isInterfaceIncluded(const IPv4Address& ifAddress, const IPv4Address& ifMask, int *resultNetId)
{
    std::vector<EigrpNetwork *>::iterator it;
    int netMaskLen, ifMaskLen;

    if (ifAddress.isUnspecified())
        return false;

    for (it = networkVec.begin(); it != networkVec.end(); it++)
    {
        IPv4Address netPrefix = (*it)->getAddress();
        IPv4Address netMask = (*it)->getMask();

        netMaskLen = (netMask.isUnspecified()) ? netPrefix.getNetworkMask().getNetmaskLength() : netMask.getNetmaskLength();
        ifMaskLen = ifMask.getNetmaskLength();

        // prefix isUnspecified -> network = 0.0.0.0 -> all interfaces, or
        // mask is unspecified -> classful match or
        // mask is specified -> classless match
        if (netPrefix.isUnspecified() ||
                (netMask.isUnspecified() && netPrefix.isNetwork(ifAddress) && netMaskLen <= ifMaskLen) ||
                (netPrefix.maskedAddrAreEqual(netPrefix, ifAddress, netMask) && netMaskLen <= ifMaskLen))
        {// IP address of the interface match the prefix
            (*resultNetId) = (*it)->getNetworkId();
            return true;
        }
    }

    return false;
}
*/
