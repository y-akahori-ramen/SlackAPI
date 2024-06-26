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

#pragma once

#include "OpenAPIBaseModel.h"

namespace SlackAPI
{

/*
 * OpenAPIApiChatPostMessagePost200Response
 *
 * 
 */
class SLACKOPENAPI_API OpenAPIApiChatPostMessagePost200Response : public Model
{
public:
    virtual ~OpenAPIApiChatPostMessagePost200Response() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	TOptional<bool> Ok;
	TOptional<FString> Channel;
	TOptional<FString> Ts;
	TOptional<TSharedPtr<FJsonObject>> Message;
};

}
