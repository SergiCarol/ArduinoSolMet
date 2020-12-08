import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { ArduinoDataPage } from './arduino-data.page';

const routes: Routes = [
  {
    path: '',
    component: ArduinoDataPage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class ArduinoDataPageRoutingModule {}
