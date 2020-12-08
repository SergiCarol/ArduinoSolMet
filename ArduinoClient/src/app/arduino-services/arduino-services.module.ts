import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { ArduinoServicesPageRoutingModule } from './arduino-services-routing.module';

import { ArduinoServicesPage } from './arduino-services.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    ArduinoServicesPageRoutingModule
  ],
  declarations: [ArduinoServicesPage]
})
export class ArduinoServicesPageModule {}
