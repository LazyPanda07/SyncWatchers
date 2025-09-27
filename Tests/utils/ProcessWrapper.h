#pragma once

#include <reproc++/run.hpp>

namespace process
{
	class ProcessWrapper
	{
	private:
		reproc::process process;

	public:
		ProcessWrapper(reproc::process&& process) noexcept;

		~ProcessWrapper();
	};
}
