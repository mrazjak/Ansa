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

#include "IPv4Address.h"

#include <algorithm>

#include "EigrpIpv4TopologyTable.h"
#include "EigrpMessage_m.h"
#include "EigrpMetricHelper.h"

#define EIGRP_TT_DEBUG

Define_Module(EigrpIpv4TopologyTable);


std::ostream& operator<<(std::ostream& os, const EigrpRouteSource<IPv4Address>& source)
{
    EigrpRoute<IPv4Address> *route = source.getRouteInfo();

    const char *state = route->isActive() ? "active" : "passive";
    //const char *source = route.isInternal()() ? "internal" : "external";

    os << "ID:" << source.getSourceId() << "  ";
    os << route->getRouteAddress() << "/" << route->getRouteMask().getNetmaskLength();
    os << "  FD:" << route->getFd();
    if (source.getNextHop().isUnspecified())
        os << "  is connected ";
    else
        os << "  via " << source.getNextHop();
    os << " (" << source.getMetric() << "/" << source.getRd() << ")";
    if (source.isSuccessor()) os << "  is successor";
    os << ",  IF:" << source.getIfaceName() << "(" << source.getIfaceId() << ")";
    os << "  state:" << state;
    os << "  originator:" << source.getOriginator();

    return os;
}

std::ostream& operator<<(std::ostream& os, const EigrpRoute<IPv4Address>& route)
{
    os << "ID: " << route.getRouteId() << "  ";
    os << route.getRouteAddress() << "/" << route.getRouteMask().getNetmaskLength();
    os << "  queryOrigin:" << route.getQueryOrigin();
    os << "  replyStatus:" << route.getReplyStatusSum();

    return os;
}

void EigrpIpv4TopologyTable::initialize()
{
    WATCH(routerID);
    WATCH_PTRVECTOR(routeVec);
#ifdef EIGRP_TT_DEBUG
    //WATCH_PTRVECTOR(routeInfoVec);
#endif
}

void EigrpIpv4TopologyTable::handleMessage(cMessage *msg)
{
    throw cRuntimeError("This module does not process messages");
}

EigrpIpv4TopologyTable::~EigrpIpv4TopologyTable()
{
    int cnt = routeVec.size();
    EigrpRouteSource<IPv4Address> *rt;

    for (int i = 0; i < cnt; i++)
    {
        rt = routeVec[i];
        routeVec[i] = NULL;
        delete rt;
    }
    routeVec.clear();
}

/**
 */
EigrpRouteSource<IPv4Address> *EigrpIpv4TopologyTable::findRoute(const IPv4Address& routeAddr, const IPv4Address& routeMask, const IPv4Address& nextHop)
{
    RouteVector::iterator it;
    EigrpRoute<IPv4Address> *route;

    for (it = routeVec.begin(); it != routeVec.end(); it++)
    {
        route = (*it)->getRouteInfo();
        if (route->getRouteAddress() == routeAddr && route->getRouteMask() == routeMask &&
                (*it)->getNextHop() == nextHop && (*it)->isValid())
        {
            return *it;
        }
    }

    return NULL;
}

EigrpRouteSource<IPv4Address> *EigrpIpv4TopologyTable::findRoute(const IPv4Address& routeAddr, const IPv4Address& routeMask, int nextHopId)
{
    RouteVector::iterator it;
    EigrpRoute<IPv4Address> *route;

    for (it = routeVec.begin(); it != routeVec.end(); it++)
    {
        route = (*it)->getRouteInfo();
        if (route->getRouteAddress() == routeAddr && route->getRouteMask() == routeMask &&
                (*it)->getNexthopId() == nextHopId && (*it)->isValid())
        {
            return *it;
        }
    }

    return NULL;
}

uint64_t EigrpIpv4TopologyTable::findRouteDMin(EigrpRoute<IPv4Address> *route)
{
    uint64_t dmin = EigrpMetricHelper::METRIC_INF;
    uint64_t tempD;
    RouteVector::iterator it;
    int routeId = route->getRouteId();

    for (it = routeVec.begin(); it != routeVec.end(); it++)
    {
        if ((*it)->getRouteId() == routeId /* do not check FC here */  && (*it)->isValid())
        {
            tempD = (*it)->getMetric();
            if (tempD < dmin)
            {
                dmin = tempD;
            }
        }
    }

    return dmin;
}

/**
 * Finds successor for the route with minimal next hop IP
 */
EigrpRouteSource<IPv4Address> *EigrpIpv4TopologyTable::getFirstSuccessor(EigrpRoute<IPv4Address> *route)
{
    RouteVector::iterator it;
    int routeId = route->getRouteId();
    EigrpRouteSource<IPv4Address> *tempSrc = NULL;

    for (it = routeVec.begin(); it != routeVec.end(); it++)
    {
        if ((*it)->getRouteId() == routeId && (*it)->isSuccessor() && (*it)->isValid())
        {
            if (tempSrc != NULL)
            {
                if ((*it)->getNextHop() < tempSrc->getNextHop())
                    tempSrc = *it;
            }
            else
                tempSrc = *it;
        }
    }

    return tempSrc;
}

/*EigrpRouteSource<IPv4Address> *EigrpIpv4TopologyTable::getFirstSuccessor(const IPv4Address& address, const IPv4Address& mask)
{
    RouteInfoVector::iterator it;

    for (it = routeInfoVec.begin(); it != routeInfoVec.end(); it++)
    {
        if ((*it)->getRouteAddress() == address && (*it)->getRouteMask() == mask && (*it)->isValid())
        {
            return (*it)->getSuccessor();
        }
    }

    return NULL;
}*/

EigrpRouteSource<IPv4Address> *EigrpIpv4TopologyTable::getFirstSuccessorByIf(EigrpRoute<IPv4Address> *route, int ifaceId)
{
    RouteVector::iterator it;
    int routeId = route->getRouteId();

    for (it = routeVec.begin(); it != routeVec.end(); it++)
    {
        if ((*it)->getRouteId() == routeId && (*it)->isSuccessor() &&
                (*it)->getIfaceId() == ifaceId && (*it)->isValid())
        {
            return *it;
        }
    }
    return NULL;
}

/**
 * Finds feasible successor and minimal distance to the destination.
 *
 * @params resultDmin Return value with minimal distance of all FS.
 */
bool EigrpIpv4TopologyTable::hasFeasibleSuccessor(EigrpRoute<IPv4Address> *route, uint64_t& resultDmin)
{
    RouteVector::iterator it;
    int routeId = route->getRouteId();
    bool hasFs = false;
    uint64_t tempD;

    resultDmin = EigrpMetricHelper::METRIC_INF;
    ev << "EIGRP: Search feasible successor for route " << route->getRouteAddress();
    ev << ", FD is " << route->getFd() << endl;

    for (it = routeVec.begin(); it != routeVec.end(); it++)
    {
        if ((*it)->getRouteId() == routeId && (*it)->isValid())
        {
            ev << "    Next hop " << (*it)->getNextHop();
            ev << " (" << (*it)->getMetric() << "/" << (*it)->getRd() << ") ";

            tempD = (*it)->getMetric();
            if (tempD < resultDmin)
            {
                hasFs = false;  // FS must have minimal distance
                resultDmin = tempD;
            }

            if ((*it)->getRd() < route->getFd() && tempD == resultDmin)
            {
                ev << "satisfies FC" << endl;
                hasFs = true;
            }
            else
                ev << "not satisfies FC" << endl;
        }
    }
    if (hasFs) ev << "    FS found, dmin is " << resultDmin << endl;
    else ev << "    FS not found, dmin is " << resultDmin << endl;

    return hasFs;
}

void EigrpIpv4TopologyTable::addRoute(EigrpRouteSource<IPv4Address> *source)
{
    //RouteInfoVector::reverse_iterator rit;
    RouteVector::iterator it = routeVec.end();
    int routeId = source->getRouteId();

    source->setSourceId(sourceIdCounter);
    sourceIdCounter++;

    // Find last route with routeId
    while (it != routeVec.begin())
    {
         --it;
         if ((*it)->getRouteId() == routeId && (*it)->isValid())
         {
             break;
         }
    }

    // Insert item after found item
    if (it == routeVec.end())
        routeVec.push_back(source);
    else
        routeVec.insert(it + 1, source);
}

/*
void EigrpIpv4TopologyTable::moveSuccessorAhead(EigrpRouteSource<IPv4Address> *source)
{
    RouteVector::iterator it, backit;
    EigrpRouteSource<IPv4Address> *tempSource = NULL;

    if ((it = std::find(routeVec.begin(), routeVec.end(), source)) == routeVec.end())
    {
        return;
    }

    if (it == routeVec.begin())
        return;

    backit = it - 1;

    // Move successor before all nonsuccessors
    while (it != routeVec.begin())
    {
         if ((*it)->getRouteId() != (*backit)->getRouteId())
             break;

         if ((*backit)->isSuccessor() && (*backit)->getNextHop() <= (*it)->getNextHop())
             break;

         // Swap items
         tempSource = *it;
         *it = *backit;
         *backit = tempSource;

         --it;
         --backit;
    }
}*/

/*
void EigrpIpv4TopologyTable::moveRouteBack(EigrpRouteSource<IPv4Address> *source)
{
    RouteVector::iterator it, nextit;
    EigrpRouteSource<IPv4Address> *tempSource = NULL;

    // Get iterator of source
    if ((it = std::find(routeVec.begin(), routeVec.end(), source)) == routeVec.end())
    {
        return;
    }

    // Move source behind all successors
    for (nextit = it + 1; nextit != routeVec.end(); it++, nextit ++)
    {
         if ((*it)->getRouteId() != (*nextit)->getRouteId())
             break;

         if (!(*nextit)->isSuccessor())
             break;

         // Swap items
         tempSource = *it;
         *it = *nextit;
         *nextit = tempSource;
    }
}*/

/**
 * Removes neighbor form the table, but the record still exists.
 */
EigrpRouteSource<IPv4Address> *EigrpIpv4TopologyTable::removeRoute(EigrpRouteSource<IPv4Address> *source)
{
    RouteVector::iterator it;

    if ((it = std::find(routeVec.begin(), routeVec.end(), source)) != routeVec.end())
    {
        removeRoute(it);
        return source;
    }

    return NULL;
}

void EigrpIpv4TopologyTable::delayedRemove(int neighId)
{
    RouteVector::iterator it;

    for (it = routeVec.begin(); it != routeVec.end(); )
    {
        if ((*it)->getNexthopId() == neighId && (*it)->isSetDelayedRemove())
            it = removeRoute(it);
        else
            ++it;
    }
}

/**
 * Deletes invalid sources.
 */
void EigrpIpv4TopologyTable::purgeTable()
{
    RouteVector::iterator it;

    for (it = routeVec.begin(); it != routeVec.end(); )
    {
        if (!(*it)->isValid())
        {  // Remove invalid source from TT
            it = removeRoute(it);
        }
        else
            ++it;
    }
}

std::vector<EigrpRouteSource<IPv4Address> *>::iterator EigrpIpv4TopologyTable::removeRoute(RouteVector::iterator routeIt)
{
    EigrpRoute<IPv4Address> *route = NULL;
    EigrpRouteSource<IPv4Address> *source = NULL;

    source = *routeIt;
    route = source->getRouteInfo();

    EV << "EIGRP: remove route " << route->getRouteAddress();
    EV << " via " << source->getNextHop() << " from TT" << endl;

    if (route->getRefCnt() == 1 && !route->isActive())
        removeRouteInfo(route);

    delete source;
    return routeVec.erase(routeIt);
}

EigrpRouteSource<IPv4Address> *EigrpIpv4TopologyTable::findRouteById(int sourceId)
{
    RouteVector::iterator it;
    for (it = routeVec.begin(); it != routeVec.end(); it++)
    {
        if ((*it)->getSourceId() == sourceId && (*it)->isValid())
        return *it;
    }
    return NULL;
}

EigrpRouteSource<IPv4Address> *EigrpIpv4TopologyTable::findRouteByNextHop(int routeId, int nextHopId)
{
    RouteVector::iterator it;
    for (it = routeVec.begin(); it != routeVec.end(); it++)
    {
        if ((*it)->getRouteId() == routeId && (*it)->getNexthopId() == nextHopId && (*it)->isValid())
        return *it;
    }
    return NULL;
}



/**
 * @param sourceNewResult return value.
 */
EigrpRouteSource<IPv4Address> * EigrpIpv4TopologyTable::findOrCreateRoute(IPv4Address& routeAddr, IPv4Address& routeMask, IPv4Address& routerId,
        EigrpInterface *eigrpIface, int nextHopId, bool *sourceNew)
{
    EigrpRoute<IPv4Address> *route = NULL;
    EigrpRouteSource<IPv4Address> *source = NULL;
    (*sourceNew) = false;

    // Find source of route with given next hop ID
    source = findRoute(routeAddr, routeMask, nextHopId);
    if (source == NULL)
    {
        if ((route = findRouteInfo(routeAddr, routeMask)) == NULL)
        { // Create route
            route = new EigrpRoute<IPv4Address>(routeAddr, routeMask);
            addRouteInfo(route);
        }
        // Create source of route
        source = new EigrpRouteSource<IPv4Address>(eigrpIface->getInterfaceId(), eigrpIface->getInterfaceName(), nextHopId, route->getRouteId(), route);
        source->setOriginator(routerId);
        (*sourceNew) = true;
        addRoute(source);
    }

    return source;
}

EigrpRoute<IPv4Address> *EigrpIpv4TopologyTable::removeRouteInfo(EigrpRoute<IPv4Address> *route)
{
    RouteInfoVector::iterator it;

    if ((it = std::find(routeInfoVec.begin(), routeInfoVec.end(), route)) != routeInfoVec.end())
    {
        routeInfoVec.erase(it);
        return route;
    }

    return NULL;
}

EigrpRoute<IPv4Address> *EigrpIpv4TopologyTable::findRouteInfo(const IPv4Address& routeAddr, const IPv4Address& routeMask)
{
    RouteInfoVector::iterator it;

    for (it = routeInfoVec.begin(); it != routeInfoVec.end(); it++)
    {
        if ((*it)->getRouteAddress() == routeAddr && (*it)->getRouteMask() == routeMask)
            return *it;
    }

    return NULL;

}
