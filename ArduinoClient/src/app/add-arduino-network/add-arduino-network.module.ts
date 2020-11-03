import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { AddArduinoNetworkPageRoutingModule } from './add-arduino-network-routing.module';

import { AddArduinoNetworkPage } from './add-arduino-network.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    AddArduinoNetworkPageRoutingModule
  ],
  declarations: [AddArduinoNetworkPage]
})
export class AddArduinoNetworkPageModule {}
