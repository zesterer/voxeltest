#ifndef LIBVOLUME_COMMON_IO_H
#define LIBVOLUME_COMMON_IO_H

//----LIBRARY----
#include "string"

namespace LibVolume
{
	namespace IO
	{
		enum OutputType
		{
			Info,
			Debug,
			Error
		};

		void output(std::string message, OutputType type = OutputType::Debug);

		bool test(bool test, std::string label = "NULL Test", bool critical = false, bool silent = false);
	}
}

#endif
