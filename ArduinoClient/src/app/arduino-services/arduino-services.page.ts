import { Component, OnInit, Input, HostListener  } from '@angular/core';
import { Arduino } from '../connector/arduinos';
import { AuthResponse } from  '../auth/auth-response';
import { Router, NavigationEnd } from  "@angular/router";
import { ConnectService } from '../connector/connect.service';
import { Services, Service } from '../connector/services';
import { ToastController } from '@ionic/angular';

@Component({
  selector: 'app-arduino-services',
  templateUrl: './arduino-services.page.html',
  styleUrls: ['./arduino-services.page.scss'],
})
export class ArduinoServicesPage implements OnInit  {
  user: AuthResponse;
  arduino: Arduino;
  show: boolean = false;
  @Input() services: Services;

  services_names = [
    {name: 'Water Pump 1', value: 'water_pump_1'},
    {name: 'Water Pump 2', value: 'water_pump_2'},
    {name: 'Fan', value: 'fan'},
    {name: 'Fan 2', value: 'fan_2'},
    {name: 'Air Pump', value: 'air_pump'},
  ]
  constructor(
    private  router:  Router,
    private  connector:  ConnectService,
    public toastController: ToastController
  ) { 
    this.router.routeReuseStrategy.shouldReuseRoute = function() { return false; }
    this.router.events.subscribe((evt) => {
      if (evt instanceof NavigationEnd) {
         // trick the Router into believing it's last link wasn't previously loaded
         this.router.navigated = false;
         // if you need to scroll back to top, here is the right place
         window.scrollTo(0, 0);
         this.ngOnInit()
      }
  });;
    }

  ngOnInit() {
    this.arduino = this.router.getCurrentNavigation().extras.state.arduino as Arduino;
    this.user = this.router.getCurrentNavigation().extras.state.user as AuthResponse;
    console.log("User", this.user.api_key);
    console.log("Arduino", this.arduino.api_key);
    this.connector.getServices(this.user.api_key, this.arduino.api_key).subscribe((res) =>
      this.services = res
    );
  }

  updateService(service: Service) {
    console.log("Service", service);
    this.connector.updateService(this.user.api_key, this.arduino.api_key, service).subscribe(res => {
        if (res == "ok"){
          this.presentToast('Your settings have been saved.');
        }
        else {
          console.log("Received message error", res);
          this.presentToast('An error occurred while saving');
        }
      });
  }

  addService(){
    this.router.navigate(['add-service'], {state: {
      arduino: this.arduino,
      user: this.user}});
  }

  async presentToast(message: string) {
    const toast = await this.toastController.create({
      message: message,
      duration: 2000
    });
    toast.present();
  }
  get serviceItems() {
    return (this.services) ? this.services.arduino  : null 
  }

  getName(service: string): string{
    for (let element of this.services_names){
      if (element.value == service){
        return element.name}
    }
  }


}
