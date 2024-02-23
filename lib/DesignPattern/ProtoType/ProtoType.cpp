#include "ProtoType.h"

#include <spdlog/spdlog.h>

#include "ProtoType.h"

using namespace ProtoType;

std::array<ObserveHelper*, MaxSize> ObserveHelper::observe_part{nullptr};

NavigationObserver NavigationObserver::m_navi_observe{};
SlamObserver SlamObserver::m_slam_observe{};

int main(void) {
    ObserveHelper& observer = ObserveHelper::getInstance();

    observer.onCancelHandle();

    const ObserveHelper& tmp =
        observer.getMoudlePrototype(ModuleType::kNavigationObserve);

    tmp.setSubmoduleState(BaseMotionAction);

    observer.onCancelHandle();

    spdlog::info("Show Prototype code");
}
