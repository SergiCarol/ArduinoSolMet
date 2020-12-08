import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { ArduinoDataPageRoutingModule } from './arduino-data-routing.module';

import { ArduinoDataPage } from './arduino-data.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    ArduinoDataPageRoutingModule
  ],
  declarations: [ArduinoDataPage]
})
export class ArduinoDataPageModule {}
