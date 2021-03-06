/*
 * Copyright (c) 2017 - 2018, Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once
#include "runtime/utilities/api_intercept.h"

namespace OCLRT {
// IDs for easy system times identification
enum SystemCallsIds {
    SYSTIMER_ID_OPENADAPTERFROMHDC = 1,
    SYSTIMER_ID_OPENADAPTERFROMLUID = 2,
    SYSTIMER_ID_CLOSEADAPTER = 3,
    SYSTIMER_ID_QUERYADAPTERINFO = 4,
    SYSTIMER_ID_ESCAPE = 5,
    SYSTIMER_ID_CREATEDEVICE = 6,
    SYSTIMER_ID_DESTROYDEVICE = 7,
    SYSTIMER_ID_CREATECONTEXT = 8,
    SYSTIMER_ID_DESTROYCONTEXT = 9,
    SYSTIMER_ID_CREATEALLOCATION = 10,
    SYSTIMER_ID_DESTROYALLOCATION = 11,
    SYSTIMER_ID_OPENRESOURCE = 12,
    SYSTIMER_ID_QUERYRESOURCEINFO = 13,
    SYSTIMER_ID_LOCK = 14,
    SYSTIMER_ID_UNLOCK = 15,
    SYSTIMER_ID_RENDER = 16,
    SYSTIMER_ID_CREATESYNCHRONIZATIONOBJECT = 17,
    SYSTIMER_ID_DESTROYSYNCHRONIZATIONOBJECT = 18,
    SYSTIMER_ID_SIGNALSYNCHRONIZATIONOBJECT = 19,
    SYSTIMER_ID_WAITFORSYNCHRONIZATIONOBJECT = 20,
    SYSTIMER_ID_CREATESYNCHRONIZATIONOBJECT2 = 21,
    SYSTIMER_ID_GETDEVICESTATE = 22,
    SYSTIMER_ID_MAKERESIDENT = 23,
    SYSTIMER_ID_EVICT = 24,
    SYSTIMER_ID_WAITFORSYNCHRONIZATIONOBJECTFROMCPU = 25,
    SYSTIMER_ID_SIGNALSYNCHRONIZATIONOBJECTFROMCPU = 26,
    SYSTIMER_ID_WAITFORSYNCHRONIZATIONOBJECTFROMGPU = 27,
    SYSTIMER_ID_SIGNALSYNCHRONIZATIONOBJECTFROMGPU = 28,
    SYSTIMER_ID_CREATEPAGINGQUEUE = 29,
    SYSTIMER_ID_D3DDDI_DESTROYPAGINGQUEUE = 30,
    SYSTIMER_ID_LOCK2 = 31,
    SYSTIMER_ID_UNLOCK2 = 32,
    SYSTIMER_ID_INVALIDATECACHE = 33,
    SYSTIMER_ID_D3DDDI_MAPGPUVIRTUALADDRESS = 34,
    SYSTIMER_ID_D3DDDI_RESERVEGPUVIRTUALADDRESS = 35,
    SYSTIMER_ID_FREEGPUVIRTUALADDRESS = 36,
    SYSTIMER_ID_UPDATEGPUVIRTUALADDRESS = 37,
    SYSTIMER_ID_CREATECONTEXTVIRTUAL = 38,
    SYSTIMER_ID_SUBMITCOMMAND = 39,
    SYSTIMER_ID_OPENSYNCOBJECTFROMNTHANDLE2 = 40,
    SYSTIMER_ID_OPENSYNCOBJECTNTHANDLEFROMNAME = 41,
    SYSTIMER_ID_DESTROYALLOCATION2 = 42,
    SYSTIMER_ID_REGISTERTRIMNOTIFICATION = 43,
    SYSTIMER_ID_UNREGISTERTRIMNOTIFICATION = 44,
    SYSTIMER_ID_QUERYRESOURCEINFOFROMNTHANDLE = 45,
    SYSTIMER_ID_OPENRESOURCEFROMNTHANDLE = 46,

    SYSTIMER_ID_SLEEP_0 = 100,
    SYSTIMER_ID_WAIT_FOR_KMD = 200,
    SYSTIMER_ID_CMD_COMPLETE = 300,

    SYSTIMER_ID_OGL = 400,
};

template <bool UseTimer, typename Param>
class ThkWrapper {
    typedef NTSTATUS(APIENTRY *Func)(Param);

  public:
    Func mFunc = nullptr;

    inline NTSTATUS operator()(Param param) const {
        if (UseTimer) {
            SYSTEM_ENTER()
            NTSTATUS Status;
            Status = mFunc(param);
            unsigned int ID = getId<Param>();
            SYSTEM_LEAVE(ID);
            return Status;
        } else {
            return mFunc(param);
        }
    }

    template <class T>
    inline T &operator=(T func) {
        return mFunc = func;
    }

    // This operator overload is for implicit casting ThkWrapper struct to Function Pointer in GetPfn methods like GetEscapePfn() or for comparing against NULL function pointer
    operator Func() const {
        return mFunc;
    }

  private:
    // Default template for GetID( ) for Thk function, causing compilation error !!
    // Returns ID for specific ThkWrapper type
    template <class Param>
    unsigned int getId() const {
        static_assert(0, "Template specialization for GetID is required for each new THKWrapper");
        return 0;
    }

// Template specializations of GetID(), required for every new Thk function
#define GET_ID(TYPE, VALUE)            \
    template <>                        \
    unsigned int getId<TYPE>() const { \
        return VALUE;                  \
    }

    GET_ID(D3DKMT_OPENADAPTERFROMHDC *, SYSTIMER_ID_OPENADAPTERFROMHDC)
    GET_ID(D3DKMT_OPENADAPTERFROMLUID *, SYSTIMER_ID_OPENADAPTERFROMLUID)
    GET_ID(CONST D3DKMT_CLOSEADAPTER *, SYSTIMER_ID_CLOSEADAPTER)
    GET_ID(CONST D3DKMT_QUERYADAPTERINFO *, SYSTIMER_ID_QUERYADAPTERINFO)
    GET_ID(CONST D3DKMT_ESCAPE *, SYSTIMER_ID_ESCAPE)
    GET_ID(D3DKMT_CREATEDEVICE *, SYSTIMER_ID_CREATEDEVICE)
    GET_ID(CONST D3DKMT_DESTROYDEVICE *, SYSTIMER_ID_DESTROYDEVICE)
    GET_ID(D3DKMT_CREATECONTEXT *, SYSTIMER_ID_CREATECONTEXT)
    GET_ID(CONST D3DKMT_DESTROYCONTEXT *, SYSTIMER_ID_DESTROYCONTEXT)
    GET_ID(D3DKMT_CREATEALLOCATION *, SYSTIMER_ID_CREATEALLOCATION)
    GET_ID(CONST D3DKMT_DESTROYALLOCATION *, SYSTIMER_ID_DESTROYALLOCATION)
    GET_ID(D3DKMT_OPENRESOURCE *, SYSTIMER_ID_OPENRESOURCE)
    GET_ID(D3DKMT_QUERYRESOURCEINFO *, SYSTIMER_ID_QUERYRESOURCEINFO)
    GET_ID(D3DKMT_LOCK *, SYSTIMER_ID_LOCK)
    GET_ID(CONST D3DKMT_UNLOCK *, SYSTIMER_ID_UNLOCK)
    GET_ID(D3DKMT_RENDER *, SYSTIMER_ID_RENDER)
    GET_ID(D3DKMT_CREATESYNCHRONIZATIONOBJECT *, SYSTIMER_ID_CREATESYNCHRONIZATIONOBJECT)
    GET_ID(CONST D3DKMT_DESTROYSYNCHRONIZATIONOBJECT *, SYSTIMER_ID_DESTROYSYNCHRONIZATIONOBJECT)
    GET_ID(CONST D3DKMT_SIGNALSYNCHRONIZATIONOBJECT *, SYSTIMER_ID_SIGNALSYNCHRONIZATIONOBJECT)
    GET_ID(D3DKMT_WAITFORSYNCHRONIZATIONOBJECT *, SYSTIMER_ID_WAITFORSYNCHRONIZATIONOBJECT)
    GET_ID(D3DKMT_CREATESYNCHRONIZATIONOBJECT2 *, SYSTIMER_ID_CREATESYNCHRONIZATIONOBJECT2)
    GET_ID(D3DKMT_GETDEVICESTATE *, SYSTIMER_ID_GETDEVICESTATE)
    GET_ID(D3DDDI_MAKERESIDENT *, SYSTIMER_ID_MAKERESIDENT)
    GET_ID(D3DKMT_EVICT *, SYSTIMER_ID_EVICT)
    GET_ID(CONST D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMCPU *, SYSTIMER_ID_WAITFORSYNCHRONIZATIONOBJECTFROMCPU)
    GET_ID(CONST D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMCPU *, SYSTIMER_ID_SIGNALSYNCHRONIZATIONOBJECTFROMCPU)
    GET_ID(CONST D3DKMT_WAITFORSYNCHRONIZATIONOBJECTFROMGPU *, SYSTIMER_ID_WAITFORSYNCHRONIZATIONOBJECTFROMGPU)
    GET_ID(CONST D3DKMT_SIGNALSYNCHRONIZATIONOBJECTFROMGPU *, SYSTIMER_ID_SIGNALSYNCHRONIZATIONOBJECTFROMGPU)
    GET_ID(D3DKMT_CREATEPAGINGQUEUE *, SYSTIMER_ID_CREATEPAGINGQUEUE)
    GET_ID(D3DDDI_DESTROYPAGINGQUEUE *, SYSTIMER_ID_D3DDDI_DESTROYPAGINGQUEUE)
    GET_ID(D3DKMT_LOCK2 *, SYSTIMER_ID_LOCK2)
    GET_ID(CONST D3DKMT_UNLOCK2 *, SYSTIMER_ID_UNLOCK2)
    GET_ID(CONST D3DKMT_INVALIDATECACHE *, SYSTIMER_ID_INVALIDATECACHE)
    GET_ID(D3DDDI_MAPGPUVIRTUALADDRESS *, SYSTIMER_ID_D3DDDI_MAPGPUVIRTUALADDRESS)
    GET_ID(D3DDDI_RESERVEGPUVIRTUALADDRESS *, SYSTIMER_ID_D3DDDI_RESERVEGPUVIRTUALADDRESS)
    GET_ID(CONST D3DKMT_FREEGPUVIRTUALADDRESS *, SYSTIMER_ID_FREEGPUVIRTUALADDRESS)
    GET_ID(CONST D3DKMT_UPDATEGPUVIRTUALADDRESS *, SYSTIMER_ID_UPDATEGPUVIRTUALADDRESS)
    GET_ID(D3DKMT_CREATECONTEXTVIRTUAL *, SYSTIMER_ID_CREATECONTEXTVIRTUAL)
    GET_ID(CONST D3DKMT_SUBMITCOMMAND *, SYSTIMER_ID_SUBMITCOMMAND)
    GET_ID(D3DKMT_OPENSYNCOBJECTFROMNTHANDLE2 *, SYSTIMER_ID_OPENSYNCOBJECTFROMNTHANDLE2)
    GET_ID(D3DKMT_OPENSYNCOBJECTNTHANDLEFROMNAME *, SYSTIMER_ID_OPENSYNCOBJECTNTHANDLEFROMNAME)
    GET_ID(CONST D3DKMT_DESTROYALLOCATION2 *, SYSTIMER_ID_DESTROYALLOCATION2)
    GET_ID(D3DKMT_REGISTERTRIMNOTIFICATION *, SYSTIMER_ID_REGISTERTRIMNOTIFICATION)
    GET_ID(D3DKMT_UNREGISTERTRIMNOTIFICATION *, SYSTIMER_ID_UNREGISTERTRIMNOTIFICATION)
    GET_ID(D3DKMT_OPENRESOURCEFROMNTHANDLE *, SYSTIMER_ID_OPENRESOURCEFROMNTHANDLE)
    GET_ID(D3DKMT_QUERYRESOURCEINFOFROMNTHANDLE *, SYSTIMER_ID_QUERYRESOURCEINFOFROMNTHANDLE)
};
} // namespace OCLRT
