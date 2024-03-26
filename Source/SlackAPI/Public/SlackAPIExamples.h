#pragma once
#include "CoreMinimal.h"

#if !UE_BUILD_SHIPPING

namespace SlackAPIExamples
{
	SLACKAPI_API void PostText(const FString& Token, const FString& Channel);
	SLACKAPI_API void PostBlock(const FString& Token, const FString& Channel);
	SLACKAPI_API void PostAttachments(const FString& Token, const FString& Channel);
	SLACKAPI_API void PostThread(const FString& Token, const FString& Channel);
	SLACKAPI_API void UploadFile(const FString& Token, const FString& Channel);
	SLACKAPI_API void UploadMultiFiles(const FString& Token, const FString& Channel);
};

#endif
