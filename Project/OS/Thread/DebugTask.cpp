//
// Created by liaohy on 24-11-15.
//

#include <algorithm>
#include <WDG/SuperIWDG.hpp>
#include "usbd_cdc_if.h"
#include "cmsis_os.h"
#include "Components/Detect.h"
#include "Components/Count.h"
#include "Components/Manager.h"
#include "Components/Format.h"
#include "SysTick/SysTick.hpp"

using namespace EP::Component;

template<String auto Prefix>
class Logger {
    char buffer[1024]{};
    std::size_t mLen = 0;
public:
    Logger() {
        for (int i = 0; i < Prefix.len() - 1; ++i) {
            buffer[i] = Prefix.chars[i];
        }
    }

    template<String auto fmt, typename... Args>
    void log(Args &&... args) {
        using ArgTuple = std::tuple<Args...>;
        using Indices = std::make_index_sequence<sizeof...(Args)>;
        constexpr int len = Prefix.len() - 1;
        mLen += for_each_impl<fmt, ArgTuple, Indices>::execute(
            buffer+len+mLen, std::forward_as_tuple(std::forward<Args>(args)...));
    }

    // void print(const char *msg, uint32_t mLen) {
    //     constexpr int len = Prefix.len() - 1;
    //     memcpy(buffer+len, msg, mLen);
    //     CDC_Transmit_HS(reinterpret_cast<uint8_t*>(buffer), len + mLen);
    // }

    void flush() {
        constexpr int len = Prefix.len() - 1;
        CDC_Transmit_HS(reinterpret_cast<uint8_t *>(buffer), len + mLen);
        mLen = 0;
    }
};
std::size_t getSys() {
    return osKernelSysTick();
}
Logger<Str{"----------\nlog:\n"}> logger;

class DebugUnit {
public:
    DebugUnit() {
        detector = Manager<Detect<getSys>,20>::instance().make_managed(1000,
            detectDep::function{.func = [](detectDep::State s, void * argument) {
                auto &self = *static_cast<DebugUnit *>(argument);
                switch (s) {
                    case detectDep::WORKING:
                        logger.log<Str{"I am WORKING.\n"}>();
                        break;
                    case detectDep::MISSING:
                        self.count->add();
                        logger.log<Str{"detector id: {} MISSING count: {}.\n"}>(self.detector.getIndex(), self.count->getCount());
                        break;
                    case detectDep::RECOVER:
                        logger.log<Str{"RECOVER time: {}.\n"}>(getSys());
                        break;
                    case detectDep::LOSE:
                        logger.log<Str{"LOSE time: {}.\n"}>(getSys());
                        break;
                }
            }, .param = this});
        count = Manager<Count,20>::instance().make_managed();
    }
    Manager<Count,20>::managed_ptr count;
private:
    Manager<Detect<getSys>,20>::managed_ptr detector;
};

DebugUnit debugUnit{};
DebugUnit debugUnit2{};

extern "C" void OS_DebugTask(void const *argument) {
    uint32_t time    = 0;
    SuperIWDG::instance().update();
    auto& detectInstance = Manager<Detect<getSys>,20>::instance();
    auto& countInstance = Manager<Count,20>::instance();
    while (1) {
        SuperIWDG::instance().update();
        ++time;
        // debugUnit.count->add();
        if (time % 500==0) {
            auto start = getSys();
            auto start_ = Sys::get_tick();
            for (auto & detector: detectInstance) {
                detector.callbackFun();
            }
            logger.flush();
            int32_t end_ = start_ - Sys::get_tick();
            auto end = getSys() - start;
            if (end_ < 0) {--end_;}
            logger.log<Str{"last time spent: {.3}.\n"}>(end + end_/480'000.f);
            int load = SysTick->LOAD;
            logger.log<Str{"time load: {}.\n"}>(load);
            // for (auto & count: countInstance) {
            //     count.record();
            //     logger.log<Str{"my count: {}.\n"}>(count.getMax());
            // }
        }
        if (time % 2000 == 0) {
            for (auto & detector: detectInstance) {
                detector.update();
            }
        }
        osDelay(1);
    }
}
