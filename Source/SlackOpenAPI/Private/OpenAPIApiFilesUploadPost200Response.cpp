/**
 * Slack Web API
 * No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
 *
 * OpenAPI spec version: 1.0.0
 * 
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * https://github.com/OpenAPITools/openapi-generator
 * Do not edit the class manually.
 */

#include "OpenAPIApiFilesUploadPost200Response.h"

#include "SlackOpenAPIModule.h"
#include "OpenAPIHelpers.h"

#include "Templates/SharedPointer.h"

namespace SlackAPI
{

void OpenAPIApiFilesUploadPost200Response::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (Ok.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("ok")); WriteJsonValue(Writer, Ok.GetValue());
	}
	if (File.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("file")); WriteJsonValue(Writer, File.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool OpenAPIApiFilesUploadPost200Response::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("ok"), Ok);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("file"), File);

	return ParseSuccess;
}

}
