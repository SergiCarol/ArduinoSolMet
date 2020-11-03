import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { AddArduinoDataPage } from './add-arduino-data.page';

const routes: Routes = [
  {
    path: '',
    component: AddArduinoDataPage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class AddArduinoDataPageRoutingModule {}
