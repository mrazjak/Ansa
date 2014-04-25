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

#ifndef OSPFV3LINKTABLEIPV6_H_
#define OSPFV3LINKTABLEIPV6_H_

#include "IPv6Address.h"

class OSPFv3LinkIPv6
{
  protected:
    int linkId;
    IPv6Address prefix;
    int prefixLen;

  public:
    OSPFv3LinkIPv6(IPv6Address &prefix, int &prefixLen, int id) :
            linkId(id), prefix(prefix), prefixLen(prefixLen) {}

    const IPv6Address& getPrefix() const {
        return prefix;
    }

    void setPrefix(const IPv6Address& prfx) {
        this->prefix = prfx;
    }

    const int getPrefixLen() const {
        return prefixLen;
    }

    void setPrefixLen(const int prfxLen) {
        this->prefixLen = prfxLen;
    }

    const int getLinkId() const {
        return linkId;
    }

    void setLinkId(int lId) {
        this->linkId = lId;
    }
};

class OSPFv3LinkTableIPv6 : cObject
{
  protected:
    std::vector<OSPFv3LinkIPv6 *> linkVec;
    int linkCnt;

  public:
    static const int UNSPEC_NETID = 0;

    OSPFv3LinkTableIPv6() : linkCnt(1) { }
    virtual ~OSPFv3LinkTableIPv6();

    OSPFv3LinkIPv6 *addLink(IPv6Address& address, int& mask);
//    EigrpNetwork *findNetworkById(int netId);
//    std::vector<EigrpNetwork *> *getAllNetworks() { return &networkVec; }
//    bool isAddressIncluded(IPv4Address& address, IPv4Address& mask);
//    bool isInterfaceIncluded(const IPv4Address& ifAddress, const IPv4Address& ifMask, int *resultNetId);
};

#endif /* OSPFV3LINKTABLEIPV6_H_ */
