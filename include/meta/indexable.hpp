#pragma once

#include <map>
#include <string>

/** Generates an individual entry to an indexable map */
#define MAP_PTR(X) {#X, &X}

// Forward declare some classes
class Axis3;
class Quaternion;

/** 
 * Map of indexable parameters
 */
struct IndexMap
{
    const std::map<std::string, const class Indexable*> PtrMapIndexable{};
    const std::map<std::string, const int*> PtrMapInt{};
    const std::map<std::string, const double*> PtrMapDouble{};
    const std::map<std::string, const bool*> PtrMapBool{};
    const std::map<std::string, const Axis3*> PtrMapAxis3{};
    const std::map<std::string, const Quaternion*> PtrMapQuaternion{};
};

/** 
 * Inheritable functionality for a dynamically indexable object. 
 */
class Indexable
{
public:

    /** 
     * Instantiate the dynamic map
     */
    Indexable(const IndexMap& pMap) : mMap{pMap} { }

    virtual ~Indexable() {}

    /** 
     * Gets an immutable pointer to an int type index by the given `Key`
     * @param Key Key of the indexable integer parameter to retrieve
     * @return constant pointer reference to the parameter of interest. Returns 
     * nullptr if the given `Key` cannot be retrieved
     */
    const int* GetPtrInt(const std::string& Key) const;

    /** 
     * Gets an immutable pointer to an double type index by the given `Key`
     * @param Key Key of the indexable double parameter to retrieve
     * @return constant pointer reference to the parameter of interest. Returns 
     * nullptr if the given `Key` cannot be retrieved
     */
    const double* GetPtrDouble(const std::string& Key) const;

    /** 
     * Gets an immutable pointer to a bool type index by the given `Key`
     * @param Key Key of the indexable bool parameter to retrieve
     * @return constant pointer reference to the parameter of interest. Returns 
     * nullptr if the given `Key` cannot be retrieved
     */
    const bool* GetPtrBool(const std::string& Key) const;

    /** 
     * Gets an immutable pointer to an Axis3 type index by the given `Key`
     * @param Key Key of the indexable Axis3 parameter to retrieve
     * @return constant pointer reference to the parameter of interest. Returns 
     * nullptr if the given `Key` cannot be retrieved    
     */
    const Axis3* GetPtrAxis3(const std::string& Key) const;

    /** 
     * Gets an immutable pointer to an Quaternion type index by the given `Key`
     * @param Key Key of the indexable Quaternion parameter to retrieve
     * @return constant pointer reference to the parameter of interest. Returns 
     * nullptr if the given `Key` cannot be retrieved    
     */
    const Quaternion* GetPtrQuaternion(const std::string& Key) const;    

private:
    // Dynamic field map
    const IndexMap mMap{};
};