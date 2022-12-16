//
// Created by Flo on 14/12/2022.
//

#ifndef PATHFINDER_RTTI_HPP
#define PATHFINDER_RTTI_HPP

#include <string>
#include <utility>

#define DECLARE_RTTI(ClassName, ParentType) \
    static Engine::RTTI* getClassRTTI() { static Engine::RTTI rtti{#ClassName, ParentType::getClassRTTI()}; return &rtti; } \
    virtual Engine::RTTI* getInstanceRTTI() { return getClassRTTI(); }

namespace Engine
{
    class RTTI
    {
    public:
        using Typename = std::string;

        inline RTTI(Typename className, RTTI* parentRTTI)
            : m_className(std::move(className))
            , m_parentRTTI(parentRTTI)
        {}

        [[nodiscard]] inline const Typename& getClassName() const { return m_className; }
        [[nodiscard]] inline const RTTI* getParentRTTI() const { return m_parentRTTI; }

    private:
        Typename m_className;
        RTTI* m_parentRTTI;
    };

    class IHasRTTI
    {
    public:
        virtual RTTI* getInstanceRTTI() = 0;
    };

    class NoRTTIAncestor
    {
    public:
        static RTTI* getClassRTTI() { return nullptr; }
    };
}

#endif //PATHFINDER_RTTI_HPP
