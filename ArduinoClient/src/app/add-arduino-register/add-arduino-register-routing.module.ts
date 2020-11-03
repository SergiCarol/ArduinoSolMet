import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { AddArduinoRegisterPage } from './add-arduino-register.page';

const routes: Routes = [
  {
    path: '',
    component: AddArduinoRegisterPage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class AddArduinoRegisterPageRoutingModule {}
