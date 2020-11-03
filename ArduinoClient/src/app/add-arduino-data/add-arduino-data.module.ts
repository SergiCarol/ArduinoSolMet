import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { AddArduinoDataPageRoutingModule } from './add-arduino-data-routing.module';

import { AddArduinoDataPage } from './add-arduino-data.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    AddArduinoDataPageRoutingModule
  ],
  declarations: [AddArduinoDataPage]
})
export class AddArduinoDataPageModule {}
