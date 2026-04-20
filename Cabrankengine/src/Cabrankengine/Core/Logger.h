#pragma once

#include <Common/Logger.h>

#include "Core.h"

// Backwards-compatible alias so existing engine code using cbk::Logger keeps working
namespace cbk {
	using Logger = ::cbk::common::Logger;
}
