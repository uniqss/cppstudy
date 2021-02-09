#pragma once

namespace AlgoImpl
{
	template <typename T>
	inline void Reverse(T* Array, int ArraySize)
	{
		for (int32 i = 0, i2 = ArraySize - 1; i < ArraySize / 2 /*rounding down*/; ++i, --i2)
		{
			Swap(Array[i], Array[i2]);
		}
	}
}

namespace Algo
{
	template <typename T, int ArraySize>
	inline void Reverse(T(&Array)[ArraySize])
	{
		return AlgoImpl::Reverse((T*)Array, ArraySize);
	}
	template <typename T>
	inline void Reverse(T* Array, int ArraySize)
	{
		return AlgoImpl::Reverse(Array, ArraySize);
	}
	template <typename ContainerType>
	inline void Reverse(ContainerType& Container)
	{
		return AlgoImpl::Reverse(Container.GetData(), Container.size());
	}
}
