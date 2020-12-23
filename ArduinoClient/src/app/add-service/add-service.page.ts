import { Component, OnInit, Input } from '@angular/core';
import { Arduino } from '../connector/arduinos';
import { AuthResponse } from  '../auth/auth-response';
import { Router } from  "@angular/router";
import { ConnectService } from '../connector/connect.service';
import { Services, Service, ServiceClass } from '../connector/services';

@Component({
  selector: 'app-add-service',
  templateUrl: './add-service.page.html',
  styleUrls: ['./add-service.page.scss'],
})
export class AddServicePage implements OnInit {
  user: AuthResponse;
  arduino: Arduino;
  new_service: Service;

  services = [
    {name: 'Water Pump 1', value: 'water_pump_1'},
    {name: 'Water Pump 2', value: 'water_pump_2'},
    {name: 'Fan', value: 'fan '},
    {name: 'Fan 2', value: 'fan_2'},
    {name: 'Air Pump', value: 'air_pump'},
  ]

  service = new ServiceClass('', '', '', '');

  constructor(
    private  router:  Router,
    private  connector:  ConnectService,
  ) { 
    this.router.routeReuseStrategy.shouldReuseRoute = function() { return false; };

  }

  ngOnInit() {
    this.arduino = this.router.getCurrentNavigation().extras.state.arduino as Arduino;
    this.user = this.router.getCurrentNavigation().extras.state.user as AuthResponse;
    console.log("User", this.user.api_key);
    console.log("Arduino", this.arduino.api_key);
  }

  addService(){
    console.log(this.service.name);
    let new_service = {
      name: this.getName(this.service.name),
      end_time: this.service.end_time,
      start_time: this.service.start_time,
      active: this.service.active
    };
   
    console.log(new_service);

    this.connector.updateService(this.user.api_key, this.arduino.api_key, new_service, true).subscribe(res => {
      this.router.navigate(['arduino-services'], {state: {
        arduino: this.arduino,
        user: this.user}});    });

  }

  getName(service: any): string{
    for (let element of this.services){
      if (element.name == service.name){
        return element.value}
    }
  }

  goBack(){
    this.router.navigate(['arduino-services'])
  }
}
