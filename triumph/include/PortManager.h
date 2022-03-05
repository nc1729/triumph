#pragma once

#include "Port.h"
#include "Tryte.h"

#include <map>
#include <memory>

/*
-- PortManager --
    A mapping from port_numbers (Tryte values between -9841 and 9841) to active Ports.
*/
class PortManager
{
private:
    std::map<int64_t, Port> ports_;
    Port dummy{ Port::Status::CLOSED };
public:
    // access a port at port_number - if this port doesn't exist, expose a dummy closed port
    Port& operator[](Tryte const& port_number);

    // add a port at a given port_number and with a given port_status and return a ptr to this new port
    Port* connect_port(Tryte const& port_number,
                       Port::Status const& port_status = Port::Status::OPEN);

    // delete a port at a given port_number
    void disconnect_port(Tryte const& port_number);
};