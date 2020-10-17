#include "meta/indexable.hpp"

/** 
 * Defines a dynamic indexing functionality for a particular type
 */
#define IMPLEMENT_PROPERTY_MAP(Type, FuncName, PtrMap)\
const Type* Indexable::FuncName(const std::string& Key) const\
{\
    size_t PeriodFound = Key.find(".");\
\
    if (PeriodFound != std::string::npos)\
    {\
        std::string SubobjectKey {Key, 0, PeriodFound};\
        std::string FieldString {Key, PeriodFound + 1, Key.size()};\
    \
        auto It = mMap.PtrMapIndexable.find(SubobjectKey);\
        if (It != mMap.PtrMapIndexable.end())\
        {\
            if (It->second != nullptr)\
            {\
                return It->second->FuncName(FieldString);\
            }\
        }\
    }\
    \
    auto It = mMap.PtrMap.find(Key);\
    \
    if (It != mMap.PtrMap.end())\
    {\
        return It->second;\
    }\
    \
    return nullptr;\
}

IMPLEMENT_PROPERTY_MAP(int, GetPtrInt, PtrMapInt)
IMPLEMENT_PROPERTY_MAP(double, GetPtrDouble, PtrMapDouble)
IMPLEMENT_PROPERTY_MAP(bool, GetPtrBool, PtrMapBool)
IMPLEMENT_PROPERTY_MAP(Axis3, GetPtrAxis3, PtrMapAxis3)
IMPLEMENT_PROPERTY_MAP(Quaternion, GetPtrQuaternion, PtrMapQuaternion)
