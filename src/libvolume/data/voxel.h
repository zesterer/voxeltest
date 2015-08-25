#ifndef LIBVOLUME_DATA_VOXEL_H
#define LIBVOLUME_DATA_VOXEL_H

namespace LibVolume
{
	namespace Data
	{
		struct Voxel
		{
			public:
				int type;
				float density;
				
				void* metadata;
			
				int getType();
				float getDensity();
				void* getMeta();
		};
	}
}

#endif
