import { NgModule } from '@angular/core';
import { PreloadAllModules, RouterModule, Routes } from '@angular/router';

const routes: Routes = [
  {
    path: 'home',
    loadChildren: () => import('./home/home.module').then( m => m.HomePageModule)
  },
  {
    path: '',
    redirectTo: 'login',
    pathMatch: 'full'
  },
  {
    path: 'login',
    loadChildren: () => import('./login/login.module').then( m => m.LoginPageModule)
  },
  {
    path: 'register',
    loadChildren: () => import('./register/register.module').then( m => m.RegisterPageModule)
  },
  {
    path: 'add-arduino-register',
    loadChildren: () => import('./add-arduino-register/add-arduino-register.module').then( m => m.AddArduinoRegisterPageModule)
  },
  {
    path: 'add-arduino-network',
    loadChildren: () => import('./add-arduino-network/add-arduino-network.module').then( m => m.AddArduinoNetworkPageModule)
  },  {
    path: 'arduino-data',
    loadChildren: () => import('./arduino-data/arduino-data.module').then( m => m.ArduinoDataPageModule)
  },
  {
    path: 'arduino-services',
    loadChildren: () => import('./arduino-services/arduino-services.module').then( m => m.ArduinoServicesPageModule)
  },
  {
    path: 'add-service',
    loadChildren: () => import('./add-service/add-service.module').then( m => m.AddServicePageModule)
  }

];

@NgModule({
  imports: [
    RouterModule.forRoot(routes, { preloadingStrategy: PreloadAllModules })
  ],
  exports: [RouterModule]
})
export class AppRoutingModule { }
