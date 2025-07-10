#include "CWWeatherService.h"

WeatherData CWWeatherService::getCurrentWeather() {
  if (millis() - lastUpdate > UPDATE_INTERVAL) {
    updateWeather();
  }
  return currentWeather;
}

void CWWeatherService::updateWeather() {
  String apiKey = ClockwiseParams::getInstance()->weatherApiKey;
  String cityId = ClockwiseParams::getInstance()->weatherCityId;
  
  if (apiKey.isEmpty() || cityId.isEmpty()) {
    Serial.println("Weather API key or city ID not configured");
    currentWeather.isValid = false;
    return;
  }
  
  Serial.println("Updating weather data...");
  String response = makeWeatherRequest();
  if (!response.isEmpty()) {
    if (parseWeatherResponse(response, currentWeather)) {
      Serial.printf("Weather updated: %s, %d°C\n", currentWeather.condition.c_str(), currentWeather.temperature);
    }
  } else {
    Serial.println("Failed to get weather data");
  }
  
  lastUpdate = millis();
}

String CWWeatherService::makeWeatherRequest() {
  String apiKey = ClockwiseParams::getInstance()->weatherApiKey;
  String cityId = ClockwiseParams::getInstance()->weatherCityId;
  
  if (apiKey.isEmpty() || cityId.isEmpty()) {
    return "";
  }
  
  client.setInsecure();
  client.setTimeout(10000);
  
  if (!client.connect("api.openweathermap.org", 443)) {
    Serial.println("Failed to connect to OpenWeather API");
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