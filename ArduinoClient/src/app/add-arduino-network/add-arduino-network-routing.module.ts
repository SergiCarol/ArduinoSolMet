import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { AddArduinoNetworkPage } from './add-arduino-network.page';

const routes: Routes = [
  {
    path: '',
    component: AddArduinoNetworkPage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class AddArduinoNetworkPageRoutingModule {}
