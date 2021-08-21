#pragma once

namespace VeNo::Events {
class Event {
public:
    virtual void handle() = 0;
};
}