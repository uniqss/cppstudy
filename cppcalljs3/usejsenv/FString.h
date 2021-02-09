#pragma once

#include "stl.h"

#include "TArray.h"


#define FString std::string

template < class ContainerT >
void tokenize(const std::string& str, ContainerT& tokens,
	const std::string& delimiters = " ", bool trimEmpty = false)
{
	std::string::size_type pos, lastPos = 0, length = str.length();

	using value_type = typename ContainerT::value_type;
	using size_type = typename ContainerT::size_type;

	while (lastPos < length + 1)
	{
		pos = str.find_first_of(delimiters, lastPos);
		if (pos == std::string::npos)
		{
			pos = length;
		}

		if (pos != lastPos || !trimEmpty)
			tokens.push_back(value_type(str.data() + lastPos,
				(size_type)pos - lastPos));

		lastPos = pos + 1;
	}
}

static int ParseIntoArray(const FString& strInput, TArray<FString>& arrResult, const char* delimiter)
{
	tokenize(strInput, arrResult, delimiter);

	return 0;
}

template <typename RangeType>
static FString Join(const RangeType& Range, const char* Separator)
{
	FString Result;
	bool    First = true;
	for (const auto& Element : Range)
	{
		if (First)
		{
			First = false;
		}
		else
		{
			Result += Separator;
		}

		Result += Element;
	}

	return Result;
}

