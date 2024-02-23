#pragma once

#include <spdlog/spdlog.h>

#include <array>
#include <type_traits>
#include <vector>

namespace ProtoType {

/**
 * @brief ModuleType 模块类别
 */
enum ModuleType : int {
    kNavigationObserve = (0),  // 导航
    kSlamObserve,              // slam
    kModuleMaxNum,             // 最大数
};
// 编译期获取观测者数量
constexpr std::size_t MaxSize = ModuleType::kModuleMaxNum;

/**
 * @brief 子模块当前状态
 */
enum ModuleState : int {
    /** 导航相关 BEGIN *******************************/
    kNavigationBegin = (0),

    BaseMotionAction,
    ExploreAction,
    AutoCleanAction,

    kNavigationEnd,
    /** 导航相关 END *********************************/

    /** SLAM相关 BEGIN ******************************/
    kSlamBegin,

    kIdle,
    /* 建图相关 */
    kMappingStarted,
    kMappingPaused,
    /* 地图相关 */
    kLoadMapOK,
    kLoadMapFailed,
    /* 重定位相关 */
    kRelocating,
    kRelocateSuccess,
    kRelocateFaild,
    kFinishedSlam,

    kSlamEnd,
    /** SLAM相关 END ********************************/

    /** 底盘相关 BEGIN *******************************/
    kChassisBegin,

    kChassisEnd,
    /** 底盘相关 END *********************************/

    /** 基站相关 BEGIN *******************************/
    kDockBegin,

    kDockEnd,
    /** 基站相关 END *********************************/

};

/**
 * @brief 观测者基类
 */
class ObserveHelper {
public:
    ObserveHelper() {}
    virtual ~ObserveHelper() = default;

    static ObserveHelper& getInstance() {
        static ObserveHelper tmp;
        return tmp;
    }

public:
    /**
     * @brief 查表找到原型，根据原型生成新的观测对象
     *        tag : 暂时没用到
     *
     * @param type 生成对象类型
     * @return ObserveHelper* 指向观测对象的父类指针
     */
    static ObserveHelper* findAndClone(ModuleType type) {
        for (int i = 0; i < ModuleType::kModuleMaxNum; i++) {
            if (observe_part[i]->returnType() == type) {
                return observe_part[i]->clone();
            }
        }
    }

    /**
     * @brief 当cancel操作触发时，各模块对应处理接口
     *
     */
    void onCancelHandle() {
        for (int i = 0; i < ModuleType::kModuleMaxNum; i++) {
            observe_part[i]->onCancel();
        }
    }

    /**
     * @brief Get the Moudle Prototype object
     *
     * @param index
     * @return ObserveHelper*
     */
    const ObserveHelper& getMoudlePrototype(ModuleType index) {
        return *observe_part[index];
    }

    /* 子类接口：设置子模块状态 */
    virtual void setSubmoduleState(ModuleState state) const {};

protected:
    /* 子类接口：复制一份自己 */
    virtual ObserveHelper* clone(){};

    /* 子类接口：返回自己类型 */
    virtual ModuleType returnType(){};

    /* 子类接口：取消任务时模块回调处理 */
    virtual void onCancel(){};

    /**
     * @brief 在子类构造时，把自身添加到父类中去统一管理
     *
     * @param obs 子类自身
     */
    template <class Type>
    static void addPrototype(ObserveHelper* obs) {
        static_assert(
            std::is_base_of<ObserveHelper, Type>::value,
            "[addPrototype] accepts only classed derived from ObserveHelper");

        constexpr bool construct_clone =
            std::is_constructible<Type, int>::value;
        static_assert(construct_clone,
                      "[addPrototype] derived class should have "
                      "default_construct and clone_construct");

        if (observe_part.at(obs->returnType()) == nullptr) {
            spdlog::info("insert object module , {} , {}", obs->returnType(),
                         MaxSize);
            observe_part.at(obs->returnType()) = obs;
        } else {
            spdlog::warn("duplicate object , {}", obs->returnType());
        }
    }

protected:
    mutable ModuleState m_module_state;

private:
    static std::array<ObserveHelper*, MaxSize> observe_part;
};  // namespace ProtoType

/**
 * @brief navigation观察者，作用如下：
 *          1、观测navigation的状态（当前处于哪个动作）
 *          2、根据模块状态，执行相应的处理
 */
class NavigationObserver : protected ObserveHelper {
public:
    void onCancel() {
        spdlog::info("call NavigationObserver onCancel , {}", m_module_state);
    }

    ModuleType returnType() { return ModuleType::kNavigationObserve; }

    ObserveHelper* clone() { return new NavigationObserver(0); }

    void setSubmoduleState(ModuleState state) const {
        if (state < kNavigationBegin || state > kNavigationEnd) {
            spdlog::error("error input state : {}", state);
        }
        m_module_state = state;
    }

    explicit NavigationObserver(int n) {}

private:
    NavigationObserver() { addPrototype<NavigationObserver>(this); }

private:
    static NavigationObserver m_navi_observe;

};  // class NavigationObserver

/**
 * @brief 观察slam模块状态，根据不同状态做不同处理
 */
class SlamObserver : protected ObserveHelper {
public:
    void onCancel() {
        spdlog::info("call SlamObserver onCancel , {}", m_module_state);
    }

    ModuleType returnType() { return ModuleType::kSlamObserve; }

    ObserveHelper* clone() { return new SlamObserver(0); }

    void setSubmoduleState(ModuleState state) const {
        if (state < kSlamBegin || state > kSlamEnd) {
            spdlog::error("error input state : {}", state);
        }
        m_module_state = state;
    }

    explicit SlamObserver(int n) {}

private:
    SlamObserver() { addPrototype<SlamObserver>(this); }

private:
    static SlamObserver m_slam_observe;
};  // class SlamObserver
};  // namespace ProtoType

// file end