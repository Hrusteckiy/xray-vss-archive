#pragma warning(push)
#pragma warning(disable:4503)

#include "stdafx.h"

namespace boost {
	void throw_exception(const std::exception& A) {
		Debug.fatal("Boost exception raised %s", A.what());
	}
}
