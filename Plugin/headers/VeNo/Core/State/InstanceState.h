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
    size_t m_id;
};
} // namespace VeNo::State