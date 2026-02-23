#pragma once

#include "il2cpp/UnityResolve.hpp"
#include <string>
#include <unordered_map>

class classmap {
public:
    classmap() = default;

    class assemblymap {
    public:
        assemblymap(UnityResolve::Assembly* assembly) : m_Assembly(assembly) {}

        class namespacemap {
        public:
            namespacemap(UnityResolve::Assembly* assembly, const std::string& ns) 
                : m_Assembly(assembly), m_Namespace(ns) {}

            class class_struc {
            public:
                class_struc(UnityResolve::Assembly* assembly, const std::string& ns, const std::string& className) 
                    : m_Assembly(assembly), m_Namespace(ns), m_ClassName(className) {
                    m_Class = nullptr;
                    if (m_Assembly) {
                        m_Class = m_Assembly->Get(m_ClassName, m_Namespace);
                    }
                }

                class method_struc {
                public:
                    method_struc(UnityResolve::Method* method) : m_Method(method) {}

                    template<typename T = void*, typename... Args>
                    T operator()(void* instance, Args... args) {
                        if (!m_Method) return T();
                        return m_Method->Invoke<T>(instance, args...);
                    }

                    template<typename T, typename... Args>
                    T call(void* instance = nullptr, Args... args) {
                        if (!m_Method) return T();
                        return m_Method->Invoke<T>(instance, args...);
                    }

                    void* funcptr() const { return m_Method ? m_Method->function : nullptr; }
                    UnityResolve::Method* get() const { return m_Method; }

                private:
                    UnityResolve::Method* m_Method;
                };

                class field_struc {
                public:
                    field_struc(UnityResolve::Field* field) : m_Field(field) {}

                    template<typename T = void*>
                    T get(void* instance = nullptr) {
                        if (!m_Field) return nullptr;
                        if (instance) {
                            return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(instance) + m_Field->offset);
                        }
                        T value;
                        m_Field->GetStaticValue(&value);
                        return value;
                    }

                    template<typename T>
                    void set(void* instance, T value) {
                        if (!m_Field) return;
                        if (instance) {
                            *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(instance) + m_Field->offset) = value;
                        } else {
                            m_Field->SetStaticValue(&value);
                        }
                    }

                    int getOffset() const { return m_Field ? m_Field->offset : 0; }
                    UnityResolve::Field* getField() const { return m_Field; }

                private:
                    UnityResolve::Field* m_Field;
                };

                method_struc operator[](const char* methodName) {
                    if (!m_Class) return method_struc(nullptr);
                    return method_struc(m_Class->Get<UnityResolve::Method>(methodName));
                }

                method_struc operator[](const std::string& methodName) {
                    return (*this)[methodName.c_str()];
                }

                field_struc field(const char* fieldName) {
                    if (!m_Class) return field_struc(nullptr);
                    return field_struc(m_Class->Get<UnityResolve::Field>(fieldName));
                }

                field_struc field(const std::string& fieldName) {
                    return field(fieldName.c_str());
                }

                UnityResolve::Class* getClass() const { return m_Class; }

                template<typename T>
                std::vector<T> FindObjectsByType() {
                    if (!m_Class) return {};
                    return m_Class->FindObjectsByType<T>();
                }

                template<typename T>
                T* New() {
                    if (!m_Class) return nullptr;
                    return m_Class->New<T>();
                }

            private:
                UnityResolve::Assembly* m_Assembly;
                std::string m_Namespace;
                std::string m_ClassName;
                UnityResolve::Class* m_Class;
            };

            class_struc operator[](const char* className) {
                return class_struc(m_Assembly, m_Namespace, className);
            }

            class_struc operator[](const std::string& className) {
                return class_struc(m_Assembly, m_Namespace, className);
            }

        private:
            UnityResolve::Assembly* m_Assembly;
            std::string m_Namespace;
        };

        namespacemap operator[](const char* namespaceName) {
            return namespacemap(m_Assembly, namespaceName);
        }

        namespacemap operator[](const std::string& namespaceName) {
            return namespacemap(m_Assembly, namespaceName);
        }

    private:
        UnityResolve::Assembly* m_Assembly;
    };

    assemblymap operator[](const char* assemblyName) {
        auto* assembly = UnityResolve::Get(assemblyName);
        return assemblymap(assembly);
    }

    assemblymap operator[](const std::string& assemblyName) {
        return (*this)[assemblyName.c_str()];
    }
};