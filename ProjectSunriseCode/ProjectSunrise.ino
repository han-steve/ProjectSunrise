#include "HomeSpan.h"
#include "DEV_Blinds.h"

void setup() {
  Serial.begin(115200);

  homeSpan.begin(Category::WindowCoverings, "Project Sunrise");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("Big Blind");

      new Characteristic::Manufacturer("SteveHan");
      new Characteristic::SerialNumber("696969");
      new Characteristic::Model("69");
      new Characteristic::FirmwareRevision(".69");

      new Characteristic::Identify();

    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

    new DEV_BigBlind();
//    new Service::WindowCovering();
//      new Characteristic::CurrentPosition(0);
//      new Characteristic::TargetPosition(0);
//      new Characteristic::PositionState(2);

// separate controls for blinds not working bc idk why
//  new SpanAccessory();
//    new Service::AccessoryInformation();
//      new Characteristic::Name("Small Blind");
//
//      new Characteristic::Manufacturer("SteveHan");
//      new Characteristic::SerialNumber("696969");
//      new Characteristic::Model("69");
//      new Characteristic::FirmwareRevision(".69");
//
//      new Characteristic::Identify();
//
//    new Service::HAPProtocolInformation();
//      new Characteristic::Version("1.1.0");
//
//    new DEV_SmallBlind();
    
}

void loop() {
  // put your main code here, to run repeatedly:
  homeSpan.poll();
}
