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
    Port& operator[](Tryte const& port_number);
    Port const& operator[](Tryte const& port_number) const;

    void connect_port(Tryte const& port_number);
    void disconnect_port(Tryte const& port_number);
};