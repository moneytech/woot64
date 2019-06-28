#pragma once

template<class T>
class Sequencer
{
    T value;
public:
    Sequencer(T initialValue) :
        value(initialValue)
    {
    }

    T GetNext()
    {
        return __sync_fetch_and_add(&value, 1);
    }
};
