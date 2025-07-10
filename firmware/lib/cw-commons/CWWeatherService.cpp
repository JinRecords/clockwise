#include "CWWeatherService.h"

WeatherData CWWeatherService::getCurrentWeather() {
  // If startup is not complete and we haven't exhausted retries, attempt startup fetch
  if (!startupComplete && startupRetryCount < MAX_STARTUP_RETRIES) {
    if (millis() - lastRetryTime >= RETRY_INTERVAL) {
      Serial.printf("Weather startup attempt %d/%d\n", startupRetryCount + 1, MAX_STARTUP_RETRIES);
      if (attemptWeatherFetch()) {
        startupComplete = true;
        Serial.println("Weather startup successful");
      } else {
        startupRetryCount++;
        if (startupRetryCount >= MAX_STARTUP_RETRIES) {
          Serial.println("Weather startup failed after all retries");
          startupComplete = true; // Mark as complete to stop retrying
        }
      }
      lastRetryTime = millis();
    }
  }
  
  // Regular update interval check (only after startup is complete)
  if (startupComplete && millis() - lastUpdate > UPDATE_INTERVAL) {
    updateWeather();
  }
  
  return currentWeather;
}

void CWWeatherService::setConnecting() {
  currentWeather.status = WEATHER_CONNECTING;
  currentWeather.isValid = false;
}

void CWWeatherService::updateWeather() {
  if (attemptWeatherFetch()) {
    lastUpdate = millis();
  }
}

bool CWWeatherService::attemptWeatherFetch() {
  String apiKey = ClockwiseParams::getInstance()->weatherApiKey;
  String cityId = ClockwiseParams::getInstance()->weatherCityId;
  
  if (apiKey.isEmpty() || cityId.isEmpty()) {
    Serial.println("Weather API key or city ID not configured");
    currentWeather.isValid = false;
    currentWeather.status = WEATHER_ERROR;
    return false;
  }
  
  Serial.println("Updating weather data...");
  
  String response = makeWeatherRequest();
  if (!response.isEmpty()) {
    if (parseWeatherResponse(response, currentWeather)) {
      currentWeather.status = WEATHER_OK;
      Serial.printf("Weather updated: %s, %d°C\n", currentWeather.condition.c_str(), currentWeather.temperature);
      return true;
    } else {
      currentWeather.status = WEATHER_ERROR;
      Serial.println("Failed to parse weather data");
      return false;
    }
  } else {
    currentWeather.status = WEATHER_ERROR;
    Serial.println("Failed to get weather data");
    return false;
  }
}

String CWWeatherService::makeWeatherRequest() {
  String apiKey = ClockwiseParams::getInstance()->weatherApiKey;
  String cityId = ClockwiseParams::getInstance()->weatherCityId;
  
  if (apiKey.isEmpty() || cityId.isEmpty()) {
    return "";
  }
  
  client.setInsecure();
  client.setTimeout(10000);
  
  // Set connecting state
  currentWeather.status = WEATHER_CONNECTING;
  
  if (!client.connect("api.openweathermap.org", 443)) {
    Serial.println("Failed to connect to OpenWeather API");
    currentWeather.status = WEATHER_ERROR;
    return "";
  }
  
  String request = "GET /data/2.5/weather?id=" + cityId + "&appid=" + apiKey + "&units=metric HTTP/1.1\r\n";
  request += "Host: api.openweathermap.org\r\n";
  request += "Connection: close\r\n\r\n";
  
  Serial.printf("Making weather request for city ID: %s\n", cityId.c_str());
  client.print(request);
  
  // Wait for response
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("Weather API request timeout");
      client.stop();
      currentWeather.status = WEATHER_ERROR;
      return "";
    }
  }
  
  // Read response
  String response = "";
  while (client.available()) {
    response += client.readString();
  }
  
  client.stop();
  
  // Extract JSON from HTTP response
  int jsonStart = response.indexOf('{');
  if (jsonStart == -1) {
    Serial.println("No JSON found in weather response");
    currentWeather.status = WEATHER_ERROR;
    return "";
  }
  
  return response.substring(jsonStart);
}

bool CWWeatherService::parseWeatherResponse(const String& response, WeatherData& weather) {
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, response);
  
  if (error) {
    Serial.print("Weather JSON parsing failed: ");
    Serial.println(error.c_str());
    weather.isValid = false;
    return false;
  }
  
  // Check for API errors
  if (doc.containsKey("cod") && doc["cod"] != 200) {
    Serial.printf("Weather API error: %d - %s\n", doc["cod"].as<int>(), doc["message"].as<const char*>());
    weather.isValid = false;
    return false;
  }
  
  if (doc.containsKey("weather") && doc["weather"].is<JsonArray>()) {
    JsonArray weatherArray = doc["weather"];
    if (weatherArray.size() > 0) {
      JsonObject weatherObj = weatherArray[0];
      weather.condition = getWeatherCondition(weatherObj["id"].as<String>());
      weather.description = weatherObj["description"].as<String>();
    }
  }
  
  if (doc.containsKey("main")) {
    JsonObject main = doc["main"];
    weather.temperature = main["temp"].as<int>();
    weather.humidity = main["humidity"].as<int>();
  }
  
  weather.isValid = true;
  return true;
}

String CWWeatherService::getWeatherCondition(const String& weatherId) {
  int id = weatherId.toInt();
  
  // Thunderstorm
  if (id >= 200 && id < 300) return "thunder";
  
  // Drizzle
  if (id >= 300 && id < 400) return "drizzle";
  
  // Rain
  if (id >= 500 && id < 600) {
    if (id >= 511 && id <= 531) return "rain";
    return "rain";
  }
  
  // Snow
  if (id >= 600 && id < 700) return "snow";
  
  // Atmosphere (fog, mist, etc.)
  if (id >= 700 && id < 800) return "fog";
  
  // Clear
  if (id == 800) return "clear";
  
  // Clouds
  if (id >= 801 && id <= 804) {
    if (id == 801) return "partly";
    if (id == 802) return "cloudy";
    return "overcast";
  }
  
  return "unknown";
} 