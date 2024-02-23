#pragma once

#include <spdlog/spdlog.h>

#include <atomic>
#include <csignal>
#include <functional>  // std::function
#include <memory>
#include <unordered_map>

class ObserverBase {
public:
    ObserverBase() = default;
    virtual ~ObserverBase() = default;

public:
    virtual void onStart(void) = 0;
    virtual void onPause(void) = 0;
    virtual void onResume(void) = 0;
    virtual void onCancel(void) = 0;

    virtual std::string getModuleName() = 0;
};

template <class Type>
class ModuleObserver : public ObserverBase {
    ModuleObserver(const ModuleObserver&) = delete;
    ModuleObserver operator=(const ModuleObserver&) = delete;

public:
    ModuleObserver() = delete;
    ModuleObserver(std::function<void(const Type&)> onStart,
                   std::function<void(const Type&)> onPause,
                   std::function<void(const Type&)> onResume,
                   std::function<void(const Type&)> onCancel)
        : m_cbOnStart(onStart),
          m_cbOnPause(onPause),
          m_cbOnResume(onResume),
          m_cbOnCancel(onCancel) {
        if (!m_state.is_lock_free()) {
            spdlog::warn("{} is not lock free", typeid(Type).name());
        }
    }

    ~ModuleObserver() = default;

    void setModuleState(const Type& type) { m_state.store(type); }
    Type getModuleState() { return m_state.load(); }

    std::string getModuleName() { return typeid(Type).name(); }

private:
    void onStart(void) {
        if (m_cbOnStart != nullptr) {
            auto state = this->getModuleState();
            m_cbOnStart(state);
        }
    }

    void onPause(void) {
        if (m_cbOnPause != nullptr) {
            auto state = this->getModuleState();
            m_cbOnPause(state);
        }
    }

    void onResume(void) {
        if (m_cbOnResume != nullptr) {
            auto state = this->getModuleState();
            m_cbOnResume(state);
        }
    }

    void onCancel(void) {
        if (m_cbOnCancel != nullptr) {
            auto state = this->getModuleState();
            m_cbOnCancel(state);
        }
    }

private:
    std::function<void(const Type&)> m_cbOnStart{nullptr};
    std::function<void(const Type&)> m_cbOnPause{nullptr};
    std::function<void(const Type&)> m_cbOnResume{nullptr};
    std::function<void(const Type&)> m_cbOnCancel{nullptr};

    std::atomic<Type> m_state;
};

class Subject {
public:
    Subject() {}
    ~Subject() {}

    template <class Type>
    std::shared_ptr<ModuleObserver<Type>> createModuleObserver(
        std::function<void(const Type&)> onStart,
        std::function<void(const Type&)> onPause,
        std::function<void(const Type&)> onResume,
        std::function<void(const Type&)> onCancel) {
        auto ret = std::make_shared<ModuleObserver<Type>>(onStart, onPause,
                                                          onResume, onCancel);
        addObserver<Type>(ret);
        return ret;
    }

    void onStart() {
        for (const auto& tmp : m_observerFactory) {
            tmp.second->onStart();
        }
    }

    void onPause() {
        for (const auto& tmp : m_observerFactory) {
            tmp.second->onPause();
        }
    }

    void onResume() {
        for (const auto& tmp : m_observerFactory) {
            tmp.second->onResume();
        }
    }

    void onCancel() {
        for (const auto& tmp : m_observerFactory) {
            tmp.second->onCancel();
        }
    }

private:
    template <class Type>
    void addObserver(std::shared_ptr<ObserverBase> prt) {
        m_observerFactory.insert({prt->getModuleName(), prt});
    }

private:
    std::unordered_multimap<std::string, std::shared_ptr<ObserverBase>>
        m_observerFactory;
};
