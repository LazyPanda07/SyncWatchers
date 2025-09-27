#include "ProcessWrapper.h"

namespace process
{
	ProcessWrapper::ProcessWrapper(reproc::process&& process) noexcept :
		process(std::move(process))
	{

	}

	ProcessWrapper::~ProcessWrapper()
	{
		process.kill();
	}
}
