#pragma once

#include <cpu.hpp>
#include <errno.h>
#include <semaphore.hpp>
#include <vararg.h>

template<class T>
class MessageQueue
{
    T *data;
    Semaphore s, m;
    size_t cap, h, t;
public:
    typedef bool (*Comparer)(T *a, T *b, VarArgs args);

    MessageQueue(size_t capacity) :
        data(new T[capacity]),
        s(capacity), m(0),
        cap(capacity), h(0), t(0)
    {
    }

    int Read(T *buf, int timeout)
    {
        if(!buf) return -EINVAL;
        int timeleft = m.Wait(timeout < 0 ? 0 : timeout, timeout == 0, false);
        if(timeleft < 0) return -ETIMEOUT;
        *buf = data[t];
        t = (t + 1) % cap;
        s.Signal(nullptr);
        return timeleft;
    }

    int Write(T val, int timeout)
    {
        int timeleft = s.Wait(timeout < 0 ? 0 : timeout, timeout == 0, false);
        if(timeleft < 0) return -ETIMEOUT;
        data[h] = val;
        h = (h + 1) % cap;
        m.Signal(nullptr);
        return timeleft;
    }

    int Peek(T *buf, unsigned offset)
    {
        bool ints = cpuAreInterruptsEnabled();
        int res = m.Wait(0, true, true);
        if(res < 0) return -ENOENT;
        int size = m.GetCount() + 1;
        if(offset >= size)
        {
            m.Signal(nullptr);
            cpuRestoreInterrupts(ints);
            return -ENOENT;
        }

        if(buf) *buf = data[(t + offset) % cap];

        m.Signal(nullptr);
        cpuRestoreInterrupts(ints);
        return 0;
    }

    int Wait(T *buf, Comparer comparer, int timeout, ...)
    {
        // FIXME: this will fail if there is no more place in the queue for new messages

        int timeLeft = timeout;
        int waitCount = 0;
        VarArgs args;
        do
        {
            timeLeft = m.Wait(timeout < 0 ? 0 : timeout, timeout == 0, false);
            if(timeLeft < 0) break;
            T *cur = data + ((t + waitCount) % cap);
            ++waitCount;
            VarArgStart(args, timeout);
            int cmp = comparer(buf, cur, args);
            VarArgEnd(args);
            if(cmp)
            {
                if(buf) *buf = *cur;
                break;
            }
        } while(timeout < 0 || timeLeft);

        while(waitCount--)
            m.Signal(nullptr);
        return timeLeft;

        return -ENOSYS;
    }

    ~MessageQueue()
    {
        if(data) delete[] data;
    }
};
