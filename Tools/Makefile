gen-api:
	rm -rf SlackOpenAPI 
	docker run --rm -v "$(CURDIR)":/local openapitools/openapi-generator-cli:v6.2.0 generate \
	    -i /local/openapi.yml \
		-g cpp-ue4 \
		-o /local/SlackOpenAPI \
		--additional-properties=unrealModuleName=SlackOpenAPI,cppNamespace=SlackAPI	\
		--skip-validate-spec	
	rm -rf ../Source/SlackOpenAPI
	cp -r SlackOpenAPI/ ../Source/SlackOpenAPI
	rm -rf SlackOpenAPI 
.PHONY: gen-api
