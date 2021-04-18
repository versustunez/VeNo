#pragma once

#include "ComponentStates.h"
#include <cstdlib>
namespace VeNo::State
{
class InstanceState
{
public:
    explicit InstanceState (size_t id);

    ComponentStates components;

protected:
    size_t m_id;
};
} // namespace VeNo::State