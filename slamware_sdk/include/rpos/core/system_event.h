#pragma once

namespace rpos {
    namespace core {

        enum InternalSystemEvent
        {
            InternalSystemEventRelocalizationFail = 1,
            InternalSystemEventRelocalizationSuccess,
            InternalSystemEventBackHomeFail,
            InternalSystemEventAlignMap,
            InternalSystemEventSideBrushStall,
            InternalSystemEventRollingBrushStall,
            InternalSystemEventBumperDown,
            InternalSystemEventWallIrDown,
            InternalSystemEventCliffDown,
            InternalSystemEventBlowerStall,
            InternalSystemEventDustbinGone,
            InternalSystemEventMotionJam,
            InternalSystemEventIRDown,
            InternalSystemEventIMUDown,
            InternalSystemEventMopDown,
            InternalSystemEventMopUp,
            InternalSystemEventSetMapDone,
            InternalSystemEventSetPoseDone,
            InternalSystemEventMapUpdateEnable,
            InternalSystemEventMapUpdateDisable,
            InternalSystemEventPathOccupied,
            InternalSystemEventTrappedInForbiddenArea
        };

    }
}