#pragma once

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "CWPreferences.h"

enum WeatherStatus {
  WEATHER_OK,
  WEATHER_CONNECTING,
  WEATHER_ERROR
};

struct WeatherData {
  String condition;
  String description;
  int temperature;
  int humidity;
  bool isValid;
  WeatherStatus status;
  
  WeatherData() : temperature(0), humidity(0), isValid(false), status(WEATHER_ERROR) {}
};

struct CWWeatherService {
  static CWWeatherService* getInstance() {
    static CWWeatherService instance;
    return &instance;
  }

  WeatherData getCurrentWeather();
  void updateWeather();
  String getWeatherCondition(const String& weatherId);
  void setConnecting();
  
private:
  WiFiClientSecure client;
  WeatherData currentWeather;
  unsigned long lastUpdate = 0;
  const unsigned long UPDATE_INTERVAL = 300000; // 5 minutes
  
  // Retry logic for startup
  bool startupComplete = false;
  int startupRetryCount = 0;
  const int MAX_STARTUP_RETRIES = 5;
  unsigned long lastRetryTime = 0;
  const unsigned long RETRY_INTERVAL = 10000; // 10 seconds between retries
  
  String makeWeatherRequest();
  bool parseWeatherResponse(const String& response, WeatherData& weather);
  bool attemptWeatherFetch();
}; 