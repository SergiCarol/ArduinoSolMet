import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { ArduinoServicesPage } from './arduino-services.page';

const routes: Routes = [
  {
    path: '',
    component: ArduinoServicesPage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class ArduinoServicesPageRoutingModule {}
