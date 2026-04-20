#pragma once

#include <Common/Logger.h>

#include "Core.h"

// Backwards-compatible alias so existing engine code using cabrankengine::Logger keeps working
namespace cabrankengine {
	using Logger = ::cbk::common::Logger;
}
