#include "SlackAPIExamples.h"
#include "OpenAPIFileUploadApiOperations.h"
#include "SlackAPIClient.h"

#if !UE_BUILD_SHIPPING

namespace SlackAPIExamples
{
	void PostText(const FString& Token, const FString& Channel)
	{
		TSharedRef<FSlackAPIClient> Client = MakeShared<FSlackAPIClient>(Token);

		SlackAPI::OpenAPIApiChatPostMessagePostRequest Request;
		Request.Channel = Channel;
		Request.Text = TEXT("Text Message");
		Client->PostMessage(Request);
	}

	void PostBlock(const FString& Token, const FString& Channel)
	{
		TSharedRef<FSlackAPIClient> Client = MakeShared<FSlackAPIClient>(Token);

		const FString BlockPayload = TEXT(R"({
	"blocks": [
		{
			"type": "section",
			"text": {
				"type": "mrkdwn",
				"text": "*Block Message*"
			}
		},
		{
			"type": "divider"
		},
	]
})");

		SlackAPI::OpenAPIApiChatPostMessagePostRequest Request;
		Request.Channel = Channel;
		TArray<TSharedPtr<FJsonObject>> Blocks;
		if (FSlackAPIClient::TryParseBlockKibBuilderPayload(BlockPayload, Blocks))
		{
			Request.Blocks = Blocks;
			Client->PostMessage(Request);
		}
	}

	void PostAttachments(const FString& Token, const FString& Channel)
	{
		TSharedRef<FSlackAPIClient> Client = MakeShared<FSlackAPIClient>(Token);

		const TArray<FString> AttachmentPayloads = {
			TEXT(R"({"pretext": "pre-text1", "text": "text-1", "color": "danger"})"),
			TEXT(R"({"title": "Title", "color": "good"})")
		};

		TArray<TSharedPtr<FJsonObject>> Attachments;
		for (const FString& Payload : AttachmentPayloads)
		{
			TSharedPtr<FJsonObject> JsonPayload = MakeShared<FJsonObject>();
			if (FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Payload), JsonPayload))
			{
				Attachments.Add(JsonPayload);
			}
		}

		SlackAPI::OpenAPIApiChatPostMessagePostRequest Request;
		Request.Channel = Channel;
		Request.Attachments = Attachments;
		Client->PostMessage(Request);
	}

	void PostThread(const FString& Token, const FString& Channel)
	{
		TSharedRef<FSlackAPIClient> Client = MakeShared<FSlackAPIClient>(Token);

		SlackAPI::OpenAPIApiChatPostMessagePostRequest PostMessageRequest;
		PostMessageRequest.Channel = Channel;
		PostMessageRequest.Text = TEXT("Parent");

		const TOptional<SlackAPI::OpenAPIApiChatPostMessagePost200Response> Res = Client->PostMessage(PostMessageRequest);
		if (Res.IsSet() && Res->Ts.IsSet())
		{
			const FString Thread_Ts = Res->Ts.GetValue();
			PostMessageRequest.Text = TEXT("Thread");
			PostMessageRequest.ThreadTs = Thread_Ts;
			Client->PostMessage(PostMessageRequest);

			SlackAPI::OpenAPIFileUploadApi::ApiFilesUploadPostRequest FileUploadRequest;
			FileUploadRequest.Channels = Channel;
			FileUploadRequest.Filename = TEXT("hello.text");
			FileUploadRequest.ThreadTs = Thread_Ts;
			FileUploadRequest.Content = TEXT("Hello World!");
			Client->FilesUpload(FileUploadRequest);
		}
	}

	void UploadFile(const FString& Token, const FString& Channel)
	{
		TSharedRef<FSlackAPIClient> Client = MakeShared<FSlackAPIClient>(Token);

		const FString JsonData = TEXT(R"({
	"name": "Example",
	"location":
	{
		"x": 1,
		"y": 2,
		"z": 3
	},
	"bIsEnabled": true
})");

		SlackAPI::OpenAPIFileUploadApi::ApiFilesUploadPostRequest Request;
		Request.Channels = Channel;
		Request.InitialComment = TEXT("UploadFile");
		Request.Filename = TEXT("example.json");
		Request.Content = JsonData;

		Client->FilesUpload(Request);
	}

	void UploadMultiFiles(const FString& Token, const FString& Channel)
	{
		TSharedRef<FSlackAPIClient> Client = MakeShared<FSlackAPIClient>(Token);

		// For more information: https://stackoverflow.com/questions/59939261/send-multiple-files-to-slack-via-api

		TArray<FString> Permalinks;
		const TArray<FString> Files = {
			TEXT("file1.text"),
			TEXT("file2.text"),
		};
		for (const FString& File : Files)
		{
			SlackAPI::OpenAPIFileUploadApi::ApiFilesUploadPostRequest Request;
			Request.Filename = File;
			Request.Content = FString::Printf(TEXT("File content of %s"), *File);
			TOptional<SlackAPI::OpenAPIApiFilesUploadPost200Response> Response = Client->FilesUpload(Request);
			if (Response.IsSet() && Response->File.IsSet())
			{
				FString Permalink;
				if (Response->File.GetValue()->TryGetStringField(TEXT("permalink"), Permalink))
				{
					Permalinks.Add(Permalink);
				}
			}
		}

		FString Message = TEXT("Files uploaded:\n");
		for (const FString& Permalink : Permalinks)
		{
			Message += FString::Printf(TEXT("<%s|>\n"), *Permalink);
		}

		SlackAPI::OpenAPIApiChatPostMessagePostRequest Request;
		Request.Channel = Channel;
		Request.Text = Message;
		Client->PostMessage(Request);
	}
}
#endif
