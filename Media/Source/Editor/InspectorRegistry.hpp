#pragma once
#include <Platform.hpp>
#include "IInspectable.hpp"
#include <unordered_map>

namespace MultiStation {


    class InspectorRegistry
    {
    public:
        /**
         * 
         */
        template<typename T>
        void RegisterInspectable(IInspectable& inspector);

        IInspectable* GetInspectable(ObjID id) ;

        bool HasInspectable(ObjID id) ;

    private:
        std::unordered_map<ObjID, IInspectable*> m_Inspectables;
    };



    template<typename T>
    void InspectorRegistry::RegisterInspectable(IInspectable& inspector) {

        ObjID id = GetObjectID<T>();
        m_Inspectables[id] = &inspector;
    }



}
