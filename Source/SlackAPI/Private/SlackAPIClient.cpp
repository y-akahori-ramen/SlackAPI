#include "SlackAPIClient.h"

#include "HttpManager.h"
#include "HttpModule.h"
#include "OpenAPIChatApiOperations.h"
#include "OpenAPIFileUploadApiOperations.h"


FSlackAPIClient::FSlackAPIClient(const FString& APIToken)
	: APIToken(APIToken),
	  ChatAPI(MakeShared<SlackAPI::OpenAPIChatApi>()),
	  FileUploadAPI(MakeShared<SlackAPI::OpenAPIFileUploadApi>())
{
	ChatAPI->SetURL(TEXT("https://slack.com"));
	FileUploadAPI->SetURL(TEXT("https://slack.com"));

	ChatAPI->AddHeaderParam(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *APIToken));
	FileUploadAPI->AddHeaderParam(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *APIToken));
}

TOptional<SlackAPI::OpenAPIApiChatPostMessagePost200Response> FSlackAPIClient::PostMessage(const SlackAPI::OpenAPIApiChatPostMessagePostRequest& Request)
{
	SlackAPI::OpenAPIChatApi::ApiChatPostMessagePostRequest request;
	request.OpenAPIApiChatPostMessagePostRequest = Request;
	SlackAPI::OpenAPIChatApi::FApiChatPostMessagePostDelegate postDelegate;

	TOptional<SlackAPI::OpenAPIApiChatPostMessagePost200Response> result;
	postDelegate.BindLambda([&result](const SlackAPI::OpenAPIChatApi::ApiChatPostMessagePostResponse& Response)
	{
		if (Response.IsSuccessful())
		{
			result = Response.Content;
		}
	});


	FHttpRequestPtr r = ChatAPI->ApiChatPostMessagePost(request, postDelegate);
	FlushHttpRequests();
	return result;
}

TOptional<SlackAPI::OpenAPIApiFilesUploadPost200Response> FSlackAPIClient::FilesUpload(const SlackAPI::OpenAPIFileUploadApi::ApiFilesUploadPostRequest& Request)
{
	SlackAPI::OpenAPIFileUploadApi::FApiFilesUploadPostDelegate postDelegate;

	TOptional<SlackAPI::OpenAPIApiFilesUploadPost200Response> result;
	postDelegate.BindLambda([&result](const SlackAPI::OpenAPIFileUploadApi::ApiFilesUploadPostResponse& Response)
	{
		if (Response.IsSuccessful())
		{
			result = Response.Content;
		}
	});

	FHttpRequestPtr r = FileUploadAPI->ApiFilesUploadPost(Request, postDelegate);
	FlushHttpRequests();
	return result;
}

bool FSlackAPIClient::TryParseBlockKibBuilderPayload(const FString& Payload, TArray<TSharedPtr<FJsonObject>>& Out)
{
	Out.Reset();

	TSharedPtr<FJsonObject> root = MakeShared<FJsonObject>();
	const TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Payload);

	if (FJsonSerializer::Deserialize(reader, root))
	{
		const TArray<TSharedPtr<FJsonValue>>* Blocks;
		if (root->TryGetArrayField(TEXT("blocks"), Blocks))
		{
			for (TSharedPtr<FJsonValue> block : *Blocks)
			{
				TSharedPtr<FJsonObject> blockObject = block->AsObject();
				if (blockObject.IsValid())
				{
					Out.Add(blockObject);
				}
			}
		}
	}

	return Out.Num() > 0;
}


void FSlackAPIClient::FlushHttpRequests()
{
	if (IsInGameThread())
	{
		FHttpModule::Get().GetHttpManager().Flush(EHttpFlushReason::Default);
	}
	else
	{
		constexpr float timeoutSec = 5.0f;
		FPlatformProcess::Sleep(timeoutSec);
	}
}
