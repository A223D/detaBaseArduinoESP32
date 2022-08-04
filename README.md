# Library For Interacting with a [Deta.sh](https://www.deta.sh/) Base Instance Using an Arduino-Based ESP32

## Tutorials using this library
Here is a 2 part tutorial that uses this library to send and receive static(fixed minified JSON strings) and dynamic data(such as values from a sensor): [https://github.com/A223D/ESP32DetaGuides](https://github.com/A223D/ESP32DetaGuides)

## Introduction
This library is abstracts away the networking aspect of interacting with a [Deta.sh Base Instance](https://docs.deta.sh/docs/home/#deta-base), a free online NoSQL data base. This library uses [Deta's provided HTTP API](https://docs.deta.sh/docs/base/http), and exposes all functions listed there. It is meant to be used on an ESP32 running the Arduino core provided by Espressif.

#### Required items to use this library:
* Deta Project ID
* Deta Project key (aka API Key)
* Deta Base name
> The Deta Project ID and Project Key are created when you a make new Deta project. Please note that the Project Key is displayed only **once**. So make sure to note it down. The Deta Base name is set when a new Base instance is created. Read the [provided docs](https://docs.deta.sh/docs/base/about) to get started.
* Wifi Connection (using the inbuilt WiFi.h library)
> Please note that enterprise connections are not accepted. The best way to experiment is using a mobile hotspot created with a phone or laptop.

## Theory of Operation
### Result Struct
A `result` struct is internally defined in the following manner:
```
typedef struct {
	int statusCode;
	String reply;
} result;
```
All functions that interact with Deta return a `result` structure containing the status code of the response received from the server, as well as the reply. The reply is not the complete response text, bu just the payload of the complete response in JSON. This payload along with the status describes either the success or failure of a request(or function) along with the reason. The [Deta Base HTTP API docs](https://docs.deta.sh/docs/base/http) describe all the types of requests and their possible responses. In addition to the docs, here is a list of all [HTTP status codes](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status#client_error_responses) and their meanings.
### Methods exposed in the library in a `DetaBaseObject` Object:
The strings being passed to each function all correspond to the payload required by each HTTP request given in the [docs](https://docs.deta.sh/docs/base/http). Consequently, they must be in the same format mentioned. They can however be one-line objects instead of the multi-line format. **Make sure each `"` has a preceding `\` to make it an escape character.
#### Constructors:
* `DetaBaseObject(WiFiClientSecure  wifiObject, char*  detaID, char*  detaBaseName, char*  apiKey);`
  * Creates `DetaBaseObject` on which functions below can be called. Takes a `WiFiClientSecure` object, which does not need anything done to it. You can just declare a `WiFiClientSecure`object and then pass it here. Requires a Deta project ID, Deta Base name, and Deta Project Key/API Key. It also allocated memory for a base URI (described in the [docs](https://docs.deta.sh/docs/base/http/)), creates, and assigns it to an internal variable.
* `DetaBaseObject(WiFiClientSecure  wifiObject, char*  detaID, char*  detaBaseName, char*  apiKey, bool  debugOption);`
  * Same as previous, but enables debugging statements if `true` is passed for the `debugOption` parameter.
#### Functions:
Each function exposed corresponds to a request listed in the [docs](https://docs.deta.sh/docs/base/http).
* `result putObject(char* jsonObject);`
  * `jsonObject` must be in the same format as described in the [Base HTTP docs](https://docs.deta.sh/docs/base/http), i.e. `{"items":[{"key":{key},"field1":"value1"}]}` with key being optional. In a C++ string though, a `\` character must be added before each `"`, but the resulting `\"` is counted as 1 character. Hence the `jsonObject` becomes `{\"items\":[{\"key\":{key},\"field1\":\"value1\"}]}`. The key, if provided, **must also be a string**. Otherwise it is automatically assigned by Deta and is returned with the full object in the `reply` in the returned `result` object. You can also put another JSON object in the value of an item.
* `result getObject(char* key);`
  * This request does not require a JSON payload, but only a key. `key` can be a valid or invalid key in the database, and the status code will reflect that. `reply` will contain either the full object with the key provided or an error.
* `result deleteObject(char* key);`
  * Similar to `getObject`, this does not require a JSON payload, but only a valid or invalid key. The status code returned will always be `200(successful)` and the reply will contain the key provided originally. By design, there is no way to know if an entry with that `key` existed, or if the key was invalid.
* `result insertObject(char* jsonObject);`
  * Similar to `getObject`, but with slightly different payload/`jsonObject` format. According to the [docs](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status#client_error_responses), the `"items"` key does not require an array, but just an object to be created in the database, with an optional key. When a key is provided, an entry is only created if there no other entry with the same key. If a key is not provided, it is the same as `putObject`.
* `result updateObject(char* jsonObject, char* key)`;
  * Updates an entry only if an entry with that `key` exists according to the payload `jsonObject`. See [here](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status#client_error_responses) to understand the format of the payload/`jsonObject` required with an example.
* `result query(char* queryObject);`
  * `queryObject` must be in the format listed [here](https://docs.deta.sh/docs/base/http#query-items). The `reply` in the `result` object returned will contain all the entries that matched the query. More information on Deta Base queries is available [here](https://docs.deta.sh/docs/base/queries/).
* `char* getDetaID();`
  * Simply returns Deta ID provided.
* `char* getBaseURI();`
  * `getBaseURI()` returns a character array in the following format: `/v1/{project_id}/{base_name}`
#### Destructor:
* `~DetaBaseObject();`
	* The destructor just frees the the memory allocated to hold the Base URI.


#### Methods exposed separately in the library:
* `void printResult(result resultObject);`
	* This method just prints the status code and JSON payload received in the response to a request(or function call) with a `\n` character in the end. The `\n` character is added by this function for convenience and is not present in the `reply` of the returned `result` struct.

## Usage Instructions
Examples are provided for reference and can be accessed though the Arduino menu or through the `examples` directory in this repository.

### 1. WiFiClientSecure
```
WiFiClientSecure client;
DetaBaseObject detaObj(client, detaID, detaBaseName, apiKey, true);
```
A `WiFiClientSecure` object is needed to initialize a `DetaBaseObject` object, and must be passed to it. There is no needed modification to the `WiFiClientSecure` object. 

### 2. WiFi Connection
A WiFi connection must be established using the inbuilt `WiFi` library like so:
```arduino
WiFiClientSecure client;
DetaBaseObject  detaObj(client, detaID, detaBaseName, apiKey, true);

void setup() {
	Serial.begin(115200);
	WiFi.begin("SSID", "PASSWORD");
	Serial.println("Waiting to connect to WiFi");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println();
	digitalWrite(LED, HIGH);
}

void loop(){
	//do something here with detaObj
}
```

