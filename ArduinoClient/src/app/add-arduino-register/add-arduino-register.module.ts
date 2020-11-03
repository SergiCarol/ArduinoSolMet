import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { AddArduinoRegisterPageRoutingModule } from './add-arduino-register-routing.module';

import { AddArduinoRegisterPage } from './add-arduino-register.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    AddArduinoRegisterPageRoutingModule
  ],
  declarations: [AddArduinoRegisterPage]
})
export class AddArduinoRegisterPageModule {}
