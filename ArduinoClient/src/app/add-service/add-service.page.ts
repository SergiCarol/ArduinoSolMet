import { Component, OnInit, Input } from '@angular/core';
import { Arduino } from '../connector/arduinos';
import { AuthResponse } from  '../auth/auth-response';
import { Router } from  "@angular/router";
import { ConnectService } from '../connector/connect.service';
import { Services, Service } from '../connector/services';

@Component({
  selector: 'app-add-service',
  templateUrl: './add-service.page.html',
  styleUrls: ['./add-service.page.scss'],
})
export class AddServicePage implements OnInit {
  user: AuthResponse;
  arduino: Arduino;
  @Input() services = {
    'water_pump_1': 'Water Pump 1',
    'water_pump_2': 'Water Pump 2',
    'fan_1': 'Fan',
    'fan_2': 'Fan 2',
    'air_pump': 'Air Pump',
  }

  constructor(
    private  router:  Router,
    private  connector:  ConnectService,
  ) { }

  ngOnInit() {
    this.arduino = this.router.getCurrentNavigation().extras.state.arduino as Arduino;
    this.user = this.router.getCurrentNavigation().extras.state.user as AuthResponse;
    console.log("User", this.user.api_key);
    console.log("Arduino", this.arduino.api_key);
  }

  addService(form){
    console.log(form.value);
    /*
    this.connector.updateService(this.user.api_key, this.arduino.api_key, form.value).subscribe(res => {
      this.router.navigate(['arduino-services'], {state: {
        arduino: this.arduino,
        user: this.user}});    });
    */
      }
}
